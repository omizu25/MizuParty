//==================================================
// 
// 3Dゲーム制作 ( billboard.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "effect.h"
#include "field.h"
#include "particle.h"
#include "player.h"
#include "setup.h"
#include "wall.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_BILLBOARD		(256)			// ビルボードの最大数
#define MAX_TEXTURE			(256)			// テクスチャの最大数
#define DO_NOT_ROT_Y		(0)				// Y軸回転をしない数値
#define DO_NOT_RESULT		(0)				// リザルトで表示しない
#define TARGET_WIDTH		(250.0f)		// 幅
#define TARGET_HEIGHT		(600.0f)		// 高さ
#define CHEAT_WIDTH			(300.0f)		// 幅
#define CHEAT_HEIGHT		(2000.0f)		// 高さ
#define PLAYER_WIDTH		(300.0f)		// 幅
#define PLAYER_HEIGHT		(1500.0f)		// 高さ

//--------------------------------------------------
// 構造体
//--------------------------------------------------

/*↓ ビルボード ↓*/

typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	D3DXVECTOR3				move;			// 移動量
	D3DXMATRIX				mtxWorld;		// ワールドマトリックス
	float					fWidth;			// 幅
	float					fHeight;		// 高さ
	bool					bUse;			// 使用しているかどうか
	bool					bYRot;			// Y軸回転をするかどうか
	bool					bResult;		// リザルトだけで表示する
	bool					bCamera;		// カメラ何番目か
	LPDIRECT3DTEXTURE9		pTexture;		// テクスチャ
}Billboard;

/*↓ 読み込む内容 ↓*/

typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	float					fWidth;			// 幅
	float					fHeight;		// 高さ
	int						nTexIdx;		// テクスチャ番号
	int						nYRot;			// Y軸回転をするかどうか
	int						nResult;		// リザルトで表示する
	int						nCamera;		// カメラ何番目か
	LPDIRECT3DTEXTURE9		pTexture;		// テクスチャ
}Text;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			*s_pTexture;					// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// 頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureTarget;				// 目標地点のテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureTargetCheat;			// ずるをした目標地点のテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTexturePlayer;				// 自転車のテクスチャへのポインタ
static Billboard					s_billboard[MAX_BILLBOARD];		// ビルボードの情報
static int							s_nUseTex;						// テクスチャの使用数

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void System(FILE *pFile, char *aFile);
static void Load(FILE *pFile);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitBillboard(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//メモリのクリア
	memset(s_billboard, 0, sizeof(s_billboard));

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		//全ての初期化処理
		InitAll(pVtx);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitBillboard(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの解放
		for (int i = 0; i < s_nUseTex; i++)
		{
			if (s_pTexture[i] != NULL)
			{
				s_pTexture[i]->Release();
				s_pTexture[i] = NULL;
			}
		}

		delete[] s_pTexture;
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pTextureTarget != NULL)
	{// テクスチャの解放
		s_pTextureTarget->Release();
		s_pTextureTarget = NULL;
	}

	if (s_pTextureTargetCheat != NULL)
	{// テクスチャの解放
		s_pTextureTargetCheat->Release();
		s_pTextureTargetCheat = NULL;
	}

	if (s_pTexturePlayer != NULL)
	{// テクスチャの解放
		s_pTexturePlayer->Release();
		s_pTexturePlayer = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateBillboard(void)
{
	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (!pBillboard->bUse || pBillboard->pTexture != s_pTexturePlayer)
		{//使用されていない
			continue;
		}

		D3DXVECTOR3 pos = D3DXVECTOR3(GetPlayer()->pos.x, GetPlayer()->pos.y, 0.0f);

		pBillboard->pos = pos;
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawBillboard(bool bResult, bool bCamera)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (!pBillboard->bUse || pBillboard->bResult != bResult || pBillboard->bCamera != bCamera)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pBillboard->mtxWorld);
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラの逆行列を設定
		if (pBillboard->bYRot)
		{// Y軸回転をする
			pBillboard->mtxWorld._11 = mtxView._11;
			pBillboard->mtxWorld._12 = mtxView._21;
			pBillboard->mtxWorld._13 = mtxView._31;
			pBillboard->mtxWorld._21 = mtxView._12;
			pBillboard->mtxWorld._22 = mtxView._22;
			pBillboard->mtxWorld._23 = mtxView._32;
			pBillboard->mtxWorld._31 = mtxView._13;
			pBillboard->mtxWorld._32 = mtxView._23;
			pBillboard->mtxWorld._33 = mtxView._33;
		}
		else
		{// Y軸回転をしない
			pBillboard->mtxWorld._11 = mtxView._11;
			pBillboard->mtxWorld._13 = mtxView._31;
			pBillboard->mtxWorld._31 = mtxView._13;
			pBillboard->mtxWorld._33 = mtxView._33;
		}

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);
		D3DXMatrixMultiply(&pBillboard->mtxWorld, &pBillboard->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pBillboard->mtxWorld);

		// テクスチャの設定
		pDevice->SetTexture(0, pBillboard->pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			i * 4,						// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zバッファの値を元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// 新規深度値 <= Zバッファ深度値 (初期設定)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, bool bYRot, bool bResult, bool bCamera, LPDIRECT3DTEXTURE9 *pTexture)
{
	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (pBillboard->bUse)
		{//使用されている
			continue;
		}

		/*↓ 使用されていない ↓*/

		pBillboard->pos = pos;
		pBillboard->move = move;
		pBillboard->fWidth = fWidth;
		pBillboard->fHeight = fHeight;
		pBillboard->pTexture = *pTexture;
		pBillboard->bYRot = bYRot;
		pBillboard->bResult = bResult;
		pBillboard->bCamera = bCamera;
		pBillboard->bUse = true;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		// 頂点座標の設定
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		// 頂点の法線の設定
		Setnor(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadBillboard(void)
{
	FILE *pFile;			// ファイルポインタを宣言

	char aFile[MAX_TEXT];

	// ファイルを開く
	pFile = fopen(FILE_NAME, "r");

	// システム
	System(pFile, aFile);

	// ファイルを開く
	pFile = fopen(aFile, "r");

	// 読み込み
	Load(pFile);
}

//--------------------------------------------------
// システム
//--------------------------------------------------
static void System(FILE *pFile, char *aFile)
{
	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};

		while (strcmp(&aRead[0], "SCRIPT") != 0)
		{// 始まりが来るまで繰り返す
			fscanf(pFile, "%s", &aRead);
		}

		while (strcmp(&aRead[0], "END_SCRIPT") != 0)
		{// 終わりが来るまで繰り返す
			fscanf(pFile, "%s", &aRead);

			if (strncmp(&aRead[0], "#-", 2) == 0)
			{// コメント
				continue;
			}
			else if (strncmp(&aRead[0], "#", 1) == 0)
			{// コメント
				fgets(aRead, MAX_TEXT, pFile);
				continue;
			}

			if (strcmp(&aRead[0], "BILLBOARD_FILENAME") == 0)
			{// モデルの情報
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", aFile);
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
static void Load(FILE *pFile)
{
	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};
		char aTexture[MAX_TEXT];
		int nTex = 0, nText = 0, nNumBillBoard = 0;
		Text *pText;

		//メモリのクリア
		memset(&pText, 0, sizeof(pText));

		while (strcmp(&aRead[0], "SCRIPT") != 0)
		{// 始まりが来るまで繰り返す
			fscanf(pFile, "%s", &aRead);
		}

		while (strcmp(&aRead[0], "END_SCRIPT") != 0)
		{// 終わりが来るまで繰り返す
			fscanf(pFile, "%s", &aRead);

			if (strncmp(&aRead[0], "#-", 2) == 0)
			{// コメント
				continue;
			}
			else if (strncmp(&aRead[0], "#", 1) == 0)
			{// コメント
				fgets(aRead, MAX_TEXT, pFile);
				continue;
			}

			if (strcmp(&aRead[0], "NUM_TEXTURE") == 0)
			{// テクスチャの使用数
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%d", &s_nUseTex);

				// txtに書いてる最大数分のテクスチャの配列を用意する
				s_pTexture = new LPDIRECT3DTEXTURE9[s_nUseTex];
			}
			else if (strcmp(&aRead[0], "TEXTURE_FILENAME") == 0)
			{// テクスチャの情報
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", aTexture);

				// デバイスへのポインタの取得
				LPDIRECT3DDEVICE9 pDevice = GetDevice();

				// テクスチャの読み込み
				D3DXCreateTextureFromFile(
					pDevice,
					&aTexture[0],
					&s_pTexture[nTex]);

				nTex++;
			}
			else if (strcmp(&aRead[0], "NUM_MODEL") == 0)
			{// ビルボードの使用数
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%d", &nNumBillBoard);

				// txtに書いてる最大数分の読み込み用の配列を用意する
				pText = new Text[nNumBillBoard];
			}
			else if (strcmp(&aRead[0], "BILLBOARD_SET") == 0)
			{// ビルボードの情報
				while (strcmp(&aRead[0], "END_BILLBOARD_SET") != 0)
				{// 終わりが来るまで繰り返す
					fscanf(pFile, "%s", &aRead);

					if (strncmp(&aRead[0], "#", 1) == 0)
					{// コメント
						fgets(aRead, MAX_TEXT, pFile);
						continue;
					}

					if (strcmp(&aRead[0], "TEXIDX") == 0)
					{// テクスチャ番号
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pText[nText].nTexIdx);
					}
					else if (strcmp(&aRead[0], "YROT") == 0)
					{// Y回転をするかどうか
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pText[nText].nYRot);
					}
					else if (strcmp(&aRead[0], "RESULT") == 0)
					{// リザルトで表示する
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pText[nText].nResult);
					}
					else if (strcmp(&aRead[0], "CAMERA") == 0)
					{// カメラが何番目か
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pText[nText].nCamera);
					}
					else if (strcmp(&aRead[0], "WIDTH") == 0)
					{// 幅
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pText[nText].fWidth);
					}
					else if (strcmp(&aRead[0], "HEIGHT") == 0)
					{// 高さ
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pText[nText].fHeight);
					}
					else if (strcmp(&aRead[0], "POS") == 0)
					{// 位置
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pText[nText].pos.x);
						fscanf(pFile, "%f", &pText[nText].pos.y);
						fscanf(pFile, "%f", &pText[nText].pos.z);
					}
				}
				nText++;
			}
		}

		// ファイルを閉じる
		fclose(pFile);

		for (int i = 0; i < nNumBillBoard; i++)
		{
			pText[i].pTexture = s_pTexture[pText[i].nTexIdx];

			bool bYRot = true;
			bool bResult = true;
			bool bCamera = true;

			if (pText[i].nYRot == DO_NOT_ROT_Y)
			{// Y軸回転をしない数値の時
				bYRot = false;
			}

			if (pText[i].nResult == DO_NOT_RESULT)
			{// Y軸回転をしない数値の時
				bResult = false;
			}

			if (pText[i].nCamera == 0)
			{// Y軸回転をしない数値の時
				bCamera = false;
			}

			// 設定
			SetBillboard(pText[i].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pText[i].fWidth, pText[i].fHeight, bYRot, bResult, bCamera, &pText[i].pTexture);
		}

		delete[] pText;

		pText = NULL;
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//--------------------------------------------------
// 坂
//--------------------------------------------------
void InitBillboardSlope(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/target000.png",
		&s_pTextureTarget);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/target001.png",
		&s_pTextureTargetCheat);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/player.png",
		&s_pTexturePlayer);

	float fWidth = TARGET_WIDTH * 0.5f;
	float fHeight = TARGET_HEIGHT * 0.5f;

	float fPosX = (GetField()->pos.x + GetField()->vtxMax.x);
	
	D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, fHeight + 100.0f, 30.0f);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ビルボードの設定
	SetBillboard(pos, move, fWidth, fHeight, true, false, false, &s_pTextureTarget);

	fWidth = CHEAT_WIDTH * 0.5f;
	fHeight = CHEAT_HEIGHT * 0.5f;

	fPosX = (GetField()->pos.x + GetField()->vtxMax.x);

	pos = D3DXVECTOR3(fPosX, 0.0f, 0.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ビルボードの設定
	SetBillboard(pos, move, fWidth, fHeight, true, false, true, &s_pTextureTargetCheat);
	
	fWidth = PLAYER_WIDTH * 0.5f;
	fHeight = PLAYER_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(GetPlayer()->pos.x * 0.5f, GetPlayer()->pos.y + 400.0f, 30.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ビルボードの設定
	SetBillboard(pos, move, fWidth, fHeight, true, false, true, &s_pTexturePlayer);
}
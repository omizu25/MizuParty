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
#include "fade.h"
#include "field.h"
#include "frame.h"
#include "game.h"
#include "particle.h"
#include "player.h"
#include "setup.h"
#include "sound.h"
#include "title.h"
#include "wall.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_TEXTURE			(256)			// テクスチャの最大数
#define DO_NOT_ROT_Y		(0)				// Y軸回転をしない数値
#define DO_NOT_RESULT		(0)				// リザルトで表示しない
#define TARGET_WIDTH		(250.0f)		// 目標地点の幅
#define TARGET_HEIGHT		(600.0f)		// 目標地点の高さ
#define CHEAT_WIDTH			(300.0f)		// ずるをした目標地点の幅
#define CHEAT_HEIGHT		(2000.0f)		// ずるをした目標地点の高さ
#define PLAYER_WIDTH		(300.0f)		// 自転車の幅
#define PLAYER_HEIGHT		(2000.0f)		// 自転車の高さ
#define TITLE_WIDTH			(130.0f)		// メニューの幅
#define TITLE_HEIGHT		(120.0f)		// メニューの高さ
#define RULE_WIDTH			(180.0f)		// ルールの幅
#define RULE_HEIGHT			(180.0f)		// ルールの高さ
#define MOVE_WIDTH			(180.0f)		// 移動の幅
#define MOVE_HEIGHT			(80.0f)			// 移動の高さ
#define REMIX_WIDTH			(150.0f)		// リミックスの幅
#define REMIX_HEIGHT		(60.0f)			// リミックスの高さ
#define MIZU_WIDTH			(150.0f)		// Mizuの幅
#define MIZU_HEIGHT			(100.0f)		// Mizuの高さ
#define PARTY_WIDTH			(150.0f)		// Partyの幅
#define PARTY_HEIGHT		(100.0f)		// Partyの高さ
#define LOGO_WIDTH			(100.0f)		// !!の幅
#define LOGO_HEIGHT			(100.0f)		// !!の高さ
#define LOGO_CHANGE			(0.4f)			// ロゴの変化

//--------------------------------------------------
// 構造体
//--------------------------------------------------
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
static LPDIRECT3DTEXTURE9			s_pTextureWalking;				// ウォーキングのテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureRotation;				// 回転のテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureStop;					// 止めるのテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureSlope;				// 坂のテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureRule;					// 説明のテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMove;					// 移動のテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureRemix;				// リミックスのテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMizu;					// Mizuのテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureParty;				// Partyのテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureLogo;					// !!のテクスチャへのポインタ
static Billboard					s_billboard[MAX_BILLBOARD];		// ビルボードの情報
static int							s_nUseTex;						// テクスチャの使用数
static int							s_nTime;						// タイム

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void System(FILE *pFile, char *aFile);
static void Load(FILE *pFile);
static void TitleMenu(void);
static void TitleRule(void);
static void TitleLogo(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitBillboard(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nTime = 0;

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
			{// テクスチャの解放
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

	if (s_pTextureWalking != NULL)
	{// テクスチャの解放
		s_pTextureWalking->Release();
		s_pTextureWalking = NULL;
	}

	if (s_pTextureRotation != NULL)
	{// テクスチャの解放
		s_pTextureRotation->Release();
		s_pTextureRotation = NULL;
	}

	if (s_pTextureStop != NULL)
	{// テクスチャの解放
		s_pTextureStop->Release();
		s_pTextureStop = NULL;
	}

	if (s_pTextureSlope != NULL)
	{// テクスチャの解放
		s_pTextureSlope->Release();
		s_pTextureSlope = NULL;
	}

	if (s_pTextureRule != NULL)
	{// テクスチャの解放
		s_pTextureRule->Release();
		s_pTextureRule = NULL;
	}

	if (s_pTextureMove != NULL)
	{// テクスチャの解放
		s_pTextureMove->Release();
		s_pTextureMove = NULL;
	}

	if (s_pTextureRemix != NULL)
	{// テクスチャの解放
		s_pTextureRemix->Release();
		s_pTextureRemix = NULL;
	}

	if (s_pTextureMizu != NULL)
	{// テクスチャの解放
		s_pTextureMizu->Release();
		s_pTextureMizu = NULL;
	}

	if (s_pTextureParty != NULL)
	{// テクスチャの解放
		s_pTextureParty->Release();
		s_pTextureParty = NULL;
	}

	if (s_pTextureLogo != NULL)
	{// テクスチャの解放
		s_pTextureLogo->Release();
		s_pTextureLogo = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateBillboard(void)
{
	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	float fSize = 0.0f, fWidth = 0.0f, fHeight = 0.0f;

	switch (GetMode())
	{
	case MODE_TITLE:		// タイトル

		s_nTime++;
		fSize = sinf((s_nTime * 0.01f) * (D3DX_PI * 2.0f)) * LOGO_CHANGE;
		fSize = fSize;

		for (int i = 0; i < MAX_BILLBOARD; i++)
		{
			Billboard *pBillboard = &s_billboard[i];

			if (!pBillboard->bUse)
			{//使用されていない
				continue;
			}
			
			if (pBillboard->pTexture != s_pTextureMizu &&
				pBillboard->pTexture != s_pTextureParty &&
				pBillboard->pTexture != s_pTextureLogo)
			{// テクスチャが違う
				continue;
			}

			/*↓ 使用している ↓*/

			fWidth = pBillboard->fWidth;
			fHeight = pBillboard->fHeight;

			pBillboard->fWidth += fSize;
			pBillboard->fHeight += fSize;

			// 頂点情報をロックし、頂点情報へのポインタを取得
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (i * 4);		//該当の位置まで進める

			// 頂点座標の設定
			Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pBillboard->fWidth, pBillboard->fHeight, 0.0f, SETPOS_MIDDLE);

			// 頂点バッファをアンロックする
			s_pVtxBuff->Unlock();
		}

		break;

	case MODE_GAME:			// ゲーム

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

		break;

	default:
		assert(false);
		break;
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
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight, bool bYRot, bool bResult, bool bCamera, LPDIRECT3DTEXTURE9 *pTexture)
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
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f, SETPOS_MIDDLE);

		// 頂点の法線の設定
		Setnor(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Billboard *GetBillboard(void)
{
	return &s_billboard[0];
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
// 当たり判定
//--------------------------------------------------
void CollisionBillboard(void)
{
	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (!pBillboard->bUse)
		{//使用されていない
			continue;
		}

		if (pBillboard->pTexture != s_pTextureWalking &&
			pBillboard->pTexture != s_pTextureRotation &&
			pBillboard->pTexture != s_pTextureStop && 
			pBillboard->pTexture != s_pTextureSlope &&
			pBillboard->pTexture != s_pTextureRemix)
		{// テクスチャが違う
			continue;
		}

		if (GetTitleCnt() < REMIX_OK)
		{
			if (pBillboard->pTexture == s_pTextureRemix)
			{// テクスチャが違う
				continue;
			}
		}

		D3DXVECTOR3 pos = GetPlayer()->pos;

		if (pBillboard->pos.x + pBillboard->fWidth >= pos.x &&
			pBillboard->pos.x - pBillboard->fWidth <= pos.x &&
			pBillboard->pos.z + pBillboard->fHeight >= pos.z &&
			pBillboard->pos.z - pBillboard->fHeight <= pos.z)
		{
			if (pBillboard->pTexture == s_pTextureWalking)
			{
				// タイトルの設定
				SetTitle(MENU_WALKING);

				// リミックス設定
				SetRemix(false);
			}
			else if (pBillboard->pTexture == s_pTextureRotation)
			{
				// タイトルの設定
				SetTitle(MENU_ROTATION);

				// リミックス設定
				SetRemix(false);
			}
			else if (pBillboard->pTexture == s_pTextureStop)
			{
				// タイトルの設定
				SetTitle(MENU_STOP);

				// リミックス設定
				SetRemix(false);
			}
			else if (pBillboard->pTexture == s_pTextureSlope)
			{
				// タイトルの設定
				SetTitle(MENU_SLOPE);

				// リミックス設定
				SetRemix(false);
			}
			else if (pBillboard->pTexture == s_pTextureRemix)
			{
				// タイトルの設定
				SetTitle(MENU_WALKING);

				// リミックス設定
				SetRemix(true);
			}

			// フェードの設定
			SetFade(MODE_GAME);

			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_決定音);
		}
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
	
	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureTarget);

	fWidth = CHEAT_WIDTH * 0.5f;
	fHeight = CHEAT_HEIGHT * 0.5f;

	fPosX = (GetField()->pos.x + GetField()->vtxMax.x);

	pos = D3DXVECTOR3(fPosX, 0.0f, 0.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, true, &s_pTextureTargetCheat);

	fWidth = PLAYER_WIDTH * 0.5f;
	fHeight = PLAYER_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(GetPlayer()->pos.x * 0.5f, GetPlayer()->pos.y + 400.0f, 30.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, true, &s_pTexturePlayer);
}

//--------------------------------------------------
// タイトル
//--------------------------------------------------
void InitBillboardTitle(void)
{
	// タイトルの説明
	TitleRule();

	// タイトルのメニュー
	TitleMenu();

	// タイトルロゴ
	TitleLogo();
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
			SetBillboard(pText[i].pos, pText[i].fWidth, pText[i].fHeight, bYRot, bResult, bCamera, &pText[i].pTexture);
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
// タイトルのメニュー
//--------------------------------------------------
static void TitleMenu(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/menu000.png",
		&s_pTextureWalking);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/menu003.png",
		&s_pTextureRotation);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/menu001.png",
		&s_pTextureStop);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/menu002.png",
		&s_pTextureSlope);

	float fWidth = TITLE_WIDTH * 0.5f;
	float fHeight = TITLE_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(-210.0f, 0.0f, -90.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureWalking);

	// 枠の設定
	SetFrame(pos, fWidth, fHeight, &s_pTextureWalking);

	pos.x = -70.0f;

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureRotation);

	// 枠の設定
	SetFrame(pos, fWidth, fHeight, &s_pTextureRotation);

	pos.x = 70.0f;

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureStop);

	// 枠の設定
	SetFrame(pos, fWidth, fHeight, &s_pTextureStop);

	pos.x = 210.0f;

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureSlope);

	// 枠の設定
	SetFrame(pos, fWidth, fHeight, &s_pTextureSlope);
}

//--------------------------------------------------
// タイトルの説明
//--------------------------------------------------
static void TitleRule(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title000.png",
		&s_pTextureRule);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title001.png",
		&s_pTextureMove);

	float fWidth = RULE_WIDTH * 0.5f;
	float fHeight = RULE_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(-210.0f, 0.0f, 100.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureRule);

	if (GetTitleCnt() >= REMIX_OK)
	{// リミックス出来る
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			"data/TEXTURE/Remix.png",
			&s_pTextureRemix);

		fWidth = REMIX_WIDTH * 0.5f;
		fHeight = REMIX_HEIGHT * 0.5f;

		pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);

		// ビルボードの設定
		SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureRemix);

		// 枠の設定
		SetFrame(pos, fWidth, fHeight, &s_pTextureRemix);
	}

	fWidth = MOVE_WIDTH * 0.5f;
	fHeight = MOVE_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(200.0f, 0.0f, 50.0f);
	
	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureMove);
}

//--------------------------------------------------
// タイトルロゴ
//--------------------------------------------------
static void TitleLogo(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title002.png",
		&s_pTextureMizu);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title003.png",
		&s_pTextureParty);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title004.png",
		&s_pTextureLogo);

	float fWidth = MIZU_WIDTH * 0.5f;
	float fHeight = MIZU_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(-30.0f, 0.0f, 130.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureMizu);

	fWidth = PARTY_WIDTH * 0.5f;
	fHeight = PARTY_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(140.0f, 0.0f, 130.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureParty);

	fWidth = LOGO_WIDTH * 0.5f;
	fHeight = LOGO_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(275.0f, 0.0f, 125.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureLogo);
}
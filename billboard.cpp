//==================================================
// 
// 3Dゲーム制作 ( billboard.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "billboard.h"
#include "main.h"
#include "setup.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define FILE_NAME		"data\\TXT\\billboard.txt"		//ファイルの名前
#define MAX_WALL				(256)					//ビルボードの最大数
#define MAX_TEXTURE				(256)					//テクスチャの最大数

//--------------------------------------------------
// 構造体を定義
//--------------------------------------------------

/*↓ ビルボード ↓*/

typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	D3DXMATRIX				mtxWorld;		// ワールドマトリックス
	bool					bUse;			// 使用しているかどうか
	LPDIRECT3DTEXTURE9		pTexture;		// テクスチャ
}Billboard;

/*↓ 読み込む内容 ↓*/

typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	float					fWidth;			// 幅
	float					fHeight;		// 高さ
	int						nIdxTex;		// テクスチャ番号
	LPDIRECT3DTEXTURE9		pTexture;		// テクスチャ
}Text;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			*s_pTexture;				// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファのポインタ
static Billboard					s_billboard[MAX_WALL];		// ビルボードの情報
static int							s_nUseTex;					// テクスチャの使用数

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitBillboard(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//メモリのクリア
	memset(s_billboard, NULL, sizeof(s_billboard));

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_WALL; i++)
	{
		//全ての初期化処理
		InitAll3D(pVtx);

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

		delete[](s_pTexture);
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateBillboard(void)
{
	
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawBillboard(void)
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

	for (int i = 0; i < MAX_WALL; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (!pBillboard->bUse)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pBillboard->mtxWorld);
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラの逆行列を設定
		pBillboard->mtxWorld._11 = mtxView._11;
		pBillboard->mtxWorld._12 = mtxView._21;
		pBillboard->mtxWorld._13 = mtxView._31;
		pBillboard->mtxWorld._21 = mtxView._12;
		pBillboard->mtxWorld._22 = mtxView._22;
		pBillboard->mtxWorld._23 = mtxView._32;
		pBillboard->mtxWorld._31 = mtxView._13;
		pBillboard->mtxWorld._32 = mtxView._23;
		pBillboard->mtxWorld._33 = mtxView._33;

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
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight, LPDIRECT3DTEXTURE9 *pTexture)
{
	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	for (int i = 0; i < MAX_WALL; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (pBillboard->bUse)
		{//使用されている
			continue;
		}

		/*↓ 使用されていない ↓*/

		pBillboard->pos = pos;
		pBillboard->pTexture = *pTexture;
		pBillboard->bUse = true;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		// 頂点座標の設定
		Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		// 頂点の法線の設定
		Setnor3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadBillboard(HWND hWnd)
{
	FILE *pFile;			// ファイルポインタを宣言
	int nUseText = 0;		// テキストで読み込んだビルボードの使用数

	// ファイルを開く
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		// 使用数の読み込み
		fscanf(pFile, "%d", &s_nUseTex);
		fscanf(pFile, "%d", &nUseText);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		MessageBox(hWnd, "テキストファイルの読み込みに失敗！\nエラー場所  : [ ビルボード ]", "警告！", MB_ICONWARNING);
		assert(false);
	}

	// txtに書いてる最大数分の読み込み用の配列を用意する
	Text *pText = new Text[nUseText];

	char aTexture[MAX_TEXTURE][1024];

	// ファイルを開く
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		// 使用数の読み込み
		fscanf(pFile, "%d", &s_nUseTex);
		fscanf(pFile, "%d", &nUseText);

		for (int i = 0; i < s_nUseTex; i++)
		{//テクスチャ名の読み込み
			fscanf(pFile, "%s", aTexture[i]);
		}

		for (int i = 0; i < nUseText; i++)
		{//テクスチャの番号・幅・高さ・位置の読み込み
			fscanf(pFile, "%d", &pText[i].nIdxTex);
			fscanf(pFile, "%f", &pText[i].fWidth);
			fscanf(pFile, "%f", &pText[i].fHeight);
			fscanf(pFile, "%f", &pText[i].pos.x);
			fscanf(pFile, "%f", &pText[i].pos.y);
			fscanf(pFile, "%f", &pText[i].pos.z);
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		MessageBox(hWnd, "テキストファイルの読み込みに失敗！\nエラー場所  : [ ビルボード ]", "警告！", MB_ICONWARNING);
		assert(false);
	}

	for (int i = 0; i < nUseText; i++)
	{
		if (pText[i].nIdxTex >= s_nUseTex)
		{// 該当しないテクスチャ番号
			MessageBox(hWnd, "該当しないテクスチャ番号です！\nエラー場所  : [ ビルボード ]", "警告！", MB_ICONWARNING);
			assert(false);
		}
	}

	// txtに書いてる最大数分のテクスチャの配列を用意する
	s_pTexture = new LPDIRECT3DTEXTURE9[s_nUseTex];

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < s_nUseTex; i++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			&aTexture[i][0],
			&s_pTexture[i]);
	}

	for (int i = 0; i < nUseText; i++)
	{
		pText[i].pTexture = s_pTexture[pText[i].nIdxTex];

		// 設定
		SetBillboard(pText[i].pos, pText[i].fWidth, pText[i].fHeight, &pText[i].pTexture);
	}

	delete[](pText);
}
//==================================================
// 
// 3Dゲーム制作 ( locus.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "input.h"
#include "locus.h"
#include "setup.h"
#include "wall.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_WIDTH		(150.0f)		// 幅の最大値
#define MAX_HEIGHT		(0.0f)			// 高さの最大値
#define MAX_DEPTH		(150.0f)		// 奥行きの最大値
#define MAX_SIZE		(10)			// サイズの最大値
#define MIN_SIZE		(1)				// サイズの最小値
#define START_SIZE		(3)				// サイズの最初の値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static LPDIRECT3DINDEXBUFFER9		s_pIdxBuff = NULL;		// インデックスバッファへのポインタ
static Locus						s_locus;				// メッシュフィールドの情報
static int							s_nHorizontal;			// 横
static int							s_nVertical;			// 縦
static int							s_nVtx;					// 頂点数
static int							s_nIdx;					// インデックス数
static int							s_nPolygon;				// ポリゴン数

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitLocus(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko000.jpg",
		&s_pTexture);

	// メモリのクリア
	memset(&s_locus, 0, sizeof(s_locus));

	// 横・縦の初期化
	s_nHorizontal = START_SIZE;
	s_nVertical = START_SIZE;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitLocus(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの解放
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pIdxBuff != NULL)
	{// インデックスバッファの解放
		s_pIdxBuff->Release();
		s_pIdxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateLocus(void)
{
	
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawLocus(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_locus.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_locus.rot.y, s_locus.rot.x, s_locus.rot.z);
	D3DXMatrixMultiply(&s_locus.mtxWorld, &s_locus.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_locus.pos.x, s_locus.pos.y, s_locus.pos.z);
	D3DXMatrixMultiply(&s_locus.mtxWorld, &s_locus.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_locus.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(s_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	// ポリゴン描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点バッファ
		0,							// インデックスの最小値
		s_nVtx,						// 頂点数
		0,							// 描画する最初の頂点インデックス
		s_nPolygon);				// プリミティブ(ポリゴン)数

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetLocus(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nXLine = s_nHorizontal + 1;
	int nZLine = s_nVertical + 1;

	// 頂点数を計算
	s_nVtx = nXLine * nZLine;

	// インデックス数を計算
	s_nIdx = ((nXLine * 2) * s_nVertical) + ((s_nVertical - 1) * 2);

	// ポリゴン数を計算
	s_nPolygon = (s_nHorizontal * s_nVertical * 2) + ((s_nVertical - 1) * 4);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * s_nVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// メモリのクリア
	memset(&s_locus, 0, sizeof(s_locus));

	// 幅・高さ・奥行きの設定
	s_locus.fWidth = MAX_WIDTH * (s_nHorizontal * 0.5f);
	s_locus.fHeight = MAX_HEIGHT;
	s_locus.fDepth = MAX_DEPTH * (s_nVertical * 0.5f);

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < s_nVtx; i++)
	{
		float fXPos = (float)(i % nXLine) - (s_nHorizontal * 0.5f);
		float fZPos = ((float)(i / nXLine) - (s_nVertical * 0.5f)) * -1.0f;

		// 頂点座標の設定
		pVtx[i].pos = D3DXVECTOR3(MAX_WIDTH * fXPos, 0.0f, MAX_DEPTH * fZPos);

		// 各頂点の法線の設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		float fUTex = (float)(i % nXLine);
		float fVTex = (float)(i / nXLine);

		// テクスチャ座標の設定
		pVtx[i].tex = D3DXVECTOR2(fUTex, fVTex);
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(VERTEX_3D) * s_nIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		NULL);

	WORD *pIdx = NULL;		// インデックス情報へのポインタ

	// インデックスバッファをロック
	s_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	for (int x = 0, z = 0; z < s_nVertical; x++, z++)
	{
		for (; x < (nXLine * (z + 1)) + z; x++)
		{
			pIdx[x * 2] = (WORD)(x - z + nXLine);
			pIdx[(x * 2) + 1] = (WORD)(x - z);
		}

		if (z < s_nVertical - 1)
		{// これで終わりじゃないなら
			pIdx[x * 2] = (WORD)(x - (z + 1));
			pIdx[(x * 2) + 1] = (WORD)((x * 2) - (z * (s_nHorizontal + 3)));
		}
	}

	// インデックスバッファをアンロックする
	s_pIdxBuff->Unlock();
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Locus *GetLocus(void)
{
	return &s_locus;
}

//==================================================
// 
// 3Dゲーム制作 ( meshfield.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "input.h"
#include "main.h"
#include "meshfield.h"
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
static MeshField					s_meshfield;			// メッシュフィールドの情報
static int							s_nMeshFieldSize;		// メッシュフィールドのサイズ
static int							s_nVtxNumber;			// 頂点数

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void ResetBuff(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMeshField(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko000.jpg",
		&s_pTexture);

	s_nMeshFieldSize = START_SIZE;

	// メモリのクリア
	memset(&s_meshfield, NULL, sizeof(s_meshfield));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMeshField(void)
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
void UpdateMeshField(void)
{
	if (GetKeyboardTrigger(DIK_V))
	{// Vキーが押された
		s_nMeshFieldSize++;
	}
	else if (GetKeyboardTrigger(DIK_B))
	{// Bキーが押された
		s_nMeshFieldSize--;
	}

	if (GetKeyboardTrigger(DIK_V) || GetKeyboardTrigger(DIK_B))
	{// V,Bキーが押された

		if (s_nMeshFieldSize >= MAX_SIZE)
		{// 指定の数以上
			s_nMeshFieldSize = MAX_SIZE;
		}
		else if (s_nMeshFieldSize <= MIN_SIZE)
		{// 指定の数以下
			s_nMeshFieldSize = MIN_SIZE;
		}

		// バッファのリセット
		ResetBuff();

		// 設定
		SetMeshField();

		// 壁のリセット
		ResetWall();

		// 壁の設置
		InstallationWall();
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMeshField(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_meshfield.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_meshfield.rot.y, s_meshfield.rot.x, s_meshfield.rot.z);
	D3DXMatrixMultiply(&s_meshfield.mtxWorld, &s_meshfield.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_meshfield.pos.x, s_meshfield.pos.y, s_meshfield.pos.z);
	D3DXMatrixMultiply(&s_meshfield.mtxWorld, &s_meshfield.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_meshfield.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(s_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	int nPolygonNumber = (s_nVtxNumber * 2) - 6;

	// ポリゴン描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点バッファ
		0,							// インデックスの最小値
		s_nVtxNumber,				// 頂点数
		0,							// 描画する最初の頂点インデックス
		nPolygonNumber);			// プリミティブ(ポリゴン)数

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetMeshField(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点数を計算
	s_nVtxNumber = (int)pow(s_nMeshFieldSize + 1, 2.0f);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * s_nVtxNumber,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// メモリのクリア
	memset(&s_meshfield, NULL, sizeof(s_meshfield));

	// 幅・高さ・奥行きの設定
	s_meshfield.fWidth = MAX_WIDTH * (s_nMeshFieldSize * 0.5f);
	s_meshfield.fHeight = MAX_HEIGHT;
	s_meshfield.fDepth = MAX_DEPTH * (s_nMeshFieldSize * 0.5f);

	int nWrapping = s_nMeshFieldSize + 1;

	for (int i = 0; i < s_nVtxNumber; i++)
	{
		float fXPos = (float)(i % nWrapping) - (s_nMeshFieldSize * 0.5f);
		float fZPos = ((float)(i / nWrapping) - (s_nMeshFieldSize * 0.5f)) * -1.0f;

		// 頂点座標の設定
		pVtx[i].pos = D3DXVECTOR3(MAX_WIDTH * fXPos, 0.0f, MAX_DEPTH * fZPos);

		// 各頂点の法線の設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		float fUTex = (float)(i % nWrapping);
		float fVTex = (float)(i / nWrapping);

		// テクスチャ座標の設定
		pVtx[i].tex = D3DXVECTOR2(fUTex, fVTex);
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// インデックス数を計算
	int nPolygonNumber = (s_nVtxNumber * 2) - 6;
	int nIdxNumber = nPolygonNumber + 2;

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(VERTEX_3D) * nIdxNumber,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		NULL);

	WORD *pIdx = NULL;		// インデックス情報へのポインタ

	// インデックスバッファをロック
	s_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	for (int x = 0, z = 0; z < s_nMeshFieldSize; x++, z++)
	{
		for (; x < (nWrapping * (z + 1)) + z; x++)
		{
			pIdx[x * 2] = (WORD)x - (WORD)z + (WORD)nWrapping;
			pIdx[(x * 2) + 1] = (WORD)x - (WORD)z;
		}

		if (z < s_nMeshFieldSize - 1)
		{// これで終わりじゃないなら
			int nData = (x % nWrapping) * nWrapping;

			pIdx[x * 2] = (WORD)nData + (WORD)s_nMeshFieldSize;
			pIdx[(x * 2) + 1] = (WORD)nData + (WORD)(nWrapping * 2);
		}
	}

	// インデックスバッファをアンロックする
	s_pIdxBuff ->Unlock();
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
MeshField *GetMeshField(void)
{
	return &s_meshfield;
}

//--------------------------------------------------
// バッファのリセット
//--------------------------------------------------
static void ResetBuff(void)
{
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
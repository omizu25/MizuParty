//==================================================
// 
// 3Dゲーム制作 ( mesh_sky.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "input.h"
#include "main.h"
#include "mesh_sky.h"
#include "setup.h"
#include "wall.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_SIZE				(1000.0f)		// サイズの最大値
#define MAX_HORIZONTAL			(30)			// 横の最大値
#define MIN_HORIZONTAL			(5)				// 横の最小値
#define START_HORIZONTAL		(8)				// 横の最初の値
#define MAX_VERTICAL			(30)			// 縦の最大値
#define MIN_VERTICAL			(5)				// 縦の最小値
#define START_VERTICAL			(10)			// 縦の最初の値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffCone = NULL;		// 円錐の頂点バッファへのポインタ
static LPDIRECT3DINDEXBUFFER9		s_pIdxBuff = NULL;			// インデックスバッファへのポインタ
static MeshSky						s_meshSky;					// メッシュ空の情報
static MeshSkyNumber				s_Number;					// メッシュ空の数系の情報

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void Input(void);
static void ResetBuff(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMeshSky(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko003.jpg",
		&s_pTexture);

	// メモリのクリア
	memset(&s_meshSky, NULL, sizeof(s_meshSky));
	memset(&s_Number, NULL, sizeof(s_Number));

	// 横・縦の初期化
	s_Number.nHorizontal = START_HORIZONTAL;
	s_Number.nVertical = START_VERTICAL;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMeshSky(void)
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

	if (s_pVtxBuffCone != NULL)
	{// 円錐の頂点バッファの解放
		s_pVtxBuffCone->Release();
		s_pVtxBuffCone = NULL;
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
void UpdateMeshSky(void)
{
	// 入力
	Input();

	if (GetKeyboardTrigger(DIK_V) || GetKeyboardTrigger(DIK_B) ||
		GetKeyboardTrigger(DIK_N) || GetKeyboardTrigger(DIK_M))
	{// V, B, N, Mキーが押された

		// 指定の値以上・以下
		Specified(&s_Number.nHorizontal, MAX_HORIZONTAL, MIN_HORIZONTAL);

		// 指定の値以上・以下
		Specified(&s_Number.nVertical, MAX_VERTICAL, MIN_VERTICAL);

		// バッファのリセット
		ResetBuff();

		// 設定
		SetMeshSky();

		// 壁のリセット
		ResetWall();

		// 壁の設置
		InstallationWall();
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMeshSky(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_meshSky.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_meshSky.rot.y, s_meshSky.rot.x, s_meshSky.rot.z);
	D3DXMatrixMultiply(&s_meshSky.mtxWorld, &s_meshSky.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_meshSky.pos.x, s_meshSky.pos.y, s_meshSky.pos.z);
	D3DXMatrixMultiply(&s_meshSky.mtxWorld, &s_meshSky.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_meshSky.mtxWorld);

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
		s_Number.nVtx,				// 頂点数
		0,							// 描画する最初の頂点インデックス
		s_Number.nPolygon);			// プリミティブ(ポリゴン)数

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffCone, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLEFAN,			// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		s_Number.nHorizontal);		// プリミティブ(ポリゴン)数
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetMeshSky(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nXLine = s_Number.nHorizontal + 1;
	int nYLine = s_Number.nVertical + 1;

	// 頂点数を計算
	s_Number.nVtx = nXLine * nYLine;

	// インデックス数を計算
	s_Number.nIdx = ((nXLine * 2) * s_Number.nVertical) + ((s_Number.nVertical - 1) * 2);

	// ポリゴン数を計算
	s_Number.nPolygon = (s_Number.nHorizontal * s_Number.nVertical * 2) + ((s_Number.nVertical - 1) * 4);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * s_Number.nVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// メモリのクリア
	memset(&s_meshSky, NULL, sizeof(s_meshSky));

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int y = 0; y < nYLine; y++)
	{
		float fYRot = (((D3DX_PI * 0.25f) / s_Number.nVertical) * y) + (D3DX_PI * 0.25f);

		float fYPos = cosf(fYRot) * MAX_SIZE;

		for (int x = 0; x < nXLine; x++)
		{
			float fRot = ((D3DX_PI * 2.0f) / s_Number.nHorizontal) * x;

			// 角度の正規化
			NormalizeRot(&fRot);

			float fXPos = sinf(fRot) * sinf(fYRot) * MAX_SIZE;
			float fZPos = cosf(fRot) * sinf(fYRot) * MAX_SIZE;
			D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

			// 頂点座標の設定
			pVtx[x + (y * nXLine)].pos = pos;

			D3DXVECTOR3 vec;

			// 正規化する ( 大きさ 1 のベクトルにする )
			D3DXVec3Normalize(&vec, &pos);

			// 各頂点の法線の設定
			pVtx[x + (y * nXLine)].nor = vec;

			// 頂点カラーの設定
			pVtx[x + (y * nXLine)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			float fVTex = (1.0f / s_Number.nVertical) * y;

			// テクスチャ座標の設定
			pVtx[x + (y * nXLine)].tex = D3DXVECTOR2((float)x, fVTex);
		}
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(VERTEX_3D) * s_Number.nIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		NULL);

	WORD *pIdx = NULL;		// インデックス情報へのポインタ
			
	// インデックスバッファをロック
	s_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	for (int x = 0, y = 0; y < s_Number.nVertical; x++, y++)
	{
		for (; x < (nXLine * (y + 1)) + y; x++)
		{
			pIdx[x * 2] = (WORD)(x - y + nXLine);
			pIdx[(x * 2) + 1] = (WORD)(x - y);
			x = x;
		}

		if (y < s_Number.nVertical - 1)
		{// これで終わりじゃないなら
			pIdx[x * 2] = (WORD)(x - (y + 1));
			pIdx[(x * 2) + 1] = (WORD)((x * 2) - (y * (s_Number.nHorizontal + 3)));
			x = x;
		}
	}

	// インデックスバッファをアンロックする
	s_pIdxBuff->Unlock();

	// 円錐の頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * (s_Number.nHorizontal + 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffCone,
		NULL);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffCone->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < nXLine; i++)
	{
		float fYRot = D3DX_PI * 0.25f;
		float fRot = ((D3DX_PI * 2.0f) / s_Number.nHorizontal) * i;

		// 角度の正規化
		NormalizeRot(&fRot);

		float fXPos = sinf(-fRot) * sinf(fYRot) * MAX_SIZE;
		float fYPos = cosf(fYRot) * MAX_SIZE;
		float fZPos = cosf(-fRot) * sinf(fYRot) * MAX_SIZE;
		D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

		// 頂点座標の設定
		pVtx[i + 1].pos = pos;

		D3DXVECTOR3 vec;

		// 正規化する ( 大きさ 1 のベクトルにする )
		D3DXVec3Normalize(&vec, &pos);

		// 各頂点の法線の設定
		pVtx[i + 1].nor = vec;

		// 頂点カラーの設定
		pVtx[i + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[i + 1].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	float fYRot = ((D3DX_PI * 0.25f) / s_Number.nVertical);

	float fYPos = cosf(fYRot) * MAX_SIZE;

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, fYPos, 0.0f);

	// 頂点座標の設定
	pVtx[0].pos = pos;

	D3DXVECTOR3 vec;

	// 正規化する ( 大きさ 1 のベクトルにする )
	D3DXVec3Normalize(&vec, &pos);

	// 各頂点の法線の設定
	pVtx[0].nor = vec;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	// 頂点バッファをアンロックする
	s_pVtxBuffCone->Unlock();
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
MeshSky *GetMeshSky(void)
{
	return &s_meshSky;
}

//--------------------------------------------------
// 数系の取得
//--------------------------------------------------
MeshSkyNumber *GetMeshSkyNumber(void)
{
	return &s_Number;
}

//--------------------------------------------------
// 入力
//--------------------------------------------------
static void Input(void)
{
	if (GetDebug() == DEBUG_MESH)
	{// デバッグ表示がメッシュの時
		if (GetKeyboardTrigger(DIK_V))
		{// Vキーが押された
			s_Number.nHorizontal++;
		}
		else if (GetKeyboardTrigger(DIK_B))
		{// Bキーが押された
			s_Number.nHorizontal--;
		}

		if (GetKeyboardTrigger(DIK_N))
		{// Nキーが押された
			s_Number.nVertical++;
		}
		else if (GetKeyboardTrigger(DIK_M))
		{// Mキーが押された
			s_Number.nVertical--;
		}
	}
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

	if (s_pVtxBuffCone != NULL)
	{// 円錐の頂点バッファの解放
		s_pVtxBuffCone->Release();
		s_pVtxBuffCone = NULL;
	}
}
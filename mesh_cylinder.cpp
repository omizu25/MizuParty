//==================================================
// 
// 3Dゲーム制作 ( mesh_cylinder.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "input.h"
#include "main.h"
#include "mesh_cylinder.h"
#include "setup.h"
#include "wall.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_WIDTH				(50.0f)		// 幅の最大値
#define MAX_HEIGHT				(10.0f)		// 高さの最大値
#define MAX_DEPTH				(50.0f)		// 奥行きの最大値
#define MAX_HORIZONTAL			(30)		// 横の最大値
#define MIN_HORIZONTAL			(3)			// 横の最小値
#define START_HORIZONTAL		(8)			// 横の最初の値
#define MAX_VERTICAL			(10)		// 縦の最大値
#define MIN_VERTICAL			(1)			// 縦の最小値
#define START_VERTICAL			(8)			// 縦の最初の値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static LPDIRECT3DINDEXBUFFER9		s_pIdxBuff = NULL;		// インデックスバッファへのポインタ
static MeshCylinder					s_meshCylinder;			// メッシュ円柱の情報
static MeshCylinderNumber			s_Number;				// メッシュ円柱の数系の情報

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void ResetBuff(void);
static void Specified(int *pNumber, int nMax, int nMin);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMeshCylinder(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko003.jpg",
		&s_pTexture);

	// メモリのクリア
	memset(&s_meshCylinder, NULL, sizeof(s_meshCylinder));
	memset(&s_Number, NULL, sizeof(s_Number));

	//横・縦の初期化
	s_Number.nHorizontal = START_HORIZONTAL;
	s_Number.nVertical = START_VERTICAL;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMeshCylinder(void)
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
void UpdateMeshCylinder(void)
{
	if (GetKeyboardTrigger(DIK_1))
	{// 1キーが押された
		s_Number.nHorizontal++;
	}
	else if (GetKeyboardTrigger(DIK_2))
	{// 2キーが押された
		s_Number.nHorizontal--;
	}

	if (GetKeyboardTrigger(DIK_3))
	{// 3キーが押された
		s_Number.nVertical++;
	}
	else if (GetKeyboardTrigger(DIK_4))
	{// 4キーが押された
		s_Number.nVertical--;
	}

	if (GetKeyboardTrigger(DIK_1) || GetKeyboardTrigger(DIK_2) ||
		GetKeyboardTrigger(DIK_3) || GetKeyboardTrigger(DIK_4))
	{// 1, 2, 3, 4キーが押された

		// 指定の値以上・以下
		Specified(&s_Number.nHorizontal, MAX_HORIZONTAL, MIN_HORIZONTAL);

		// 指定の値以上・以下
		Specified(&s_Number.nVertical, MAX_VERTICAL, MIN_VERTICAL);

		// バッファのリセット
		ResetBuff();

		// 設定
		SetMeshCylinder();

		// 壁のリセット
		ResetWall();

		// 壁の設置
		InstallationWall();
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMeshCylinder(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// レンダーステートの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_meshCylinder.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_meshCylinder.rot.y, s_meshCylinder.rot.x, s_meshCylinder.rot.z);
	D3DXMatrixMultiply(&s_meshCylinder.mtxWorld, &s_meshCylinder.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_meshCylinder.pos.x, s_meshCylinder.pos.y, s_meshCylinder.pos.z);
	D3DXMatrixMultiply(&s_meshCylinder.mtxWorld, &s_meshCylinder.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_meshCylinder.mtxWorld);

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

	// 元に戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetMeshCylinder(void)
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
	memset(&s_meshCylinder, NULL, sizeof(s_meshCylinder));

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int y = 0; y < nYLine; y++)
	{
		float fYPos = (MAX_HEIGHT * s_Number.nVertical) - (MAX_HEIGHT * y);

		for (int x = 0; x < nXLine; x++)
		{
			float fRot = (D3DX_PI * 2.0f) / s_Number.nHorizontal * x;

			// 角度の正規化
			NormalizeRot(&fRot);

			float fXPos = cosf(fRot) * MAX_WIDTH;
			float fZPos = sinf(fRot) * MAX_DEPTH;
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

			float fUTex = (float)((x + (y * nXLine)) % nXLine);
			float fVTex = (1.0f / s_Number.nVertical) * y;

			// テクスチャ座標の設定
			pVtx[x + (y * nXLine)].tex = D3DXVECTOR2(fUTex, fVTex);
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
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
MeshCylinder *GetMeshCylinder(void)
{
	return &s_meshCylinder;
}

//--------------------------------------------------
// 数系の取得
//--------------------------------------------------
MeshCylinderNumber *GetMeshCylinderNumber(void)
{
	return &s_Number;
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

//--------------------------------------------------
// 指定の値以上・以下
//--------------------------------------------------
static void Specified(int *pNumber, int nMax, int nMin)
{
	if (*pNumber >= nMax)
	{// 指定の値以上
		*pNumber = nMax;
	}
	else if (*pNumber <= nMin)
	{// 指定の値以下
		*pNumber = nMin;
	}
}
//==================================================
// 
// 3Dゲーム制作 ( mesh_cylinder.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "input.h"
#include "mesh_cylinder.h"
#include "player.h"
#include "setup.h"
#include "wall.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_HEIGHT				(50.0f)			// 高さの最大値
#define START_POS_Z				(-35.0f)		// Zの位置の最初の値
#define START_HORIZONTAL		(50)			// 横の最初の値
#define START_VERTICAL			(1)				// 縦の最初の値
#define MOVE_X					(5.0f)			// Xの移動量
#define SIZE_RATIO				(0.01f)			// サイズの比率

//--------------------------------------------------
// 構造体
//--------------------------------------------------

/*↓ メッシュ ↓*/

typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	float			fSize;			// サイズ
}Mesh;

/*↓ 数系 ↓*/

typedef struct
{
	int		nHorizontal;		// 横
	int		nVertical;			// 縦
	int		nVtx;				// 頂点数
	int		nIdx;				// インデックス数
	int		nPolygon;			// ポリゴン数
}Number;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファへのポインタ
static LPDIRECT3DINDEXBUFFER9		s_pIdxBuff = NULL;			// インデックスバッファへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureLid = NULL;		// ふたのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffLid = NULL;		// ふたの頂点バッファへのポインタ
static Mesh							s_mesh;						// メッシュの情報
static Number						s_Number;					// 数系の情報
static float						s_fStartSize;				// 始まりのサイズ

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
		"data\\TEXTURE\\lid.png",
		&s_pTextureLid);

	// メモリのクリア
	memset(&s_mesh, 0, sizeof(s_mesh));
	memset(&s_Number, 0, sizeof(s_Number));

	//横・縦の初期化
	s_Number.nHorizontal = START_HORIZONTAL;
	s_Number.nVertical = START_VERTICAL;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMeshCylinder(void)
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

	if (s_pTextureLid != NULL)
	{// テクスチャの解放
		s_pTextureLid->Release();
		s_pTextureLid = NULL;
	}

	if (s_pVtxBuffLid != NULL)
	{// 頂点バッファの解放
		s_pVtxBuffLid->Release();
		s_pVtxBuffLid = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateMeshCylinder(void)
{
	if (s_mesh.fSize > 0.0f)
	{// サイズが指定の値より大きい
		s_mesh.pos.x = GetCamera(0)->posV.x;

		int nXLine = s_Number.nHorizontal + 1;
		int nYLine = s_Number.nVertical + 1;

		float fPosDiff = GetPlayer()->pos.x - s_mesh.pos.x;

		if (fPosDiff < 0.0f)
		{// マイナス
			fPosDiff *= -1.0f;
		}
		
		s_mesh.fSize = sqrtf(fPosDiff);
		s_mesh.rot.z = s_mesh.fSize;

		if (s_mesh.fSize <= 0.1f)
		{// 指定の値以下
			s_mesh.fSize = 0.0f;
		}

		s_mesh.pos.y = s_mesh.fSize;

		VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int y = 0; y < nYLine; y++)
		{
			float fYPos = MAX_HEIGHT * y;

			for (int x = 0; x < nXLine; x++)
			{
				int nVtx = x + (y * nXLine);
				float fRot = (D3DX_PI * 2.0f) / s_Number.nHorizontal * x;

				// 角度の正規化
				NormalizeRot(&fRot);

				float fXPos = cosf(fRot) * s_mesh.fSize;
				float fZPos = sinf(fRot) * s_mesh.fSize;
				D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

				// 頂点座標の設定
				pVtx[nVtx].pos = pos;

				D3DXVECTOR3 vec;

				// 正規化する ( 大きさ 1 のベクトルにする )
				D3DXVec3Normalize(&vec, &pos);

				// 各頂点の法線の設定
				pVtx[nVtx].nor = vec;
			}
		}

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffLid->Lock(0, 0, (void**)&pVtx, 0);

		float fScaleUV = s_mesh.fSize / s_fStartSize;

		for (int i = 1; i <= nXLine; i++)
		{
			float fRot = ((D3DX_PI * 2.0f) / s_Number.nHorizontal) * i;

			// 角度の正規化
			NormalizeRot(&fRot);

			float fXPos = cosf(fRot) * s_mesh.fSize;
			float fYPos = sinf(fRot) * s_mesh.fSize;

			// 頂点座標の設定
			pVtx[i].pos = D3DXVECTOR3(fXPos, MAX_HEIGHT, fYPos);

			float fTexU = cosf(fRot)/* * 0.5f */ * fScaleUV + 0.5f;
			float fTexV = sinf(fRot)/* * 0.5f */ * fScaleUV + 0.5f;

			// テクスチャ座標の設定
			pVtx[i].tex = D3DXVECTOR2(fTexU, fTexV);
		}

		// 頂点バッファをアンロックする
		s_pVtxBuffLid->Unlock();
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
	D3DXMatrixIdentity(&s_mesh.mtxWorld);

	// Xの向きを反映
	D3DXMatrixRotationX(&mtxRot, s_mesh.rot.x);
	D3DXMatrixMultiply(&s_mesh.mtxWorld, &s_mesh.mtxWorld, &mtxRot);

	// Zの向きを反映
	D3DXMatrixRotationZ(&mtxRot, s_mesh.rot.z);
	D3DXMatrixMultiply(&s_mesh.mtxWorld, &s_mesh.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_mesh.pos.x, s_mesh.pos.y, s_mesh.pos.z);
	D3DXMatrixMultiply(&s_mesh.mtxWorld, &s_mesh.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_mesh.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(s_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴン描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点バッファ
		0,							// インデックスの最小値
		s_Number.nVtx,				// 頂点数
		0,							// 描画する最初の頂点インデックス
		s_Number.nPolygon);			// プリミティブ(ポリゴン)数

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffLid, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureLid);

	// テクスチャのU, Vの繰り返し方を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLEFAN,			// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		s_Number.nHorizontal);		// プリミティブ(ポリゴン)数

	// テクスチャのU, Vの繰り返し方を元に戻す
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

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
	memset(&s_mesh, 0, sizeof(s_mesh));

	float fPosDiff = GetPlayer()->pos.x - s_mesh.pos.x;

	if (fPosDiff < 0.0f)
	{// マイナス
		fPosDiff *= -1.0f;
	}

	s_mesh.fSize = sqrtf(fPosDiff);

	if (s_mesh.fSize < 0.0f)
	{// マイナス
		s_mesh.fSize *= -1.0f;
	}

	s_fStartSize = s_mesh.fSize;

	s_mesh.pos = D3DXVECTOR3(0.0f, s_mesh.fSize, START_POS_Z);
	s_mesh.rot = D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f);

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int y = 0; y < nYLine; y++)
	{
		float fYPos = MAX_HEIGHT * y;

		for (int x = 0; x < nXLine; x++)
		{
			int nVtx = x + (y * nXLine);
			float fRot = (D3DX_PI * 2.0f) / s_Number.nHorizontal * x;

			// 角度の正規化
			NormalizeRot(&fRot);

			float fXPos = cosf(fRot) * s_mesh.fSize;
			float fZPos = sinf(fRot) * s_mesh.fSize;
			
			// 頂点座標の設定
			pVtx[nVtx].pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

			D3DXVECTOR3 vec;

			// 正規化する ( 大きさ 1 のベクトルにする )
			D3DXVec3Normalize(&vec, &pVtx[x + (y * nXLine)].pos);

			// 各頂点の法線の設定
			pVtx[nVtx].nor = vec;

			// 頂点カラーの設定
			pVtx[nVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			float fVTex = (1.0f / s_Number.nVertical) * y;

			// テクスチャ座標の設定
			pVtx[nVtx].tex = D3DXVECTOR2((float)x, fVTex);
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
		&s_pVtxBuffLid,
		NULL);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffLid->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, MAX_HEIGHT, 0.0f);

	// 各頂点の法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.5f, 0.5f);

	for (int i = 1; i <= nXLine; i++)
	{
		float fRot = ((D3DX_PI * 2.0f) / s_Number.nHorizontal) * i;

		// 角度の正規化
		NormalizeRot(&fRot);

		float fXPos = cosf(fRot) * s_mesh.fSize;
		float fYPos = sinf(fRot) * s_mesh.fSize;

		// 頂点座標の設定
		pVtx[i].pos = D3DXVECTOR3(fXPos, MAX_HEIGHT, fYPos);

		// 各頂点の法線の設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		float fTexU = (cosf(fRot) + 1.0f)/* * 0.5f */;
		float fTexV = (sinf(fRot) + 1.0f)/* * 0.5f */;

		// テクスチャ座標の設定
		pVtx[i].tex = D3DXVECTOR2(fTexU, fTexV);
	}

	// 頂点バッファをアンロックする
	s_pVtxBuffLid->Unlock();
}

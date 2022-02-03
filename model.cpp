//==================================================
// 
// 3Dゲーム制作 ( model.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "input.h"
#include "line.h"
#include "model.h"
#include "setup.h"
#include "shadow.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define START_POS_Y		(400.0f)		// スタートの高さ	
#define MAX_MOVE		(3.0f)			// 移動量

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Model		s_model;		// モデルの情報
static int			s_nTime;		// タイム

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitModel(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nTime = 0;

	int nNumVtx;		// 頂点数
	DWORD SizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVexBuff;		// 頂点バッファへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data\\MODEL\\プリン.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_model.pBuffMat,
		NULL,
		&s_model.nNumMat,
		&s_model.pMesh);

	s_model.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_model.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// 頂点数を取得
	nNumVtx = s_model.pMesh->GetNumVertices();

	// フォーマットのサイズを取得
	SizeFVF = D3DXGetFVFVertexSize(s_model.pMesh->GetFVF());

	// 頂点バッファのロック
	s_model.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

	for (int i = 0; i < nNumVtx; i++)
	{
		// 頂点情報の代入
		D3DXVECTOR3 pos = *(D3DXVECTOR3*)pVexBuff;

		// 小さい・大きい [x]
		VtxSmallBig(&s_model.vtxMin.x, &s_model.vtxMax.x, pos.x);

		// 小さい・大きい [y]
		VtxSmallBig(&s_model.vtxMin.y, &s_model.vtxMax.y, pos.y);

		// 小さい・大きい [z]
		VtxSmallBig(&s_model.vtxMin.z, &s_model.vtxMax.z, pos.z);

		// 頂点フォーマットのサイズ分ポインタを進める
		pVexBuff += SizeFVF;
	}

	// 頂点バッファのアンロック
	s_model.pMesh->UnlockVertexBuffer();

	// メッシュに使用されているテクスチャ用の配列を用意する
	s_model.pTexture = new LPDIRECT3DTEXTURE9[s_model.nNumMat];

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_model.pBuffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)s_model.nNumMat; i++)
	{
		s_model.pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_model.pTexture[i]);
		}
		else
		{
			s_model.pTexture[i] = NULL;
		}
	}

	s_model.pos = D3DXVECTOR3(0.0f, START_POS_Y, 0.0f);
	s_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_model.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 影の設定
	s_model.nIdxShadow = SetShadow(s_model.pos, s_model.rot, s_model.vtxMax.x);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitModel(void)
{
	if (s_model.pTexture != NULL)
	{// テクスチャの解放
		for (int i = 0; i < (int)s_model.nNumMat; i++)
		{
			if (s_model.pTexture[i] != NULL)
			{
				s_model.pTexture[i]->Release();
				s_model.pTexture[i] = NULL;
			}
		}

		delete[](s_model.pTexture);
		s_model.pTexture = NULL;
	}

	if (s_model.pMesh != NULL)
	{// メッシュの解放
		s_model.pMesh->Release();
		s_model.pMesh = NULL;
	}

	if (s_model.pBuffMat != NULL)
	{// マテリアルの解放
		s_model.pBuffMat->Release();
		s_model.pBuffMat = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateModel(void)
{
	s_model.pos.y -= MAX_MOVE;
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawModel(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_model.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_model.rot.y, s_model.rot.x, s_model.rot.z);
	D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_model.pos.x, s_model.pos.y, s_model.pos.z);
	D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_model.mtxWorld);

	// 現在のマテリアル保持
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)s_model.pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)s_model.nNumMat; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_model.pTexture[i]);

		// モデルパーツの描画
		s_model.pMesh->DrawSubset(i);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Model *GetModel(void)
{
	return &s_model;
}

//--------------------------------------------------
// 当たり判定
//--------------------------------------------------
void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size)
{
	float fLeft = s_model.pos.x + s_model.vtxMin.x;
	float fRight = s_model.pos.x + s_model.vtxMax.x;
	float fBottom = s_model.pos.y + s_model.vtxMin.y;
	float fTop = s_model.pos.y + s_model.vtxMax.y;
	float fFront = s_model.pos.z + s_model.vtxMin.z;
	float fBack = s_model.pos.z + s_model.vtxMax.z;

	if ((pPos->x + size.x > fLeft) && (pPos->x - size.x < fRight) &&
		(pPos->y + size.y > fBottom) && (pPos->y < fTop))
	{// x, yが範囲内
		if ((pPosOld->z + size.z <= fFront) && (pPos->z + size.z > fFront))
		{// 前
			pPos->z = fFront - size.z;
		}
		else if ((pPosOld->z - size.z >= fBack) && (pPos->z - size.z < fBack))
		{// 後
			pPos->z = fBack + size.z;
		}
	}

	if ((pPos->z + size.z > fFront) && (pPos->z - size.z < fBack) &&
		(pPos->y + size.y > fBottom) && (pPos->y < fTop))
	{// z, yが範囲内
		if ((pPosOld->x + size.x <= fLeft) && (pPos->x + size.x > fLeft))
		{// 左
			pPos->x = fLeft - size.x;
		}
		else if ((pPosOld->x - size.x >= fRight) && (pPos->x - size.x < fRight))
		{// 右
			pPos->x = fRight + size.x;
		}
	}

	if ((pPos->x + size.x > fLeft) && (pPos->x - size.x < fRight) &&
		(pPos->z + size.z > fFront) && (pPos->z - size.z < fBack))
	{// x, zが範囲内
		if ((pPosOld->y + size.y <= fBottom) && (pPos->y + size.y > fBottom))
		{// 下
			pPos->y = fBottom - size.y;
		}
		if ((pPosOld->y >= fTop) && (pPos->y < fTop))
		{// 上
			pPos->y = fTop;
		}
	}
}

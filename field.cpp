//==================================================
// 
// 3Dゲーム制作 ( field.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "game.h"
#include "input.h"
#include "field.h"
#include "setup.h"
#include "shadow.h"
#include "particle.h"
#include "result.h"
#include "title.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define START_POS_Y		(300.0f)		// スタートの高さ
#define START_POS_Z		(-15.0f)		// スタートの奥行き
#define MAX_MOVE		(5.0f)			// 移動量
#define MAX_RANDOM		(650)			// ランダムの最大値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Field		s_field;		// モデルの情報
static int			s_nRand;		// ランダム

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitField(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;		// 頂点数
	DWORD SizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVexBuff;		// 頂点バッファへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data\\MODEL\\field.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_field.pBuffMat,
		NULL,
		&s_field.nNumMat,
		&s_field.pMesh);

	s_field.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_field.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// 頂点数を取得
	nNumVtx = s_field.pMesh->GetNumVertices();

	// フォーマットのサイズを取得
	SizeFVF = D3DXGetFVFVertexSize(s_field.pMesh->GetFVF());

	// 頂点バッファのロック
	s_field.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

	for (int i = 0; i < nNumVtx; i++)
	{
		// 頂点情報の代入
		D3DXVECTOR3 pos = *(D3DXVECTOR3*)pVexBuff;

		// 小さい・大きい [x]
		VtxSmallBig(&s_field.vtxMin.x, &s_field.vtxMax.x, pos.x);

		// 小さい・大きい [y]
		VtxSmallBig(&s_field.vtxMin.y, &s_field.vtxMax.y, pos.y);

		// 小さい・大きい [z]
		VtxSmallBig(&s_field.vtxMin.z, &s_field.vtxMax.z, pos.z);

		// 頂点フォーマットのサイズ分ポインタを進める
		pVexBuff += SizeFVF;
	}

	// 頂点バッファのアンロック
	s_field.pMesh->UnlockVertexBuffer();

	// メッシュに使用されているテクスチャ用の配列を用意する
	s_field.pTexture = new LPDIRECT3DTEXTURE9[s_field.nNumMat];

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_field.pBuffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)s_field.nNumMat; i++)
	{
		s_field.pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_field.pTexture[i]);
		}
		else
		{
			s_field.pTexture[i] = NULL;
		}
	}

	float fRand = (float)(rand() % MAX_RANDOM);

	s_field.pos = D3DXVECTOR3(fRand, 0.0f, 0.0f);
	s_field.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitField(void)
{
	if (s_field.pTexture != NULL)
	{// テクスチャの解放
		for (int i = 0; i < (int)s_field.nNumMat; i++)
		{
			if (s_field.pTexture[i] != NULL)
			{
				s_field.pTexture[i]->Release();
				s_field.pTexture[i] = NULL;
			}
		}

		delete[](s_field.pTexture);
		s_field.pTexture = NULL;
	}

	if (s_field.pMesh != NULL)
	{// メッシュの解放
		s_field.pMesh->Release();
		s_field.pMesh = NULL;
	}

	if (s_field.pBuffMat != NULL)
	{// マテリアルの解放
		s_field.pBuffMat->Release();
		s_field.pBuffMat = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateField(void)
{
	
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawField(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_field.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_field.rot.y, s_field.rot.x, s_field.rot.z);
	D3DXMatrixMultiply(&s_field.mtxWorld, &s_field.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_field.pos.x, s_field.pos.y, s_field.pos.z);
	D3DXMatrixMultiply(&s_field.mtxWorld, &s_field.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_field.mtxWorld);

	// 現在のマテリアル保持
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)s_field.pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)s_field.nNumMat; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_field.pTexture[i]);

		// モデルパーツの描画
		s_field.pMesh->DrawSubset(i);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 取得 (モデル情報)
//--------------------------------------------------
Field *GetField(void)
{
	return &s_field;
}

//--------------------------------------------------
// 当たり判定
//--------------------------------------------------
bool CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld)
{
	bool bCollision = false;

	float fLeft = s_field.pos.x + s_field.vtxMin.x;
	float fRight = s_field.pos.x + s_field.vtxMax.x;
	float fTop = s_field.pos.y + s_field.vtxMax.y;
	float fFront = s_field.pos.z + s_field.vtxMin.z;
	float fBack = s_field.pos.z + s_field.vtxMax.z;

	if ((pPos->x > fLeft) && (pPos->x < fRight) &&
		(pPos->z > fFront) && (pPos->z < fBack))
	{// x, zが範囲内
		if ((pPosOld->y >= fTop) && (pPos->y < fTop))
		{// 上
			pPos->y = fTop;
			bCollision = true;
		}
	}

	return bCollision;
}

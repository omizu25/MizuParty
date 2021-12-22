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
#define MAX_MOVE			(1.0f)			//移動量の最大値
#define MAX_ROTATION		(0.035f)		//回転の最大値
#define MAX_ATTENUATION		(0.1f)			//減衰係数の最大値
#define MAX_HEIGHT			(80.0f)			//高さの最大値
#define MIN_HEIGHT			(10.0f)			//高さの最小値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Model		s_model;		// モデルの情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitModel(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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

	s_model.pos = D3DXVECTOR3(25.0f, 20.0f, 0.0f);
	s_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_model.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 影の設定
	s_model.nIdxShadow = SetShadow(s_model.pos, s_model.rot);

	D3DXCOLOR col = D3DXCOLOR(0.615f, 0.215f, 0.341f, 1.0f);

	/*↓ Y棒 ↓*/

	D3DXVECTOR3 start = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMax.y, s_model.vtxMin.z);
	D3DXVECTOR3 end = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMin.y, s_model.vtxMin.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);

	start = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMax.y, s_model.vtxMin.z);
	end = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMin.y, s_model.vtxMin.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);

	start = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMax.y, s_model.vtxMax.z);
	end = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMin.y, s_model.vtxMax.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);

	start = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMax.y, s_model.vtxMax.z);
	end = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMin.y, s_model.vtxMax.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);

	/*↓ X棒 ↓*/

	start = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMax.y, s_model.vtxMax.z);
	end = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMax.y, s_model.vtxMax.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);

	start = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMax.y, s_model.vtxMin.z);
	end = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMax.y, s_model.vtxMin.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);

	start = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMin.y, s_model.vtxMax.z);
	end = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMin.y, s_model.vtxMax.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);

	start = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMin.y, s_model.vtxMin.z);
	end = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMin.y, s_model.vtxMin.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);

	/*↓ Z棒 ↓*/

	start = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMax.y, s_model.vtxMax.z);
	end = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMax.y, s_model.vtxMin.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);

	start = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMax.y, s_model.vtxMax.z);
	end = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMax.y, s_model.vtxMin.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);

	start = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMin.y, s_model.vtxMax.z);
	end = D3DXVECTOR3(s_model.vtxMax.x, s_model.vtxMin.y, s_model.vtxMin.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);

	start = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMin.y, s_model.vtxMax.z);
	end = D3DXVECTOR3(s_model.vtxMin.x, s_model.vtxMin.y, s_model.vtxMin.z);

	// 線の設定
	SetLine(s_model.pos, s_model.rot, start, end, col);
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
void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fWidth, float fDepth)
{
	float fModelLeft = s_model.pos.x + s_model.vtxMin.x;
	float fModelRight = s_model.pos.x + s_model.vtxMax.x;
	float fModelFront = s_model.pos.z + s_model.vtxMin.z;
	float fModelBack = s_model.pos.z + s_model.vtxMax.z;

	if ((pPos->x + fWidth > fModelLeft) && (pPos->x - fWidth < fModelRight))
	{// xが範囲内
		if ((pPosOld->z + fDepth <= fModelFront) && (pPos->z + fDepth > fModelFront))
		{// 前端
			pPos->z = fModelFront - fDepth;
		}
		else if ((pPosOld->z - fDepth >= fModelBack) && (pPos->z - fDepth < fModelBack))
		{// 後端
			pPos->z = fModelBack + fDepth;
		}
	}

	if ((pPos->z + fDepth > fModelFront) && (pPos->z - fDepth < fModelBack))
	{// zが範囲内
		if ((pPosOld->x + fWidth <= fModelLeft) && (pPos->x + fWidth > fModelLeft))
		{// 左端
			pPos->x = fModelLeft - fWidth;
		}
		else if ((pPosOld->x - fWidth >= fModelRight) && (pPos->x - fWidth < fModelRight))
		{// 右端
			pPos->x = fModelRight + fWidth;
		}
	}
}

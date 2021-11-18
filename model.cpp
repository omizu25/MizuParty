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
#include "model.h"

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPD3DXMESH		s_pMesh = NULL;			// メッシュ情報へのポインタ
static LPD3DXBUFFER		s_pBuffMat = NULL;		// マテリアル情報へのポインタ
static DWORD			s_nNumMat = 0;			// マテリアル情報の数
static D3DXVECTOR3		s_pos;					// 位置
static D3DXVECTOR3		s_rot;					// 向き
static D3DXMATRIX		s_mtxWorld;				// ワールドマトリックス

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitModel(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data\\リムル.x",
		/*"data\\てるてる４期生.x",*/
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

		s_pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		s_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitModel(void)
{
	if (s_pMesh != NULL)
	{// メッシュの破棄
		s_pMesh->Release();
		s_pMesh = NULL;
	}

	if (s_pBuffMat != NULL)
	{// マテリアルの破棄
		s_pBuffMat->Release();
		s_pBuffMat = NULL;
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
	D3DXMatrixIdentity(&s_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_rot.y, s_rot.x, s_rot.z);
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_pos.x, s_pos.y, s_pos.z);
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_mtxWorld);

	// 現在のマテリアル保持
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// モデルパーツの描画
		s_pMesh->DrawSubset(i);
	}

	// 保存していたマテリアルを
	pDevice->SetMaterial(&matDef);
}
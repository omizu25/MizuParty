//==================================================
// 
// 3Dゲーム制作 ( model.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "camera.h"
#include "input.h"
#include "main.h"
#include "model.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_MOVE		(1.0f)		//移動量の最大値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPD3DXMESH		s_pMesh = NULL;			// メッシュ情報へのポインタ
static LPD3DXBUFFER		s_pBuffMat = NULL;		// マテリアル情報へのポインタ
static DWORD			s_nNumMat = 0;			// マテリアル情報の数
static Model			s_model;				// モデルの情報

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void Move(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitModel(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		/*"data\\MODEL\\リムル.x",*/
		"data\\MODEL\\てるてる４期生.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

	s_model.pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	s_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	// 移動
	Move();
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
	pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// モデルパーツの描画
		s_pMesh->DrawSubset(i);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Model *GetModel(void)
{
	return &s_model;
}

//--------------------------------------------------
// 移動
//--------------------------------------------------
static void Move(void)
{ 
	Camera *pCamera = GetCamera();		//カメラの取得
	float fRot = 0.0f;

	if (GetKeyboardPress(DIK_LEFT))
	{// ←キーが押された
		if (GetKeyboardPress(DIK_UP))
		{// ↑キーが押された
			fRot = pCamera->rot.y + (-D3DX_PI * 0.25f);

			s_model.rot.y = pCamera->rot.y + (D3DX_PI * 0.75f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ↓キーが押された
			fRot = pCamera->rot.y + (-D3DX_PI * 0.75f);

			s_model.rot.y = pCamera->rot.y + (D3DX_PI * 0.25f);
		}
		else
		{
			fRot = pCamera->rot.y + (-D3DX_PI * 0.5f);

			s_model.rot.y = pCamera->rot.y + (D3DX_PI * 0.5f);
		}

		s_model.pos.x += sinf(fRot) * MAX_MOVE;
		s_model.pos.z += cosf(fRot) * MAX_MOVE;
	}
	else if (GetKeyboardPress(DIK_RIGHT))
	{// →キーが押された
		if (GetKeyboardPress(DIK_UP))
		{// ↑キーが押された
			fRot = pCamera->rot.y + (D3DX_PI * 0.25f);

			s_model.rot.y = pCamera->rot.y + (-D3DX_PI * 0.75f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ↓キーが押された
			fRot = pCamera->rot.y + (D3DX_PI * 0.75f);

			s_model.rot.y = pCamera->rot.y + (-D3DX_PI * 0.25f);
		}
		else
		{
			fRot = pCamera->rot.y + (D3DX_PI * 0.5f);

			s_model.rot.y = pCamera->rot.y + (-D3DX_PI * 0.5f);
		}

		s_model.pos.x += sinf(fRot) * MAX_MOVE;
		s_model.pos.z += cosf(fRot) * MAX_MOVE;
	}
	else if (GetKeyboardPress(DIK_UP))
	{// ↑キーが押された
		fRot = pCamera->rot.y;

		s_model.pos.x += sinf(fRot) * MAX_MOVE;
		s_model.pos.z += cosf(fRot) * MAX_MOVE;

		s_model.rot.y = pCamera->rot.y + D3DX_PI;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{// ↓キーが押された
		fRot = pCamera->rot.y + D3DX_PI;

		s_model.pos.x += sinf(fRot) * MAX_MOVE;
		s_model.pos.z += cosf(fRot) * MAX_MOVE;

		s_model.rot.y = pCamera->rot.y;
	}
}
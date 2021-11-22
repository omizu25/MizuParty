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
#define MAX_MOVE			(1.0f)			//移動量の最大値
#define MAX_ROTATION		(0.035f)		//回転の最大値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPD3DXMESH				s_pMesh = NULL;			// メッシュ情報へのポインタ
static LPD3DXBUFFER				s_pBuffMat = NULL;		// マテリアル情報へのポインタ
static LPDIRECT3DTEXTURE9		*s_pTexture = NULL;		// テクスチャへのポインタ
static DWORD					s_nNumMat = 0;			// マテリアル情報の数
static Model					s_model;				// モデルの情報

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

	//テクスチャ実験中
	/*// メッシュに使用されているテクスチャ用の配列を用意する
	s_pTexture = new LPDIRECT3DTEXTURE9[s_nNumMat];

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		if (pMat[i].pTextureFilename != NULL)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_pTexture[i]);
		}
		else
		{
			s_pTexture[i] = NULL;
		}
	}*/

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

		//テクスチャ実験中
		/*// テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[i]);*/

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

	/* ↓モデルの移動↓ */

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

	/* ↓モデルの回転↓ */

	if (GetKeyboardPress(DIK_LSHIFT))
	{// 左シフトキーが押された
		s_model.rot.y += -MAX_ROTATION;
	}
	else if (GetKeyboardPress(DIK_RSHIFT))
	{//右シフトキーが押された
		s_model.rot.y += MAX_ROTATION;
	}
}
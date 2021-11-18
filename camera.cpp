//==================================================
// 
// 3Dゲーム制作 ( camera.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "camera.h"
#include "input.h"
#include "main.h"

//--------------------------------------------------
// カメラの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		posV;				// 視点
	D3DXVECTOR3		posR;				// 注視点
	D3DXVECTOR3		vecU;				// 上方向ベクトル
	D3DXVECTOR3		rot;				// 向き
	D3DXMATRIX		mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX		mtxView;			// ビューマトリックス
}Camera;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Camera		s_camera;		// カメラの情報

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void UpdateInput(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitCamera(void)
{
	// 視点・注視点・上方向を設定する
	s_camera.posV = D3DXVECTOR3(0.0f, 50.0f, -100.0f);
	s_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 固定でいい
	s_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitCamera(void)
{
	
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateCamera(void)
{
	// 入力
	//UpdateInput();
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetCamera(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&s_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(
		&s_camera.mtxView,
		&s_camera.posV,
		&s_camera.posR,
		&s_camera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &s_camera.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&s_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(
		&s_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		1000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &s_camera.mtxProjection);
}

//--------------------------------------------------
// 入力
//--------------------------------------------------
static void UpdateInput(void)
{
	
	if (GetKeyboardPress(DIK_A))
	{// Aキーが押された
		s_camera.posV.x += sinf(-D3DX_PI * 0.5f);
		s_camera.posR.x += sinf(-D3DX_PI * 0.5f);
	}
	else if (GetKeyboardPress(DIK_D))
	{// Dキーが押された
		s_camera.posV.x += sinf(D3DX_PI * 0.5f);
		s_camera.posR.x += sinf(D3DX_PI * 0.5f);
	}

	if (GetKeyboardPress(DIK_W))
	{// Wキーが押された
		s_camera.posV.z += sinf(-D3DX_PI * 0.5f);
		s_camera.posR.z += sinf(-D3DX_PI * 0.5f);
	}
	else if (GetKeyboardPress(DIK_S))
	{// Sキーが押された
		s_camera.posV.z += -1.0f;
		s_camera.posR.z += -1.0f;
	}
	
	if (GetKeyboardPress(DIK_Y))
	{// Yキーが押された
		s_camera.posR.z += 1.0f;
		s_camera.rot.z += D3DX_PI * 0.1f;
	}
	else if (GetKeyboardPress(DIK_N))
	{// Nキーが押された
		s_camera.posR.z += -1.0f;
	}

	if (GetKeyboardPress(DIK_T))
	{// Tキーが押された
		//s_camera.posR.y += -5.0f;
		s_camera.posV.y += 1.0f;
	}
	else if (GetKeyboardPress(DIK_B))
	{// Bキーが押された
		//s_camera.posR.y += 5.0f;
		s_camera.posV.y += -1.0f;
	}
}
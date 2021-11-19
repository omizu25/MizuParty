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

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_ROTATION		(0.035f)		//回転の最大数

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
	// 視点・注視点・上方向・向き・距離を設定する
	s_camera.posV = D3DXVECTOR3(0.0f, 50.0f, -100.0f);
	s_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 固定でいい
	s_camera.rot  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fDifX, fDifZ;

	fDifX = s_camera.posV.x - s_camera.posR.x;
	fDifZ = s_camera.posV.z - s_camera.posR.z;

	s_camera.fDistance = sqrtf((fDifX * fDifX) + (fDifZ * fDifZ));
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
	UpdateInput();

	// 角度の正規化
	if (s_camera.rot.y >= D3DX_PI)
	{// 3.14より大きい
		s_camera.rot.y -= D3DX_PI * 2.0f;
	}
	else if (s_camera.rot.y <= -D3DX_PI)
	{// -3.14より小さい
		s_camera.rot.y += D3DX_PI * 2.0f;
	}

	// 注視点の移動
	s_camera.posR.x = s_camera.posV.x + sinf(s_camera.rot.y) * s_camera.fDistance;
	s_camera.posR.z = s_camera.posV.z + cosf(s_camera.rot.y) * s_camera.fDistance;
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
// 取得
//--------------------------------------------------
Camera *GetCamera(void)
{
	return &s_camera;
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

	// 注視点の旋回
	if (GetKeyboardPress(DIK_Q))
	{// Qキーが押された
		s_camera.rot.y += MAX_ROTATION;
	}
	else if (GetKeyboardPress(DIK_E))
	{// Eキーが押された
		s_camera.rot.y += -MAX_ROTATION;
	}
}
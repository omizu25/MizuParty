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
#define MAX_MOVE			(1.0f)			//移動量の最大値
#define MAX_ROTATION		(0.035f)		//回転の最大数

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Camera		s_camera;		// カメラの情報

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void Input(void);
static void Move(void);
static void Rot(void);
static void RotNormalization(D3DXVECTOR3 *pRot);

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
	Input();

	/* ↓旋回の移動↓ */

	if (GetKeyboardPress(DIK_Z) || GetKeyboardPress(DIK_C))
	{// Z, Cキーが押された
		// 視点の移動
		s_camera.posV.x = s_camera.posR.x - sinf(s_camera.rot.y) * s_camera.fDistance;
		s_camera.posV.z = s_camera.posR.z - cosf(s_camera.rot.y) * s_camera.fDistance;
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
static void Input(void)
{
	// 移動
	Move();

	// 回転
	Rot();
	
	if (GetKeyboardPress(DIK_Y))
	{// Yキーが押された
		s_camera.rot.x += D3DX_PI * 0.1f;
	}
	else if (GetKeyboardPress(DIK_H))
	{// Hキーが押された
		s_camera.rot.x += D3DX_PI * 0.1f;
	}
}

//--------------------------------------------------
// 移動
//--------------------------------------------------
static void Move(void)
{
	float fRot = 0.0f;

	/* ↓視点の移動↓ */

	if (GetKeyboardPress(DIK_A))
	{// Aキーが押された
		if (GetKeyboardPress(DIK_W))
		{// Wキーが押された
			fRot = s_camera.rot.y + (-D3DX_PI * 0.25f);
		}	
		else if (GetKeyboardPress(DIK_S))
		{// Sキーが押された
			fRot = s_camera.rot.y + (-D3DX_PI * 0.75f);
		}
		else
		{
			fRot = s_camera.rot.y + (-D3DX_PI * 0.5f);
		}

		s_camera.posV.x += sinf(fRot) * MAX_MOVE;
		s_camera.posV.z += cosf(fRot) * MAX_MOVE;
	}
	else if (GetKeyboardPress(DIK_D))
	{// Dキーが押された
		if (GetKeyboardPress(DIK_W))
		{// Wキーが押された
			fRot = s_camera.rot.y + (D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_S))
		{// Sキーが押された
			fRot = s_camera.rot.y + (D3DX_PI * 0.75f);
		}
		else
		{
			fRot = s_camera.rot.y + (D3DX_PI * 0.5f);
		}

		s_camera.posV.x += sinf(fRot) * MAX_MOVE;
		s_camera.posV.z += cosf(fRot) * MAX_MOVE;
	}
	else if (GetKeyboardPress(DIK_W))
	{// Wキーが押された
		fRot = s_camera.rot.y;

		s_camera.posV.x += sinf(fRot) * MAX_MOVE;
		s_camera.posV.z += cosf(fRot) * MAX_MOVE;
	}
	else if (GetKeyboardPress(DIK_S))
	{// Sキーが押された
		fRot = s_camera.rot.y + (-D3DX_PI);

		s_camera.posV.x += sinf(fRot) * MAX_MOVE;
		s_camera.posV.z += cosf(fRot) * MAX_MOVE;
	}
}

//--------------------------------------------------
// 回転
//--------------------------------------------------
static void Rot(void)
{
	/* ↓視点の旋回↓ */

	if (GetKeyboardPress(DIK_Z))
	{// Zキーが押された
		s_camera.rot.y += MAX_ROTATION;
	}

	if (GetKeyboardPress(DIK_C))
	{// Cキーが押された
		s_camera.rot.y += -MAX_ROTATION;
	}

	/* ↓注視点の旋回↓ */

	if (GetKeyboardPress(DIK_Q))
	{// Qキーが押された
		s_camera.rot.y += -MAX_ROTATION;
	}

	if (GetKeyboardPress(DIK_E))
	{// Eキーが押された
		s_camera.rot.y += MAX_ROTATION;
	}

	// 角度の正規化
	RotNormalization(&s_camera.rot);
}

//--------------------------------------------------
// 角度の正規化
//--------------------------------------------------
static void RotNormalization(D3DXVECTOR3 *pRot)
{
	if (pRot->y >= D3DX_PI)
	{// 3.14より大きい
		pRot->y -= D3DX_PI * 2.0f;
	}
	else if (pRot->y <= -D3DX_PI)
	{// -3.14より小さい
		pRot->y += D3DX_PI * 2.0f;
	}
}

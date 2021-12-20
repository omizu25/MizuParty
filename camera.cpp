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
#include "player.h"
#include "setup.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_NEAR			(10.0f)			// ニアの最大値
#define MAX_FAR				(1500.0f)		// ファーの最大値
#define MAX_MOVE			(2.0f)			// 移動量の最大値
#define MAX_ROTATION		(0.035f)		// 回転の最大値
#define MAX_DISTANCE		(50.0f)			// 距離の最大値
#define MIN_DISTANCE		(0.0f)			// 距離の最小値
#define START_DISTANCE		(20.0f)			// 距離の最初の値
#define MAX_POS_FACTOR		(0.05f)			// 位置の減衰係数
#define MAX_ROT_FACTOR		(0.2f)			// 向きの減衰係数
#define START_POS_Y			(100.0f)		// Yの位置の最初の値
#define START_POS_Z			(-200.0f)		// Zの位置の最初の値
#define STOP_TIME			(120)			// 止まっている時間

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Camera		s_camera;			// カメラの情報

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void FollowMove(void);
static void FollowWrap(void);
static void Move(void);
static void Rot(void);
static void ResetCamera(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitCamera(void)
{
	// 視点・注視点・上方向・向き・距離を設定する
	s_camera.posV = D3DXVECTOR3(0.0f, START_POS_Y, START_POS_Z);
	s_camera.posR = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
	s_camera.posVDest = s_camera.posV;
	s_camera.posRDest = s_camera.posR;
	s_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 固定でいい
	s_camera.rot  = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);
	s_camera.rotDest = s_camera.rot;

	float fDisX, fDisZ;

	fDisX = s_camera.posV.x - s_camera.posR.x;
	fDisZ = s_camera.posV.z - s_camera.posR.z;

	s_camera.fDistance = sqrtf((fDisX * fDisX) + (fDisZ * fDisZ));
	s_camera.fDisPlayer = START_DISTANCE;

	s_camera.bFollow = true;
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
	if (GetKeyboardTrigger(DIK_F6))
	{// F6キーが押された
		s_camera.bFollow = !s_camera.bFollow;
	
		// リセット
		ResetCamera();
	}

	if (s_camera.bFollow)
	{// 追従する
		if (GetKeyboardPress(DIK_1))
		{// 1キーが押された
			s_camera.fDisPlayer -= 1.0f;
		}
		else if (GetKeyboardPress(DIK_2))
		{// 2キーが押された
			s_camera.fDisPlayer += 1.0f;
		}

		// 指定の値以上・以下
		Specified(&s_camera.fDisPlayer, MAX_DISTANCE, MIN_DISTANCE);

		// 追従の移動
		FollowMove();

		// 追従の回り込み
		//FollowWrap();
	}
	else
	{// 追従しない
		// 移動
		Move();

		// 回転
		Rot();

		/* ↓旋回の移動↓ */

		if (GetKeyboardPress(DIK_Z) || GetKeyboardPress(DIK_C) ||
			GetKeyboardPress(DIK_U) || GetKeyboardPress(DIK_J))
		{// Z, C, U, Jキーが押された
		 // 視点の移動
			s_camera.posV.x = s_camera.posR.x - sinf(s_camera.rot.y) * s_camera.fDistance;
			s_camera.posV.z = s_camera.posR.z - cosf(s_camera.rot.y) * s_camera.fDistance;
		}

		// 注視点の移動
		s_camera.posR.x = s_camera.posV.x + sinf(s_camera.rot.y) * s_camera.fDistance;
		s_camera.posR.z = s_camera.posV.z + cosf(s_camera.rot.y) * s_camera.fDistance;
		s_camera.posR.y = s_camera.posV.y + tanf(-s_camera.rot.x + (D3DX_PI * 0.5f)) * s_camera.fDistance;
	}
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
		MAX_NEAR,
		MAX_FAR);

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
// 追従の移動
//--------------------------------------------------
static void FollowMove(void)
{
	Player *pPlayer = GetPlayer();		// プレイヤーの情報

	float fAngle = s_camera.rot.y - pPlayer->rot.y;

	// 角度の正規化
	NormalizeRot(&fAngle);

	// 目的の注視点
	s_camera.posRDest.x = pPlayer->pos.x + sinf(fAngle) * s_camera.fDisPlayer;
	s_camera.posRDest.z = pPlayer->pos.z - cosf(fAngle) * s_camera.fDisPlayer;

	// 目的の視点
	s_camera.posVDest.x = pPlayer->pos.x + sinf(s_camera.rot.y) * s_camera.fDistance;
	s_camera.posVDest.z = pPlayer->pos.z - cosf(s_camera.rot.y) * s_camera.fDistance;

	// 注視点の移動
	s_camera.posR.x += (s_camera.posRDest.x - s_camera.posR.x) * MAX_POS_FACTOR;
	s_camera.posR.z += (s_camera.posRDest.z - s_camera.posR.z) * MAX_POS_FACTOR;

	// 視点の移動
	s_camera.posV.x += (s_camera.posVDest.x - s_camera.posV.x) * MAX_POS_FACTOR;
	s_camera.posV.z += (s_camera.posVDest.z - s_camera.posV.z) * MAX_POS_FACTOR;
}

//--------------------------------------------------
// 追従の回り込み
//--------------------------------------------------
static void FollowWrap(void)
{
	Player *pPlayer = GetPlayer();		// プレイヤーの情報

	if (pPlayer->nStopTime >= STOP_TIME)
	{// 指定の値以上
		float fAngle = pPlayer->rot.y - s_camera.rot.y;

		// 角度の正規化
		NormalizeRot(&fAngle);

		s_camera.rotDest.y = cosf(fAngle + (D3DX_PI * 0.5f));

		s_camera.rot.y += (s_camera.rotDest.y) * MAX_ROT_FACTOR;
	}
}

//--------------------------------------------------
// 移動
//--------------------------------------------------
static void Move(void)
{
	float fRot = 0.0f;

	if (GetDebug() != DEBUG_MESH)
	{// デバッグ表示がメッシュではない時

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

		if (GetKeyboardPress(DIK_T))
		{// Tキーが押された
			fRot = D3DX_PI * 0.5f;

			s_camera.posV.y += sinf(fRot) * MAX_MOVE;
		}
		else if (GetKeyboardPress(DIK_G))
		{// Gキーが押された
			fRot = -D3DX_PI * 0.5f;

			s_camera.posV.y += sinf(fRot) * MAX_MOVE;
		}

		/* ↓視点と注視点の距離↓ */

		if (GetKeyboardPress(DIK_U))
		{// Uキーが押された
			s_camera.fDistance += -MAX_MOVE;
		}
		else if (GetKeyboardPress(DIK_J))
		{// Jキーが押された
			s_camera.fDistance += MAX_MOVE;
		}

		// 指定の値以上・以下
		Specified(&s_camera.fDistance, MAX_FAR, MAX_NEAR);
	}
}

//--------------------------------------------------
// 回転
//--------------------------------------------------
static void Rot(void)
{
	if (GetDebug() != DEBUG_MESH)
	{// デバッグ表示がメッシュではない時

		/* ↓視点・注視点の旋回↓ */

		if (GetKeyboardPress(DIK_Z) || GetKeyboardPress(DIK_E))
		{// Z, Eキーが押された
			s_camera.rot.y += MAX_ROTATION;
		}

		if (GetKeyboardPress(DIK_C) || GetKeyboardPress(DIK_Q))
		{// C, Qキーが押された
			s_camera.rot.y += -MAX_ROTATION;
		}

		// 角度の正規化
		NormalizeRot(&s_camera.rot.y);

		/* ↓注視点の上下↓ */

		if (GetKeyboardPress(DIK_Y))
		{// Yキーが押された
			s_camera.rot.x += -MAX_ROTATION;
		}
		else if (GetKeyboardPress(DIK_H))
		{// Hキーが押された
			s_camera.rot.x += MAX_ROTATION;
		}

		// 指定の値以上・以下
		Specified(&s_camera.rot.x, 3.0f, 0.1f);
	}
}

//--------------------------------------------------
// リセット
//--------------------------------------------------
static void ResetCamera(void)
{
	Player *pPlayer = GetPlayer();		// プレイヤーの情報

	s_camera.posV.x = pPlayer->pos.x;
	s_camera.posV.y = pPlayer->pos.y + START_POS_Y;
	s_camera.posV.z = pPlayer->pos.z + START_POS_Z;
	s_camera.posR = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
	s_camera.posVDest = s_camera.posV;
	s_camera.posRDest = s_camera.posR;
	s_camera.rot = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);
	s_camera.rotDest = s_camera.rot;
}

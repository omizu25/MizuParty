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
#include "countdown.h"
#include "field.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "setup.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_CAMERA			(2)				// カメラの最大数
#define MAX_NEAR			(10.0f)			// ニアの最大値
#define MAX_FAR				(1500.0f)		// ファーの最大
#define MAX_ROTATION		(0.035f)		// 回転の最大値
#define MAX_DISTANCE		(50.0f)			// 距離の最大値
#define MIN_DISTANCE		(0.0f)			// 距離の最小値
#define START_DISTANCE		(30.0f)			// 距離の最初の値
#define MAX_POS_FACTOR		(0.05f)			// 位置の減衰係数
#define MAX_ROT_FACTOR		(0.2f)			// 向きの減衰係数
#define START_WALKING_Y		(100.0f)		// Yの位置の最初の値
#define START_WALKING_Z		(-300.0f)		// Zの位置の最初の値
#define START_STOP_Y		(180.0f)		// Yの位置の最初の値
#define START_STOP_Z		(-500.0f)		// Zの位置の最初の値
#define START_SLOPE_X		(100.0f)		// Yの位置の最初の値
#define START_SLOPE_Y		(100.0f)		// Yの位置の最初の値
#define START_SLOPE_Z		(-700.0f)		// Zの位置の最初の値
#define MOVE_Y				(5.0f)			// Yの移動量
#define MOVE_Z				(-3.0f)			// Zの移動量
#define STOP_POS_Y			(100.0f)		// Yの位置の止まる場所

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Camera		s_camera[MAX_CAMERA];		// カメラの情報
static bool			s_bOverlap;					// プレイヤーと重なったかどうか
static bool			s_bStop;					// 止まるかどうか

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void FollowMove(void);
static void ResultMove(void);
static void Overlap(float fPosX);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitCamera(void)
{
	float fPosX = (GetField()->pos.x + GetField()->vtxMax.x) * 0.75f;

	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング

		s_camera[0].posV = D3DXVECTOR3(0.0f, START_WALKING_Y, START_WALKING_Z);
		s_camera[0].posR = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
		s_camera[0].rot = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);

		break;

	case MENU_STOP:			// 止める

		s_camera[0].posV = D3DXVECTOR3(0.0f, START_STOP_Y, START_STOP_Z);
		s_camera[0].posR = D3DXVECTOR3(0.0f, START_STOP_Y, 0.0f);
		s_camera[0].rot = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);

		break;

	case MENU_SLOPE:		// 坂

		s_camera[0].posV = D3DXVECTOR3(fPosX, START_SLOPE_Y, START_SLOPE_Z);
		s_camera[0].posR = D3DXVECTOR3(fPosX, START_SLOPE_Y, 0.0f);
		s_camera[0].rot = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);

		break;

	default:
		assert(false);
		break;
	}

	// 視点・注視点・上方向・向き・距離を設定する
	s_camera[0].posVDest = s_camera[0].posV;
	s_camera[0].posRDest = s_camera[0].posR;
	s_camera[0].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 固定でいい
	s_camera[0].rotDest = s_camera[0].rot;

	float fDisX, fDisZ;

	fDisX = s_camera[0].posV.x - s_camera[0].posR.x;
	fDisZ = s_camera[0].posV.z - s_camera[0].posR.z;

	s_camera[0].fDistance = sqrtf((fDisX * fDisX) + (fDisZ * fDisZ));
	s_camera[0].fDisPlayer = START_DISTANCE;

	s_camera[0].viewport.X = (DWORD)0.0f;
	s_camera[0].viewport.Y = (DWORD)0.0f;
	s_camera[0].viewport.Width = (DWORD)(SCREEN_WIDTH * 0.5f);
	s_camera[0].viewport.Height = (DWORD)(SCREEN_HEIGHT * 0.5f);
	s_camera[0].viewport.MinZ = 0.0f;
	s_camera[0].viewport.MaxZ = 1.0f;

	if (GetTitle() == MENU_WALKING)
	{
		s_bOverlap = false;
	}
	else
	{
		s_bOverlap = true;;
	}
	
	s_bStop = false;
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
	switch (GetTitle())
	{
	case MENU_STOP:			// 止める

		if (GetCountdown())
		{
			if (!GetStop())
			{
				if (!s_bStop)
				{// 止まらない
					if (GetGame().gameState == GAMESTATE_NORMAL)
					{
						s_camera[0].posV.z += MOVE_Y;
						s_camera[0].posR.z += MOVE_Y;
						s_camera[0].posV.y += MOVE_Z;
						s_camera[0].posR.y += MOVE_Z;
					}

					if (s_camera[0].posV.y <= STOP_POS_Y)
					{
						s_bStop = true;
					}
				}
			}
		}

		break;

	case MENU_WALKING:		// ウォーキング
	case MENU_SLOPE:		// 坂

		switch (GetGame().gameState)
		{
		case GAMESTATE_NORMAL:			// ゲーム中
		case GAMESTATE_END:				// 終わり
		case GAMESTATE_COUNTDOWN:		// カウントダウン

			// 追従の移動
			FollowMove();

			if (GetTitle() == MENU_SLOPE)
			{
				if (GetPlayer()->pos.y >= 0.0f)
				{
					s_camera[0].posV.y = GetPlayer()->pos.y + START_SLOPE_Y;
				}
			}

			break;

		case GAMESTATE_RESULT:			// リザルト

			// リザルトの移動
			ResultMove();
			break;

		case GAMESTATE_NONE:			// 何もなし
		case GAMESTATE_START:			// 始まり

			/* 処理なし */

			break;

		default:
			assert(false);
			break;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawCamera(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューポートの設定
	pDevice->SetViewport(&s_camera[0].viewport);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// 必ず成功する
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zバッファの値を元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// 新規深度値 <= Zバッファ深度値 (初期設定)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetCamera(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&s_camera[0].mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(
		&s_camera[0].mtxView,
		&s_camera[0].posV,
		&s_camera[0].posR,
		&s_camera[0].vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &s_camera[0].mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&s_camera[0].mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(
		&s_camera[0].mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		MAX_NEAR,
		MAX_FAR);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &s_camera[0].mtxProjection);
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Camera *GetCamera(void)
{
	return &s_camera[0];
}

//--------------------------------------------------
// 重なってるかどうかを取得
//--------------------------------------------------
bool GetOverlap(void)
{
	return s_bOverlap;
}

//--------------------------------------------------
// 追従の移動
//--------------------------------------------------
static void FollowMove(void)
{
	Player *pPlayer = GetPlayer();		// プレイヤーの情報

	float fAngle = s_camera[0].rot.y - pPlayer->rot.y;

	// 角度の正規化
	NormalizeRot(&fAngle);

	// 目的の注視点
	s_camera[0].posRDest.x = pPlayer->pos.x + sinf(fAngle) * s_camera[0].fDisPlayer;
	s_camera[0].posRDest.z = pPlayer->pos.z - cosf(fAngle) * s_camera[0].fDisPlayer;

	// 目的の視点
	s_camera[0].posVDest.x = pPlayer->pos.x + sinf(s_camera[0].rot.y) * s_camera[0].fDistance;
	s_camera[0].posVDest.z = pPlayer->pos.z - cosf(s_camera[0].rot.y) * s_camera[0].fDistance;

	// 注視点の移動
	s_camera[0].posR.x += (s_camera[0].posRDest.x - s_camera[0].posR.x) * MAX_POS_FACTOR;
	s_camera[0].posR.z += (s_camera[0].posRDest.z - s_camera[0].posR.z) * MAX_POS_FACTOR;

	// 視点の移動
	s_camera[0].posV.x += (s_camera[0].posVDest.x - s_camera[0].posV.x) * MAX_POS_FACTOR;
	s_camera[0].posV.z += (s_camera[0].posVDest.z - s_camera[0].posV.z) * MAX_POS_FACTOR;
}

//--------------------------------------------------
// リザルトの移動
//--------------------------------------------------
static void ResultMove(void)
{
	if (!s_bOverlap)
	{// 重なってない
		D3DXVECTOR3 pos = GetPlayer()->pos;
		float fMove = GetPlayer()->fMove;
		bool bDirection = true;		// true : 右向き  false : 左向き

		if (pos.x <= 0.0f)
		{
			fMove *= -1.0f;
			bDirection = false;
		}

		// 目的の注視点
		s_camera[0].posRDest.x += fMove;

		// 目的の視点
		s_camera[0].posVDest.x += fMove;

		// 注視点の移動
		s_camera[0].posR.x += (s_camera[0].posRDest.x - s_camera[0].posR.x) * MAX_POS_FACTOR;

		// 視点の移動
		s_camera[0].posV.x += (s_camera[0].posVDest.x - s_camera[0].posV.x) * MAX_POS_FACTOR;

		if (bDirection)
		{// 右向き
			if (pos.x <= s_camera[0].posR.x)
			{
				// 重ねる
				Overlap(pos.x);
			}
		}
		else
		{// 左向き
			if (pos.x >= s_camera[0].posR.x)
			{
				// 重ねる
				Overlap(pos.x);
			}
		}
	}
}

//--------------------------------------------------
// 重ねる
//--------------------------------------------------
static void Overlap(float fPosX)
{
	// 目的の注視点
	s_camera[0].posRDest.x = fPosX;

	// 目的の視点
	s_camera[0].posVDest.x = fPosX;

	// 注視点の移動
	s_camera[0].posR.x = fPosX;

	// 視点の移動
	s_camera[0].posV.x = fPosX;

	s_bOverlap = true;
}
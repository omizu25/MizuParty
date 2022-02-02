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
#include "game.h"
#include "input.h"
#include "player.h"
#include "setup.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_NEAR			(10.0f)			// ニアの最大値
#define MAX_FAR				(1500.0f)		// ファーの最大
#define MAX_ROTATION		(0.035f)		// 回転の最大値
#define MAX_DISTANCE		(50.0f)			// 距離の最大値
#define MIN_DISTANCE		(0.0f)			// 距離の最小値
#define START_DISTANCE		(30.0f)			// 距離の最初の値
#define MAX_POS_FACTOR		(0.05f)			// 位置の減衰係数
#define MAX_ROT_FACTOR		(0.2f)			// 向きの減衰係数
#define START_POS_Y			(100.0f)		// Yの位置の最初の値
#define START_POS_Z			(-300.0f)		// Zの位置の最初の値
#define STOP_TIME			(120)			// 止まっている時間

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Camera		s_camera;			// カメラの情報
static bool			s_bOverlap;			// プレイヤーと重なったかどうか

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

	s_camera.viewport.X = (DWORD)0.0f;
	s_camera.viewport.Y = (DWORD)0.0f;
	s_camera.viewport.Width = SCREEN_WIDTH;
	s_camera.viewport.Height = SCREEN_HEIGHT;
	s_camera.viewport.MinZ = 0.0f;
	s_camera.viewport.MaxZ = 1.0f;

	s_bOverlap = false;
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
	switch (GetGame().gameState)
	{
	case GAMESTATE_NORMAL:		// ゲーム中
	case GAMESTATE_END:			// 終わり

		// 追従の移動
		FollowMove();

		break;

	case GAMESTATE_RESULT:			// リザルト
		
		// リザルトの移動
		ResultMove();
		break;

	case GAMESTATE_NONE:			// 何もなし
	case GAMESTATE_START:			// 始まり
	case GAMESTATE_COUNTDOWN:		// カウントダウン

		/* 処理なし */

		break;

	default:
		assert(false);
		break;
	}

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
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawCamera(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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
		s_camera.posRDest.x += fMove;

		// 目的の視点
		s_camera.posVDest.x += fMove;

		// 注視点の移動
		s_camera.posR.x += (s_camera.posRDest.x - s_camera.posR.x) * MAX_POS_FACTOR;

		// 視点の移動
		s_camera.posV.x += (s_camera.posVDest.x - s_camera.posV.x) * MAX_POS_FACTOR;

		if (bDirection)
		{// 右向き
			if (pos.x <= s_camera.posR.x)
			{
				// 重ねる
				Overlap(pos.x);
			}
		}
		else
		{// 左向き
			if (pos.x >= s_camera.posR.x)
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
	s_camera.posRDest.x = fPosX;

	// 目的の視点
	s_camera.posVDest.x = fPosX;

	// 注視点の移動
	s_camera.posR.x = fPosX;

	// 視点の移動
	s_camera.posV.x = fPosX;

	s_bOverlap = true;
}
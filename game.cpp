//==================================================
// 
// 3Dゲーム制作 ( game.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "bullet.h"
#include "camera.h"
#include "effect.h"
#include "game.h"
#include "input.h"
#include "light.h"
#include "line.h"
#include "mesh_cylinder.h"
#include "mesh_field.h"
#include "mesh_sky.h"
#include "mesh_sphere.h"
#include "model.h"
#include "number.h"
#include "player.h"
#include "particle.h"
#include "polygon.h"
#include "shadow.h"
#include "target.h"
#include "time.h"
#include "wall.h"

#include <assert.h>
//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Game		s_game;		// ゲーム情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitGame(HWND hWnd)
{
	// ポリゴンの初期化
	//InitPolygon();

	// メッシュフィールドの初期化
	InitMeshField();

	// メッシュフィールドの設定
	SetMeshField();

	// メッシュ円柱の初期化
	InitMeshCylinder();

	// メッシュ円柱の設定
	SetMeshCylinder();

	// メッシュ球の初期化
	InitMeshSphere();

	// メッシュ球の設定
	SetMeshSphere();

	// メッシュ空の初期化
	InitMeshSky();

	// メッシュ空の設定
	SetMeshSky();

	// 壁の初期化
	InitWall();

	// 壁の設置
	InstallationWall();

	// ビルボードの初期化
	InitBillboard();

	// ビルボードの読み込み
	LoadBillboard(hWnd);

	// パーティクルの初期化
	InitParticle();

	// エフェクトの初期化
	InitEffect();

	// 影の初期化
	InitShadow();

	// 線の初期化
	InitLine();

	// モデルの初期化
	InitModel();

	// プレイヤーの読み込み
	LoadPlayer(hWnd);

	// プレイヤーの初期化
	InitPlayer();

	// 弾の初期化
	InitBullet();

	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	// 数の初期化
	InitNumber();

	// タイムの初期化
	InitTime();

	// ターゲットの初期化
	InitTarget();

	s_game.gameState = GAMESTATE_NORMAL;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitGame(void)
{
	// ポリゴンの終了
	//UninitPolygon();

	// メッシュフィールドの終了
	UninitMeshField();

	// メッシュ円柱の終了
	UninitMeshCylinder();

	// メッシュ球の終了
	UninitMeshSphere();

	// メッシュ空の終了
	UninitMeshSky();

	// 壁の終了
	UninitWall();

	// ビルボードの終了
	UninitBillboard();

	// エフェクトの終了
	UninitEffect();

	// 影の終了
	UninitShadow();

	// 線の終了
	UninitLine();

	// モデルの終了
	UninitModel();

	// プレイヤーの終了
	UninitPlayer();

	// 弾の終了
	UninitBullet();

	// カメラの終了
	UninitCamera();

	// ライトの終了
	UninitLight();

	// 数の終了
	UninitNumber();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateGame(void)
{
	switch (s_game.gameState)
	{
	case GAMESTATE_START:		// 開始状態(ゲーム開始中)
						 
		break;

	case GAMESTATE_NORMAL:		// 通常状態(ゲーム進行中)

		if (!s_game.bPause)
		{
			// ポリゴンの更新
			//UpdatePolygon();

			// メッシュフィールドの更新
			UpdateMeshField();

			// メッシュ円柱の更新
			UpdateMeshCylinder();

			// メッシュ球の更新
			UpdateMeshSphere();

			// メッシュ空の更新
			UpdateMeshSky();

			// 壁の更新
			UpdateWall();

			// ビルボードの更新
			UpdateBillboard();

			// パーティクルの更新
			UpdateParticle();

			// エフェクトの更新
			UpdateEffect();

			// モデルの更新
			UpdateModel();

			// プレイヤーの更新
			UpdatePlayer();

			// 線の更新
			UpdateLine();

			// 弾の更新
			UpdateBullet();

			// 影の更新
			UpdateShadow();

			// カメラの更新
			UpdateCamera();

			// ライトの更新
			UpdateLight();
		}

		if (GetKeyboardTrigger(DIK_F4))
		{// F4キーが押された
			s_game.bPause = !s_game.bPause;
		}

		if (GetKeyboardTrigger(DIK_F5))
		{// F5キーが押された
			s_game.bWireframe = !s_game.bWireframe;

			// デバイスへのポインタの取得
			LPDIRECT3DDEVICE9 pDevice = GetDevice();

			if (s_game.bWireframe)
			{// 表示
				// レンダーステートの設定
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else
			{// 非表示
				// レンダーステートを元に戻す
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
		}
		
		// タイムの減算
		SubTime(1);

		// 数の更新
		UpdateNumber();

		break;

	case GAMESTATE_END:		// 終了状態(ゲーム終了時)

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawGame(void)
{
	// カメラの設定
	SetCamera();

	// ポリゴンの描画
	//DrawPolygon();

	// メッシュ空の描画
	DrawMeshSky();

	// メッシュフィールドの描画
	DrawMeshField();

	// モデルの描画
	DrawModel();

	// プレイヤーの描画
	DrawPlayer();

	// 線の描画
	DrawLine();

	// 影の描画
	DrawShadow();

	// メッシュ球の描画
	//DrawMeshSphere();

	// メッシュ円柱の描画
	//DrawMeshCylinder();

	// ビルボードの描画
	DrawBillboard(false);

	// 壁の描画
	DrawWall();

	// エフェクトの描画
	DrawEffect();

	// ビルボードの描画
	DrawBillboard(true);

	// 数の描画
	DrawNumber();
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	s_game.gameState = state;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Game GetGame(void)
{
	return s_game;
}

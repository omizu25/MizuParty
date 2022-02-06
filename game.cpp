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
#include "countdown.h"
#include "effect.h"
#include "fade.h"
#include "field.h"
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
#include "result.h"
#include "rule.h"
#include "shadow.h"
#include "target.h"
#include "time.h"
#include "title.h"
#include "wall.h"

#include <assert.h>
//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Game		s_game;			// ゲーム情報
static int		s_nTime;		// タイム

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitGame(void)
{
	switch (GetTitle())
	{// どのゲーム？
	case MENU_WALKING:		// ウォーキング
	case MENU_STOP:			// 止める

		// ポリゴンの初期化
		InitPolygon();

		break;

	case MENU_SLOPE:		// 坂

		// フィールドの初期化
		InitField();

		// メッシュフィールドの初期化
		InitMeshField();

		// メッシュフィールドの設定
		SetMeshField();

		// 壁の初期化
		InitWall();

		// 壁の設置
		InstallationWall();

		break;

	default:
		assert(false);
		break;
	}

	// メッシュ円柱の初期化
	//InitMeshCylinder();

	// メッシュ円柱の設定
	//SetMeshCylinder();

	// メッシュ球の初期化
	//InitMeshSphere();

	// メッシュ球の設定
	//SetMeshSphere();

	// メッシュ空の初期化
	//InitMeshSky();

	// メッシュ空の設定
	//SetMeshSky();

	// パーティクルの初期化
	InitParticle();

	// エフェクトの初期化
	InitEffect();

	// 影の初期化
	InitShadow();

	// 線の初期化
	//InitLine();

	// モデルの初期化
	InitModel();

	// プレイヤーの読み込み
	LoadPlayer();

	// プレイヤーの初期化
	InitPlayer();

	// ビルボードの初期化
	InitBillboard();

	switch (GetTitle())
	{// どのゲーム？
	case MENU_WALKING:		// ウォーキング

		// ビルボードの読み込み
		LoadBillboard();

		break;

	case MENU_STOP:			// 止める

		/* 処理なし */

		break;

	case MENU_SLOPE:		// 坂

		// 坂のビルボードの初期化
		InitBillboardSlope();

		break;

	default:
		assert(false);
		break;
	}

	// 弾の初期化
	//InitBullet();

	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	// 数の初期化
	InitNumber();

	if (GetTitle() != MENU_STOP)
	{// 止めるじゃない
		// タイムの初期化
		InitTime();
	}

	// ターゲットの初期化
	InitTarget();

	// カウントダウンの初期化
	InitCountdown();

	// ルールの初期化
	InitRule();

	s_game.gameState = GAMESTATE_START;

	s_nTime = 0;

	// リザルトの設定
	SetResult(RESULT_NONE);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitGame(void)
{
	// ポリゴンの終了
	UninitPolygon();

	// フィールドの終了
	UninitField();

	// メッシュフィールドの終了
	UninitMeshField();

	// メッシュ円柱の終了
	//UninitMeshCylinder();

	// メッシュ球の終了
	//UninitMeshSphere();

	// メッシュ空の終了
	//UninitMeshSky();

	// 壁の終了
	UninitWall();

	// ビルボードの終了
	UninitBillboard();

	// エフェクトの終了
	UninitEffect();

	// 影の終了
	UninitShadow();

	// 線の終了
	//UninitLine();

	// モデルの終了
	UninitModel();

	// プレイヤーの終了
	UninitPlayer();

	// 弾の終了
	//UninitBullet();

	// カメラの終了
	UninitCamera();

	// ライトの終了
	UninitLight();

	// 数の終了
	UninitNumber();

	// リザルトの終了
	UninitResult();

	// カウントダウンの終了
	UninitCountdown();

	// ターゲットの終了
	UninitTarget();

	// ルールの終了
	UninitRule();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateGame(void)
{
	switch (s_game.gameState)
	{
	case GAMESTATE_START:		// 開始状態(ゲーム開始中)
		s_nTime++;

		if (s_nTime >= 120)
		{
			s_nTime = 0;

			// ゲームの設定
			SetGameState(GAMESTATE_COUNTDOWN);
		}

		break;

	case GAMESTATE_COUNTDOWN:		// カウントダウン状態 (ゲーム開始中)

		//カウントダウンの加算
		AddCountdown(1);

		//カウントダウンの更新
		UpdateCountdown();

		switch (GetTitle())
		{// どのゲーム？
		case MENU_WALKING:		// ウォーキング
		case MENU_SLOPE:		// 坂

			if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
			{// ←, →, ↑, ↓キーが押された
			 // 次のモーション
				NextMotion(MOTION_MOVE);
			}

			break;

		case MENU_STOP:			// 止める

			/* 処理なし */

			break;

		default:
			assert(false);
			break;
		}

		break;

	case GAMESTATE_NORMAL:		// 通常状態(ゲーム進行中)

		//カウントダウンの加算
		AddCountdown(1);

		//カウントダウンの更新
		UpdateCountdown();

		if (GetCountdown())
		{
			if (!s_game.bPause)
			{
				switch (GetTitle())
				{// どのゲーム？
				case MENU_WALKING:		// ウォーキング
				case MENU_STOP:			// 止める

					// ポリゴンの更新
					UpdatePolygon();

					break;

				case MENU_SLOPE:		// 坂

					// フィールドの更新
					UpdateField();

					// メッシュフィールドの更新
					UpdateMeshField();

					// 壁の更新
					UpdateWall();

					break;

				default:
					assert(false);
					break;
				}

				// メッシュ円柱の更新
				//UpdateMeshCylinder();

				// メッシュ球の更新
				//UpdateMeshSphere();

				// メッシュ空の更新
				//UpdateMeshSky();

				switch (GetTitle())
				{// どのゲーム？
				case MENU_WALKING:		// ウォーキング
				case MENU_SLOPE:		// 坂

					// ビルボードの更新
					UpdateBillboard();

					break;

				case MENU_STOP:			// 止める

					/* 処理なし */

					break;

				default:
					assert(false);
					break;
				}

				// モデルの更新
				UpdateModel();

				// 線の更新
				//UpdateLine();

				// 弾の更新
				//UpdateBullet();
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

			if (GetTitle() != MENU_STOP)
			{// 止めるじゃない
				// タイムの減算
				SubTime(1);
			}
		}

		break;

	case GAMESTATE_END:		// 終了状態(ゲーム終了時)


		switch (GetTitle())
		{// どのゲーム？
		case MENU_SLOPE:		// 坂

			/* 処理なし */

			break;

		case MENU_WALKING:		// ウォーキング
		case MENU_STOP:			// 止める

			s_nTime++;

			if (s_nTime >= 120)
			{
				s_nTime = 0;

				// リザルトの初期化
				InitResult();

				// ゲームの設定
				SetGameState(GAMESTATE_RESULT);

				if (GetTitle() == MENU_WALKING)
				{// 止める
					// カメラの初期化
					InitCamera();
				}
			}

			break;

		default:
			assert(false);
			break;
		}

		break;

	case GAMESTATE_RESULT:		// リザルト状態(ゲーム終了後)

		if (GetOverlap())
		{// 重なった
			// リザルトの更新
			UpdateResult();
		}

		break;

	default:
		assert(false);
		break;
	}

	// パーティクルの更新
	UpdateParticle();

	// エフェクトの更新
	UpdateEffect();

	// 数の更新
	UpdateNumber();

	// ルールの更新
	UpdateRule();

	// ターゲットの更新
	UpdateTarget();

	// プレイヤーの更新
	UpdatePlayer();

	// 影の更新
	UpdateShadow();

	// カメラの更新
	UpdateCamera();

	// ライトの更新
	UpdateLight();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawGame(void)
{
	// カメラの設定
	SetCamera();

	switch (GetTitle())
	{// どのゲーム？
	case MENU_WALKING:		// ウォーキング
	case MENU_STOP:			// 止める

		// ポリゴンの描画
		DrawPolygon();

		break;

	case MENU_SLOPE:		// 坂

		// フィールドの描画
		DrawField();

		// メッシュフィールドの描画
		DrawMeshField();

		// 壁の描画
		DrawWall();

		break;

	default:
		assert(false);
		break;
	}

	// メッシュ空の描画
	//DrawMeshSky();

	// モデルの描画
	DrawModel();

	// プレイヤーの描画
	DrawPlayer();

	// 線の描画
	//DrawLine();

	// 影の描画
	DrawShadow();

	// メッシュ球の描画
	//DrawMeshSphere();

	// メッシュ円柱の描画
	//DrawMeshCylinder();

	switch (GetTitle())
	{// どのゲーム？
	case MENU_WALKING:		// ウォーキング
	case MENU_SLOPE:		// 坂

		// ビルボードの描画
		DrawBillboard(false);

		break;

	case MENU_STOP:			// 止める

		/* 処理なし */

		break;

	default:
		assert(false);
		break;
	}

	// エフェクトの描画
	DrawEffect();

	// ターゲットの描画
	DrawTarget();

	if (s_game.gameState != GAMESTATE_START)
	{
		// 数の描画
		DrawNumber(USE_GAME);

		// ルールの描画
		DrawRule();
	}

	if (s_game.gameState == GAMESTATE_START)
	{
		// 数の描画
		DrawNumber(USE_START);
	}

	if (s_game.gameState == GAMESTATE_NORMAL ||
		s_game.gameState == GAMESTATE_COUNTDOWN ||
		s_game.gameState == GAMESTATE_END)
	{
		// 数の描画
		DrawNumber(USE_GAME_ONLY);
	}

	if (s_game.gameState == GAMESTATE_RESULT)
	{
		switch (GetTitle())
		{// どのゲーム？
		case MENU_WALKING:		// ウォーキング
		case MENU_SLOPE:		// 坂

			// ビルボードの描画
			DrawBillboard(true);

			break;

		case MENU_STOP:			// 止める

			/* 処理なし */

			break;

		default:
			assert(false);
			break;
		}

		if (GetOverlap())
		{// 重なった
			// リザルトの描画
			DrawResult();

			// 数の描画
			DrawNumber(USE_RESULT);
		}
	}

	//カウントダウンの描画
	DrawCountdown();
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

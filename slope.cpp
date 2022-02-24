//==================================================
// 
// 3Dゲーム制作 ( slope.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "camera.h"
#include "countdown.h"
#include "field.h"
#include "game.h"
#include "light.h"
#include "mesh_field.h"
#include "number.h"
#include "player.h"
#include "result.h"
#include "rule.h"
#include "shadow.h"
#include "slope.h"
#include "sound.h"
#include "target.h"
#include "time.h"
#include "wall.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define CHANGE_TIME		(120)		// タイムの次への変更値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static int		s_nTime;		// タイム

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitSlope(void)
{
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

	// 影の初期化
	InitShadow();

	// プレイヤーの読み込み
	LoadPlayer();

	// プレイヤーの初期化
	InitPlayer();

	// ビルボードの初期化
	InitBillboard();

	// 坂のビルボードの初期化
	InitBillboardSlope();

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

	// カウントダウンの初期化
	InitCountdown();

	// ルールの初期化
	InitRule();

	s_nTime = 0;

	// リザルトの設定
	SetResult(RESULT_NONE);

	// サウンドの再生
	PlaySound(SOUND_LABEL_坂);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitSlope(void)
{
	// サウンドの停止
	StopSound();

	// フィールドの終了
	UninitField();

	// メッシュフィールドの終了
	UninitMeshField();

	// 壁の終了
	UninitWall();

	// ビルボードの終了
	UninitBillboard();

	// 影の終了
	UninitShadow();

	// プレイヤーの終了
	UninitPlayer();

	// カメラの終了
	UninitCamera();

	// ライトの終了
	UninitLight();

	// 数の終了
	UninitNumber();

	// カウントダウンの終了
	UninitCountdown();

	// ターゲットの終了
	UninitTarget();

	// ルールの終了
	UninitRule();

	// リザルトの終了
	UninitResult();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateSlope(void)
{
	switch (GetGame())
	{
	case GAMESTATE_START:		// 開始状態(ゲーム開始中)
		s_nTime++;

		if (s_nTime >= CHANGE_TIME)
		{// 指定の値を越した
			s_nTime = 0;

			// ゲームの設定
			SetGameState(GAMESTATE_COUNTDOWN);

			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_カウントダウン);
		}

		break;

	case GAMESTATE_COUNTDOWN:		// カウントダウン状態 (ゲーム開始中)

		//カウントダウンの加算
		AddCountdown(1);

		//カウントダウンの更新
		UpdateCountdown();

		break;

	case GAMESTATE_NORMAL:		// 通常状態(ゲーム進行中)

		//カウントダウンの加算
		AddCountdown(1);

		//カウントダウンの更新
		UpdateCountdown();

		if (GetCountdown())
		{
			// フィールドの更新
			UpdateField();

			// メッシュフィールドの更新
			UpdateMeshField();

			// 壁の更新
			UpdateWall();

			// タイムの減算
			SubTime(1);
		}

		break;

	case GAMESTATE_END:		// 終了状態(ゲーム終了時)

		/* 処理なし */

		break;

	case GAMESTATE_RESULT:		// リザルト状態(ゲーム終了後)

		// リザルトの更新
		UpdateResult();

		break;

	default:
		assert(false);
		break;
	}

	// ビルボードの更新
	UpdateBillboard();

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
void DrawSlope(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nMax = 1;

	switch (GetGame())
	{
	case GAMESTATE_START:			// 開始状態(ゲーム開始中)

		/* 処理なし */

		break;

	case GAMESTATE_END:				// 終了状態(ゲーム終了時)

		GetCamera(1)->viewport.X = (DWORD)0.0f;
		GetCamera(1)->viewport.Y = (DWORD)(SCREEN_HEIGHT * 0.0f);
		GetCamera(1)->viewport.Width = (DWORD)(SCREEN_WIDTH * 1.0f);
		GetCamera(1)->viewport.Height = (DWORD)(SCREEN_HEIGHT * 1.0f);

		/* breakなし */

	case GAMESTATE_COUNTDOWN:		// カウントダウン状態 (ゲーム開始中)
	case GAMESTATE_NORMAL:			// 通常状態(ゲーム進行中)
	case GAMESTATE_RESULT:			// リザルト状態(ゲーム終了後)

		nMax = 2;

		break;

	default:
		assert(false);
		break;
	}

	D3DVIEWPORT9 viewport;
	pDevice->GetViewport(&viewport);

	for (int nCntCamera = 0; nCntCamera < nMax; nCntCamera++)
	{
		bool bCamera = false;

		if (nCntCamera == 0)
		{
			bCamera = false;
		}
		else if (nCntCamera == 1)
		{
			bCamera = true;
		}

		// ビューボードのクリア
		pDevice->SetViewport(&GetCamera(nCntCamera)->viewport);

		// 画面クリア(バックバッファ＆Zバッファのクリア)
		pDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		// カメラの設定
		SetCamera(nCntCamera);

		// フィールドの描画
		DrawField();

		// メッシュフィールドの描画
		DrawMeshField();

		// 壁の描画
		DrawWall();

		// プレイヤーの描画
		DrawPlayer();

		// 影の描画
		DrawShadow();

		// ビルボードの描画
		DrawBillboard(false, bCamera);

		// ターゲットの描画
		DrawTarget();

		if (GetGame() != GAMESTATE_START)
		{
			// 数の描画
			DrawNumber2D(USE_GAME);
		}

		switch (GetGame())
		{
		case GAMESTATE_START:			// 開始状態(ゲーム開始中)

			// 数の描画
			DrawNumber2D(USE_START);

			break;

		case GAMESTATE_COUNTDOWN:		// カウントダウン状態 (ゲーム開始中)
		case GAMESTATE_NORMAL:			// 通常状態(ゲーム進行中)

			// ルールの描画
			DrawRule();

			// 数の描画
			DrawNumber2D(USE_GAME_ONLY);

			break;

		case GAMESTATE_END:				// 終了状態(ゲーム終了時)

			/* 処理なし */

			break;

		case GAMESTATE_RESULT:			// リザルト状態(ゲーム終了後)

			// ビルボードの描画
			DrawBillboard(true, bCamera);

			// リザルトの描画
			DrawResult();

			// 数の描画
			DrawNumber2D(USE_RESULT);

			break;

		default:
			assert(false);
			break;
		}

		//カウントダウンの描画
		DrawCountdown();
	}

	// ビューボードのクリア
	pDevice->SetViewport(&viewport);
}

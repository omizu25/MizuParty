//==================================================
// 
// 3Dゲーム制作 ( walking.cpp )
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
#include "game.h"
#include "light.h"
#include "loop.h"
#include "number.h"
#include "player.h"
#include "polygon.h"
#include "result.h"
#include "rule.h"
#include "shadow.h"
#include "sound.h"
#include "target.h"
#include "time.h"
#include "walking.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define CHANGE_TIME		(120)		// タイムの次への変更値
#define MAX_CAMERA		(1)			// カメラの最大数

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static int		s_nTime;		// タイム

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitWalking(void)
{
	// ポリゴンの初期化
	InitPolygon();

	// 影の初期化
	InitShadow();

	// プレイヤーの読み込み
	LoadPlayer();

	// プレイヤーの初期化
	InitPlayer();

	// ビルボードの初期化
	InitBillboard();

	// ビルボードの読み込み
	LoadBillboard();

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
	PlaySound(SOUND_LABEL_ウォーキング);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitWalking(void)
{
	// サウンドの停止
	StopSound();

	// ポリゴンの終了
	UninitPolygon();

	// 影の終了
	UninitShadow();

	// プレイヤーの終了
	UninitPlayer();

	// ビルボードの終了
	UninitBillboard();

	// カメラの終了
	UninitCamera();

	// ライトの終了
	UninitLight();

	// 数の終了
	UninitNumber();

	// ターゲットの終了
	UninitTarget();

	// カウントダウンの終了
	UninitCountdown();

	// ルールの終了
	UninitRule();

	// リザルトの終了
	UninitResult();

	// ループの終了
	UninitLoop();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateWalking(void)
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
			// ポリゴンの更新
			UpdatePolygon();

			// タイムの減算
			SubTime(1);
		}

		break;

	case GAMESTATE_END:		// 終了状態(ゲーム終了時)

		s_nTime++;

		if (s_nTime >= CHANGE_TIME)
		{// 指定の値を越した
			s_nTime = 0;

			// ゲームの設定
			SetGameState(GAMESTATE_RESULT);

			// カメラの初期化
			InitCamera();
		}

		break;

	case GAMESTATE_RESULT:		// リザルト状態(ゲーム終了後)

		if (GetOverlap())
		{// 重なった
			// リザルトの更新
			UpdateResult();
		}

		break;

	case GAMESTATE_LOOP:		// 繰り返し状態 (リザルト終了後)

		UpdateLoop();

		break;

	default:
		assert(false);
		break;
	}

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
void DrawWalking(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DVIEWPORT9 viewport;
	pDevice->GetViewport(&viewport);

	// ビューボードのクリア
	pDevice->SetViewport(&GetCamera(0)->viewport);

	// 画面クリア(バックバッファ＆Zバッファのクリア)
	pDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// カメラの設定
	SetCamera(0);

	// ポリゴンの描画
	DrawPolygon();

	// プレイヤーの描画
	DrawPlayer();

	// 影の描画
	DrawShadow();

	// ビルボードの描画
	DrawBillboard(false, false);

	if (GetGame() != GAMESTATE_LOOP)
	{
		// ターゲットの描画
		DrawTarget();

		if (GetGame() != GAMESTATE_START)
		{
			// 数の描画
			DrawNumber2D(USE_GAME);
		}
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
		DrawBillboard(true, false);
		
		if (GetOverlap())
		{// 重なった
			// リザルトの描画
			DrawResult();

			// 数の描画
			DrawNumber2D(USE_RESULT);
		}

		break;
	case GAMESTATE_LOOP:			// 繰り返し状態 (リザルト終了後)

		// ループの描画
		DrawLoop();

		break;

	default:
		assert(false);
		break;
	}

	//カウントダウンの描画
	DrawCountdown();

	// ビューボードのクリア
	pDevice->SetViewport(&viewport);
}

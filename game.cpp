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
#include "game.h"
#include "rotation.h"
#include "slope.h"
#include "stop.h"
#include "title.h"
#include "walking.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define CHANGE_TIME		(120)		// タイムの次への変更値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static GAMESTATE		s_gameState;					// ゲーム情報
static int				s_nTime;						// タイム
static bool				s_bRemix;						// リミックス
static bool				s_bRemixWalking = false;		// リミックスのウォーキングをしたかどうか

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitGame(void)
{
	s_gameState = GAMESTATE_START;

	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
		InitWalking();
		break;

	case MENU_ROTATION:		// 回転
		InitRotation();
		break;

	case MENU_STOP:			// 止める
		InitStop();
		break;

	case MENU_SLOPE:		// 坂
		InitSlope();
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitGame(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
		UninitWalking();
		break;

	case MENU_ROTATION:		// 回転
		UninitRotation();
		break;

	case MENU_STOP:			// 止める
		UninitStop();
		break;

	case MENU_SLOPE:		// 坂
		UninitSlope();
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateGame(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
		UpdateWalking();
		break;

	case MENU_ROTATION:		// 回転
		UpdateRotation();
		break;

	case MENU_STOP:			// 止める
		UpdateStop();
		break;

	case MENU_SLOPE:		// 坂
		UpdateSlope();
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
	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
		DrawWalking();
		break;

	case MENU_ROTATION:		// 回転
		DrawRotation();
		break;

	case MENU_STOP:			// 止める
		DrawStop();
		break;

	case MENU_SLOPE:		// 坂
		DrawSlope();
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	s_gameState = state;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
GAMESTATE GetGame(void)
{
	return s_gameState;
}

//--------------------------------------------------
// リミックスの設定
//--------------------------------------------------
void SetRemix(bool bRemix)
{
	s_bRemix = bRemix;
	s_bRemixWalking = false;
}

//--------------------------------------------------
// リミックスの取得
//--------------------------------------------------
bool GetRemix(void)
{
	return s_bRemix;
}
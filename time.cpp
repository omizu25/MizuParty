//==================================================
// 
// 3Dゲーム制作 ( time.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "game.h"
#include "number.h"
#include "time.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define NUMBER_WIDTH		(80.0f)			// 幅
#define NUMBER_HEIGHT		(140.0f)		// 高さ
#define WIDTH_INTERVAL		(5.0f)			// 幅の間隔
#define START_TIME			(10)			// 最初の値
#define ONE_SECONDS			(60)			// １秒

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static D3DXVECTOR3		s_pos;			// 位置
static int				s_nTime;		// 値
static int				s_nSecond;		// １秒間隔を数える

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitTime(void)
{
	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.15f;

	// 位置を初期化
	s_pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	s_nTime = START_TIME;

	int nTime = s_nTime;
	int aNumber[MAX_TIME];

	for (int i = 0; i < MAX_TIME; i++)
	{
		aNumber[i] = 0;
	}

	for (int i = 0; i < MAX_TIME; i++)
	{// １桁ずつに分ける
		aNumber[i] = nTime % 10;
		nTime /= 10;

		float fInterval = (NUMBER_WIDTH * i) + (WIDTH_INTERVAL * i);
		float fCenter = (NUMBER_WIDTH * 0.5) * (MAX_TIME - 1) + (WIDTH_INTERVAL * 0.5f) * (MAX_TIME - 1);

		// 数の設定
		SetRightNumber(D3DXVECTOR3((s_pos.x) - fInterval + fCenter + (NUMBER_WIDTH * 0.5f), s_pos.y, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_GAME_ONLY);
	}
}

//--------------------------------------------------
// 減算
//--------------------------------------------------
void SubTime(int nValue)
{
	s_nSecond += nValue;

	if (s_nSecond % ONE_SECONDS == 0 && s_nTime > 0)
	{// １秒経過、まだ時間がある
		s_nTime--;

		int nTime = s_nTime;
		int aNumber[MAX_TIME];

		for (int i = 0; i < MAX_TIME; i++)
		{// １桁ずつに分ける
			aNumber[i] = nTime % 10;
			nTime /= 10;

			// 数のテクスチャ
			TexNumber(aNumber[i], i, USE_GAME_ONLY);
		}
	}
	else if (s_nTime <= 0)
	{// 制限時間が来た
		// ゲームの設定
		SetGameState(GAMESTATE_END);
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
int GetTime(void)
{
	return s_nTime;
}

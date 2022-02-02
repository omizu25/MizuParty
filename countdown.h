//--------------------------------------------------
//
// 3Dゲーム制作 ( countdown.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _COUNTDOWN_H_		//このマクロ定義がされてなかったら
#define _COUNTDOWN_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitCountdown(void);
void UninitCountdown(void);
void UpdateCountdown(void);
void DrawCountdown(void);
void AddCountdown(int nValue);
void SetCountdown(int Countdown);
bool GetCountdown(void);

#endif // !_COUNTDOWN_H_
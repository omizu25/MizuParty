//==================================================
//
// 3Dゲーム制作 ( time.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _TIME_H_		// このマクロ定義がされてなかったら
#define _TIME_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_TIME		(2)		// タイムの最大桁数

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitTime(void);
void SubTime(int nValue);
int GetTime(void);

#endif // !_TIME_H_

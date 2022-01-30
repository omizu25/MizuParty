//==================================================
//
// 3Dゲーム制作 ( result.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _RESULT_H_		//このマクロ定義がされてなかったら
#define _RESULT_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
// 列挙型
//--------------------------------------------------
typedef enum
{
	RESULT_NONE = -1,		// まだ決まってない
	RESULT_WIN,				// 勝ち
	RESULT_LOSE,			// 負け
	RESULT_MAX
}RESULT;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResult(RESULT Result);

#endif // !_RESULT_H_
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
	RESULT_NONE = 0,		// まだ決まってない
	RESULT_CLEAR,			// クリア
	RESULT_GAMEOVER,		// ゲームオーバー
}RESULT;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResult(RESULT result);
RESULT GetResult(void);

#endif // !_RESULT_H_
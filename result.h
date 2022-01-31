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
// プロトタイプ宣言
//--------------------------------------------------
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif // !_RESULT_H_
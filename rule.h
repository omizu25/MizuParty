//==================================================
//
// 3Dゲーム制作 ( rule.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _RULE_H_		//このマクロ定義がされてなかったら
#define _RULE_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitRule(void);
void UninitRule(void);
void UpdateRule(void);
void DrawRule(void);

#endif // !_RULE_H_
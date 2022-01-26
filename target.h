//==================================================
//
// 3Dゲーム制作 ( target.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _TARGET_H_		// このマクロ定義がされてなかったら
#define _TARGET_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitTarget(void);
void UninitTarget(void);
void UpdateTarget(void);
void DrawTarget(void);

#endif // !_TARGET_H_

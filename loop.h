//==================================================
//
// 3Dゲーム制作 ( loop.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _LOOP_H_		//このマクロ定義がされてなかったら
#define _LOOP_H_		//２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitLoop(void);
void UninitLoop(void);
void UpdateLoop(void);
void DrawLoop(void);

#endif // !_LOOP_H_
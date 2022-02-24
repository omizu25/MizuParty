//==================================================
//
// 3Dゲーム制作 ( walking.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _WALKING_H_		// このマクロ定義がされてなかったら
#define _WALKING_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitWalking(void);
void UninitWalking(void);
void UpdateWalking(void);
void DrawWalking(void);

#endif // !_WALKING_H_

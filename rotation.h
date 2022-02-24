//==================================================
//
// 3Dゲーム制作 ( rotation.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _ROTATION_H_		// このマクロ定義がされてなかったら
#define _ROTATION_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitRotation(void);
void UninitRotation(void);
void UpdateRotation(void);
void DrawRotation(void);

#endif // !_ROTATION_H_

//==================================================
//
// 3Dゲーム制作 ( camera.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _CAMERA_H_		// このマクロ定義がされてなかったら
#define _CAMERA_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

#endif // !_CAMERA_H_
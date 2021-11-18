//==================================================
//
// 3Dゲーム制作 ( polygon.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _POLYGON_H_		// このマクロ定義がされてなかったら
#define _POLYGON_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);

#endif // !_POLYGON_H_
//==================================================
//
// 3Dゲーム制作 ( measure.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MEASURE_H_		// このマクロ定義がされてなかったら
#define _MEASURE_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitMeasure(void);
void UninitMeasure(void);
void UpdateMeasure(void);
void DrawMeasure(void);

#endif // !_MEASURE_H_
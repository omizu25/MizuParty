//==================================================
//
// 3Dゲーム制作 ( model.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MODEL_H_		// このマクロ定義がされてなかったら
#define _MODEL_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);

#endif // !_MODEL_H_
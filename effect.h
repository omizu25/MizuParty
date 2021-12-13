//--------------------------------------------------
//
// 3Dゲーム制作 ( effect.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _EFFECT_H_		// このマクロ定義がされてなかったら
#define _EFFECT_H_		// ２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nLife, bool bAdd);

#endif // !_EFFECT_H_

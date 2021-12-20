//--------------------------------------------------
//
// 3Dゲーム制作 ( particle.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _PARTICLE_H_		// このマクロ定義がされてなかったら
#define _PARTICLE_H_		// ２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitParticle(void);
void UpdateParticle(void);
void SetParticle(D3DXVECTOR3 pos, float fSize, bool bAdd);

#endif // !_PARTICLE_H_

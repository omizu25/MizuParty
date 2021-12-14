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
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nLife, bool bAdd);

#endif // !_PARTICLE_H_

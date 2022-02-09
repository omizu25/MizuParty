//==================================================
//
// 3Dゲーム制作 ( frame.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _FRAME_H_		// このマクロ定義がされてなかったら
#define _FRAME_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitFrame(void);
void UninitFrame(void);
void UpdateFrame(void);
void DrawFrame(void);
void SetFrame(D3DXVECTOR3 pos, float fWidth, float fHeight, LPDIRECT3DTEXTURE9 *pTexture);
void CollisionFrame(D3DXVECTOR3 *pPos);

#endif // !_FRAME_H_

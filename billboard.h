//==================================================
//
// 3Dゲーム制作 ( billboard.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _BILLBOARD_H_		// このマクロ定義がされてなかったら
#define _BILLBOARD_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(bool bResult, bool bCamera);
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, bool bYRot, bool bResult, bool bCamera, LPDIRECT3DTEXTURE9 *pTexture);
void LoadBillboard(void);
void InitBillboardSlope(void);

#endif // !_BILLBOARD_H_

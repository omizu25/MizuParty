//==================================================
//
// 3Dゲーム制作 ( shadow.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _SHADOW_H_		// このマクロ定義がされてなかったら
#define _SHADOW_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize);
void SetPosShadow(int nIdxShadow, D3DXVECTOR3 pos);
void UseStopShadow(int nIdxShadow);

#endif // !_SHADOW_H_

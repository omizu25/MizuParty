//==================================================
//
// 3Dゲーム制作 ( line.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _LINE_H_		// このマクロ定義がされてなかったら
#define _LINE_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitLine(void);
void UninitLine(void);
void UpdateLine(void);
void DrawLine(void);
void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col);

#endif // !_LINE_H_

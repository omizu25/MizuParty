//==================================================
//
// 3Dゲーム制作 ( number.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _NUMBER_H_		//このマクロ定義がされてなかったら
#define _NUMBER_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitNumber(void);
void UninitNumber(void);
void UpdateNumber(void);
void DrawNumber(void);
void SetRightNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, int nRank);
void SetMiddleNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber);
void TexNumber(int nNumber, int nDigit);
void UseNumber(int nDigit);

#endif // !_NUMBER_H_

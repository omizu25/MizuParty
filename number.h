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
// 列挙型
//--------------------------------------------------
typedef enum
{
	USE_NONE = 0,		// なし
	USE_GAME,			// ゲーム
	USE_RESULT,			// リザルト
	USE_MAX
}USE;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitNumber(void);
void UninitNumber(void);
void UpdateNumber(void);
void DrawNumber(USE use);
void SetRightNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, USE use);
void SetMiddleNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, USE use);
void TexNumber(int nNumber, int nDigit, USE use);

#endif // !_NUMBER_H_

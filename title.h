//==================================================
//
// 3Dゲーム制作 ( title.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _TITLE_H_		//このマクロ定義がされてなかったら
#define _TITLE_H_		//２重インクルード防止のマクロ定義

//--------------------------------------------------
// 列挙型
//--------------------------------------------------
typedef enum
{
	MENU_WALKING = 0,		// ウォーキング
	MENU_STOP,				// 止める
	MENU_SLOPE,				// 坂
	MENU_MAX
}MENU;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void SetTitle(MENU menu);
MENU GetTitle(void);

#endif // !_TITLE_H_
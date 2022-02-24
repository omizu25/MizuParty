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
	MENU_ROTATION,			// 回転
	MENU_STOP,				// 止める
	MENU_SLOPE,				// 坂
	MENU_MAX
}MENU;

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define REMIX_OK		(2)		// リミックスが出来るようになる数

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void SetTitle(MENU menu);
MENU GetTitle(void);
void SetTitleCnt(void);
int GetTitleCnt(void);

#endif // !_TITLE_H_
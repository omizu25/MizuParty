//==================================================
// 
// 3Dゲーム制作 ( target.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MIN_TARGET		(50)		// 目標の最小値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static int		s_nTarget;		// 目標地点

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitTarget(void)
{
	s_nTarget = (rand() % 50) + MIN_TARGET;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTarget(void)
{
	
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateTarget(void)
{
	
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawTarget(void)
{
	
}

//==================================================
//
// 3Dゲーム制作 ( ranking.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _RANKING_H_		//このマクロ定義がされてなかったら
#define _RANKING_H_		//２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitRanking(void);
void UninitRanking(void);
void LoadRanking(void);
void SaveRanking(void);
void SetScoreWalking(int nScore);
void SetScoreStop(int nScore);
void SetScoreSlope(int nScore);

#endif // !_RANKING_H_
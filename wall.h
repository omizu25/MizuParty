//==================================================
//
// 3Dゲーム制作 ( wall.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _WALL_H_		// このマクロ定義がされてなかったら
#define _WALL_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, bool bCulling);
void InstallationWall(void);

#endif // !_WALL_H_
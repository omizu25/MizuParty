//==================================================
//
// 3Dゲーム制作 ( wall.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _WALL_H_		// このマクロ定義がされてなかったら
#define _WALL_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_WALL		(64)		// 壁の最大数

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	bool			bUse;			// 使用しているかどうか
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
}Wall;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, float fURight, bool bCulling);
void InstallationWall(void);
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size);
Wall *GetWall(void);
void ResetWall(void);

#endif // !_WALL_H_

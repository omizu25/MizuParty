//==================================================
//
// 3Dゲーム制作 ( player.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _PLAYER_H_		// このマクロ定義がされてなかったら
#define _PLAYER_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXVECTOR3		rotDest;		// 目的の向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	int				nIdxShadow;		// 使用している影の番号
	int				nStopTime;		// 止まっている時間
}Player;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif // !_PLAYER_H_

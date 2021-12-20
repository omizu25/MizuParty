//==================================================
//
// 3Dゲーム制作 ( player.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _PLAYER_H_		// このマクロ定義がされてなかったら
#define _PLAYER_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_PARTS		(2)		// パーツの最大数

//--------------------------------------------------
// 構造体
//--------------------------------------------------

/*↓ プレイヤーパーツ ↓*/

typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	D3DXVECTOR3				rot;			// 向き
	D3DXVECTOR3				rotDest;		// 目的の向き
	LPD3DXMESH				pMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER			pBuffMat;		// マテリアル情報へのポインタ
	LPDIRECT3DTEXTURE9		*pTexture;		// テクスチャへのポインタ
	DWORD					nNumMat;		// マテリアル情報の数
	D3DXMATRIX				mtxWorld;		// ワールドマトリックス
	int						nIdxParent;		// 親の番号
}PlayerParts;

/*↓ プレイヤー ↓*/

typedef struct
{
	D3DXVECTOR3		pos;					// 位置
	D3DXVECTOR3		posOld;					// 前回の位置
	D3DXVECTOR3		vtxMin;					// 最小値
	D3DXVECTOR3		vtxMax;					// 最大値
	D3DXVECTOR3		rot;					// 向き
	D3DXVECTOR3		rotDest;				// 目的の向き
	D3DXMATRIX		mtxWorld;				// ワールドマトリックス
	PlayerParts		parts[MAX_PARTS];		// パーツの情報
	int				nIdxShadow;				// 使用している影の番号
	int				nStopTime;				// 止まっている時間
}Player;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void LoadPlayer(HWND hWnd);

#endif // !_PLAYER_H_

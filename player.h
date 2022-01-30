//==================================================
//
// 3Dゲーム制作 ( player.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _PLAYER_H_		// このマクロ定義がされてなかったら
#define _PLAYER_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 列挙型
//--------------------------------------------------
typedef enum
{
	MOTION_NEUTRAL = 0,		// ニュートラル
	MOTION_MOVE,			// 移動
	MOTION_ATTACK,			// 攻撃
	MOTION_JUMP,			// ジャンプ
	MOTION_LANDING,			// 着地
	MOTION_MAX
}MOTION;

//--------------------------------------------------
// 構造体
//--------------------------------------------------

/*↓ キー ↓*/

typedef struct
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
}Key;

/*↓ キー設定 ↓*/

typedef struct
{
	int		nFrame;		// フレーム数
	Key		*key;		// キー
}KeySet;

/*↓ モーション設定 ↓*/

typedef struct
{
	bool		bLoop;			// ループするかどうか
	int			nNumKey;		// キー数
	KeySet		*keySet;		// キー設定
}MotionSet;

/*↓ プレイヤーパーツ ↓*/

typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	D3DXVECTOR3				posOld;			// 前回の位置
	D3DXVECTOR3				posSet;			// 設定の位置
	D3DXVECTOR3				rot;			// 向き
	D3DXVECTOR3				rotOld;			// 前回の向き
	D3DXVECTOR3				rotSet;			// 設定の向き
	LPD3DXMESH				pMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER			pBuffMat;		// マテリアル情報へのポインタ
	LPDIRECT3DTEXTURE9		*pTexture;		// テクスチャへのポインタ
	DWORD					nNumMat;		// マテリアル情報の数
	D3DXMATRIX				mtxWorld;		// ワールドマトリックス
	int						nIdxModel;		// 使用するモデルの番号
	int						nIdxParent;		// 親の番号
}PlayerParts;

/*↓ プレイヤー ↓*/

typedef struct
{
	D3DXVECTOR3		pos;					// 位置
	D3DXVECTOR3		posOld;					// 前回の位置
	D3DXVECTOR3		rot;					// 向き
	D3DXVECTOR3		rotDest;				// 目的の向き
	D3DXMATRIX		mtxWorld;				// ワールドマトリックス
	PlayerParts		*parts;					// パーツの情報
	MotionSet		*Motion;				// モーションの情報
	int				nNumMotion;				// モーション数
	int				nNumParts;				// パーツ数
	int				nNumName;				// ファイルネーム数
	int				nIdxShadow;				// 使用している影の番号
	int				nStopTime;				// 止まっている時間
	float			fMove;					// 移動量
	float			fJump;					// ジャンプ量
	float			fSize;					// サイズ
	float			fHeight;				// 高さ
	char			aText[MAX_TEXT];		// テキスト名
}Player;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void LoadPlayer(void);

#endif // !_PLAYER_H_

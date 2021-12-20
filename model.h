//==================================================
//
// 3Dゲーム制作 ( model.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MODEL_H_		// このマクロ定義がされてなかったら
#define _MODEL_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_PARTS		(2)		// パーツの最大数

//--------------------------------------------------
// 構造体
//--------------------------------------------------

/*↓ モデルパーツ ↓*/

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
}ModelParts;

/*↓ モデル ↓*/

typedef struct
{
	D3DXVECTOR3				pos;					// 位置
	D3DXVECTOR3				vtxMin;					// 最小値
	D3DXVECTOR3				vtxMax;					// 最大値
	D3DXVECTOR3				rot;					// 向き
	D3DXVECTOR3				rotDest;				// 目的の向き
	D3DXMATRIX				mtxWorld;				// ワールドマトリックス
	ModelParts				parts[MAX_PARTS];		// パーツの情報
	int						nIdxShadow;				// 使用している影の番号
}Model;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model *GetModel(void);

#endif // !_MODEL_H_

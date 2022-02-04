//==================================================
//
// 3Dゲーム制作 ( model.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MODEL_H_		// このマクロ定義がされてなかったら
#define _MODEL_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	D3DXVECTOR3				vtxMin;			// 最小値
	D3DXVECTOR3				vtxMax;			// 最大値
	D3DXVECTOR3				rot;			// 向き
	D3DXVECTOR3				rotDest;		// 目的の向き
	D3DXMATRIX				mtxWorld;		// ワールドマトリックス
	LPD3DXMESH				pMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER			pBuffMat;		// マテリアル情報へのポインタ
	LPDIRECT3DTEXTURE9		*pTexture;		// テクスチャへのポインタ
	DWORD					nNumMat;		// マテリアル情報の数
	float					fMove;			// 移動量
	int						nIdxShadow;		// 使用している影の番号
	bool					bUse;			// 使用しているかどうか
}Model;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model *GetModel(void);
bool GetStop(void);
void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size);
void CollisionStop(D3DXVECTOR3 *pPos, D3DXVECTOR3 size);

#endif // !_MODEL_H_

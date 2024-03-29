//==================================================
//
// 3Dゲーム制作 ( field.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _FIELD_H_		// このマクロ定義がされてなかったら
#define _FIELD_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	D3DXVECTOR3				vtxMin;			// 最小値
	D3DXVECTOR3				vtxMax;			// 最大値
	D3DXVECTOR3				rot;			// 向き
	D3DXMATRIX				mtxWorld;		// ワールドマトリックス
	LPD3DXMESH				pMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER			pBuffMat;		// マテリアル情報へのポインタ
	LPDIRECT3DTEXTURE9		*pTexture;		// テクスチャへのポインタ
	DWORD					nNumMat;		// マテリアル情報の数
	int						nIdxShadow;		// 使用している影の番号
}Field;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
Field *GetField(void);
bool CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);

#endif // !_FIELD_H_

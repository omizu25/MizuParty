//==================================================
//
// 3Dゲーム制作 ( audience.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _AUDIENCE_H_		// このマクロ定義がされてなかったら
#define _AUDIENCE_H_		// ２重インクルード防止のマクロ定義

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
	bool					bDraw;			// 描画するかどうか
}Audience;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitAudience(void);
void UninitAudience(void);
void UpdateAudience(void);
void DrawAudience(void);
void LoadAudience(void);
Audience *GetAudience(int nAudience);
int GetNumAudience(void);

#endif // !_AUDIENCE_H_

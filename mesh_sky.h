//==================================================
//
// 3Dゲーム制作 ( mesh_sky.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MESH_SKY_H_		// このマクロ定義がされてなかったら
#define _MESH_SKY_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 構造体
//--------------------------------------------------

/*↓ メッシュ空 ↓*/

typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
}MeshSky;

/*↓ メッシュ空の数系 ↓*/

typedef struct
{
	int		nHorizontal;		// 横
	int		nVertical;			// 縦
	int		nVtx;				// 頂点数
	int		nIdx;				// インデックス数
	int		nPolygon;			// ポリゴン数
}MeshSkyNumber;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitMeshSky(void);
void UninitMeshSky(void);
void UpdateMeshSky(void);
void DrawMeshSky(void);
void SetMeshSky(void);
MeshSky *GetMeshSky(void);
MeshSkyNumber *GetMeshSkyNumber(void);

#endif // !_MESH_SKY_H
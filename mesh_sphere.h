//==================================================
//
// 3Dゲーム制作 ( mesh_sphere.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MESH_SPHERE_H_		// このマクロ定義がされてなかったら
#define _MESH_SPHERE_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 構造体
//--------------------------------------------------

/*↓ メッシュ球 ↓*/

typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
}MeshSphere;

/*↓ メッシュ球の数系 ↓*/

typedef struct
{
	int		nHorizontal;		// 横
	int		nVertical;			// 縦
	int		nVtx;				// 頂点数
	int		nIdx;				// インデックス数
	int		nPolygon;			// ポリゴン数
}MeshSphereNumber;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitMeshSphere(void);
void UninitMeshSphere(void);
void UpdateMeshSphere(void);
void DrawMeshSphere(void);
void SetMeshSphere(void);
MeshSphere *GetMeshSphere(void);
MeshSphereNumber *GetMeshSphereNumber(void);

#endif // !_MESH_SPHERE_H_
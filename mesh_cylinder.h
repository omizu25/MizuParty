//==================================================
//
// 3Dゲーム制作 ( mesh_cylinder.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MESH_CYLINDER_H_		// このマクロ定義がされてなかったら
#define _MESH_CYLINDER_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 構造体
//--------------------------------------------------

/*↓ メッシュ円柱 ↓*/

typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
}MeshCylinder;

/*↓ メッシュ円柱の数系 ↓*/

typedef struct
{
	int		nHorizontal;		// 横
	int		nVertical;			// 縦
	int		nVtx;				// 頂点数
	int		nIdx;				// インデックス数
	int		nPolygon;			// ポリゴン数
}MeshCylinderNumber;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);
void SetMeshCylinder(void);
MeshCylinder *GetMeshCylinder(void);
MeshCylinderNumber *GetMeshCylinderNumber(void);

#endif // !_MESH_CYLINDER_H_

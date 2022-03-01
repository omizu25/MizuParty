//==================================================
//
// 3Dゲーム制作 ( mesh_cylinder.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MESH_CYLINDER_H_		// このマクロ定義がされてなかったら
#define _MESH_CYLINDER_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);
void SetMeshCylinder(void);

#endif // !_MESH_CYLINDER_H_

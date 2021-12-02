//==================================================
//
// 3Dゲーム制作 ( meshfield.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MESHFIELD_H_		// このマクロ定義がされてなかったら
#define _MESHFIELD_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	float			fDepth;			// 奥行き
}MeshField;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
MeshField *GetMeshField(void);

#endif // !_MESHFIELD_H_

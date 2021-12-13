//==================================================
//
// 3Dゲーム制作 ( polygon.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _POLYGON_H_		// このマクロ定義がされてなかったら
#define _POLYGON_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	float			fDepth;			// 奥行き
}polygon;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);
polygon *GetPolygon(void);

#endif // !_POLYGON_H_

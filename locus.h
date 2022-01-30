//==================================================
//
// 3Dゲーム制作 ( locus.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _LOCUS_H_		// このマクロ定義がされてなかったら
#define _LOCUS_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 構造体
//--------------------------------------------------

/*↓ 軌跡 ↓*/

typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	float			fDepth;			// 奥行き
}Locus;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitLocus(void);
void UninitLocus(void);
void UpdateLocus(void);
void DrawLocus(void);
void SetLocus(void);
Locus *GetLocus(void);

#endif // !_LOCUS_H_

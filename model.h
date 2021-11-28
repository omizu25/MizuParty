//==================================================
//
// 3Dゲーム制作 ( model.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MODEL_H_		// このマクロ定義がされてなかったら
#define _MODEL_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// モデルの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXVECTOR3		rotDest;		// 目的の向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
}Model;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model *GetModel(void);

#endif // !_MODEL_H_
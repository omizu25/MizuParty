//==================================================
//
// 3Dゲーム制作 ( billboard.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _BILLBOARD_H_		// このマクロ定義がされてなかったら
#define _BILLBOARD_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_BILLBOARD		(256)		// ビルボードの最大数

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	D3DXMATRIX				mtxWorld;		// ワールドマトリックス
	float					fWidth;			// 幅
	float					fHeight;		// 高さ
	bool					bUse;			// 使用しているかどうか
	bool					bYRot;			// Y軸回転をするかどうか
	bool					bResult;		// リザルトだけで表示する
	bool					bCamera;		// カメラ何番目か
	LPDIRECT3DTEXTURE9		pTexture;		// テクスチャ
}Billboard;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(bool bResult, bool bCamera);
void SetBillboard(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, bool bYRot, bool bResult, bool bCamera, LPDIRECT3DTEXTURE9 *pTexture);
Billboard *GetBillboard(void);
void LoadBillboard(void);
void CollisionBillboard(void);
void InitBillboardSlope(void);
void InitBillboardTitle(void);

#endif // !_BILLBOARD_H_

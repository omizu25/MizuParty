//==================================================
//
// 3Dゲーム制作 ( camera.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _CAMERA_H_		// このマクロ定義がされてなかったら
#define _CAMERA_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// 構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		posV;				// 視点
	D3DXVECTOR3		posR;				// 注視点
	D3DXVECTOR3		vecU;				// 上方向ベクトル
	D3DXVECTOR3		rot;				// 向き
	D3DXMATRIX		mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX		mtxView;			// ビューマトリックス
	float			fDistance;			// 視点から注視点の距離
}Camera;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera *GetCamera(void);

#endif // !_CAMERA_H_
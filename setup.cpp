//==================================================
// 
// 3Dゲーム制作 ( setup.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "setup.h"

#include <assert.h>

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void SetMiddlepos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetLeftpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetRightpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetToppos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetBottompos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);

//--------------------------------------------------
// 頂点座標の設定 [2D]
//--------------------------------------------------
void Setpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight, SETPOS setpos)
{
	switch (setpos)
	{
	case SETPOS_MIDDLE:		// 真ん中

		//頂点座標の設定 [2D] (真ん中に中心)
		SetMiddlepos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_LEFT:		// 左

		//頂点座標の設定 [2D] (左に中心)
		SetLeftpos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_RIGHT:		// 右

		//頂点座標の設定 [2D] (右に中心)
		SetRightpos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_TOP:		// 上

		//頂点座標の設定 [2D] (上に中心)
		SetToppos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_BOTTOM:		// 下

		//頂点座標の設定 [2D] (下に中心)
		SetBottompos(pVtx, pos, fWidth, fHeight);

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//頂点座標の設定 [2D] (真ん中に中心)
//--------------------------------------------------
static void SetMiddlepos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);
}

//--------------------------------------------------
//頂点座標の設定 [2D] (左に中心)
//--------------------------------------------------
static void SetLeftpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(0.0f, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(0.0f, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);
}

//--------------------------------------------------
//頂点座標の設定 [2D] (右に中心)
//--------------------------------------------------
static void SetRightpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(0.0f, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(0.0f, fHeight, 0.0f);
}

//--------------------------------------------------
//頂点座標の設定 [2D] (上に中心)
//--------------------------------------------------
static void SetToppos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, 0.0f, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, 0.0f, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);
}

//--------------------------------------------------
//頂点座標の設定 [2D] (下に中心)
//--------------------------------------------------
static void SetBottompos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, 0.0f, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, 0.0f, 0.0f);
}

//--------------------------------------------------
// 頂点カラーの設定 [2D]
//--------------------------------------------------
void Setcol(VERTEX_2D *pVtx, D3DXCOLOR col)
{
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
}

//--------------------------------------------------
// テクスチャの設定 [2D]
//--------------------------------------------------
void Settex(VERTEX_2D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom)
{
	pVtx[0].tex = D3DXVECTOR2(fULeft,  fVTop);
	pVtx[1].tex = D3DXVECTOR2(fURight, fVTop);
	pVtx[2].tex = D3DXVECTOR2(fULeft,  fVBottom);
	pVtx[3].tex = D3DXVECTOR2(fURight, fVBottom);
}

//--------------------------------------------------
// 頂点座標の初期化 [2D]
//--------------------------------------------------
void Initpos(VERTEX_2D *pVtx)
{
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// rhwの初期化 [2D]
//--------------------------------------------------
void Initrhw(VERTEX_2D *pVtx)
{
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//--------------------------------------------------
// 頂点カラーの初期化 [2D]
//--------------------------------------------------
void Initcol(VERTEX_2D *pVtx)
{
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//--------------------------------------------------
// テクスチャの初期化 [2D]
//--------------------------------------------------
void Inittex(VERTEX_2D *pVtx)
{
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//--------------------------------------------------
// 全ての初期化 [2D]
//--------------------------------------------------
void InitAll(VERTEX_2D *pVtx)
{
	// 頂点座標の初期化 [2D]
	Initpos(pVtx);

	// rhwの初期化 [2D]
	Initrhw(pVtx);

	// 頂点カラーの初期化 [2D]
	Initcol(pVtx);

	// テクスチャの初期化 [2D]
	Inittex(pVtx);
}

//--------------------------------------------------
// 頂点座標の設定 [3D]
//--------------------------------------------------
void Setpos(VERTEX_3D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth,  fHeight,  fDepth);
	pVtx[1].pos = pos + D3DXVECTOR3( fWidth,  fHeight,  fDepth);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, -fDepth);
	pVtx[3].pos = pos + D3DXVECTOR3( fWidth, -fHeight, -fDepth);
}

//--------------------------------------------------
// 頂点の法線の設定 [3D]
//--------------------------------------------------
void Setnor(VERTEX_3D * pVtx, D3DXVECTOR3 nor)
{
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;
}

//--------------------------------------------------
// 頂点カラーの設定 [3D]
//--------------------------------------------------
void Setcol(VERTEX_3D *pVtx, D3DXCOLOR col)
{
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
}

//--------------------------------------------------
// テクスチャの設定 [3D]
//--------------------------------------------------
void Settex(VERTEX_3D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom)
{
	pVtx[0].tex = D3DXVECTOR2(fULeft,  fVTop);
	pVtx[1].tex = D3DXVECTOR2(fURight, fVTop);
	pVtx[2].tex = D3DXVECTOR2(fULeft,  fVBottom);
	pVtx[3].tex = D3DXVECTOR2(fURight, fVBottom);
}

//--------------------------------------------------
// 頂点座標の初期化 [3D]
//--------------------------------------------------
void Initpos(VERTEX_3D *pVtx)
{
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// 頂点の法線の初期化 [3D]
//--------------------------------------------------
void Initnor(VERTEX_3D *pVtx)
{
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

//--------------------------------------------------
// 頂点カラーの初期化 [3D]
//--------------------------------------------------
void Initcol(VERTEX_3D *pVtx)
{
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//--------------------------------------------------
// テクスチャの初期化 [3D]
//--------------------------------------------------
void Inittex(VERTEX_3D *pVtx)
{
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//--------------------------------------------------
// 全ての初期化 [3D]
//--------------------------------------------------
void InitAll(VERTEX_3D *pVtx)
{
	// 頂点座標の初期化 [3D]
	Initpos(pVtx);

	// 頂点の法線の初期化[3D]
	Initnor(pVtx);

	// 頂点カラーの初期化 [3D]
	Initcol(pVtx);

	// テクスチャの初期化 [3D]
	Inittex(pVtx);
}

//--------------------------------------------------
// 頂点座標の設定 [線]
//--------------------------------------------------
void Setpos(VERTEX_LINE *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	pVtx[0].pos = pos + start;
	pVtx[1].pos = pos + end;
}

//--------------------------------------------------
// 頂点カラーの設定 [線]
//--------------------------------------------------
void Setcol(VERTEX_LINE *pVtx, D3DXCOLOR col)
{
	pVtx[0].col = col;
	pVtx[1].col = col;
}

//--------------------------------------------------
// 頂点座標の初期化 [線]
//--------------------------------------------------
void Initpos(VERTEX_LINE *pVtx)
{
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// 頂点カラーの初期化 [線]
//--------------------------------------------------
void Initcol(VERTEX_LINE *pVtx)
{
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//--------------------------------------------------
// 全ての初期化 [線]
//--------------------------------------------------
void InitAll(VERTEX_LINE *pVtx)
{
	// 頂点座標の初期化 [線]
	Initpos(pVtx);

	// 頂点カラーの初期化 [線]
	Initcol(pVtx);
}

//--------------------------------------------------
// 角度の正規化
//--------------------------------------------------
void NormalizeRot(float *pRot)
{
	if (*pRot >= D3DX_PI)
	{// 3.14より大きい
		*pRot -= D3DX_PI * 2.0f;
	}
	else if (*pRot <= -D3DX_PI)
	{// -3.14より小さい
		*pRot += D3DX_PI * 2.0f;
	}
}

//--------------------------------------------------
// 指定の値以上・以下 [int]
//--------------------------------------------------
void Specified(int *pNumber, int nMax, int nMin)
{
	if (*pNumber >= nMax)
	{// 指定の値以上
		*pNumber = nMax;
	}
	else if (*pNumber <= nMin)
	{// 指定の値以下
		*pNumber = nMin;
	}
}

//--------------------------------------------------
// 指定の値以上・以下 [float]
//--------------------------------------------------
void Specified(float *pNumber, float fMax, float fMin)
{
	if (*pNumber >= fMax)
	{// 指定の値以上
		*pNumber = fMax;
	}
	else if (*pNumber <= fMin)
	{// 指定の値以下
		*pNumber = fMin;
	}
}

//--------------------------------------------------
// バーテックスの小さい・大きい
//--------------------------------------------------
void VtxSmallBig(float *pfMin, float *pfMax, float fPos)
{
	if (fPos < *pfMin)
	{// 小さい
		*pfMin = fPos;
	}

	if (fPos > *pfMax)
	{// 大きい
		*pfMax = fPos;
	}
}

//--------------------------------------------------
// 2Dベクトルの外積
//--------------------------------------------------
float Vec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
	return (v1->x * v2->z) - (v1->z * v2->x);
}

//--------------------------------------------------
// 2Dベクトルの内積
//--------------------------------------------------
float Vec2Dot(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
	return (v1->x * v2->x) + (v1->z * v2->z);
}
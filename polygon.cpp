//==================================================
// 
// 3Dゲーム制作 ( polygon.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "polygon.h"
#include "setup.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_WIDTH		(200.0f)		// 幅の最大値
#define MAX_HEIGHT		(0.0f)			// 高さの最大値
#define MAX_DEPTH		(200.0f)		// 奥行きの最大値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static polygon						s_polygon;				// ポリゴンの情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitPolygon(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko000.jpg",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_polygon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_polygon.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_polygon.fWidth = MAX_WIDTH;
	s_polygon.fHeight = MAX_HEIGHT;
	s_polygon.fDepth = MAX_DEPTH;

	// 頂点座標の設定
	Setpos(pVtx, s_polygon.pos, s_polygon.fWidth, s_polygon.fHeight, s_polygon.fDepth);

	// 各頂点の法線の設定
	Initnor(pVtx);

	// 頂点カラーの設定
	Initcol(pVtx);

	// テクスチャ座標の設定
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitPolygon(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの解放
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdatePolygon(void)
{

}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawPolygon(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_polygon.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_polygon.rot.y, s_polygon.rot.x, s_polygon.rot.z);
	D3DXMatrixMultiply(&s_polygon.mtxWorld, &s_polygon.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_polygon.pos.x, s_polygon.pos.y, s_polygon.pos.z);
	D3DXMatrixMultiply(&s_polygon.mtxWorld, &s_polygon.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_polygon.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
polygon *GetPolygon(void)
{
	return &s_polygon;
}

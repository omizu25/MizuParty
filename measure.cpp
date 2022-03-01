//==================================================
// 
// 3Dゲーム制作 ( measure.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "measure.h"
#include "player.h"
#include "setup.h"

#include <assert.h>
//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_HEIGHT		(1.0f)			// 高さの最大値
#define MAX_DEPTH		(50.0f)			// 奥行きの最大値
#define START_WIDTH		(0.0f)			// 幅の最初の値
#define START_POS_Z		(-30.0f)		// Zの最初の位置

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	float			fWidth;			// 幅
}Measure;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static Measure						s_Measure;				// メジャーの情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMeasure(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	s_Measure.pos = D3DXVECTOR3(0.0f, 0.0f, START_POS_Z);
	s_Measure.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_Measure.fWidth = START_WIDTH;

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	Initpos(pVtx);

	// 各頂点の法線の設定
	Initnor(pVtx);

	// 頂点カラーの設定
	Setcol(pVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// テクスチャ座標の設定
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMeasure(void)
{
	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateMeasure(void)
{
	s_Measure.fWidth = GetCamera(0)->posV.x;

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (GetPlayer()->pos.x >= 0.0f)
	{// プラス
		// 頂点座標の設定
		Setpos(pVtx, s_Measure.pos, s_Measure.fWidth, MAX_HEIGHT, MAX_DEPTH * 0.5f, SETPOS_LEFT);
	}
	else
	{// マイナス
		// 頂点座標の設定
		Setpos(pVtx, s_Measure.pos, -s_Measure.fWidth, MAX_HEIGHT, MAX_DEPTH * 0.5f, SETPOS_RIGHT);
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMeasure(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_Measure.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Measure.rot.y, s_Measure.rot.x, s_Measure.rot.z);
	D3DXMatrixMultiply(&s_Measure.mtxWorld, &s_Measure.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_Measure.pos.x, s_Measure.pos.y, s_Measure.pos.z);
	D3DXMatrixMultiply(&s_Measure.mtxWorld, &s_Measure.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_Measure.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
	
	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}

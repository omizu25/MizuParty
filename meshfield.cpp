//==================================================
// 
// 3Dゲーム制作 ( meshfield.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "meshfield.h"
#include "setup.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_WIDTH		(200.0f)		//幅の最大値
#define MAX_HEIGHT		(0.0f)			//高さの最大値
#define MAX_DEPTH		(200.0f)		//奥行きの最大値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファのポインタ
static MeshField					s_meshfield;			// メッシュフィールドの情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMeshField(void)
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
		sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_meshfield.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_meshfield.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_meshfield.fWidth = MAX_WIDTH;
	s_meshfield.fHeight = MAX_HEIGHT;
	s_meshfield.fDepth = MAX_DEPTH;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, MAX_DEPTH);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, MAX_DEPTH);
	pVtx[4].pos = D3DXVECTOR3(MAX_WIDTH, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(MAX_WIDTH, 0.0f, MAX_DEPTH);
	pVtx[6].pos = D3DXVECTOR3(MAX_WIDTH, 0.0f, MAX_DEPTH);
	pVtx[7].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, -MAX_DEPTH);
	pVtx[8].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, -MAX_DEPTH);
	pVtx[9].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(0.0f, 0.0f, -MAX_DEPTH);
	pVtx[11].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[12].pos = D3DXVECTOR3(MAX_WIDTH, 0.0f, -MAX_DEPTH);
	pVtx[13].pos = D3DXVECTOR3(MAX_WIDTH, 0.0f, 0.0f);

	for (int i = 0; i < 14; i++)
	{
		// 各頂点の法線の設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMeshField(void)
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
void UpdateMeshField(void)
{

}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMeshField(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_meshfield.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_meshfield.rot.y, s_meshfield.rot.x, s_meshfield.rot.z);
	D3DXMatrixMultiply(&s_meshfield.mtxWorld, &s_meshfield.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_meshfield.pos.x, s_meshfield.pos.y, s_meshfield.pos.z);
	D3DXMatrixMultiply(&s_meshfield.mtxWorld, &s_meshfield.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_meshfield.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴン描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		12);						// プリミティブ(ポリゴン)数

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
MeshField *GetMeshField(void)
{
	return &s_meshfield;
}
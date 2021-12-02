//==================================================
// 
// 3Dゲーム制作 ( shadow.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "polygon.h"
#include "setup.h"
#include "shadow.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_SHADOW		(256)		//影の最大数
#define BASIC_WIDTH		(15.0f)		//幅の基準値
#define BASIC_HEIGHT	(0.1f)		//高さの基準値
#define BASIC_DEPTH		(15.0f)		//奥行きの基準値

//--------------------------------------------------
// 構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	bool			bUse;			// 使用しているかどうか
}Shadow;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファのポインタ
static Shadow						s_shadow[MAX_SHADOW];		// 影の情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitShadow(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//メモリのクリア
	memset(s_shadow, NULL, sizeof(s_shadow));

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_SHADOW; i++)
	{
		// 全ての初期化
		InitAll3D(pVtx);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitShadow(void)
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
void UpdateShadow(void)
{

}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawShadow(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// 減算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_SHADOW; i++)
	{
		Shadow *pShadow = &s_shadow[i];

		if (!pShadow->bUse)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pShadow->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);
		D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);
		D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			i * 4,						// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// 元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int i;
	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	for (i = 0; i < MAX_SHADOW; i++)
	{
		Shadow *pShadow = &s_shadow[i];

		if (pShadow->bUse)
		{//使用されている
			continue;
		}

		/*↓ 使用されていない ↓*/

		pShadow->pos = pos;
		pShadow->rot = rot;
		pShadow->bUse = true;

		polygon *pPolygon = GetPolygon();

		pShadow->pos.y = pPolygon->pos.y + 0.1f;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		float fSize = pos.y * 0.15f;

		// 頂点座標の設定
		Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BASIC_WIDTH + fSize, BASIC_HEIGHT, BASIC_DEPTH + fSize);

		float Alpha = 1.0f - (pos.y * 0.005f);

		// 頂点カラーの設定
		Setcol3D(pVtx, 1.0f, 1.0f, 1.0f, Alpha);

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}

	return i;
}

//--------------------------------------------------
// 位置の設定
//--------------------------------------------------
void SetPosShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	Shadow *pShadow = &s_shadow[nIdxShadow];

	pShadow->pos = pos;

	polygon *pPolygon = GetPolygon();

	pShadow->pos.y = pPolygon->pos.y + 0.1f;

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxShadow * 4);		//該当の位置まで進める

	float fSize = pos.y * 0.15f;

	// 頂点座標の設定
	Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BASIC_WIDTH + fSize, BASIC_HEIGHT, BASIC_DEPTH + fSize);

	float Alpha = 1.0f - (pos.y * 0.005f);

	// 頂点カラーの設定
	Setcol3D(pVtx, 1.0f, 1.0f, 1.0f, Alpha);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}
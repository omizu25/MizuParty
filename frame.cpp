//==================================================
// 
// 3Dゲーム制作 ( frame.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "frame.h"
#include "player.h"
#include "setup.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_FRAME		(256)		// 枠の最大数
#define FRAME_SIZE		(5.0f)		// 枠のサイズ

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
	bool					bDraw;			// 表示するかどうか
	LPDIRECT3DTEXTURE9		pTexture;		// テクスチャ
}Frame;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static Frame						s_frame[MAX_FRAME];		// 枠の情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitFrame(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/block005.png",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_FRAME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//メモリのクリア
	memset(s_frame, 0, sizeof(s_frame));

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_FRAME; i++)
	{
		//全ての初期化処理
		InitAll(pVtx);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitFrame(void)
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
void UpdateFrame(void)
{

}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawFrame(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_FRAME; i++)
	{
		Frame *pFrame = &s_frame[i];

		if (!pFrame->bUse || !pFrame->bDraw)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pFrame->mtxWorld);
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラの逆行列を設定
		pFrame->mtxWorld._11 = mtxView._11;
		pFrame->mtxWorld._12 = mtxView._21;
		pFrame->mtxWorld._13 = mtxView._31;
		pFrame->mtxWorld._21 = mtxView._12;
		pFrame->mtxWorld._22 = mtxView._22;
		pFrame->mtxWorld._23 = mtxView._32;
		pFrame->mtxWorld._31 = mtxView._13;
		pFrame->mtxWorld._32 = mtxView._23;
		pFrame->mtxWorld._33 = mtxView._33;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pFrame->pos.x, pFrame->pos.y, pFrame->pos.z);
		D3DXMatrixMultiply(&pFrame->mtxWorld, &pFrame->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pFrame->mtxWorld);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			i * 4,						// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zバッファの値を元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// 新規深度値 <= Zバッファ深度値 (初期設定)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetFrame(D3DXVECTOR3 pos, float fWidth, float fHeight, LPDIRECT3DTEXTURE9 *pTexture)
{
	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	for (int i = 0; i < MAX_FRAME; i++)
	{
		Frame *pFrame = &s_frame[i];

		if (pFrame->bUse)
		{//使用されている
			continue;
		}

		/*↓ 使用されていない ↓*/

		pFrame->pos = pos;
		pFrame->fWidth = fWidth;
		pFrame->fHeight = fHeight;
		pFrame->pTexture = *pTexture;
		pFrame->bUse = true;
		pFrame->bDraw = false;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		// 頂点座標の設定
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth + FRAME_SIZE, fHeight + FRAME_SIZE, 0.0f, SETPOS_MIDDLE);

		// 頂点の法線の設定
		Setnor(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// 当たり判定
//--------------------------------------------------
void CollisionFrame(D3DXVECTOR3 *pPos)
{
	for (int i = 0; i < MAX_FRAME; i++)
	{
		Frame *pFrame = &s_frame[i];

		if (!pFrame->bUse)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		pFrame->bDraw = false;

	}

	Billboard *pBillboard = GetBillboard();

	for (int i = 0; i < MAX_BILLBOARD; i++, pBillboard++)
	{
		if (!pBillboard->bUse)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		if (pBillboard->pos.x + pBillboard->fWidth >= pPos->x &&
			pBillboard->pos.x - pBillboard->fWidth <= pPos->x &&
			pBillboard->pos.z + pBillboard->fHeight >= pPos->z &&
			pBillboard->pos.z - pBillboard->fHeight <= pPos->z)
		{// 当たってる
			for (int j = 0; j < MAX_FRAME; j++)
			{
				Frame *pFrame = &s_frame[j];

				if (!pFrame->bUse || pBillboard->pTexture != pFrame->pTexture)
				{//使用されていない
					continue;
				}

				/*↓ 使用されている ↓*/

				pFrame->bDraw = true;
			}
		}
	}
}

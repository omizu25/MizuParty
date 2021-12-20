//==================================================
// 
// 3Dゲーム制作 ( line.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "line.h"
#include "setup.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_LINE		(256)		//線の最大数
#define BASIC_WIDTH		(15.0f)		//幅の基準値
#define BASIC_HEIGHT	(0.1f)		//高さの基準値
#define BASIC_DEPTH		(15.0f)		//奥行きの基準値

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXCOLOR		col;			// 色
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	bool			bUse;			// 使用しているかどうか
}Line;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static Line							s_Line[MAX_LINE];		// 線の情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitLine(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_LINE) * 2 * MAX_LINE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_LINE,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//メモリのクリア
	memset(s_Line, NULL, sizeof(s_Line));

	VERTEX_LINE *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_LINE; i++)
	{
		// 全ての初期化
		InitAll(pVtx);

		pVtx += 2;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitLine(void)
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
void UpdateLine(void)
{

}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawLine(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < MAX_LINE; i++)
	{
		Line *pLine = &s_Line[i];

		if (!pLine->bUse)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pLine->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pLine->rot.y, pLine->rot.x, pLine->rot.z);
		D3DXMatrixMultiply(&pLine->mtxWorld, &pLine->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pLine->pos.x, pLine->pos.y, pLine->pos.z);
		D3DXMatrixMultiply(&pLine->mtxWorld, &pLine->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pLine->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_LINE));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_LINE);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_LINELIST,		// プリミティブの種類
			i * 2,				// 描画する最初の頂点インデックス
			1);					// プリミティブ(ポリゴン)数
	}

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	VERTEX_LINE *pVtx = NULL;		// 頂点情報へのポインタ

	for (int i = 0; i < MAX_LINE; i++)
	{
		Line *pLine = &s_Line[i];

		if (pLine->bUse)
		{//使用されている
			continue;
		}

		/*↓ 使用されていない ↓*/

		pLine->pos = pos;
		pLine->rot = rot;
		pLine->col = col;
		pLine->bUse = true;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 2);		//該当の位置まで進める

		// 頂点座標の設定
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), start, end);

		// 頂点カラーの設定
		//SetcolLine(pVtx, col);

		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

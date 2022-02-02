//==================================================
// 
// 3Dゲーム制作 ( number.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "setup.h"
#include "number.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_NUMBER		(256)		// 数の最大数
#define MAX_TEX			(10)		// texの最大数

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	int				nNumber;		// 数
	int				nDigit;			// 桁数
	USE				use;			// 使用者
	bool			bUse;			// 使用しているかどうか
}Number;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファのポインタ
static Number						s_Number[MAX_NUMBER];		// 数の情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitNumber(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\number000.png",
		&s_pTexture);

	// メモリのクリア
	memset(&s_Number, NULL, sizeof(s_Number));

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		// 全ての初期化
		InitAll(pVtx);

		pVtx += 4;		// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitNumber(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの破棄
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateNumber(void)
{
	
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawNumber(USE use)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		if (s_Number[i].bUse && s_Number[i].use == use)
		{// 数が使用されている
			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				i * 4,						// 描画する最初の頂点インデックス
				2);							// プリミティブ(ポリゴン)数
		}
	}
}

//--------------------------------------------------
// 設定 (右に中心)
// 1.位置 2.幅 3.高さ 4.数 5.桁数 6.使用者
//--------------------------------------------------
void SetRightNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (pNumber->bUse)
		{// 数が使用されている
			continue;
		}

		/*↓ 数が使用されていない ↓*/

		pNumber->pos = pos;
		pNumber->fWidth = fWidth;
		pNumber->fHeight = fHeight;
		pNumber->nNumber = nNumber;
		pNumber->nDigit = nDigit;
		pNumber->use = use;
		pNumber->bUse = true;

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// 該当の位置まで進める

		// 頂点座標の設定
		Setpos(pVtx, pos, fWidth, fHeight, SETPOS_RIGHT);

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;
		
		// テクスチャ座標の設定
		Settex(pVtx, 0.0f + fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// 設定 (真ん中に中心)
// 1.位置 2.幅 3.高さ 4.数 5.桁数 6.順位
//--------------------------------------------------
void SetMiddleNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (pNumber->bUse)
		{// 数が使用されている
			continue;
		}

		/*↓ 数が使用されていない ↓*/

		pNumber->pos = pos;
		pNumber->fWidth = fWidth;
		pNumber->fHeight = fHeight;
		pNumber->nNumber = nNumber;
		pNumber->nDigit = nDigit;
		pNumber->use = use;
		pNumber->bUse = true;

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// 該当の位置まで進める

		// 頂点座標の設定
		Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

		// 頂点カラーの設定
		Setcol(pVtx, D3DXCOLOR(0.25f, 1.0f, 0.5f, 1.0f));

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		// テクスチャ座標の設定
		Settex(pVtx, fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// テクスチャ
//--------------------------------------------------
void TexNumber(int nNumber, int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (!pNumber->bUse || pNumber->nDigit != nDigit || pNumber->use != use)
		{// 数が使用されていない、桁数が違う
			continue;
		}

		/*↓ 数が使用されている、桁数が同じ ↓*/

		pNumber->nNumber = nNumber;

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// 該当の位置まで進める

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		// テクスチャ座標の設定
		Settex(pVtx, fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
//数の使用処理
//--------------------------------------------------
void UseNumber(int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (!pNumber->bUse || pNumber->nDigit != nDigit || pNumber->use != use)
		{//数が使用されていない、桁数が違う
			continue;
		}

		//数が使用されている、桁数が同じ
		pNumber->bUse = false;

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// 該当の位置まで進める

		// 頂点カラーの初期化
		Initcol(pVtx);

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//==================================================
// 
// 3Dゲーム制作 ( result.cpp )
// Author  : katsuki mizuki
// 
//==================================================
#include "fade.h"
#include "input.h"
#include "number.h"
#include "result.h"
#include "setup.h"
#include "time.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define RESULT_WIDTH		(600.0f)		// 負けと勝ちの幅
#define RESULT_HEIGHT		(300.0f)		// 負けと勝ちの高さ
#define TIME_WIDTH			(400.0f)		// タイムの幅
#define TIME_HEIGHT			(160.0f)		// タイムの高さ
#define NUMBER_WIDTH		(100.0f)		// 数の幅
#define NUMBER_HEIGHT		(250.0f)		// 数の高さ
#define WIDTH_INTERVAL		(40.0f)			// 幅の間隔
#define HEIGHT_INTERVAL		(20.0f)			// 高さの間隔

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファのポインタ
static int							s_nTime;					// タイム

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitResult(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result004.png",
		&s_pTexture);

	s_nTime = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	/* ↓勝ち↓ */

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = RESULT_WIDTH * 0.5f;
	float fHeight = RESULT_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, RESULT_HEIGHT * 0.35f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの設定処理
	Setcol(pVtx, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	/* ↓数関連↓ */

	// 位置を初期化する
	D3DXVECTOR3 posNumber = D3DXVECTOR3(1000.0f, SCREEN_HEIGHT * 0.7f, 0.0f);

	int nTime = GetTime();
	int aNumber[MAX_TIME];

	for (int i = 0; i < MAX_TIME; i++)
	{// １桁ずつに分ける
		aNumber[i] = nTime % 10;
		nTime /= 10;

		float fInterval = (NUMBER_WIDTH * i) + (WIDTH_INTERVAL * (i / 2));

		// 数の設定処理
		SetRightNumber(D3DXVECTOR3(posNumber.x - fInterval, posNumber.y, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_RESULT);
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitResult(void)
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
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) ||
		GetJoypadTrigger(JOYKEY_B, 0) || GetJoypadTrigger(JOYKEY_START, 0))
	{// 決定キー(ENTERキー)が押されたかどうか
		// モード処理
		SetFade(MODE_TITLE);
	}

	s_nTime++;

	if (s_nTime >= 900)
	{// 15秒経ちました
		// モード処理
		SetFade(MODE_TITLE);
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawResult(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数
}

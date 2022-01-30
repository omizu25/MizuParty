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
static LPDIRECT3DTEXTURE9			s_pTextureBG[RESULT_MAX];		// 背景のテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;			// 背景の頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTexture[RESULT_MAX];			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// 頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureTime = NULL;			// タイムのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffTime = NULL;			// タイムの頂点バッファのポインタ
static RESULT						s_Result;						// リザルトの情報
static int							s_nTime;						// タイム

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void UpdateMode(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitResult(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// メモリのクリア
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));

	// メモリのクリア
	memset(&s_pTextureBG[0], NULL, sizeof(s_pTextureBG));

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result006.jpg",
		&s_pTextureBG[RESULT_LOSE]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result007.png",
		&s_pTextureBG[RESULT_WIN]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result005.png",
		&s_pTexture[RESULT_LOSE]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result004.png",
		&s_pTexture[RESULT_WIN]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result009.png",
		&s_pTextureTime);

	s_nTime = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBG,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffTime,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	/* ↓背景↓ */

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthBG = SCREEN_WIDTH * 0.5f;
	float fHeightBG = SCREEN_HEIGHT * 0.5f;
	D3DXVECTOR3 posBG = D3DXVECTOR3(fWidthBG, fHeightBG, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, posBG, fWidthBG, fHeightBG, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの初期化処理
	Initcol(pVtx);

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffBG->Unlock();

	/* ↓負けと勝ち↓ */

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (s_Result == RESULT_LOSE)
	{// 負け
		float fWidth = RESULT_WIDTH * 0.5f;
		float fHeight = RESULT_HEIGHT * 0.5f;
		D3DXVECTOR3 pos = D3DXVECTOR3(RESULT_WIDTH * 0.6f, RESULT_HEIGHT * 0.35f, 0.0f);

		// 頂点座標の設定処理
		Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);
	}
	else
	{
		float fWidth = RESULT_WIDTH * 0.5f;
		float fHeight = RESULT_HEIGHT * 0.5f;
		D3DXVECTOR3 pos = D3DXVECTOR3(fHeightBG, RESULT_HEIGHT * 0.35f, 0.0f);

		// 頂点座標の設定処理
		Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);
	}

	// rhwの初期化処理
	Initrhw(pVtx);

	if (s_Result == RESULT_WIN)
	{// 勝ち
		// 頂点カラーの設定処理
		Setcol(pVtx, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		// 頂点カラーの初期化処理
		Initcol(pVtx);
	}

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	/* ↓今回のタイム↓ */

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthTime = TIME_WIDTH * 0.5f;
	float fHeightTime = TIME_HEIGHT * 0.5f;
	D3DXVECTOR3 posTime = D3DXVECTOR3(TIME_WIDTH * 0.75f, SCREEN_HEIGHT * 0.4f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, posTime, fWidthTime, fHeightTime, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの設定処理
	Setcol(pVtx, D3DXCOLOR(0.8f, 0.0f, 0.8f, 1.0f));

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffTime->Unlock();

	/* ↓数関連↓ */

	// 数の初期化処理
	InitNumber();

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
		SetRightNumber(D3DXVECTOR3(posNumber.x - fInterval, posNumber.y, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, -1);
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitResult(void)
{
	for (int i = 0; i < RESULT_MAX; i++)
	{
		if (s_pTextureBG[i] != NULL)
		{// 背景のテクスチャの破棄
			s_pTextureBG[i]->Release();
			s_pTextureBG[i] = NULL;
		}
	}

	if (s_pVtxBuffBG != NULL)
	{// 背景の頂点バッファの破棄
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	for (int i = 0; i < RESULT_MAX; i++)
	{
		if (s_pTexture[i] != NULL)
		{// テクスチャの破棄
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pTextureTime != NULL)
	{// タイムのテクスチャの破棄
		s_pTextureTime->Release();
		s_pTextureTime = NULL;
	}

	if (s_pVtxBuffTime != NULL)
	{// タイムの頂点バッファの破棄
		s_pVtxBuffTime->Release();
		s_pVtxBuffTime = NULL;
	}

	// 数の終了処理
	UninitNumber();
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
		UpdateMode();
	}

	s_nTime++;

	if (s_nTime >= 900)
	{
		if (GetFade() == FADE_NONE)
		{// まだフェードしてない
			// モード処理
			UpdateMode();
		}
	}

	// 数の更新処理
	UpdateNumber();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawResult(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureBG[s_Result]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture[s_Result]);
	
	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	if (s_Result == RESULT_WIN)
	{// 勝ち
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuffTime, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTextureTime);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数

		// 数の描画処理
		DrawNumber();
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetResult(RESULT Result)
{
	if (s_Result == RESULT_NONE || Result == RESULT_NONE)
	{// まだリザルトが決まってない、リザルトの初期化
		s_Result = Result;		// リザルトを代入
	}
}

//--------------------------------------------------
// モード
//--------------------------------------------------
static void UpdateMode(void)
{
	switch (s_Result)
	{
	case RESULT_WIN:		// 勝ち
		// フェード設定
		SetFade(MODE_TITLE);
		break;

	case RESULT_LOSE:		// 負け
		// フェード設定
		SetFade(MODE_TITLE);
		break;

	case RESULT_NONE:		// 何もしていない状態

		// breakなし　

	default:
		assert(false);
		break;
	}
}
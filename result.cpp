//==================================================
// 
// 3Dゲーム制作 ( result.cpp )
// Author  : katsuki mizuki
// 
//==================================================
#include "fade.h"
#include "input.h"
#include "model.h"
#include "number.h"
#include "player.h"
#include "result.h"
#include "setup.h"
#include "time.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define RESULT_WIDTH		(500.0f)		// 幅
#define RESULT_HEIGHT		(200.0f)		// 高さ
#define METER_WIDTH			(100.0f)		// メートルの幅
#define METER_HEIGHT		(160.0f)		// メートルの高さ
#define MINUS_WIDTH			(60.0f)			// マイナスの幅
#define MINUS_HEIGHT		(40.0f)			// マイナスの高さ
#define GAMEOVER_WIDTH		(1000.0f)		// ゲームオーバーの幅
#define GAMEOVER_HEIGHT		(300.0f)		// ゲームオーバーの高さ
#define NUMBER_WIDTH		(140.0f)		// 数の幅
#define NUMBER_HEIGHT		(250.0f)		// 数の高さ
#define WIDTH_INTERVAL		(0.0f)			// 幅の間隔
#define MAX_RESULT			(3)				// リザルトの最大数
#define MIN_RESULT			(2)				// リザルトの最小数

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;				// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// 頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMeter = NULL;			// メートルのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMeter = NULL;			// メートルの頂点バッファのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMinus = NULL;			// マイナスの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureGameOver = NULL;		// ゲームオーバーのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffGameOver = NULL;		// ゲームオーバーの頂点バッファのポインタ
static int							s_nTime;						// タイム
static RESULT						s_result;						// リザルト

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void InitClear(void);
static void InitGameOver(void);
static void InitPosNumber(void);
static void InitMeter(void);
static void InitMinus(void);
static void DrawClear(void);
static void DrawGameOver(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitResult(void)
{
	switch (s_result)
	{
	case RESULT_CLEAR:			// クリア

		// クリア
		InitClear();

		break;

	case RESULT_GAMEOVER:		// ゲームオーバー

		// ゲームオーバー
		InitGameOver();

		break;

	case RESULT_NONE:			// まだ決まってない

		/* 処理なし */

		break;

	default:
		assert(false);
		break;
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

	if (s_pTextureMeter != NULL)
	{// テクスチャの破棄
		s_pTextureMeter->Release();
		s_pTextureMeter = NULL;
	}

	if (s_pVtxBuffMeter != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffMeter->Release();
		s_pVtxBuffMeter = NULL;
	}

	if (s_pVtxBuffMinus != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffMinus->Release();
		s_pVtxBuffMinus = NULL;
	}

	if (s_pTextureGameOver != NULL)
	{// テクスチャの破棄
		s_pTextureGameOver->Release();
		s_pTextureGameOver = NULL;
	}

	if (s_pVtxBuffGameOver != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffGameOver->Release();
		s_pVtxBuffGameOver = NULL;
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
	switch (s_result)
	{
	case RESULT_CLEAR:			// クリア

		// クリア
		DrawClear();

		break;

	case RESULT_GAMEOVER:		// ゲームオーバー

		// ゲームオーバー
		DrawGameOver();

		break;

	case RESULT_NONE:			// まだ決まってない

		/* 処理なし */

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetResult(RESULT result)
{
	if (s_result == RESULT_NONE || result == RESULT_NONE || result == RESULT_GAMEOVER)
	{
		s_result = result;
	}
}

//--------------------------------------------------
// クリア
//--------------------------------------------------
static void InitClear(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\result010.png",
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

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = RESULT_WIDTH * 0.5f;
	float fHeight = RESULT_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.75f, 0.0f);

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

	// 位置の数字
	InitPosNumber();

	// メートル
	InitMeter();

	// マイナス
	InitMinus();
}

//--------------------------------------------------
// ゲームオーバー
//--------------------------------------------------
static void InitGameOver(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/gameover.png",
		&s_pTextureGameOver);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffGameOver,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffGameOver->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = GAMEOVER_WIDTH * 0.5f;
	float fHeight = GAMEOVER_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの設定処理
	Setcol(pVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffGameOver->Unlock();
}

//--------------------------------------------------
// 位置の数字
//--------------------------------------------------
static void InitPosNumber(void)
{
	// 位置を初期化する
	D3DXVECTOR3 posNumber = D3DXVECTOR3(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.75f, 0.0f);

	int nPos = 0;
	float fModel = 0.0f, fPlayer = 0.0f;

	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
		
		nPos = (int)(GetPlayer()->pos.x / 30);

		if (GetPlayer()->pos.x <= 0.0f)
		{// マイナス
			nPos *= -1;
		}

		break;

	case MENU_STOP:			// 止める
		
		fModel = GetModel()->pos.y + GetModel()->vtxMin.y;
		fPlayer = GetPlayer()->pos.y + GetPlayer()->fHeight;

		nPos = (int)(fModel - fPlayer);

		break;

	case MENU_RANKING:		// ランキング

		break;

	default:
		assert(false);
		break;
	}

	int nNumber = nPos, nDigit = 0;

	while (1)
	{
		
		if (nNumber >= 10)
		{
			nNumber /= 10;
			nDigit++;
		}
		else
		{
			nDigit++;

			if (nDigit < MIN_RESULT)
			{
				nDigit = MIN_RESULT;
			}

			break;
		}
	}

	int aNumber[MAX_RESULT];

	for (int i = 0; i < nDigit; i++)
	{// １桁ずつに分ける
		aNumber[i] = nPos % 10;
		nPos /= 10;

		float fInterval = (NUMBER_WIDTH * i) + (WIDTH_INTERVAL * (i / 2));

		// 数の設定処理
		SetRightNumber(D3DXVECTOR3(posNumber.x - fInterval, posNumber.y, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_RESULT);
	}
}

//--------------------------------------------------
// メートル
//--------------------------------------------------
static void InitMeter(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/METER/m.png",
		&s_pTextureMeter);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMeter,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffMeter->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = METER_WIDTH * 0.5f;
	float fHeight = METER_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.825f, 0.0f);

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffMeter->Unlock();
}

//--------------------------------------------------
// マイナス
//--------------------------------------------------
static void InitMinus(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMinus,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffMinus->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = MINUS_WIDTH * 0.5f;
	float fHeight = MINUS_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.75f, 0.0f);

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffMinus->Unlock();
}

//--------------------------------------------------
// クリア
//--------------------------------------------------
static void DrawClear(void)
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

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffMeter, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureMeter);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffMinus, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	if (GetPlayer()->pos.x <= -30.0f)
	{// マイナス
		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}
}

//--------------------------------------------------
// ゲームオーバー
//--------------------------------------------------
static void DrawGameOver(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffGameOver, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureGameOver);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数
}
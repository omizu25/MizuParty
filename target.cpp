//==================================================
// 
// 3Dゲーム制作 ( target.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "game.h"
#include "number.h"
#include "setup.h"
#include "title.h"

#include <assert.h>
#include <time.h>
//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_DIGITS					(2)				// 桁数の最大値
#define MIN_TARGET					(37)			// 目標の最小値
#define MAX_RANDOM					(30)			// ランダムの最大値
#define START_NUMBER_WIDTH			(120.0f)		// スタートの数の幅
#define START_NUMBER_HEIGHT			(200.0f)		// スタートの数の高さ
#define START_WIDTH_INTERVAL		(5.0f)			// スタートの数の幅の間隔
#define START_METER_WIDTH			(80.0f)			// スタートのメートルの幅
#define START_METER_HEIGHT			(140.0f)		// スタートのメートルの高さ
#define START_WIDTH					(500.0f)		// スタートの幅
#define START_HEIGHT				(200.0f)		// スタートの高さ
#define GAME_NUMBER_WIDTH			(80.0f)			// ゲームの数の幅
#define GAME_NUMBER_HEIGHT			(140.0f)		// ゲームの数の高さ
#define GAME_WIDTH_INTERVAL			(5.0f)			// ゲームの数の幅の間隔
#define GAME_METER_WIDTH			(80.0f)			// ゲームのメートルの幅
#define GAME_METER_HEIGHT			(140.0f)		// ゲームのメートルの高さ
#define STOP_WIDTH					(1000.0f)		// ストップの幅
#define STOP_HEIGHT					(600.0f)		// ストップの高さ
#define JUST_WIDTH					(1000.0f)		// ぴったりの幅
#define JUST_HEIGHT					(400.0f)		// ぴったりの高さ
#define SLOPE_WIDTH					(400.0f)		// 坂の幅
#define SLOPE_HEIGHT				(200.0f)		// 坂の高さ

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;					// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;					// 頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMeter = NULL;				// メートルのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffStartMeter = NULL;		// スタートのメートルの頂点バッファのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffGameMeter = NULL;			// ゲームのメートルの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureStop = NULL;				// ストップのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffStop = NULL;				// ストップの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureJust = NULL;				// ぴったりのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffJust = NULL;				// ぴったりの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureSlope = NULL;				// 坂のテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffSlope = NULL;				// 坂の頂点バッファのポインタ
static int							s_nTarget;							// 目標地点

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void InitStartNumber(void);
static void InitStartMeter(void);
static void InitStartTarget(void);
static void InitGameNumber(void);
static void InitGameMeter(void);
static void DrawStartMeter(void);
static void DrawGameMeter(void);
static void InitStop(void);
static void InitJust(void);
static void InitSlope(void);
static void DrawStartSlope(void);
static void DrawGameSlope(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitTarget(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング

		//世界の種子の初期化
		srand((unsigned)time(NULL));

		s_nTarget = (rand() % MAX_RANDOM) + MIN_TARGET;

		// スタートの数の初期化
		InitStartNumber();

		// スタートのメートルの初期化
		InitStartMeter();

		// スタートの目指すの初期化
		InitStartTarget();

		// ゲームの数の初期化
		InitGameNumber();

		// ゲームのメートルの初期化
		InitGameMeter();

		break;

	case MENU_STOP:			// ストップ

		// 止める
		InitStop();

		break;

	case MENU_SLOPE:		// 坂

		// ぴったり
		InitJust();

		// 坂
		InitSlope();

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTarget(void)
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

	if (s_pVtxBuffStartMeter != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffStartMeter->Release();
		s_pVtxBuffStartMeter = NULL;
	}

	if (s_pVtxBuffGameMeter != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffGameMeter->Release();
		s_pVtxBuffGameMeter = NULL;
	}

	if (s_pTextureStop != NULL)
	{// テクスチャの破棄
		s_pTextureStop->Release();
		s_pTextureStop = NULL;
	}

	if (s_pVtxBuffStop != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffStop->Release();
		s_pVtxBuffStop = NULL;
	}

	if (s_pTextureJust != NULL)
	{// テクスチャの破棄
		s_pTextureJust->Release();
		s_pTextureJust = NULL;
	}

	if (s_pVtxBuffJust != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffJust->Release();
		s_pVtxBuffJust = NULL;
	}

	if (s_pTextureSlope != NULL)
	{// テクスチャの破棄
		s_pTextureSlope->Release();
		s_pTextureSlope = NULL;
	}

	if (s_pVtxBuffSlope != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffSlope->Release();
		s_pVtxBuffSlope = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateTarget(void)
{
	
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawTarget(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング

		if (GetGame().gameState == GAMESTATE_START)
		{// スタート状態
			// スタートの描画
			DrawStartMeter();
		}
		else if (GetGame().gameState != GAMESTATE_START)
		{// ゲーム状態
			// ゲームの描画
			DrawGameMeter();
		}
		
		break;

	case MENU_STOP:			// ストップ

		if (GetGame().gameState == GAMESTATE_START)
		{// スタート状態
			// デバイスへのポインタの取得
			LPDIRECT3DDEVICE9 pDevice = GetDevice();

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, s_pVtxBuffStop, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, s_pTextureStop);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				0,							// 描画する最初の頂点インデックス
				2);							// プリミティブ(ポリゴン)数
		}

		break;

	case MENU_SLOPE:		// 坂

		if (GetGame().gameState == GAMESTATE_START)
		{// スタート状態
			// スタートの描画
			DrawStartSlope();
		}
		else if (GetGame().gameState != GAMESTATE_START)
		{// ゲーム状態
			// ゲームの描画
			DrawGameSlope();
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// スタートの数字
//--------------------------------------------------
static void InitStartNumber(void)
{
	float fWidth = SCREEN_WIDTH * 0.25f;
	float fHeight = SCREEN_HEIGHT * 0.75f;

	// 位置を初期化
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	int aNumber[MAX_DIGITS];
	int nTarget = s_nTarget;

	for (int i = 0; i < MAX_DIGITS; i++)
	{
		aNumber[i] = 0;
	}

	for (int i = 0; i < MAX_DIGITS; i++)
	{// １桁ずつに分ける
		aNumber[i] = nTarget % 10;
		nTarget /= 10;

		float fInterval = (START_NUMBER_WIDTH * i) + (START_WIDTH_INTERVAL * i);
		float fCenter = (START_NUMBER_WIDTH * 0.5) * (MAX_DIGITS - 1) + (START_WIDTH_INTERVAL * 0.5f) * (MAX_DIGITS - 1);

		D3DXVECTOR3 posNumber = D3DXVECTOR3((pos.x) - fInterval + fCenter + (START_NUMBER_WIDTH * 0.5f), pos.y, 0.0f);

		// 数の設定
		SetRightNumber(posNumber, START_NUMBER_WIDTH, START_NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_START);
	}
}

//--------------------------------------------------
// スタートのメートル
//--------------------------------------------------
static void InitStartMeter(void)
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
		&s_pVtxBuffStartMeter,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffStartMeter->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = START_METER_WIDTH * 0.5f;
	float fHeight = START_METER_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.375f, SCREEN_HEIGHT * 0.81f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの設定処理
	Setcol(pVtx, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffStartMeter->Unlock();
}

//--------------------------------------------------
// スタートの目指す場所
//--------------------------------------------------
static void InitStartTarget(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/game000.png",
		&s_pTexture);

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

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, START_WIDTH, START_HEIGHT * 0.5f, SETPOS_LEFT);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの設定処理
	Setcol(pVtx, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// ゲームの数字
//--------------------------------------------------
static void InitGameNumber(void)
{
	float fWidth = SCREEN_WIDTH * 0.1f;
	float fHeight = SCREEN_HEIGHT * 0.15f;

	// 位置を初期化
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	int aNumber[MAX_DIGITS];
	int nTarget = s_nTarget;

	for (int i = 0; i < MAX_DIGITS; i++)
	{
		aNumber[i] = 0;
	}

	for (int i = 0; i < MAX_DIGITS; i++)
	{// １桁ずつに分ける
		aNumber[i] = nTarget % 10;
		nTarget /= 10;

		float fInterval = (GAME_NUMBER_WIDTH * i) + (GAME_WIDTH_INTERVAL * i);
		float fCenter = (GAME_NUMBER_WIDTH * 0.5) * (MAX_DIGITS - 1) + (GAME_WIDTH_INTERVAL * 0.5f) * (MAX_DIGITS - 1);

		D3DXVECTOR3 posNumber = D3DXVECTOR3((pos.x) - fInterval + fCenter + (GAME_NUMBER_WIDTH * 0.5f), pos.y, 0.0f);

		// 数の設定
		SetRightNumber(posNumber, GAME_NUMBER_WIDTH, GAME_NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_GAME);
	}
}

//--------------------------------------------------
// ゲームのメートル
//--------------------------------------------------
static void InitGameMeter(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffGameMeter,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffGameMeter->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.225f;
	float fHeight = SCREEN_HEIGHT * 0.175f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// 頂点座標の設定
	Setpos(pVtx, pos, GAME_METER_WIDTH, GAME_METER_HEIGHT * 0.5f, SETPOS_RIGHT);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffGameMeter->Unlock();
}

//--------------------------------------------------
// スタートの描画
//--------------------------------------------------
static void DrawStartMeter(void)
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
	pDevice->SetStreamSource(0, s_pVtxBuffStartMeter, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureMeter);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数
}

//--------------------------------------------------
// ゲームの描画
//--------------------------------------------------
static void DrawGameMeter(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffGameMeter, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureMeter);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数
}

//--------------------------------------------------
// 止める
//--------------------------------------------------
static void InitStop(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/game001.png",
		&s_pTextureStop);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffStop,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffStop->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = STOP_WIDTH * 0.5f;
	float fHeight = STOP_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.35f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffStop->Unlock();
}

//--------------------------------------------------
// ぴったり
//--------------------------------------------------
static void InitJust(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/game002.png",
		&s_pTextureJust);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffJust,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffJust->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = JUST_WIDTH * 0.5f;
	float fHeight = JUST_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffJust->Unlock();

}

//--------------------------------------------------
// 坂
//--------------------------------------------------
static void InitSlope(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/game003.png",
		&s_pTextureSlope);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffSlope,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffSlope->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SLOPE_WIDTH * 0.5f;
	float fHeight = SLOPE_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.15f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffSlope->Unlock();
}

//--------------------------------------------------
// 坂
//--------------------------------------------------
static void DrawStartSlope(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffJust, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureJust);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数
}

//--------------------------------------------------
// 坂
//--------------------------------------------------
static void DrawGameSlope(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffSlope, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureSlope);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数
}

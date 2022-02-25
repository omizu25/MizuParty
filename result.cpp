//==================================================
// 
// 3Dゲーム制作 ( result.cpp )
// Author  : katsuki mizuki
// 
//==================================================
//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "fade.h"
#include "field.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "number.h"
#include "player.h"
#include "ranking.h"
#include "result.h"
#include "setup.h"
#include "sound.h"
#include "target.h"
#include "time.h"
#include "title.h"

#include <assert.h>
#include <math.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define RESULT_WIDTH			(450.0f)		// 幅
#define RESULT_HEIGHT			(180.0f)		// 高さ
#define METER_WIDTH				(100.0f)		// メートルの幅
#define METER_HEIGHT			(160.0f)		// メートルの高さ
#define MINUS_WIDTH				(70.0f)			// マイナスの幅
#define MINUS_HEIGHT			(100.0f)		// マイナスの高さ
#define GAMEOVER_WIDTH			(1000.0f)		// ゲームオーバーの幅
#define GAMEOVER_HEIGHT			(300.0f)		// ゲームオーバーの高さ
#define NUMBER_WIDTH			(100.0f)		// 数の幅
#define NUMBER_HEIGHT			(200.0f)		// 数の高さ
#define DECIMAL_WIDTH			(80.0f)			// 小数点の幅
#define DECIMAL_HEIGHT			(150.0f)		// 小数点の高さ
#define DECIMAL_INTERVAL		(40.0f)			// 小数の間隔
#define MAX_RESULT				(4)				// リザルトの最大数
#define MIN_RESULT				(2)				// リザルトの最小数

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;					// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;					// 頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMeter = NULL;				// メートルのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMeter = NULL;				// メートルの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMinus = NULL;				// マイナスのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMinus = NULL;				// マイナスの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureDecimal = NULL;			// 小数点のテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffDecimal = NULL;			// 小数点の頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureGameOver = NULL;			// ゲームオーバーのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffGameOver = NULL;			// ゲームオーバーの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureOperator = NULL;			// 演算子のテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffOperator = NULL;			// 演算子の頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureDiffMeter = NULL;			// 差のメートルのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffDiffMeter = NULL;			// 差のメートルの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureDiffDecimal = NULL;		// 差の小数点のテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffDiffDecimal = NULL;		// 差の小数点の頂点バッファのポインタ
static int							s_nTime;							// タイム
static int							s_nDigit;							// 桁数
static int							s_nDiffDigit;						// 差の桁数
static RESULT						s_result;							// リザルト

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void InitClear(void);
static void InitGameOver(void);
static void InitPosNumber(void);
static void InitMeter(void);
static void InitDecimal(void);
static void InitMinus(void);
static void InitDifference(void);
static void InitDiffMeter(void);
static void InitDiffDecimal(void);
static void DrawClear(void);
static void DrawGameOver(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitResult(void)
{
	s_nTime = 0;
	s_nDigit = 0;
	s_nDiffDigit = 0;

	switch (s_result)
	{
	case RESULT_CLEAR:			// クリア

		// クリア
		InitClear();

		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_ゲームクリア);

		break;

	case RESULT_GAMEOVER:		// ゲームオーバー

		// ゲームオーバー
		InitGameOver();

		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_ゲームオーバー);

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

	if (s_pTextureMinus != NULL)
	{// テクスチャの破棄
		s_pTextureMinus->Release();
		s_pTextureMinus = NULL;
	}

	if (s_pVtxBuffMinus != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffMinus->Release();
		s_pVtxBuffMinus = NULL;
	}

	if (s_pTextureDecimal != NULL)
	{// テクスチャの破棄
		s_pTextureDecimal->Release();
		s_pTextureDecimal = NULL;
	}

	if (s_pVtxBuffDecimal != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffDecimal->Release();
		s_pVtxBuffDecimal = NULL;
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

	if (s_pTextureOperator != NULL)
	{// テクスチャの破棄
		s_pTextureOperator->Release();
		s_pTextureOperator = NULL;
	}

	if (s_pVtxBuffOperator != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffOperator->Release();
		s_pVtxBuffOperator = NULL;
	}

	if (s_pTextureDiffMeter != NULL)
	{// テクスチャの破棄
		s_pTextureDiffMeter->Release();
		s_pTextureDiffMeter = NULL;
	}

	if (s_pVtxBuffDiffMeter != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffDiffMeter->Release();
		s_pVtxBuffDiffMeter = NULL;
	}

	if (s_pTextureDiffDecimal != NULL)
	{// テクスチャの破棄
		s_pTextureDiffDecimal->Release();
		s_pTextureDiffDecimal = NULL;
	}

	if (s_pVtxBuffDiffDecimal != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffDiffDecimal->Release();
		s_pVtxBuffDiffDecimal = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateResult(void)
{
	if (GetKeyboardAllTrigger() || GetJoypadAllTrigger())
	{// ボタンが押されたかどうか

		if (GetRemix())
		{// リミックス中
			int nMenu = (int)GetTitle() + 1;

			if (nMenu >= MENU_MAX)
			{// 終わり
				// モード処理
				SetFade(MODE_TITLE);
			}
			else
			{// まだ続く
				// フェードの設定
				SetFade(MODE_GAME);
			}
		}
		else
		{// リミックスではない
			// モード処理
			SetFade(MODE_TITLE);
		}
	}

	s_nTime++;

	if (GetRemix())
	{// リミックス中
		if (s_nTime >= 180)
		{// 180秒経ちました
			int nMenu = (int)GetTitle() + 1;

			if (nMenu >= MENU_MAX)
			{// 終わり
				// モード処理
				SetFade(MODE_TITLE);
			}
			else
			{// まだ続く
				// フェードの設定
				SetFade(MODE_GAME);
			}
		}
	}
	else
	{// リミックスではない
		if (s_nTime >= 900)
		{// 15秒経ちました
			// モード処理
			SetFade(MODE_TITLE);
		}
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
// 取得
//--------------------------------------------------
RESULT GetResult(void)
{
	return s_result;
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
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.225f, SCREEN_HEIGHT * 0.75f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	switch (GetTitle())
	{// どのゲーム？
	case MENU_WALKING:		// ウォーキング
	case MENU_ROTATION:		// 回転
	case MENU_STOP:			// 止める

		// 頂点カラーの設定処理
		Setcol(pVtx, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

		break;

	case MENU_SLOPE:		// 坂

		// 頂点カラーの初期化
		Initcol(pVtx);

		break;

	default:
		assert(false);
		break;
	}

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// 位置の数字
	InitPosNumber();

	// メートル
	InitMeter();

	// 小数点
	InitDecimal();

	// マイナス
	InitMinus();

	if (GetTitle() == MENU_WALKING)
	{// ウォーキング
		// 差
		InitDifference();

		// 差のメートル
		InitDiffMeter();

		// 差の小数点
		InitDiffDecimal();
	}
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
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.75f, 0.0f);

	int nPos = 0;
	float fModel = 0.0f, fPlayer = 0.0f;

	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
		
		nPos = (int)(GetPlayer()->pos.x / 3);

		if (GetPlayer()->pos.x <= 0.0f)
		{// マイナス
			nPos *= -1;
		}

		break;

	case MENU_ROTATION:		// 回転

		fModel = GetModel()->rot.z * (180 / D3DX_PI);

		nPos = (90 + (int)fModel) * 10;

		// 止めるの設定
		SetScoreRotation(nPos);

		break;

	case MENU_STOP:			// 止める
		
		fModel = GetModel()->pos.y - GetModel()->vtxMax.x;
		fPlayer = GetPlayer()->pos.y + GetPlayer()->fHeight;

		nPos = (int)(fModel - fPlayer) * 10;

		// 止めるの設定
		SetScoreStop(nPos);

		break;

	case MENU_SLOPE:		// 坂

		fModel = GetField()->pos.x + GetField()->vtxMax.x;
		fPlayer = GetPlayer()->pos.x + GetPlayer()->fSize;

		nPos = (int)(fModel - fPlayer);

		// 坂の設定
		SetScoreSlope(nPos);

		break;

	default:
		assert(false);
		break;
	}

	int nNumber = nPos;

	while (1)
	{
		
		if (nNumber >= 10)
		{
			nNumber /= 10;
			s_nDigit++;
		}
		else
		{
			s_nDigit++;

			if (s_nDigit < MIN_RESULT)
			{
				s_nDigit = MIN_RESULT;
			}

			break;
		}
	}

	int aNumber[MAX_RESULT];

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int i = 0; i < s_nDigit; i++)
	{// １桁ずつに分ける
		aNumber[i] = nPos % 10;
		nPos /= 10;

		float fInterval = (NUMBER_WIDTH * i);

		float fDecimal = 0.0f;

		if (i >= 1)
		{
			fDecimal = DECIMAL_INTERVAL;
		}

		// 数の設定処理
		SetRightNumber(D3DXVECTOR3(pos.x - fInterval - fDecimal, pos.y, 0.0f), col, NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_RESULT);
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
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.8f, 0.0f);

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
// 小数点
//--------------------------------------------------
static void InitDecimal(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/decimal.png",
		&s_pTextureDecimal);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffDecimal,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffDecimal->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = DECIMAL_WIDTH * 0.5f;
	float fHeight = DECIMAL_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.755f, SCREEN_HEIGHT * 0.77f, 0.0f);

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffDecimal->Unlock();
}

//--------------------------------------------------
// マイナス
//--------------------------------------------------
static void InitMinus(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/minus.png",
		&s_pTextureMinus);

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

	float fPosX = (SCREEN_WIDTH * 0.85f) - DECIMAL_INTERVAL - (NUMBER_WIDTH * s_nDigit);

	D3DXVECTOR3 pos = D3DXVECTOR3(fPosX - fWidth, SCREEN_HEIGHT * 0.75f, 0.0f);

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
// 差
//--------------------------------------------------
static void InitDifference(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nPos = (int)(GetPlayer()->pos.x / 3);
	int nTarget = GetTarget() * 10;

	int nDiff = nTarget - nPos;

	// ウォーキングの設定
	SetScoreWalking(nDiff);

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (nDiff < 0)
	{// マイナス
		col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			"data/TEXTURE/plus.png",
			&s_pTextureOperator);
	}
	else
	{// プラス
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			"data/TEXTURE/minus.png",
			&s_pTextureOperator);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffOperator,
		NULL);

	if (nDiff < 0.0f)
	{// マイナス
		nDiff *= -1;
	}

	int nNumber = nDiff;

	while (1)
	{// 無限ループ
		if (nNumber >= 10)
		{// 2桁以上
			nNumber /= 10;
			s_nDiffDigit++;
		}
		else
		{// 1桁
			s_nDiffDigit++;

			if (s_nDiffDigit < MIN_RESULT)
			{
				s_nDiffDigit = MIN_RESULT;
			}

			break;
		}
	}

	int aNumber[MAX_RESULT];

	float fNumberWidth = NUMBER_WIDTH * 0.8f;
	float fNumberHeight = NUMBER_HEIGHT * 0.8f;

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.15f, 0.0f);

	for (int i = 0; i < s_nDiffDigit; i++)
	{// １桁ずつに分ける
		aNumber[i] = nDiff % 10;
		nDiff /= 10;

		float fInterval = (fNumberWidth * i);

		float fDecimal = 0.0f;

		if (i >= 1)
		{
			fDecimal = DECIMAL_INTERVAL;
		}

		// 数の設定処理
		SetRightNumber(D3DXVECTOR3(pos.x - fInterval - fDecimal, pos.y, 0.0f), col, fNumberWidth, fNumberHeight * 0.5f, aNumber[i], i, USE_RESULT);
	}

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffOperator->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = (MINUS_WIDTH * 0.8f) * 0.5f;
	float fHeight = (MINUS_HEIGHT * 0.8f) * 0.5f;

	float fPosX = (SCREEN_WIDTH * 0.9f) - DECIMAL_INTERVAL - (fNumberWidth * s_nDiffDigit);

	pos = D3DXVECTOR3(fPosX - fWidth, SCREEN_HEIGHT * 0.15f, 0.0f);

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの設定
	Setcol(pVtx, col);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffOperator->Unlock();
}

//--------------------------------------------------
// 差のメートル
//--------------------------------------------------
static void InitDiffMeter(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/METER/m.png",
		&s_pTextureDiffMeter);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffDiffMeter,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffDiffMeter->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = (METER_WIDTH * 0.8f) * 0.5f;
	float fHeight = (METER_HEIGHT * 0.8f) * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.925f, SCREEN_HEIGHT * 0.19f, 0.0f);
	
	int nPos = (int)(GetPlayer()->pos.x / 3);
	int nTarget = GetTarget() * 10;

	int nDiff = nTarget - nPos;

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (nDiff < 0)
	{// マイナス
		col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{// プラス
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの設定
	Setcol(pVtx, col);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffDiffMeter->Unlock();
}

//--------------------------------------------------
// 差の小数点
//--------------------------------------------------
static void InitDiffDecimal(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/decimal.png",
		&s_pTextureDiffDecimal);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffDiffDecimal,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffDiffDecimal->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = (DECIMAL_WIDTH * 0.8f) * 0.5f;
	float fHeight = (DECIMAL_HEIGHT * 0.8f) * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.825f, SCREEN_HEIGHT * 0.17f, 0.0f);

	int nPos = (int)(GetPlayer()->pos.x / 3);
	int nTarget = GetTarget() * 10;

	int nDiff = nTarget - nPos;

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (nDiff < 0)
	{// マイナス
		col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{// プラス
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの設定
	Setcol(pVtx, col);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffDiffDecimal->Unlock();
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
	pDevice->SetStreamSource(0, s_pVtxBuffDecimal, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureDecimal);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	if (GetPlayer()->pos.x <= -30.0f)
	{// マイナス
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuffMinus, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTextureMinus);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}

	if (GetTitle() == MENU_WALKING)
	{// ウォーキング
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuffOperator, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTextureOperator);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuffDiffMeter, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTextureDiffMeter);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuffDiffDecimal, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTextureDiffDecimal);

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
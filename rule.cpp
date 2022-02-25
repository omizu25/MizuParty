//==================================================
// 
// 3Dゲーム制作 ( rule.cpp )
// Author  : katsuki mizuki
// 
//==================================================
#include "fade.h"
#include "input.h"
#include "number.h"
#include "player.h"
#include "rule.h"
#include "setup.h"
#include "time.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define RULE_WIDTH			(160.0f)		// 説明の幅
#define RULE_HEIGHT			(100.0f)		// 説明の高さ
#define A_OR_D_WIDTH		(170.0f)		// AorDの幅
#define A_OR_D_HEIGHT		(70.0f)			// AorDの高さ
#define STICK_WIDTH			(220.0f)		// スティックの幅
#define STICK_HEIGHT		(100.0f)		// スティックの高さ
#define SPACE_WIDTH			(170.0f)		// スペースの幅
#define SPACE_HEIGHT		(70.0f)			// スペースの高さ
#define HINT_WIDTH			(300.0f)		// ヒントの幅
#define HINT_HEIGHT			(80.0f)			// ヒントの高さ

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureAorD = NULL;		// AorDのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffAorD = NULL;		// AorDの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureStick= NULL;		// スティックのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffStick = NULL;		// スティックの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureStop = NULL;		// 止めるのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffStop = NULL;		// 止めるの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureAorB = NULL;		// AorBのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffAorB = NULL;		// AorBの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureSpace = NULL;		// スペースのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffSpace = NULL;		// スペースの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureHint = NULL;		// ヒントのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffHint = NULL;		// ヒントの頂点バッファのポインタ

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void InitMove(void);
static void InitAorD(void);
static void InitStick(void);
static void InitStop(void);
static void InitAorB(void);
static void InitSpace(void);
static void InitHint(void);
static void DrawStop(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRule(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
	case MENU_SLOPE:		// 坂

		// 移動
		InitMove();

		// AorD
		InitAorD();

		// スティック
		InitStick();

		break;

	case MENU_ROTATION:		// 回転
	case MENU_STOP:			// 止める

		// 止める
		InitStop();

		// AorB
		InitAorB();

		// スペース
		InitSpace();

		// ヒント
		InitHint();

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitRule(void)
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

	if (s_pTextureAorD != NULL)
	{// テクスチャの破棄
		s_pTextureAorD->Release();
		s_pTextureAorD = NULL;
	}

	if (s_pVtxBuffAorD != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffAorD->Release();
		s_pVtxBuffAorD = NULL;
	}

	if (s_pTextureStick != NULL)
	{// テクスチャの破棄
		s_pTextureStick->Release();
		s_pTextureStick = NULL;
	}

	if (s_pVtxBuffStick != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffStick->Release();
		s_pVtxBuffStick = NULL;
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

	if (s_pTextureAorB != NULL)
	{// テクスチャの破棄
		s_pTextureAorB->Release();
		s_pTextureAorB = NULL;
	}

	if (s_pVtxBuffAorB != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffAorB->Release();
		s_pVtxBuffAorB = NULL;
	}

	if (s_pTextureSpace != NULL)
	{// テクスチャの破棄
		s_pTextureSpace->Release();
		s_pTextureSpace = NULL;
	}

	if (s_pVtxBuffSpace != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffSpace->Release();
		s_pVtxBuffSpace = NULL;
	}

	if (s_pTextureHint != NULL)
	{// テクスチャの破棄
		s_pTextureHint->Release();
		s_pTextureHint = NULL;
	}

	if (s_pVtxBuffHint != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffHint->Release();
		s_pVtxBuffHint = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateRule(void)
{

}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawRule(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
	case MENU_SLOPE:		// 坂

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
		pDevice->SetStreamSource(0, s_pVtxBuffAorD, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTextureAorD);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuffStick, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTextureStick);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数

		break;

	case MENU_ROTATION:		// 回転
	case MENU_STOP:			// 止める

		// 止める
		DrawStop();

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 移動
//--------------------------------------------------
static void InitMove(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\move.png",
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

	float fWidth = RULE_WIDTH * 0.5f;
	float fHeight = RULE_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.91f, SCREEN_HEIGHT * 0.15f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// AorD
//--------------------------------------------------
static void InitAorD(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/AorD.png",
		&s_pTextureAorD);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffAorD,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffAorD->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = A_OR_D_WIDTH * 0.5f;
	float fHeight = A_OR_D_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.085f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffAorD->Unlock();
}

//--------------------------------------------------
// スティック
//--------------------------------------------------
static void InitStick(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/stick.png",
		&s_pTextureStick);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffStick,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffStick->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = STICK_WIDTH * 0.5f;
	float fHeight = STICK_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.225f, 0.0f);

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffStick->Unlock();
}

//--------------------------------------------------
// とめる
//--------------------------------------------------
static void InitStop(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\stop.png",
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

	float fWidth = RULE_WIDTH * 0.5f;
	float fHeight = RULE_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.15f, 0.0f);

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
// AorB
//--------------------------------------------------
static void InitAorB(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/AorB.png",
		&s_pTextureAorB);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffAorB,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffAorB->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = A_OR_D_WIDTH * 0.5f;
	float fHeight = A_OR_D_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.225f, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化処理
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffAorB->Unlock();
}

//--------------------------------------------------
// スペース
//--------------------------------------------------
static void InitSpace(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/space.png",
		&s_pTextureSpace);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffSpace,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffSpace->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SPACE_WIDTH * 0.5f;
	float fHeight = SPACE_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.075f, 0.0f);

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffSpace->Unlock();
}

//--------------------------------------------------
// ヒント
//--------------------------------------------------
static void InitHint(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/hint.png",
		&s_pTextureHint);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffHint,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffHint->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = HINT_WIDTH * 0.5f;
	float fHeight = HINT_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの初期化
	Initcol(pVtx);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffHint->Unlock();
}

//--------------------------------------------------
// 止める
//--------------------------------------------------
static void DrawStop(void)
{
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

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffAorB, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureAorB);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffSpace, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureSpace);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffHint, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureHint);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数
}
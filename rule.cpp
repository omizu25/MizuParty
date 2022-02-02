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

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define RESULT_WIDTH		(400.0f)		// 幅
#define RESULT_HEIGHT		(150.0f)		// 高さ
#define A_OR_D_WIDTH		(200.0f)		// AorDの幅
#define A_OR_D_HEIGHT		(80.0f)		// AorDの高さ
#define STICK_WIDTH			(200.0f)		// スティックの幅
#define STICK_HEIGHT		(80.0f)		// スティックの高さ

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureAorD = NULL;		// AorDのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffAorD = NULL;		// AorDの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureStick= NULL;		// スティックのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffStick = NULL;		// スティックの頂点バッファのポインタ

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void InitAorD(void);
static void InitStick(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRule(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\word_move.png",
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

	float fWidth = RESULT_WIDTH * 0.5f;
	float fHeight = RESULT_HEIGHT * 0.5f;
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
	s_pVtxBuff->Unlock();

	// AorD
	InitAorD();

	// スティック
	InitStick();
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
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.075f, 0.0f);

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
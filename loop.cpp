//==================================================
// 
// 3Dゲーム制作 ( loop.cpp )
// Author  : katsuki mizuki
// 
//==================================================
//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "loop.h"
#include "setup.h"
#include "sound.h"
#include "title.h"

#include <assert.h>
#include <math.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define NEXT_TIME			(900)			// 次に行くまで時間
#define LOOP_WIDTH			(1000.0f)		// ループの幅
#define LOOP_HEIGHT			(300.0f)		// ループの高さ
#define SELECT_WIDTH		(350.0f)		// 選択肢の幅
#define SELECT_HEIGHT		(200.0f)		// 選択肢の高さ
#define DEAD_ZONE			(0.1f)			// スティックの遊び

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTextureLoop = NULL;		// ループのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffLoop = NULL;		// ループの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureEnd = NULL;		// やめるのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffEnd = NULL;		// やめるの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTexturePlay = NULL;		// 遊ぶのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffPlay = NULL;		// 遊ぶの頂点バッファのポインタ
static int							s_nTime;					// 時間
static int							s_nAlpha;					// α値をsinカーブさせる用
static bool							s_bLoop;					// ループするかどうか
static bool							s_bStickLeft;				// 左スティック入力があるかどうか
static bool							s_bStickRight;				// 右スティック入力があるかどうか

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void Input(void);
static void SelectChange(bool bDirection);
static void SelectAlpha(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitLoop(void)
{
	s_nTime = 0;
	s_nAlpha = 0;
	s_bLoop = true;
	s_bStickLeft = false;
	s_bStickRight = false;

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/loop.png",
		&s_pTextureLoop);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/end.png",
		&s_pTextureEnd);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/play.png",
		&s_pTexturePlay);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffLoop,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffEnd,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPlay,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffLoop->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = LOOP_WIDTH * 0.5f;
	float fHeight = LOOP_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f);

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの設定
	Initcol(pVtx);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffLoop->Unlock();

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	fWidth = SELECT_WIDTH * 0.5f;
	fHeight = SELECT_HEIGHT * 0.5f;
	pos = D3DXVECTOR3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.7f, 0.0f);

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの設定
	Initcol(pVtx);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffEnd->Unlock();

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffPlay->Lock(0, 0, (void**)&pVtx, 0);

	pos.x = SCREEN_WIDTH * 0.3f;

	// 頂点座標の設定
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化
	Initrhw(pVtx);

	// 頂点カラーの設定
	Initcol(pVtx);

	// テクスチャ座標の初期化
	Inittex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuffPlay->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitLoop(void)
{
	if (s_pTextureLoop != NULL)
	{// テクスチャの破棄
		s_pTextureLoop->Release();
		s_pTextureLoop = NULL;
	}

	if (s_pVtxBuffLoop != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffLoop->Release();
		s_pVtxBuffLoop = NULL;
	}

	if (s_pTextureEnd != NULL)
	{// テクスチャの破棄
		s_pTextureEnd->Release();
		s_pTextureEnd = NULL;
	}

	if (s_pVtxBuffEnd != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffEnd->Release();
		s_pVtxBuffEnd = NULL;
	}

	if (s_pTexturePlay != NULL)
	{// テクスチャの破棄
		s_pTexturePlay->Release();
		s_pTexturePlay = NULL;
	}

	if (s_pVtxBuffPlay != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuffPlay->Release();
		s_pVtxBuffPlay = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateLoop(void)
{
	// 入力
	Input();

	// 選択肢のα値変更
	SelectAlpha();

	s_nTime++;

	if (s_nTime >= NEXT_TIME)
	{// 15秒経ちました
		if (s_bLoop)
		{// ループする
			// フェードの設定
			SetFade(MODE_GAME);
		}
		else
		{// ループしない
			// モード処理
			SetFade(MODE_TITLE);
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawLoop(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffLoop, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureLoop);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffEnd, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureEnd);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffPlay, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexturePlay);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数
}

//--------------------------------------------------
// 入力
//--------------------------------------------------
static void Input(void)
{
	if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x <= DEAD_ZONE &&
		GetJoypadStick(JOYKEY_LEFT_STICK, 0).x >= -DEAD_ZONE)
	{// 左スティックが傾いてない
		s_bStickLeft = false;
	}

	if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x <= DEAD_ZONE &&
		GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x >= -DEAD_ZONE)
	{// 右スティックが傾いてない
		s_bStickRight = false;
	}

	if (!s_bStickLeft)
	{// 左スティックが傾いてない
		if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -DEAD_ZONE)
		{// 左スティックが傾いた
			s_bStickLeft = true;

			if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < 0.0f)
			{// 左
				// 選択肢の変更
				SelectChange(true);
			}
			else
			{// 右
				// 選択肢の変更
				SelectChange(false);
			}
		}
	}

	if (!s_bStickRight)
	{// 右スティックが傾いてない
		if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x < -DEAD_ZONE)
		{// 右スティックが傾いた
			s_bStickRight = true;

			if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x < 0.0f)
			{// 左
				// 選択肢の変更
				SelectChange(true);
			}
			else
			{// 右
				// 選択肢の変更
				SelectChange(false);
			}
		}
	}

	if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFT) ||
		GetJoypadTrigger(JOYKEY_LEFT))
	{// Aキーが押された
		// 選択肢の変更
		SelectChange(true);
	}
	else if(GetKeyboardTrigger(DIK_D) ||GetKeyboardTrigger(DIK_RIGHT) ||
		GetJoypadTrigger(JOYKEY_RIGHT))
	{// Dキーが押された
		// 選択肢の変更
		SelectChange(false);
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) ||
		GetJoypadTrigger(JOYKEY_B) || GetJoypadTrigger(JOYKEY_A) ||
		GetJoypadTrigger(JOYKEY_START))
	{// スペースキーが押された
		if (s_bLoop)
		{// ループする
			// フェードの設定
			SetFade(MODE_GAME);
		}
		else
		{// ループしない
			// モード処理
			SetFade(MODE_TITLE);
		}

		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_決定音);
	}
}

//--------------------------------------------------
// 選択肢の変更
// false : 右 true : 左
//--------------------------------------------------
static void SelectChange(bool bDirection)
{
	if (bDirection)
	{// 左
		s_bLoop = true;

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの初期化
		Initcol(pVtx);

		// 頂点バッファをアンロックする
		s_pVtxBuffEnd->Unlock();
	}
	else
	{// 右
		s_bLoop = false;

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffPlay->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの初期化
		Initcol(pVtx);

		// 頂点バッファをアンロックする
		s_pVtxBuffPlay->Unlock();
	}

	s_nAlpha = 0;
	s_nTime = 0;

	// サウンドの再生
	PlaySound(SOUND_LABEL_SE_選択音);
}

//--------------------------------------------------
// 選択肢のα値変更
//--------------------------------------------------
static void SelectAlpha(void)
{
	s_nAlpha++;

	if (s_bLoop)
	{// ループする
		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		float fAlpha = (sinf((s_nAlpha * 0.01f) * (D3DX_PI * 2.0f)) * 0.4f) + 0.6f;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffPlay->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		Setcol(pVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha));

		// 頂点バッファをアンロックする
		s_pVtxBuffPlay->Unlock();
	}
	else
	{// ループしない
		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		float fAlpha = (sinf((s_nAlpha * 0.01f) * (D3DX_PI * 2.0f)) * 0.4f) + 0.6f;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		Setcol(pVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha));

		// 頂点バッファをアンロックする
		s_pVtxBuffEnd->Unlock();
	}
}
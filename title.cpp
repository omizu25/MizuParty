//==================================================
// 
// 3Dゲーム制作 ( title.cpp )
// Author  : katsuki mizuki
// 
//==================================================
#include "main.h"
#include "fade.h"
#include "input.h"
#include "player.h"
#include "result.h"
#include "setup.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_TITLE			(3)				// タイトルの最大数
#define VS_WIDTH			(150.0f)		// VSの幅
#define VS_HEIGHT			(200.0f)		// VSの高さ
#define TITLE_WIDTH			(500.0f)		// タイトルの幅
#define TITLE_HEIGHT		(350.0f)		// タイトルの高さ
#define MENU_WIDTH			(550.0f)		// メニューの幅
#define MENU_HEIGHT			(175.0f)		// メニューの高さ
#define WIDTH_INTERVAL		(125.0f)		// 幅の間隔
#define HEIGHT_INTERVAL		(125.0f)		// 高さの間隔

//--------------------------------------------------
// 列挙型
//--------------------------------------------------
typedef enum
{
	MENUSTATE_IN = 0,		// 見える
	MENUSTATE_OUT,			// 見えない
	MENUSTATE_MAX
}MENUSTATE;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void UpdateInput(VERTEX_2D *pVtx);
static void UpdateMenu(void);
static void UpdateState(void);

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;			// 背景の頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTexture[MAX_TITLE];			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// 頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMenu[MENU_MAX];		// メニューのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMenu = NULL;			// メニューの頂点バッファのポインタ
static MENU							s_Menu;							// メニュー
static MENUSTATE					s_state;						// メニューの状態
static D3DXCOLOR					s_col;							// メニューの色
static float						s_fChange;						// α値の変化量

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitTitle(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// メモリのクリア
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));
	
	// メモリのクリア
	memset(&s_pTextureMenu[0], NULL, sizeof(s_pTextureMenu));

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title013.png",
		&s_pTexture[0]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title014.png",
		&s_pTexture[1]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title015.png",
		&s_pTexture[2]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title002.png",
		&s_pTextureMenu[MENU_WALKING]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title003.png",
		&s_pTextureMenu[MENU_STOP]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title004.png",
		&s_pTextureMenu[MENU_RANKING]);

	s_Menu = MENU_WALKING;
	s_state = MENUSTATE_IN;
	s_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	s_fChange = 0.025f;

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
		sizeof(VERTEX_2D) * 4 * MAX_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMenu,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// 頂点座標の設定処理
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの設定処理
	Setcol(pVtx, D3DXCOLOR(0.615f, 0.215f, 0.341f, 1.0f));

	// 頂点バッファをアンロックする
	s_pVtxBuffBG->Unlock();

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_TITLE; i++)
	{
		float fTitleWidth = 0.0f;
		float fTitleHeight = 0.0f;
		D3DXVECTOR3 TitlePos = D3DXVECTOR3(0.0f, fHeight - 175.0f, 0.0f);

		switch (i)
		{
		case 0:
			TitlePos.x = 280.0f;
			fTitleWidth = TITLE_WIDTH * 0.5f;
			fTitleHeight = TITLE_HEIGHT * 0.5f;
			break;

		case 1:
			TitlePos.x = 630.0f;
			fTitleWidth = VS_WIDTH * 0.5f;
			fTitleHeight = VS_HEIGHT * 0.5f;
			break;

		case 2:
			TitlePos.x = 1005.0f;
			fTitleWidth = TITLE_WIDTH * 0.5f;
			fTitleHeight = TITLE_HEIGHT * 0.5f;
			break;

		default:
			assert(false);
			break;
		}

		// 頂点座標の設定処理
		Setpos(pVtx, TitlePos, fTitleWidth, fTitleHeight, SETPOS_MIDDLE);

		// rhwの初期化処理
		Initrhw(pVtx);

		// 頂点カラーの初期化処理
		Initcol(pVtx);

		// テクスチャ座標の設定処理
		Settex(pVtx, 0.005f, 0.998f, 0.005f, 1.0f);

		pVtx += 4;		// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MENU_MAX; i++)
	{
		float fMenuWidth = MENU_WIDTH * 0.5f;
		float fMenuHeight = MENU_HEIGHT * 0.5f;
		D3DXVECTOR3 MenuPos = D3DXVECTOR3(fWidth, fHeight + 25.0f, 0.0f);

		// 頂点座標の設定処理
		Setpos(pVtx, MenuPos + D3DXVECTOR3(0.0f, (HEIGHT_INTERVAL * i), 0.0f), fMenuWidth, fMenuHeight, SETPOS_MIDDLE);

		// rhwの初期化処理
		Initrhw(pVtx);

		// 頂点カラーの設定処理
		Setcol(pVtx, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));

		// テクスチャ座標の初期化処理
		Inittex(pVtx);

		pVtx += 4;		// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロックする
	s_pVtxBuffMenu->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTitle(void)
{
	if (s_pVtxBuffBG != NULL)
	{// 背景の頂点バッファの破棄
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	for (int i = 0; i < MAX_TITLE; i++)
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

	for (int i = 0; i < MENU_MAX; i++)
	{
		if (s_pTextureMenu[i] != NULL)
		{// メニューのテクスチャの破棄
			s_pTextureMenu[i]->Release();
			s_pTextureMenu[i] = NULL;
		}
	}

	if (s_pVtxBuffMenu != NULL)
	{// メニューの頂点バッファの破棄
		s_pVtxBuffMenu->Release();
		s_pVtxBuffMenu = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	if (GetFade() == FADE_NONE)
	{// 何もしていない状態なら
		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		// メニューの入力時処理
		UpdateInput(pVtx);

		// メニューの選択処理
		UpdateMenu();

		// 頂点バッファをアンロックする
		s_pVtxBuffMenu->Unlock();
	}

	// 状態の更新処理
	UpdateState();

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (s_Menu * 4);		// 該当の位置まで進める

	// 頂点カラーの設定処理
	Setcol(pVtx, s_col);

	// 頂点バッファをアンロックする
	s_pVtxBuffMenu->Unlock();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawTitle(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_TITLE; i++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[i]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			i * 4,						// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MENU_MAX; i++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, s_pTextureMenu[i]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			i * 4,						// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
MENU GetTitle(void)
{
	return s_Menu;
}

//--------------------------------------------------
// メニューの入力
//--------------------------------------------------
static void UpdateInput(VERTEX_2D *pVtx)
{
	int nMenu = (int)s_Menu;		// メニューをintに

	if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_S) ||
		GetJoypadTrigger(JOYKEY_UP, 0) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// Wキー、Sキーが押されたかどうか
		// 色の初期化
		s_col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		// 状態(点滅具合)の初期化
		s_state = MENUSTATE_IN;

		pVtx += (s_Menu * 4);		// 該当の位置まで進める

		// 頂点カラーの設定処理
		Setcol(pVtx, s_col);
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, 0))
	{// Wキーが押されたかどうか
		nMenu = ((nMenu - 1) + 3) % 3;

		s_Menu = (MENU)nMenu;

		s_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// Sキーが押されたかどうか
		nMenu = ((nMenu + 1) + 3) % 3;

		s_Menu = (MENU)nMenu;

		s_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	}
}

//--------------------------------------------------
// メニューの選択
//--------------------------------------------------
static void UpdateMenu(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || 
		GetJoypadTrigger(JOYKEY_B, 0) || GetJoypadTrigger(JOYKEY_START, 0))
	{// 決定キー(ENTERキー)が押されたかどうか
		switch (s_Menu)
		{
		case MENU_WALKING:		// ゲーム
		case MENU_STOP:			// 止める
		case MENU_RANKING:		// ランキング
			// フェードの設定
			SetFade(MODE_GAME);
			break;
		
		default:
			assert(false);
			break;
		}

		s_fChange = 1.0f;
	}
}

//--------------------------------------------------
// 状態
//--------------------------------------------------
static void UpdateState(void)
{
	if (s_state == MENUSTATE_IN)
	{// フェードイン状態
		s_col.a -= s_fChange;		// ポリゴンを透明にしていく

		if (s_col.a <= 0.25f)
		{
			s_col.a = 0.25f;
			s_state = MENUSTATE_OUT;		// アウト状態に
		}
	}
	else if (s_state == MENUSTATE_OUT)
	{// フェードアウト状態
		s_col.a += s_fChange;		// ポリゴンを不透明にしていく

		if (s_col.a >= 1.0f)
		{
			s_col.a = 1.0f;
			s_state = MENUSTATE_IN;			// イン状態に
		}
	}
}
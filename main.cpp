//==================================================
// 
// 3Dゲーム制作 ( main.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "fade.h"
#include "camera.h"
#include "input.h"
#include "game.h"
#include "ranking.h"
#include "result.h"
#include "sound.h"
#include "title.h"

#include <stdio.h>
#include <assert.h>
#include <time.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define CLASS_NAME		"windowClass"		// ウインドウクラスの名前
#define WINDOW_NAME		"Mizu Party !!"		// ウインドウの名前 (キャプションに表示)

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
static void Uninit(void);
static void Update(void);
static void Draw(void);
static void DrawDebug(void);

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3D9				s_pD3D = NULL;				// Direct3Dオブジェクトへのポインタ
static LPDIRECT3DDEVICE9		s_pD3DDevice = NULL;		// Direct3Dデバイスへのポインタ
static LPD3DXFONT				s_pFont = NULL;				// フォントへのポインタ
static int						s_nCountFPS = 0;			// FPSカウンタ
static MODE						s_mode = MODE_TITLE;		// 現在のモード
static bool						s_bDebug = true;			// デバッグ表示をするか [表示  : true 非表示  : false]

//--------------------------------------------------
// main関数
//--------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{// ウインドウクラスの構造体
		sizeof(WNDCLASSEX),						// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,								// ウインドウのスタイル
		WindowProc,								// ウインドウプロシージャ
		0,										// ０にする (通常は使用しない)
		0,										// ０にする (通常は使用しない)
		hInstance,								// インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),		// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),			// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),				// クライアント領域の背景色
		NULL,									// メニューバー
		CLASS_NAME,								// ウインドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)			// ファイルのアイコン
	};

	HWND hWnd;												// ウインドウハンドル (識別子)
	MSG msg;												// メッセージを格納する変数
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};		// 画面サイズの構造体
	DWORD dwCurrentTime;									// 現在時刻
	DWORD dwExecLastTime;									// 最後に処理した時刻
	DWORD dwFrameCount;										// フレームカウント
	DWORD dwFPSLastTime;									// 最後にFPSを計算した時刻

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを作成
	hWnd = CreateWindowEx(
		0,								// 拡張ウインドウスタイル
		CLASS_NAME,						// ウインドウスタイルの名前
		WINDOW_NAME,					// ウインドウの名前
		WS_OVERLAPPEDWINDOW,			// ウインドウスタイル
		CW_USEDEFAULT,					// ウインドウの左上X座標
		CW_USEDEFAULT,					// 　　〃　　の左上Y座標
		(rect.right - rect.left),		// 　　〃　　の幅
		(rect.bottom - rect.top),		// 　　〃　　の高さ
		NULL,							// 親ウインドウのハンドル
		NULL,							// メニューハンドルまたは子ウインドウID
		hInstance,						// インスタンスハンドル
		NULL);							// ウインドウ作成データ

	// 初期化
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{// 初期化が失敗した場合
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;					// 初期化する
	dwExecLastTime = timeGetTime();		// 現在時刻を取得 (保存)
	dwFrameCount = 0;					// 初期化する
	dwFPSLastTime = timeGetTime();		// 現在時刻を取得 (保存)

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);			// ウインドウの表示状態を設定
	UpdateWindow(hWnd);					// クライアント領域を更新

	while (1)
	{// メッセージループ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{// メッセージの設定
				TranslateMessage(&msg);		// 仮想メッセージを文字のメッセージへ変換
				DispatchMessage(&msg);		// ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();		// 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
				// FPSの計算
				s_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		// FPSを計測した時刻を保存

				dwFrameCount = 0;					// フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60分の1秒経過
				dwExecLastTime = dwCurrentTime;		// 処理開始の時刻[現在時刻]を保存

				// 更新
				Update();

				// 描画
				Draw();

				dwFrameCount++;						// フレームカウントを加算
			}
		}
	}
	
	// 終了
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//--------------------------------------------------
// ウインドウプロシージャ
//--------------------------------------------------
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:		// ウインドウ破棄のメッセージ
		// WM_QOITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:		// [ESC]キーが押された
			// ウインドウを破棄する (WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
			break;
		}
		break;
	}
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);		// 規定の処理を返す
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
static HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;				// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;		// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	s_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (s_pD3D == NULL)
	{// 開かなかった時用の確認
		return E_FAIL;
	}

	if (FAILED(s_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{// 現在のディスプレイモードを取得
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							// バックバッファの形式
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// Direct3Dデバイスの生成(描画処理と頂点処理をハードウェア)
	if (FAILED(s_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&s_pD3DDevice)))
	{
		// Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行う)
		if (FAILED(s_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&s_pD3DDevice)))
		{
			// Direct3Dデバイスの生成(描画処理と頂点処理をCPUで行う)
			if (FAILED(s_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&s_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//--------------------------------------------------
	// 各種オブジェクトの初期化
	//--------------------------------------------------

	// レンダーステートの設定
	s_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	s_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	s_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	s_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// 画像を小さくしても綺麗にする
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// 画像を大きくしても綺麗にする
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// テクスチャのUの繰り返し方を設定
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// テクスチャのVの繰り返し方を設定

	// テクスチャステージステートパラメータの設定
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		// ポリゴンとテクスチャのαをまぜる
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// １つ目の色はテクスチャの色
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		// ２つ目の色は現在の色

	// デバッグ表示用フォントの生成
	D3DXCreateFont(
		s_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &s_pFont);

	//世界の種子の初期化
	srand((unsigned int)time(NULL));

	// 入力処理の初期化
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// サウンドの初期化
	InitSound(hWnd);

	// タイトルへのカウントの設定
	SetTitleCnt();

	// ランキングの読み込み
	LoadRanking();

	// フェードの設定
	InitFade(s_mode);

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
static void Uninit(void)
{
	//--------------------------------------------------
	// 各種オブジェクトの終了
	//--------------------------------------------------

	// 入力処理の終了
	UninitInput();

	// サウンドの終了
	UninitSound();

	// ランキングのセーブ
	SaveRanking();

	switch (s_mode)
	{// どのモード？
	case MODE_TITLE:		// タイトル
		UninitTitle();
		break;

	case MODE_GAME:			// ゲーム
		UninitGame();
		break;

	default:
		assert(false);
		break;
	}

	if (s_pFont != NULL)
	{// デバッグ表示用フォントの解放
		s_pFont->Release();
		s_pFont = NULL;
	}

	if (s_pD3DDevice != NULL)
	{// Direct3Dデバイスの解放
		s_pD3DDevice->Release();
		s_pD3DDevice = NULL;
	}

	if (s_pD3D != NULL)
	{// Direct3Dオブジェクトの解放
		s_pD3D->Release();
		s_pD3D = NULL;
	}
}

//--------------------------------------------------
// 更新
//-------------------------------------------------- 
static void Update(void)
{
	//--------------------------------------------------
	// 各種オブジェクトの更新
	//--------------------------------------------------

	// 入力処理の更新
	UpdateInput();

	switch (s_mode)
	{// どのモード？
	case MODE_TITLE:		// タイトル
		UpdateTitle();
		break;

	case MODE_GAME:			// ゲーム
		UpdateGame();
		break;

	default:
		assert(false);
		break;
	}

	// フェードの更新
	UpdateFade();

 #ifdef  _DEBUG

	if (GetKeyboardTrigger(DIK_F1))
	{// F1キーが押された
		s_bDebug = !s_bDebug;
	}

 #endif //  _DEBUG

}

//--------------------------------------------------
// 描画
//--------------------------------------------------
static void Draw(void)
{
	// 画面クリア(バッグバッファ＆Zバッファのクリア)
	s_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// 描画開始
	if (SUCCEEDED(s_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合
		//--------------------------------------------------
		// 各種オブジェクトの描画
		//--------------------------------------------------
		switch (s_mode)
		{// どのモード？
		case MODE_TITLE:		// タイトル
			DrawTitle();
			break;

		case MODE_GAME:			// ゲーム
			DrawGame();
			break;

		default:
			assert(false);
			break;
		}

		// フェードの描画
		DrawFade();

 #ifdef  _DEBUG

		if (s_bDebug)
		{// 表示する？
			// デバッグの表示
			DrawDebug();
		}

 #endif //  _DEBUG

		// 描画終了
		s_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	s_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//--------------------------------------------------
// デバイスの取得
//--------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return s_pD3DDevice;
}

//--------------------------------------------------
// デバッグの表示
//--------------------------------------------------
static void DrawDebug(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[4096];
	int nLength = 0;

	/* ↓文字列の代入↓ */

	sprintf(&aStr[nLength], "FPS  : %3d\n\n", s_nCountFPS);
	nLength = (int)strlen(&aStr[0]);

	// テキストの描画
	s_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(0.25f, 0.75f, 1.0f, 1.0f));
}

//--------------------------------------------------
// モードの設定
//--------------------------------------------------
void SetMode(MODE mode)
{
	// 現在の画面(モード)の終了処理
	switch (s_mode)
	{// どのモード？
	case MODE_TITLE:		// タイトル
		UninitTitle();
		break;

	case MODE_GAME:			// ゲーム
		UninitGame();
		break;

	default:
		assert(false);
		break;
	}

	s_mode = mode;		// 現在の画面(モード)を切り替える

	// 新しい画面(モード)の終了処理
	switch (mode)
	{// どのモード？
	case MODE_TITLE:		// タイトル
		InitTitle();
		break;

	case MODE_GAME:			// ゲーム
		InitGame();
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//モードの取得
//--------------------------------------------------
MODE GetMode(void)
{
	return s_mode;
}
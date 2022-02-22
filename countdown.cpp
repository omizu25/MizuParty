//--------------------------------------------------
//
// 3Dゲーム制作 ( countdown.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "game.h"
#include "number.h"
#include "result.h"
#include "setup.h"
#include "countdown.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define NUMBER_WIDTH		(300.0f)		//数の幅
#define NUMBER_HEIGHT		(500.0f)		//数の高さ
#define START_WIDTH			(700.0f)		//スタートの幅
#define START_HEIGHT		(300.0f)		//スタートの高さ
#define ONE_SECOND			(60)			//１秒
#define START_SECOND		(30)			//スタートの消える秒数

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//頂点バッファのポインタ
static D3DXVECTOR3					s_pos;					//位置
static int							s_nCountdown;			//カウントダウンの値
static int							s_nSecond;				//１秒
static bool							s_bCountdown;			//カウントダウンしてるかどうか

//--------------------------------------------------
//カウントダウンの初期化処理
//--------------------------------------------------
void InitCountdown(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\start000.png",
		&s_pTexture);

	//位置を初期化する
	s_pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	s_nCountdown = 3;
	s_nSecond = 0;
	s_bCountdown = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定処理
	Setpos(pVtx, s_pos, START_WIDTH * 0.5f, START_HEIGHT * 0.5f, SETPOS_MIDDLE);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの初期化処理
	Initcol(pVtx);

	//テクスチャの初期化処理
	Inittex(pVtx);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.25f, 0.54f, 1.0f);

	//数の設定処理
	SetMiddleNumber(s_pos, col, NUMBER_WIDTH * 0.5f, NUMBER_HEIGHT * 0.5f, s_nCountdown, -1, USE_GAME);
}

//--------------------------------------------------
//カウントダウンの終了処理
//--------------------------------------------------
void UninitCountdown(void)
{
	if (s_pTexture != NULL)
	{//テクスチャの破棄
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{//頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//カウントダウンの更新処理
//--------------------------------------------------
void UpdateCountdown(void)
{
	if (!s_bCountdown && s_nCountdown < 0)
	{//スタートしてなくて、スタートの字が消えた
		s_bCountdown = true;
	}	
}

//--------------------------------------------------
//カウントダウンの描画処理
//--------------------------------------------------
void DrawCountdown(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	if (s_nCountdown == 0)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,							//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}
}

//--------------------------------------------------
//カウントダウンの加算処理
//--------------------------------------------------
void AddCountdown(int nValue)
{
	s_nSecond += nValue;

	if (s_nCountdown > 0)
	{
		if (s_nSecond % ONE_SECOND == 0)
		{
			s_nCountdown--;
		}
	}
	else
	{
		if (s_nSecond % START_SECOND == 0)
		{
			s_nCountdown--;
		}
	}
	

	if (s_nCountdown > 0)
	{
		//数のテクスチャの処理
		TexNumber(s_nCountdown, -1, USE_GAME);
	}
	else
	{
		//数の使用処理
		UseNumber(-1, USE_GAME);

		//ゲームの設定処理
		SetGameState(GAMESTATE_NORMAL);
	}
}

//--------------------------------------------------
//カウントダウンの設定
//--------------------------------------------------
void SetCountdown(int nCountdown)
{
	s_nCountdown = nCountdown;
}

//--------------------------------------------------
//カウントダウンしてるかどうか
//--------------------------------------------------
bool GetCountdown(void)
{
	return s_bCountdown;
}
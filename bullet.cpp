//==================================================
// 
// 3Dゲーム制作 ( player.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "input.h"
#include "player.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_MOVE		(7.0f)		// 移動量の最大値
#define MAX_WIDTH		(20.0f)		// 幅の最大値
#define MAX_HEIGHT		(20.0f)		// 高さの最大値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		// テクスチャへのポインタ

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitBullet(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko000.jpg",
		&s_pTexture);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitBullet(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの解放
		s_pTexture->Release();
		s_pTexture = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateBullet(void)
{
	if (GetKeyboardTrigger(DIK_SPACE))
	{// スペースキーが押された
		D3DXVECTOR3 move = D3DXVECTOR3(MAX_MOVE, 0.0f, 0.0f);
		Player *pPlayer = GetPlayer();

		// ビルボードの設定
		SetBillboard(pPlayer->pos, move, MAX_WIDTH, MAX_HEIGHT, false, &s_pTexture);
	}
}

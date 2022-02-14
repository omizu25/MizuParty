//==================================================
// 
// 3Dゲーム制作 ( title.cpp )
// Author  : katsuki mizuki
// 
//==================================================
//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "camera.h"
#include "fade.h"
#include "frame.h"
#include "game.h"
#include "input.h"
#include "light.h"
#include "model.h"
#include "player.h"
#include "polygon.h"
#include "shadow.h"
#include "sound.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static MENU		s_Menu;			// メニュー
static int		s_nCount;		// 何回タイトルに来たかのカウント

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitTitle(void)
{
	s_nCount++; 

	// ポリゴンの初期化
	InitPolygon();

	// 影の初期化
	InitShadow();

	// プレイヤーの読み込み
	LoadPlayer();

	// プレイヤーの初期化
	InitPlayer();

	// 枠の初期化
	InitFrame();

	// ビルボードの初期化
	InitBillboard();

	// タイトルの時の読み込み
	InitBillboardTitle();

	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	// サウンドの再生
	PlaySound(SOUND_LABEL_タイトル);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTitle(void)
{
	// サウンドの停止
	StopSound();

	// ポリゴンの終了
	UninitPolygon();

	// 枠の終了
	UninitFrame();

	// ビルボードの終了
	UninitBillboard();

	// 影の終了
	UninitShadow();

	// プレイヤーの終了
	UninitPlayer();

	// カメラの終了
	UninitCamera();

	// ライトの終了
	UninitLight();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateTitle(void)
{
	// プレイヤーの更新
	UpdatePlayer();

	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) ||
		GetJoypadTrigger(JOYKEY_B) || GetJoypadTrigger(JOYKEY_A) ||
		GetJoypadTrigger(JOYKEY_START))
	{// スペースキーが押された
		// ビルボードの当たり判定
		CollisionBillboard();
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawTitle(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DVIEWPORT9 viewport;
	pDevice->GetViewport(&viewport);

	// ビューボードのクリア
	pDevice->SetViewport(&GetCamera(0)->viewport);

	// 画面クリア(バックバッファ＆Zバッファのクリア)
	pDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// カメラの設定
	SetCamera(0);

	// ポリゴンの描画
	DrawPolygon();

	// プレイヤーの描画
	DrawPlayer();

	// 枠の描画
	DrawFrame();

	// ビルボードの描画
	DrawBillboard(false, false);

	// 影の描画
	DrawShadow();

	// ビューボードのクリア
	pDevice->SetViewport(&viewport);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetTitle(MENU menu)
{
	s_Menu = menu;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
MENU GetTitle(void)
{
	return s_Menu;
}

//--------------------------------------------------
// カウントの設定
//--------------------------------------------------
void SetTitleCnt(void)
{
	s_nCount = 0;
}

//--------------------------------------------------
// カウントの取得
//--------------------------------------------------
int GetTitleCnt(void)
{
	return s_nCount;
}
//==================================================
//
// 3Dゲーム制作 ( input.h )
// Author1  : KOZUNA HIROHITO
// Author2  : ISOE JUKIA
// Author3  : KATSUKI MIZUKI
//
//	＜最後の行を見ることを推奨＞
//
//==================================================
#ifndef _INPUT_H_		// このマクロ定義がされなかったら
#define _INPUT_H_		// 2重インクルード防止のマクロ定義

//--------------------------------------------------
// ライブラリのリンク 
//--------------------------------------------------
#pragma comment(lib,"dinput8.lib")		// 入力処理に必要 (キーボード)
#pragma comment(lib,"xinput.lib")		// 入力処理に必要（ジョイパット）

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// 列挙型を定義
//--------------------------------------------------

/*↓ キー種類(ジョイパッド) ↓*/

typedef enum
{
	JOYKEY_UP = 0,				// 十字ボタン上
	JOYKEY_DOWN,				// 十字ボタン下
	JOYKEY_LEFT,				// 十字ボタン左
	JOYKEY_RIGHT,				// 十字ボタン右
	JOYKEY_START,				// スタートボタン
	JOYKEY_BACK,				// バックボタン
	JOYKEY_LEFT_THUMB,			// 左スティック押込み
	JOYKEY_RIGHT_THUMB,			// 右スティック押込み
	JOYKEY_LEFT_SHOULDER,		// L１ボタン
	JOYKEY_RIGHT_SHOULDER,		// R１ボタン
	JOYKEY_LEFT_TRIGGER,		// L２トリガー
	JOYKEY_RIGHT_TRIGGER,		// R２トリガー
	JOYKEY_A,					// Aボタン
	JOYKEY_B,					// Bボタン
	JOYKEY_X,					// Xボタン
	JOYKEY_Y,					// Yボタン
	JOYKEY_LEFT_STICK,			// 左スティック
	JOYKEY_RIGHT_STICK,			// 右スティック
	JOYKEY_MAX
}JOYKEY;

/*↓ キー種類(マウス) ↓*/

typedef enum
{
	MOUSE_INPUT_LEFT,			// マウス左ボタン
	MOUSE_INPUT_RIGHT,			// マウス右ボタン
	MOUSE_INPUT_MIDDLE,			// マウス中央ボタン
	MOUSE_INPUT_4,				// マウスボタン4
	MOUSE_INPUT_5,				// マウスボタン5
	MOUSE_INPUT_6,				// マウスボタン6
	MOUSE_INPUT_7,				// マウスボタン7
	MOUSE_INPUT_8,				// マウスボタン8
	MOUSE_MAX
}MOUSE;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------

/*↓ 入力処理のプロトタイプ宣言 ↓*/

HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);					// 入力処理全部の初期化
void UninitInput(void);												// 入力処理全部の終了処理
void UpdateInput(void);												// 入力処理全部の更新処理

/*↓ プロトタイプ宣言 (キーボード) ↓*/

bool GetKeyboardPress(int nKey);									// キーボードのプレス処理
bool GetKeyboardTrigger(int nKey);									// キーボードのトリガー処理
bool GetKeyboardReturn(int nKey);									// キーボードのリターン処理
bool GetKeyboardAllPress(void);										// キーボードの全キープレス処理
bool GetKeyboardAllTrigger(void);									// キーボードの全キートリガー処理

/*↓ プロトタイプ宣言(ジョイパッド) ↓*/

bool GetJoypadPress(JOYKEY Key, int nPlayer);						// ジョイパッドのプレス処理
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);						// ジョイパッドのトリガー処理
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);				// ジョイパッドのスティック処理
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);					// ジョイパッドのトリガーペダル処理
void JoypadVibration(int nTime, WORD nStrength, int nPlayer);		// ジョイパッドの振動制御

/*↓ プロトタイプ宣言(マウス) ↓*/

bool GetMousePress(MOUSE mouse);									// マウスのプレス処理
bool GetMouseTrigger(MOUSE mouse);									// マウスのトリガー処理
D3DXVECTOR3 GetMouse(void);											// マウスの画面内の位置

/*************************************************************************************************************
	
	//--------------------------------------------------
	// プロトタイプ宣言(ジョイパッド)の説明
	//--------------------------------------------------

	//返す値　スティックを倒せば倒すほど(1.0f or -1.0f)に近く
	//スティックの遊びはifの条件式に組む
	//例　if(GetJoypadStick(JOYKEY_RIGHT_STICK,0)  > 0.5f)
	//    {//右に半分より多く倒したら

	{第一引数はキー（スティック）,第二引数はプレイヤー番号
	D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);			// ジョイパッドスティック処理
	}

	//返す値　ペダルがどれだけ押し込まれたかをintで返す
	//ペダルの遊びはifの条件式に組む
	//例　if(GetJoypadTriggerPedal(JOYKEY_RIGHT_TRIGGER,0) > 100))
	//    {//右ペダルを100より多く倒したら

	{第一引数はキー（スティック）,第二引数はプレイヤー番号
	int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);				// ジョイパッドトリガーペダル処理
	}

	{第一引数は揺らす時間, 第二引数はどれくらいの強さで揺らすか, 第三引数はプレイヤー番号
	void JoypadVibration(int nTime, WORD nStrength, int nPlayer);	// ジョイパッド振動制御
	}

************************************************************************************************************/

#endif 

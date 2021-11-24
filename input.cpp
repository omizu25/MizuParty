//==================================================
// 
// 3Dゲーム制作 ( input.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "input.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define NUM_KEY_MAX		(256)		// キーの最大数

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECTINPUT8			s_pInput = NULL;						// DirectInputオブジェクトへのポインタ
static LPDIRECTINPUTDEVICE8		s_pDevKeyboard = NULL;					// 入力デバイス(キーボード)へのポインタ
static BYTE						s_aKeyState[NUM_KEY_MAX];				// キーボードのプレス情報
static BYTE						s_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードのトリガー情報
static BYTE						s_aKeyStateRelease[NUM_KEY_MAX];		// キーボードのリリース情報
static BYTE						s_aKeyStateRepeat[NUM_KEY_MAX];			// キーボードのリピート情報
static XINPUT_STATE				s_joyKeyState;							// ジョイパッドのプレス情報
static XINPUT_STATE				s_joyKeyStateTrigger;					// ジョイパッドのトリガー情報
static D3DXVECTOR3				s_joyKeyStick;							// ジョイパッドのスティック

//--------------------------------------------------
// キーボードの初期化
//--------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&s_pInput,
		NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(s_pInput->CreateDevice(
		GUID_SysKeyboard,
		&s_pDevKeyboard,
		NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(s_pDevKeyboard->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(s_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードのアクセス権を獲得
	s_pDevKeyboard->Acquire();

	return S_OK;
}

//--------------------------------------------------
// ジョイパッドの初期化
//--------------------------------------------------
HRESULT InitJoypad(void)
{
	// メモリのクリア
	memset(&s_joyKeyState, 0, sizeof(XINPUT_STATE));

	// XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//--------------------------------------------------
// キーボードの終了
//--------------------------------------------------
void UninitKeyboard(void)
{
	// 入力デバイス(キーボード)の解放
	if (s_pDevKeyboard != NULL)
	{
		s_pDevKeyboard->Unacquire();
		s_pDevKeyboard->Release();
		s_pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの解放
	if (s_pInput != NULL)
	{
		s_pInput->Release();
		s_pInput = NULL;
	}
}

//--------------------------------------------------
// ジョイパッドの終了
//--------------------------------------------------
void UninitJoypad(void)
{
	// XInputのステートを設定(無効にする)
	XInputEnable(false);
}

//--------------------------------------------------
// キーボードの更新
//--------------------------------------------------
void UpdateKeyboard(void)
{
	int nCntKey;
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(s_pDevKeyboard->GetDeviceState(
		sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			s_aKeyStateTrigger[nCntKey] = ~s_aKeyState[nCntKey] & aKeyState[nCntKey];		// キーボードのトリガー情報を保存
			s_aKeyStateRelease[nCntKey] = s_aKeyState[nCntKey] & ~aKeyState[nCntKey];		// キーボードのリリース情報を保存

			// まだできてない
			s_aKeyStateRepeat[nCntKey] = ~s_aKeyState[nCntKey] & ~aKeyState[nCntKey];		// キーボードのリピート情報を保存
			
			s_aKeyState[nCntKey] = aKeyState[nCntKey];										// キーボードのプレス情報を保存
		}
	}
	else
	{
		s_pDevKeyboard->Acquire();		// キーボードのアクセス権を獲得
	}
}

//--------------------------------------------------
// ジョイパッドの更新
//--------------------------------------------------
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;		// ジョイパッドの入力情報

	// ジョイパッドの状態を取得	
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{// 0の部分が操作しているのが何個目のパッドかを表してる
		// ジョイパッドのトリガー情報を保存
		s_joyKeyStateTrigger.Gamepad.wButtons = ~s_joyKeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons;

		if (s_joyKeyState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			s_joyKeyState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			s_joyKeyState.Gamepad.sThumbLX = 0;
		}

		if (s_joyKeyState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			s_joyKeyState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			s_joyKeyState.Gamepad.sThumbLY = 0;
		}

		// ジョイパッドのプレス情報を保存
		s_joyKeyState = joykeyState;
	}
}

//--------------------------------------------------
// キーボードのプレス情報を取得
//--------------------------------------------------
bool GetKeyboardPress(int nKey)
{
	return (s_aKeyState[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------
// ジョイパッドのプレス情報を取得
//--------------------------------------------------
bool GetJoypadPress(JOYKEY key)
{
	return (s_joyKeyState.Gamepad.wButtons & 0x01 << key) ? true : false;
}

//--------------------------------------------------
// キーボードのトリガー情報を取得
//--------------------------------------------------
bool GetKeyboardTrigger(int nKey)
{
	return (s_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------
// ジョイパッドのトリガー情報を取得
//--------------------------------------------------
bool GetJoypadTrigger(JOYKEY key)
{
	return (s_joyKeyStateTrigger.Gamepad.wButtons & 0x01 << key) ? true : false;
}

//--------------------------------------------------
// キーボードのリリース情報を取得
//--------------------------------------------------
bool GetKeyboardRelease(int nKey)
{
	return (s_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------
// キーボードのリピート情報を取得
//--------------------------------------------------
bool GetKeyboardRepeat(int nKey)
{
	return (s_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------
// ジョイパッドのスティック情報を取得
//--------------------------------------------------
D3DXVECTOR3 GetJoypadStick(JOYKEY key)
{
	switch (key)
	{
	case JOYKEY_L_STICK:		// Lスティック
		s_joyKeyStick = D3DXVECTOR3(s_joyKeyState.Gamepad.sThumbLX / 60000.0f, -s_joyKeyState.Gamepad.sThumbLY / 60000.0f, 0.0f);
		break;
	case JOYKEY_R_STICK:		// Rスティック
		s_joyKeyStick = D3DXVECTOR3(s_joyKeyState.Gamepad.sThumbRX / 30000.0f, -s_joyKeyState.Gamepad.sThumbRY / 30000.0f, 0.0f);
		break;
	}

	return s_joyKeyStick;
}
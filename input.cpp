//==================================================
// 
// 3Dゲーム制作 ( input.h )
// Author1  : KOZUNA HIROHITO
// Author2  : ISOE JUKIA
// Author3  : KATSUKI MIZUKI
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "input.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define NUM_KEY_MAX			(256)		// キーの最大数（キーボード）
#define MOUSE_KEY_MAX		(8)			// キーの最大数（マウス）
#define PLAYER_MAX			(4)			// プレイヤーの最大人数

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------

/*↓ キーボード ↓*/

static LPDIRECTINPUT8			s_pInput = NULL;						// DirectInputオブジェクトへのポインタ
static LPDIRECTINPUTDEVICE8		s_pDevKeyboard = NULL;					// 入力デバイス(キーボード)へのポインタ
static BYTE						s_aKeyState[NUM_KEY_MAX];				// キーボードのプレス情報
static BYTE						s_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードのトリガー情報
static BYTE						s_aKeyStateRelease[NUM_KEY_MAX];		// キーボードのリリース情報

/*↓ ジョイパッド ↓*/

static XINPUT_STATE				s_JoyKeyState[PLAYER_MAX];				// ジョイパッドのプレス情報
static XINPUT_STATE				s_JoyKeyStateTrigger[PLAYER_MAX];		// ジョイパッドのトリガー情報
static D3DXVECTOR3				s_JoyStickPos[PLAYER_MAX];				// ジョイスティックの傾き
static XINPUT_VIBRATION			s_JoyMoter[PLAYER_MAX];					// ジョイパッドのモーター
static int						s_nTime[PLAYER_MAX];					// 振動持続時間
static WORD						s_nStrength[PLAYER_MAX];				// 振動の強さ (0 - 65535)
static bool						s_bUseJoyPad[PLAYER_MAX];				// ジョイパッドを使用してるか

/*↓ マウス ↓*/

static LPDIRECTINPUT8			s_pMouseInput = NULL;					// Directinutオブジェクトへのポインタ
static LPDIRECTINPUTDEVICE8		s_pDevMouse = NULL;						// 入力でパスへのポインタ
static DIMOUSESTATE2			s_aKeyStateMouse;						// マウスのプレス処理
static DIMOUSESTATE2			s_aKeyStatetriggerMouse;				// マウスのトリガー処理
static POINT					s_MousePos;								// マウスのカーソル用
static HWND						s_hMouseWnd;							// ウィンドウハンドル

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------

/*↓ キーボード ↓*/

static HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);		// 初期化
static void UninitKeyboard(void);									// 終了処理
static void UpdateKeyboard(void);									// 更新処理

/*↓ ジョイパッド ↓*/

static HRESULT InitJoypad(void);									// 初期化
static void UninitJoypad(void);										// 終了処理
static void UpdateJoypad(void);										// 更新処理

/*↓ マウス ↓*/

static HRESULT InitMouse(HINSTANCE hlnstance, HWND hWnd);			// 初期化
static void UninitMouse(void);										// 終了処理
static void UpdateMouse(void);										// 更新処理

//**************************************************
// 入力処理全体
//**************************************************

//--------------------------------------------------
// 入力処理全部の初期化
//--------------------------------------------------
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// マウスの初期化処理
	if (FAILED(InitMouse(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ジョイパッド初期化
	InitJoypad();

	return S_OK;
}

//--------------------------------------------------
// 入力処理全部の終了処理
//--------------------------------------------------
void UninitInput(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// マウスの終了処理
	UninitMouse();

	// ジョイパッド終了処理
	UninitJoypad();
}

//--------------------------------------------------
// 入力処理全部の更新処理
//--------------------------------------------------
void UpdateInput(void)
{
	// キーボードの更新処理
	UpdateKeyboard();

	// マウスの更新処理
	UpdateMouse();

	// ジョイパッド更新処理
	UpdateJoypad();
}

//**************************************************
// キーボードの入力処理
//**************************************************

//--------------------------------------------------
// キーボードの初期化処理
//--------------------------------------------------
static HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance,DIRECTINPUT_VERSION,
		IID_IDirectInput8,(void**)&s_pInput,NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス（キーボード）の生成
	if (FAILED(s_pInput->CreateDevice(GUID_SysKeyboard,&s_pDevKeyboard,NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(s_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(s_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	s_pDevKeyboard->Acquire();

	return S_OK;
}

//--------------------------------------------------
// キーボードの終了処理
//--------------------------------------------------
static void UninitKeyboard(void)
{
	// 入力デバイス（キーボード）の放棄
	if (s_pDevKeyboard != NULL)
	{
		s_pDevKeyboard->Unacquire();		// キーボードへのアクセス権を放棄
		s_pDevKeyboard->Release();
		s_pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの破壊
	if (s_pInput != NULL)
	{
		s_pInput->Release();
		s_pInput = NULL;
	}
}

//--------------------------------------------------
// キーボードの更新処理
//--------------------------------------------------
static void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(s_pDevKeyboard->GetDeviceState(sizeof(aKeyState),&aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			s_aKeyStateTrigger[nCntKey] = ~s_aKeyState[nCntKey] & aKeyState[nCntKey];		// キーボードのトリガー情報を保存
			s_aKeyStateRelease[nCntKey] = s_aKeyState[nCntKey] & ~aKeyState[nCntKey];		// キーボードのリリース情報を保存
			s_aKeyState[nCntKey] = aKeyState[nCntKey];										// キーボードのプレス情報を保存
		}
	}
	else
	{
		s_pDevKeyboard->Acquire();			// キーボードへのアクセス権を獲得
	}

}

//--------------------------------------------------
// キーボードのプレス情報を取得
//--------------------------------------------------
bool GetKeyboardPress(int nKey)
{
	return (s_aKeyState[nKey] & 0x80) != 0;
}

//--------------------------------------------------
// キーボードのトリガー情報を取得
//--------------------------------------------------
bool GetKeyboardTrigger(int nKey)
{
	return (s_aKeyStateTrigger[nKey] & 0x80) != 0;
}

//--------------------------------------------------
// キーボードのリリース情報を取得
//--------------------------------------------------
bool GetKeyboardRelease(int nKey)
{
	return (s_aKeyStateRelease[nKey] & 0x80) != 0;
}

//--------------------------------------------------
// キーボードの全キープレス情報を取得
//--------------------------------------------------
bool GetKeyboardAllPress(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetKeyboardPress(nCntKey))
		{
			return true;
		}
	}

	return false;
}

//--------------------------------------------------
// キーボードの全キートリガー情報を取得
//--------------------------------------------------
bool GetKeyboardAllTrigger(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetKeyboardTrigger(nCntKey))
		{
			return true;
		}
	}

	return false;
}

//--------------------------------------------------
// キーボードの全キーリリース情報を取得
//--------------------------------------------------
bool GetKeyboardAllRelease(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetKeyboardRelease(nCntKey))
		{
			return true;
		}
	}

	return false;
}

//**************************************************
// ジョイパッドの入力処理
//**************************************************

//--------------------------------------------------
// ジョイパッドの初期化
//--------------------------------------------------
static HRESULT InitJoypad(void)
{
	// XInputのステートを設定（有効にする）
	XInputEnable(true);

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		// メモリーのクリア
		memset(&s_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&s_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));

		// ジョイパッドの振動制御の０クリア
		ZeroMemory(&s_JoyMoter[nCnt], sizeof(XINPUT_VIBRATION));

		// 振動制御用の初期化
		s_nStrength[nCnt] = 0;
		s_nTime[nCnt] = 0;
	}

	return S_OK;
}

//--------------------------------------------------
// ジョイパッドの終了
//--------------------------------------------------
static void UninitJoypad(void)
{
	// XInputのステートを設定（無効にする）
	XInputEnable(false);
}

//--------------------------------------------------
// ジョイパッドの更新
//--------------------------------------------------
static void UpdateJoypad(void)
{
	XINPUT_STATE JoyKeyState[PLAYER_MAX];		// ジョイパッド入力情報

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		// ジョイパッドの状態を取得
		if (XInputGetState(nCnt, &JoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			// トリガー情報を保存
			s_JoyKeyStateTrigger[nCnt].Gamepad.wButtons = ~s_JoyKeyState[nCnt].Gamepad.wButtons & JoyKeyState[nCnt].Gamepad.wButtons;

			// プレス情報を保存
			s_JoyKeyState[nCnt] = JoyKeyState[nCnt];

			s_bUseJoyPad[nCnt] = true; // 使用状況の更新
		}
		else
		{
			s_bUseJoyPad[nCnt] = false;	// 使用状況の更新
		}

		// ジョイパッドの振動
		s_JoyMoter[nCnt].wLeftMotorSpeed = s_nStrength[nCnt];
		s_JoyMoter[nCnt].wRightMotorSpeed = s_nStrength[nCnt];
		XInputSetState(nCnt, &s_JoyMoter[nCnt]);

		if (s_nTime[nCnt] > 0)
		{
			s_nTime[nCnt]--;
		}
		else
		{
			s_nStrength[nCnt] = 0;
			s_nTime[nCnt] = 0;
		}
	}
}

//--------------------------------------------------
// ジョイパッドのプレス処理(プレイヤー指定あり)
//--------------------------------------------------
bool GetJoypadIdxPress(JOYKEY Key, int nPlayer)
{
	return (s_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) != 0;
}

//--------------------------------------------------
// ジョイパッドのトリガー処理(プレイヤー指定あり)
//--------------------------------------------------
bool GetJoypadIdxTrigger(JOYKEY Key, int nPlayer)
{
	return (s_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) != 0;
}

//--------------------------------------------------
// ジョイパッドのプレス処理(プレイヤー指定なし)
//--------------------------------------------------
bool GetJoypadPress(JOYKEY Key)
{
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		if (GetJoypadIdxPress(Key, nPlayer))
		{
			return true;
		}
	}
	return false;
}

//--------------------------------------------------
// ジョイパッドのプレス処理(プレイヤー指定なし)
//--------------------------------------------------
bool GetJoypadTrigger(JOYKEY Key)
{
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		if (GetJoypadIdxTrigger((JOYKEY)Key, nPlayer))
		{
			return true;
		}
	}
	return false;
}

//--------------------------------------------------
// ジョイパッドの全キープレス処理
//--------------------------------------------------
bool GetJoypadAllPress(void)
{
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			if (GetJoypadIdxPress((JOYKEY)nCntKey, nPlayer))
			{
				return true;
			}
		}
	}
	return false;
}

//--------------------------------------------------
// ジョイパッドの全キートリガー処理
//--------------------------------------------------
bool GetJoypadAllTrigger(void)
{
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			if (GetJoypadIdxTrigger((JOYKEY)nCntKey, nPlayer))
			{
				return true;
			}
		}
	}
	return false;
}


//--------------------------------------------------
// ジョイパット（スティックプレス）処理
//--------------------------------------------------
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer)
{
	switch (Key)
	{
	case JOYKEY_LEFT_STICK:
		s_JoyStickPos[nPlayer] = D3DXVECTOR3(s_JoyKeyState[nPlayer].Gamepad.sThumbLX / 32767.0f, - s_JoyKeyState[nPlayer].Gamepad.sThumbLY / 32767.0f, 0.0f);
		break;
	case JOYKEY_RIGHT_STICK:
		s_JoyStickPos[nPlayer] = D3DXVECTOR3(s_JoyKeyState[nPlayer].Gamepad.sThumbRX / 32767.0f, - s_JoyKeyState[nPlayer].Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	}

	return s_JoyStickPos[nPlayer];
}

//--------------------------------------------------
// ジョイパット（トリガーペダル）処理
//--------------------------------------------------
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer)
{
	int nJoypadTriggerPedal = 0;

	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = s_JoyKeyState[nPlayer].Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = s_JoyKeyState[nPlayer].Gamepad.bRightTrigger;
		break;
	}
	
	return nJoypadTriggerPedal;
}

//--------------------------------------------------
// コントローラーの振動制御
//--------------------------------------------------
void JoypadVibration(int nTime, WORD nStrength, int nPlayer)
{
	s_nTime[nPlayer] = nTime;				// 振動持続時間
	s_nStrength[nPlayer] = nStrength;		// 振動の強さ
}

//--------------------------------------------------
// ジョイパッドの使用されているか返す処理
//--------------------------------------------------
bool IsJoyPadUse(int nPlayer)
{
	return s_bUseJoyPad[nPlayer];
}

//**************************************************
// マウスの入力処理
//**************************************************

//--------------------------------------------------
// マウスの初期化
//--------------------------------------------------
static HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&s_pMouseInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス（マウス）の生成
	if (FAILED(s_pMouseInput->CreateDevice(GUID_SysMouse, &s_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(s_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(s_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//ウィンドウハンドルの保管
	s_hMouseWnd = hWnd;

	// キーボードへのアクセス権を獲得
	s_pDevMouse->Acquire();

	return S_OK;
}

//--------------------------------------------------
// マウスの終了処理
//--------------------------------------------------
static void UninitMouse(void)
{
	if (s_pDevMouse != NULL)
	{
		s_pDevMouse->Unacquire();
		s_pDevMouse = NULL;
	}
}

//--------------------------------------------------
// マウスの更新処理
//--------------------------------------------------
static void UpdateMouse(void)
{
	DIMOUSESTATE2 aKeyState;	// マウスの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(s_pDevMouse->GetDeviceState(sizeof(aKeyState), &aKeyState)))
	{
		for (int nCntKey = 0; nCntKey < MOUSE_KEY_MAX; nCntKey++)
		{
			// マウスのトリガー情報を保存
			s_aKeyStatetriggerMouse.rgbButtons[nCntKey] = ~s_aKeyStateMouse.rgbButtons[nCntKey] & aKeyState.rgbButtons[nCntKey];
		}

		// マウスのプレス情報を保存
		s_aKeyStateMouse = aKeyState;
	}
	else
	{
		s_pDevMouse->Acquire();		// マウスへのアクセス権を獲得
	}
}

//--------------------------------------------------
// マウスのプレス処理
//--------------------------------------------------
bool GetMousePress(MOUSE mouse)
{
	return (s_aKeyStateMouse.rgbButtons[mouse] & 0x80) != 0;

}

//--------------------------------------------------
// マウスのトリガー処理
//--------------------------------------------------
bool GetMouseTrigger(MOUSE mouse)
{
	return (s_aKeyStatetriggerMouse.rgbButtons[mouse] & 0x80) != 0;
}

//--------------------------------------------------
// マウスポインターの位置
//--------------------------------------------------
D3DXVECTOR3 GetMouse(void)
{
	//画面上のマウスポインターの位置
	GetCursorPos(&s_MousePos);
	//ウィンドウ上のマウスポインターの位置
	ScreenToClient(s_hMouseWnd, &s_MousePos);

	return D3DXVECTOR3((float)s_MousePos.x, (float)s_MousePos.y, 0.0f);
}

//--------------------------------------------------
// マウスのホイールの動き感知
//--------------------------------------------------
int GetMouseWheel(void)
{
	return (int)s_aKeyStateMouse.lZ;
}
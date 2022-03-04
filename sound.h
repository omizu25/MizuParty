//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE = 0,				// BGM0
	SOUND_LABEL_WALKING,				// BGM1
	SOUND_LABEL_STOP,					// BGM2
	SOUND_LABEL_SLOPE,					// BGM3
	SOUND_LABEL_SE_COUNTDOWN,			// カウントダウン音
	SOUND_LABEL_SE_FALL,				// 色んな落下音
	SOUND_LABEL_SE_KO,					// ハンマーに直撃音
	SOUND_LABEL_SE_GAMEOVER,			// ゲームオーバー音
	SOUND_LABEL_SE_GAMECLEAR,			// ゲームクリア音
	SOUND_LABEL_SE_DECISION,			// 決定音
	SOUND_LABEL_SE_SELECTION,			// 選択音
	SOUND_LABEL_SE_RUN,					// 坂の走り始め音
	SOUND_LABEL_SE_STOP_BAD,			// 止めるの下手音
	SOUND_LABEL_SE_STOP_NORMAL,			// 止めるの普通音
	SOUND_LABEL_SE_STOP_CRITICAL,		// 止めるの上手音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif

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
	SOUND_LABEL_タイトル = 0,			// BGM0
	SOUND_LABEL_ウォーキング,			// BGM1
	SOUND_LABEL_止める,					// BGM2
	SOUND_LABEL_坂,						// BGM3
	SOUND_LABEL_SE_カウントダウン,		// カウントダウン音
	SOUND_LABEL_SE_落下,				// 色んな落下音
	SOUND_LABEL_SE_KO,					// ハンマーに直撃音
	SOUND_LABEL_SE_ゲームオーバー,		// ゲームオーバー音
	SOUND_LABEL_SE_ゲームクリア,		// ゲームクリア音
	SOUND_LABEL_SE_決定音,				// 決定音
	SOUND_LABEL_SE_選択音,				// 選択音
	SOUND_LABEL_SE_走る,				// 坂の走り始め音
	SOUND_LABEL_SE_止めるの下手,		// 止めるの下手音
	SOUND_LABEL_SE_止めるの普通,		// 止めるの普通音
	SOUND_LABEL_SE_止めるの上手,		// 止めるの上手音
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

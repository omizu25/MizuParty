//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE = 0,				// BGM0
	SOUND_LABEL_WALKING,				// BGM1
	SOUND_LABEL_STOP,					// BGM2
	SOUND_LABEL_SLOPE,					// BGM3
	SOUND_LABEL_SE_COUNTDOWN,			// �J�E���g�_�E����
	SOUND_LABEL_SE_FALL,				// �F��ȗ�����
	SOUND_LABEL_SE_KO,					// �n���}�[�ɒ�����
	SOUND_LABEL_SE_GAMEOVER,			// �Q�[���I�[�o�[��
	SOUND_LABEL_SE_GAMECLEAR,			// �Q�[���N���A��
	SOUND_LABEL_SE_DECISION,			// ���艹
	SOUND_LABEL_SE_SELECTION,			// �I����
	SOUND_LABEL_SE_RUN,					// ��̑���n�߉�
	SOUND_LABEL_SE_STOP_BAD,			// �~�߂�̉��艹
	SOUND_LABEL_SE_STOP_NORMAL,			// �~�߂�̕��ʉ�
	SOUND_LABEL_SE_STOP_CRITICAL,		// �~�߂�̏�艹
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif

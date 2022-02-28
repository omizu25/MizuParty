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
	SOUND_LABEL_�^�C�g�� = 0,			// BGM0
	SOUND_LABEL_�E�H�[�L���O,			// BGM1
	SOUND_LABEL_�~�߂�,					// BGM2
	SOUND_LABEL_��,						// BGM3
	SOUND_LABEL_SE_�J�E���g�_�E��,		// �J�E���g�_�E����
	SOUND_LABEL_SE_����,				// �F��ȗ�����
	SOUND_LABEL_SE_KO,					// �n���}�[�ɒ�����
	SOUND_LABEL_SE_�Q�[���I�[�o�[,		// �Q�[���I�[�o�[��
	SOUND_LABEL_SE_�Q�[���N���A,		// �Q�[���N���A��
	SOUND_LABEL_SE_���艹,				// ���艹
	SOUND_LABEL_SE_�I����,				// �I����
	SOUND_LABEL_SE_����,				// ��̑���n�߉�
	SOUND_LABEL_SE_�~�߂�̉���,		// �~�߂�̉��艹
	SOUND_LABEL_SE_�~�߂�̕���,		// �~�߂�̕��ʉ�
	SOUND_LABEL_SE_�~�߂�̏��,		// �~�߂�̏�艹
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

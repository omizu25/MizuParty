//==================================================
// 
// 3D�Q�[������ ( game.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "game.h"
#include "rotation.h"
#include "slope.h"
#include "stop.h"
#include "title.h"
#include "walking.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define CHANGE_TIME		(120)		// �^�C���̎��ւ̕ύX�l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static GAMESTATE		s_gameState;					// �Q�[�����
static int				s_nTime;						// �^�C��
static bool				s_bRemix;						// ���~�b�N�X
static bool				s_bRemixWalking = false;		// ���~�b�N�X�̃E�H�[�L���O���������ǂ���

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitGame(void)
{
	s_gameState = GAMESTATE_START;

	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
		InitWalking();
		break;

	case MENU_ROTATION:		// ��]
		InitRotation();
		break;

	case MENU_STOP:			// �~�߂�
		InitStop();
		break;

	case MENU_SLOPE:		// ��
		InitSlope();
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitGame(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
		UninitWalking();
		break;

	case MENU_ROTATION:		// ��]
		UninitRotation();
		break;

	case MENU_STOP:			// �~�߂�
		UninitStop();
		break;

	case MENU_SLOPE:		// ��
		UninitSlope();
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateGame(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
		UpdateWalking();
		break;

	case MENU_ROTATION:		// ��]
		UpdateRotation();
		break;

	case MENU_STOP:			// �~�߂�
		UpdateStop();
		break;

	case MENU_SLOPE:		// ��
		UpdateSlope();
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawGame(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
		DrawWalking();
		break;

	case MENU_ROTATION:		// ��]
		DrawRotation();
		break;

	case MENU_STOP:			// �~�߂�
		DrawStop();
		break;

	case MENU_SLOPE:		// ��
		DrawSlope();
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	s_gameState = state;
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
GAMESTATE GetGame(void)
{
	return s_gameState;
}

//--------------------------------------------------
// ���~�b�N�X�̐ݒ�
//--------------------------------------------------
void SetRemix(bool bRemix)
{
	s_bRemix = bRemix;
	s_bRemixWalking = false;
}

//--------------------------------------------------
// ���~�b�N�X�̎擾
//--------------------------------------------------
bool GetRemix(void)
{
	return s_bRemix;
}
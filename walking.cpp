//==================================================
// 
// 3D�Q�[������ ( walking.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "camera.h"
#include "countdown.h"
#include "game.h"
#include "light.h"
#include "loop.h"
#include "number.h"
#include "player.h"
#include "polygon.h"
#include "result.h"
#include "rule.h"
#include "shadow.h"
#include "sound.h"
#include "target.h"
#include "time.h"
#include "walking.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define CHANGE_TIME		(120)		// �^�C���̎��ւ̕ύX�l
#define MAX_CAMERA		(1)			// �J�����̍ő吔

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static int		s_nTime;		// �^�C��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitWalking(void)
{
	// �|���S���̏�����
	InitPolygon();

	// �e�̏�����
	InitShadow();

	// �v���C���[�̓ǂݍ���
	LoadPlayer();

	// �v���C���[�̏�����
	InitPlayer();

	// �r���{�[�h�̏�����
	InitBillboard();

	// �r���{�[�h�̓ǂݍ���
	LoadBillboard();

	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// ���̏�����
	InitNumber();

	// �^�C���̏�����
	InitTime();

	// �^�[�Q�b�g�̏�����
	InitTarget();

	// �J�E���g�_�E���̏�����
	InitCountdown();

	// ���[���̏�����
	InitRule();

	s_nTime = 0;

	// ���U���g�̐ݒ�
	SetResult(RESULT_NONE);

	// �T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_�E�H�[�L���O);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitWalking(void)
{
	// �T�E���h�̒�~
	StopSound();

	// �|���S���̏I��
	UninitPolygon();

	// �e�̏I��
	UninitShadow();

	// �v���C���[�̏I��
	UninitPlayer();

	// �r���{�[�h�̏I��
	UninitBillboard();

	// �J�����̏I��
	UninitCamera();

	// ���C�g�̏I��
	UninitLight();

	// ���̏I��
	UninitNumber();

	// �^�[�Q�b�g�̏I��
	UninitTarget();

	// �J�E���g�_�E���̏I��
	UninitCountdown();

	// ���[���̏I��
	UninitRule();

	// ���U���g�̏I��
	UninitResult();

	// ���[�v�̏I��
	UninitLoop();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateWalking(void)
{
	switch (GetGame())
	{
	case GAMESTATE_START:		// �J�n���(�Q�[���J�n��)
		s_nTime++;

		if (s_nTime >= CHANGE_TIME)
		{// �w��̒l���z����
			s_nTime = 0;

			// �Q�[���̐ݒ�
			SetGameState(GAMESTATE_COUNTDOWN);

			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_�J�E���g�_�E��);
		}

		break;

	case GAMESTATE_COUNTDOWN:		// �J�E���g�_�E����� (�Q�[���J�n��)

		//�J�E���g�_�E���̉��Z
		AddCountdown(1);

		//�J�E���g�_�E���̍X�V
		UpdateCountdown();

		break;

	case GAMESTATE_NORMAL:		// �ʏ���(�Q�[���i�s��)

		//�J�E���g�_�E���̉��Z
		AddCountdown(1);

		//�J�E���g�_�E���̍X�V
		UpdateCountdown();

		if (GetCountdown())
		{
			// �|���S���̍X�V
			UpdatePolygon();

			// �^�C���̌��Z
			SubTime(1);
		}

		break;

	case GAMESTATE_END:		// �I�����(�Q�[���I����)

		s_nTime++;

		if (s_nTime >= CHANGE_TIME)
		{// �w��̒l���z����
			s_nTime = 0;

			// �Q�[���̐ݒ�
			SetGameState(GAMESTATE_RESULT);

			// �J�����̏�����
			InitCamera();
		}

		break;

	case GAMESTATE_RESULT:		// ���U���g���(�Q�[���I����)

		if (GetOverlap())
		{// �d�Ȃ���
			// ���U���g�̍X�V
			UpdateResult();
		}

		break;

	case GAMESTATE_LOOP:		// �J��Ԃ���� (���U���g�I����)

		UpdateLoop();

		break;

	default:
		assert(false);
		break;
	}

	// ���̍X�V
	UpdateNumber();

	// ���[���̍X�V
	UpdateRule();

	// �^�[�Q�b�g�̍X�V
	UpdateTarget();

	// �v���C���[�̍X�V
	UpdatePlayer();

	// �e�̍X�V
	UpdateShadow();

	// �J�����̍X�V
	UpdateCamera();

	// ���C�g�̍X�V
	UpdateLight();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawWalking(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DVIEWPORT9 viewport;
	pDevice->GetViewport(&viewport);

	// �r���[�{�[�h�̃N���A
	pDevice->SetViewport(&GetCamera(0)->viewport);

	// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	pDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �J�����̐ݒ�
	SetCamera(0);

	// �|���S���̕`��
	DrawPolygon();

	// �v���C���[�̕`��
	DrawPlayer();

	// �e�̕`��
	DrawShadow();

	// �r���{�[�h�̕`��
	DrawBillboard(false, false);

	if (GetGame() != GAMESTATE_LOOP)
	{
		// �^�[�Q�b�g�̕`��
		DrawTarget();

		if (GetGame() != GAMESTATE_START)
		{
			// ���̕`��
			DrawNumber2D(USE_GAME);
		}
	}

	switch (GetGame())
	{
	case GAMESTATE_START:			// �J�n���(�Q�[���J�n��)
		
		// ���̕`��
		DrawNumber2D(USE_START);

		break;

	case GAMESTATE_COUNTDOWN:		// �J�E���g�_�E����� (�Q�[���J�n��)
	case GAMESTATE_NORMAL:			// �ʏ���(�Q�[���i�s��)

		// ���[���̕`��
		DrawRule();

		// ���̕`��
		DrawNumber2D(USE_GAME_ONLY);

		break;

	case GAMESTATE_END:				// �I�����(�Q�[���I����)

		/* �����Ȃ� */

		break;

	case GAMESTATE_RESULT:			// ���U���g���(�Q�[���I����)

		// �r���{�[�h�̕`��
		DrawBillboard(true, false);
		
		if (GetOverlap())
		{// �d�Ȃ���
			// ���U���g�̕`��
			DrawResult();

			// ���̕`��
			DrawNumber2D(USE_RESULT);
		}

		break;
	case GAMESTATE_LOOP:			// �J��Ԃ���� (���U���g�I����)

		// ���[�v�̕`��
		DrawLoop();

		break;

	default:
		assert(false);
		break;
	}

	//�J�E���g�_�E���̕`��
	DrawCountdown();

	// �r���[�{�[�h�̃N���A
	pDevice->SetViewport(&viewport);
}

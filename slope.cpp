//==================================================
// 
// 3D�Q�[������ ( slope.cpp )
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
#include "field.h"
#include "game.h"
#include "light.h"
#include "mesh_field.h"
#include "number.h"
#include "player.h"
#include "result.h"
#include "rule.h"
#include "shadow.h"
#include "slope.h"
#include "sound.h"
#include "target.h"
#include "time.h"
#include "wall.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define CHANGE_TIME		(120)		// �^�C���̎��ւ̕ύX�l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static int		s_nTime;		// �^�C��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitSlope(void)
{
	// �t�B�[���h�̏�����
	InitField();

	// ���b�V���t�B�[���h�̏�����
	InitMeshField();

	// ���b�V���t�B�[���h�̐ݒ�
	SetMeshField();

	// �ǂ̏�����
	InitWall();

	// �ǂ̐ݒu
	InstallationWall();

	// �e�̏�����
	InitShadow();

	// �v���C���[�̓ǂݍ���
	LoadPlayer();

	// �v���C���[�̏�����
	InitPlayer();

	// �r���{�[�h�̏�����
	InitBillboard();

	// ��̃r���{�[�h�̏�����
	InitBillboardSlope();

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
	PlaySound(SOUND_LABEL_��);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitSlope(void)
{
	// �T�E���h�̒�~
	StopSound();

	// �t�B�[���h�̏I��
	UninitField();

	// ���b�V���t�B�[���h�̏I��
	UninitMeshField();

	// �ǂ̏I��
	UninitWall();

	// �r���{�[�h�̏I��
	UninitBillboard();

	// �e�̏I��
	UninitShadow();

	// �v���C���[�̏I��
	UninitPlayer();

	// �J�����̏I��
	UninitCamera();

	// ���C�g�̏I��
	UninitLight();

	// ���̏I��
	UninitNumber();

	// �J�E���g�_�E���̏I��
	UninitCountdown();

	// �^�[�Q�b�g�̏I��
	UninitTarget();

	// ���[���̏I��
	UninitRule();

	// ���U���g�̏I��
	UninitResult();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateSlope(void)
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
			// �t�B�[���h�̍X�V
			UpdateField();

			// ���b�V���t�B�[���h�̍X�V
			UpdateMeshField();

			// �ǂ̍X�V
			UpdateWall();

			// �^�C���̌��Z
			SubTime(1);
		}

		break;

	case GAMESTATE_END:		// �I�����(�Q�[���I����)

		/* �����Ȃ� */

		break;

	case GAMESTATE_RESULT:		// ���U���g���(�Q�[���I����)

		// ���U���g�̍X�V
		UpdateResult();

		break;

	default:
		assert(false);
		break;
	}

	// �r���{�[�h�̍X�V
	UpdateBillboard();

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
void DrawSlope(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nMax = 1;

	switch (GetGame())
	{
	case GAMESTATE_START:			// �J�n���(�Q�[���J�n��)

		/* �����Ȃ� */

		break;

	case GAMESTATE_END:				// �I�����(�Q�[���I����)

		GetCamera(1)->viewport.X = (DWORD)0.0f;
		GetCamera(1)->viewport.Y = (DWORD)(SCREEN_HEIGHT * 0.0f);
		GetCamera(1)->viewport.Width = (DWORD)(SCREEN_WIDTH * 1.0f);
		GetCamera(1)->viewport.Height = (DWORD)(SCREEN_HEIGHT * 1.0f);

		/* break�Ȃ� */

	case GAMESTATE_COUNTDOWN:		// �J�E���g�_�E����� (�Q�[���J�n��)
	case GAMESTATE_NORMAL:			// �ʏ���(�Q�[���i�s��)
	case GAMESTATE_RESULT:			// ���U���g���(�Q�[���I����)

		nMax = 2;

		break;

	default:
		assert(false);
		break;
	}

	D3DVIEWPORT9 viewport;
	pDevice->GetViewport(&viewport);

	for (int nCntCamera = 0; nCntCamera < nMax; nCntCamera++)
	{
		bool bCamera = false;

		if (nCntCamera == 0)
		{
			bCamera = false;
		}
		else if (nCntCamera == 1)
		{
			bCamera = true;
		}

		// �r���[�{�[�h�̃N���A
		pDevice->SetViewport(&GetCamera(nCntCamera)->viewport);

		// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
		pDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		// �J�����̐ݒ�
		SetCamera(nCntCamera);

		// �t�B�[���h�̕`��
		DrawField();

		// ���b�V���t�B�[���h�̕`��
		DrawMeshField();

		// �ǂ̕`��
		DrawWall();

		// �v���C���[�̕`��
		DrawPlayer();

		// �e�̕`��
		DrawShadow();

		// �r���{�[�h�̕`��
		DrawBillboard(false, bCamera);

		// �^�[�Q�b�g�̕`��
		DrawTarget();

		if (GetGame() != GAMESTATE_START)
		{
			// ���̕`��
			DrawNumber2D(USE_GAME);
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
			DrawBillboard(true, bCamera);

			// ���U���g�̕`��
			DrawResult();

			// ���̕`��
			DrawNumber2D(USE_RESULT);

			break;

		default:
			assert(false);
			break;
		}

		//�J�E���g�_�E���̕`��
		DrawCountdown();
	}

	// �r���[�{�[�h�̃N���A
	pDevice->SetViewport(&viewport);
}

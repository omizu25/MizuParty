//==================================================
// 
// 3D�Q�[������ ( stop.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "countdown.h"
#include "effect.h"
#include "game.h"
#include "light.h"
#include "loop.h"
#include "model.h"
#include "number.h"
#include "player.h"
#include "particle.h"
#include "polygon.h"
#include "result.h"
#include "rule.h"
#include "shadow.h"
#include "sound.h"
#include "stop.h"
#include "target.h"

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
void InitStop(void)
{
	// �|���S���̏�����
	InitPolygon();

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �e�̏�����
	InitShadow();

	// ���f���̏�����
	InitModel();

	// �v���C���[�̓ǂݍ���
	LoadPlayer();

	// �v���C���[�̏�����
	InitPlayer();

	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// ���̏�����
	InitNumber();

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
	PlaySound(SOUND_LABEL_STOP);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitStop(void)
{
	// �T�E���h�̒�~
	StopSound();

	// �|���S���̏I��
	UninitPolygon();

	// �G�t�F�N�g�̏I��
	UninitEffect();

	// �e�̏I��
	UninitShadow();

	// ���f���̏I��
	UninitModel();

	// �v���C���[�̏I��
	UninitPlayer();

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
void UpdateStop(void)
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
			PlaySound(SOUND_LABEL_SE_COUNTDOWN);
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
		}

		break;

	case GAMESTATE_END:		// �I�����(�Q�[���I����)

		s_nTime++;

		if (s_nTime >= CHANGE_TIME)
		{// �w��̒l���z����
			s_nTime = 0;

			// ���U���g�̏�����
			InitResult();

			// �Q�[���̐ݒ�
			SetGameState(GAMESTATE_RESULT);

			D3DXVECTOR3 pos = GetModel()->pos;

			pos.y = 0.0f;

			// �p�[�e�B�N���̐ݒ�
			SetParticle(pos, 360.0f, 20.0f, true, GetModel()->nRepeat);
		}

		break;

	case GAMESTATE_RESULT:		// ���U���g���(�Q�[���I����)

		// ���U���g�̍X�V
		UpdateResult();

		break;

	case GAMESTATE_LOOP:		// �J��Ԃ���� (���U���g�I����)

		// ���[�v�̍X�V
		UpdateLoop();

		break;

	default:
		assert(false);
		break;
	}

	// ���f���̍X�V
	UpdateModel();

	// �p�[�e�B�N���̍X�V
	UpdateParticle();

	// �G�t�F�N�g�̍X�V
	UpdateEffect();

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
void DrawStop(void)
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

	// ���f���̕`��
	DrawModel();

	// �v���C���[�̕`��
	DrawPlayer();

	// �e�̕`��
	DrawShadow();

	// �G�t�F�N�g�̕`��
	DrawEffect();

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

		// ���U���g�̕`��
		DrawResult();

		// ���̕`��
		DrawNumber2D(USE_RESULT);

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

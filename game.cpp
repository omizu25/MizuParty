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
#include "billboard.h"
#include "camera.h"
#include "countdown.h"
#include "effect.h"
#include "fade.h"
#include "field.h"
#include "game.h"
#include "input.h"
#include "light.h"
#include "mesh_field.h"
#include "model.h"
#include "number.h"
#include "player.h"
#include "particle.h"
#include "polygon.h"
#include "result.h"
#include "rule.h"
#include "shadow.h"
#include "sound.h"
#include "target.h"
#include "time.h"
#include "title.h"
#include "wall.h"

#include <assert.h>
//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static GAMESTATE		s_gameState;		// �Q�[�����
static int				s_nTime;			// �^�C��
static bool				s_bRemix;			// ���~�b�N�X

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitGame(void)
{
	switch (GetTitle())
	{// �ǂ̃Q�[���H
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_STOP:			// �~�߂�

		// �|���S���̏�����
		InitPolygon();

		break;

	case MENU_SLOPE:		// ��

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

		break;

	default:
		assert(false);
		break;
	}

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

	// �r���{�[�h�̏�����
	InitBillboard();

	switch (GetTitle())
	{// �ǂ̃Q�[���H
	case MENU_WALKING:		// �E�H�[�L���O

		// �r���{�[�h�̓ǂݍ���
		LoadBillboard();

		break;

	case MENU_STOP:			// �~�߂�

		/* �����Ȃ� */

		break;

	case MENU_SLOPE:		// ��

		// ��̃r���{�[�h�̏�����
		InitBillboardSlope();

		break;

	default:
		assert(false);
		break;
	}

	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// ���̏�����
	InitNumber();

	if (GetTitle() != MENU_STOP)
	{// �~�߂邶��Ȃ�
		// �^�C���̏�����
		InitTime();
	}

	// �^�[�Q�b�g�̏�����
	InitTarget();

	// �J�E���g�_�E���̏�����
	InitCountdown();

	// ���[���̏�����
	InitRule();

	s_gameState = GAMESTATE_START;

	s_nTime = 0;

	// ���U���g�̐ݒ�
	SetResult(RESULT_NONE);

		switch (GetTitle())
	{// �ǂ̃Q�[���H
	case MENU_WALKING:		// �E�H�[�L���O

		// �T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_�ޏ��̂��Ƃ�);

		break;

	case MENU_STOP:			// �~�߂�

		// �T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_������_�A�R�[�X�e�B�b�N49);

		break;

	case MENU_SLOPE:		// ��

		// �T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_������_�T�C�o�[44);

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
	// �T�E���h�̒�o
	StopSound();

	// �|���S���̏I��
	UninitPolygon();

	// �t�B�[���h�̏I��
	UninitField();

	// ���b�V���t�B�[���h�̏I��
	UninitMeshField();

	// �ǂ̏I��
	UninitWall();

	// �r���{�[�h�̏I��
	UninitBillboard();

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

	// ���U���g�̏I��
	UninitResult();

	// �J�E���g�_�E���̏I��
	UninitCountdown();

	// �^�[�Q�b�g�̏I��
	UninitTarget();

	// ���[���̏I��
	UninitRule();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateGame(void)
{
	switch (s_gameState)
	{
	case GAMESTATE_START:		// �J�n���(�Q�[���J�n��)
		s_nTime++;

		if (s_nTime >= 120)
		{
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

		switch (GetTitle())
		{// �ǂ̃Q�[���H
		case MENU_WALKING:		// �E�H�[�L���O
		case MENU_SLOPE:		// ��

			if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
			{// ��, ��, ��, ���L�[�������ꂽ
			 // ���̃��[�V����
				NextMotion(MOTION_MOVE);
			}

			break;

		case MENU_STOP:			// �~�߂�

			/* �����Ȃ� */

			break;

		default:
			assert(false);
			break;
		}

		break;

	case GAMESTATE_NORMAL:		// �ʏ���(�Q�[���i�s��)

		//�J�E���g�_�E���̉��Z
		AddCountdown(1);

		//�J�E���g�_�E���̍X�V
		UpdateCountdown();

		if (GetCountdown())
		{
			switch (GetTitle())
			{// �ǂ̃Q�[���H
			case MENU_WALKING:		// �E�H�[�L���O
			case MENU_STOP:			// �~�߂�

				// �|���S���̍X�V
				UpdatePolygon();

				break;

			case MENU_SLOPE:		// ��

				// �t�B�[���h�̍X�V
				UpdateField();

				// ���b�V���t�B�[���h�̍X�V
				UpdateMeshField();

				// �ǂ̍X�V
				UpdateWall();

				break;

			default:
				assert(false);
				break;
			}

			// ���f���̍X�V
			UpdateModel();

			if (GetTitle() != MENU_STOP)
			{// �~�߂邶��Ȃ�
				// �^�C���̌��Z
				SubTime(1);
			}
		}

		break;

	case GAMESTATE_END:		// �I�����(�Q�[���I����)


		switch (GetTitle())
		{// �ǂ̃Q�[���H
		case MENU_SLOPE:		// ��

			/* �����Ȃ� */

			break;

		case MENU_WALKING:		// �E�H�[�L���O
		case MENU_STOP:			// �~�߂�

			s_nTime++;

			if (s_nTime >= 120)
			{
				s_nTime = 0;

				// ���U���g�̏�����
				InitResult();

				// �Q�[���̐ݒ�
				SetGameState(GAMESTATE_RESULT);

				if (GetTitle() == MENU_WALKING)
				{// �~�߂�
					// �J�����̏�����
					InitCamera();
				}

				if (GetResult() == RESULT_GAMEOVER)
				{
					// �T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_�Q�[���I�[�o�[);
				}
			}

			break;

		default:
			assert(false);
			break;
		}

		break;

	case GAMESTATE_RESULT:		// ���U���g���(�Q�[���I����)

		if (GetOverlap())
		{// �d�Ȃ���
			// ���U���g�̍X�V
			UpdateResult();
		}

		break;

	default:
		assert(false);
		break;
	}

	if (GetTitle() == MENU_SLOPE)
	{
		// �r���{�[�h�̍X�V
		UpdateBillboard();
	}

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
void DrawGame(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nMax = 1;

	if (GetTitle() == MENU_SLOPE)
	{
		switch (s_gameState)
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

		switch (GetTitle())
		{// �ǂ̃Q�[���H
		case MENU_WALKING:		// �E�H�[�L���O
		case MENU_STOP:			// �~�߂�

			// �|���S���̕`��
			DrawPolygon();

			break;

		case MENU_SLOPE:		// ��

			// �t�B�[���h�̕`��
			DrawField();

			// ���b�V���t�B�[���h�̕`��
			DrawMeshField();

			// �ǂ̕`��
			DrawWall();

			break;

		default:
			assert(false);
			break;
		}

		// ���f���̕`��
		DrawModel();

		// �v���C���[�̕`��
		DrawPlayer();

		// �e�̕`��
		DrawShadow();

		switch (GetTitle())
		{// �ǂ̃Q�[���H
		case MENU_WALKING:		// �E�H�[�L���O
		case MENU_SLOPE:		// ��

			// �r���{�[�h�̕`��
			DrawBillboard(false, bCamera);

			break;

		case MENU_STOP:			// �~�߂�

			/* �����Ȃ� */

			break;

		default:
			assert(false);
			break;
		}

		// �G�t�F�N�g�̕`��
		DrawEffect();

		// �^�[�Q�b�g�̕`��
		DrawTarget();

		if (s_gameState != GAMESTATE_START)
		{
			// ���̕`��
			DrawNumber(USE_GAME);
		}

		if (s_gameState == GAMESTATE_COUNTDOWN ||
			s_gameState == GAMESTATE_NORMAL)
		{
			// ���[���̕`��
			DrawRule();
		}

		if (s_gameState == GAMESTATE_START)
		{
			// ���̕`��
			DrawNumber(USE_START);
		}

		if (s_gameState == GAMESTATE_NORMAL ||
			s_gameState == GAMESTATE_COUNTDOWN)
		{
			// ���̕`��
			DrawNumber(USE_GAME_ONLY);
		}

		if (s_gameState == GAMESTATE_RESULT)
		{
			switch (GetTitle())
			{// �ǂ̃Q�[���H
			case MENU_WALKING:		// �E�H�[�L���O
			case MENU_SLOPE:		// ��

				// �r���{�[�h�̕`��
				DrawBillboard(true, bCamera);

				break;

			case MENU_STOP:			// �~�߂�

				/* �����Ȃ� */

				break;

			default:
				assert(false);
				break;
			}

			if (GetOverlap())
			{// �d�Ȃ���
				// ���U���g�̕`��
				DrawResult();

				// ���̕`��
				DrawNumber(USE_RESULT);
			}
		}

		//�J�E���g�_�E���̕`��
		DrawCountdown();
	}

	// �r���[�{�[�h�̃N���A
	pDevice->SetViewport(&viewport);
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
}

//--------------------------------------------------
// ���~�b�N�X�̎擾
//--------------------------------------------------
bool GetRemix(void)
{
	return s_bRemix;
}
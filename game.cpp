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
#include "bullet.h"
#include "camera.h"
#include "countdown.h"
#include "effect.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "light.h"
#include "line.h"
#include "mesh_cylinder.h"
#include "mesh_field.h"
#include "mesh_sky.h"
#include "mesh_sphere.h"
#include "model.h"
#include "number.h"
#include "player.h"
#include "particle.h"
#include "polygon.h"
#include "result.h"
#include "rule.h"
#include "shadow.h"
#include "target.h"
#include "time.h"
#include "title.h"
#include "wall.h"

#include <assert.h>
//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Game		s_game;			// �Q�[�����
static int		s_nTime;		// �^�C��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitGame(void)
{
	// �|���S���̏�����
	InitPolygon();

	// ���b�V���t�B�[���h�̏�����
	//InitMeshField();

	// ���b�V���t�B�[���h�̐ݒ�
	//SetMeshField();

	// ���b�V���~���̏�����
	//InitMeshCylinder();

	// ���b�V���~���̐ݒ�
	//SetMeshCylinder();

	// ���b�V�����̏�����
	//InitMeshSphere();

	// ���b�V�����̐ݒ�
	//SetMeshSphere();

	// ���b�V����̏�����
	//InitMeshSky();

	// ���b�V����̐ݒ�
	//SetMeshSky();

	// �ǂ̏�����
	//InitWall();

	// �ǂ̐ݒu
	//InstallationWall();

	// �r���{�[�h�̏�����
	InitBillboard();

	// �r���{�[�h�̓ǂݍ���
	LoadBillboard();

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �e�̏�����
	InitShadow();

	// ���̏�����
	//InitLine();

	if (GetTitle() == MENU_STOP)
	{// �~�߂�
		// ���f���̏�����
		InitModel();
	}

	// �v���C���[�̓ǂݍ���
	LoadPlayer();

	// �v���C���[�̏�����
	InitPlayer();

	// �e�̏�����
	//InitBullet();

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

	s_game.gameState = GAMESTATE_START;

	s_nTime = 0;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitGame(void)
{
	// �|���S���̏I��
	UninitPolygon();

	// ���b�V���t�B�[���h�̏I��
	//UninitMeshField();

	// ���b�V���~���̏I��
	//UninitMeshCylinder();

	// ���b�V�����̏I��
	//UninitMeshSphere();

	// ���b�V����̏I��
	//UninitMeshSky();

	// �ǂ̏I��
	//UninitWall();

	// �r���{�[�h�̏I��
	UninitBillboard();

	// �G�t�F�N�g�̏I��
	UninitEffect();

	// �e�̏I��
	UninitShadow();

	// ���̏I��
	//UninitLine();

	// ���f���̏I��
	UninitModel();

	// �v���C���[�̏I��
	UninitPlayer();

	// �e�̏I��
	//UninitBullet();

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
	switch (s_game.gameState)
	{
	case GAMESTATE_START:		// �J�n���(�Q�[���J�n��)
		s_nTime++;

		if (s_nTime >= 120)
		{
			s_nTime = 0;

			// �Q�[���̐ݒ�
			SetGameState(GAMESTATE_COUNTDOWN);
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
		case MENU_RANKING:		// �����L���O

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

		if (!s_game.bPause)
		{
			// �|���S���̍X�V
			UpdatePolygon();

			// ���b�V���t�B�[���h�̍X�V
			//UpdateMeshField();

			// ���b�V���~���̍X�V
			//UpdateMeshCylinder();

			// ���b�V�����̍X�V
			//UpdateMeshSphere();

			// ���b�V����̍X�V
			//UpdateMeshSky();

			// �ǂ̍X�V
			//UpdateWall();

			// �r���{�[�h�̍X�V
			UpdateBillboard();

			// �p�[�e�B�N���̍X�V
			UpdateParticle();

			// �G�t�F�N�g�̍X�V
			UpdateEffect();

			if (GetTitle() == MENU_STOP)
			{// �~�߂�
				// ���f���̍X�V
				UpdateModel();
			}

			// ���̍X�V
			//UpdateLine();

			// �e�̍X�V
			//UpdateBullet();
		}

		if (GetKeyboardTrigger(DIK_F4))
		{// F4�L�[�������ꂽ
			s_game.bPause = !s_game.bPause;
		}

		if (GetKeyboardTrigger(DIK_F5))
		{// F5�L�[�������ꂽ
			s_game.bWireframe = !s_game.bWireframe;

			// �f�o�C�X�ւ̃|�C���^�̎擾
			LPDIRECT3DDEVICE9 pDevice = GetDevice();

			if (s_game.bWireframe)
			{// �\��
				// �����_�[�X�e�[�g�̐ݒ�
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else
			{// ��\��
				// �����_�[�X�e�[�g�����ɖ߂�
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
		}

		if (GetTitle() != MENU_STOP)
		{// �~�߂邶��Ȃ�
			// �^�C���̌��Z
			SubTime(1);
		}

		break;

	case GAMESTATE_END:		// �I�����(�Q�[���I����)

		s_nTime++;

		if (s_nTime >= 120)
		{
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
void DrawGame(void)
{
	// �J�����̐ݒ�
	SetCamera();

	// �|���S���̕`��
	DrawPolygon();

	// ���b�V����̕`��
	//DrawMeshSky();

	// ���b�V���t�B�[���h�̕`��
	//DrawMeshField();

	if (GetTitle() == MENU_STOP)
	{// �~�߂�
		// ���f���̕`��
		DrawModel();
	}

	// �v���C���[�̕`��
	DrawPlayer();

	// ���̕`��
	//DrawLine();

	// �e�̕`��
	DrawShadow();

	// ���b�V�����̕`��
	//DrawMeshSphere();

	// ���b�V���~���̕`��
	//DrawMeshCylinder();

	// �r���{�[�h�̕`��
	DrawBillboard(false);

	// �ǂ̕`��
	//DrawWall();

	// �G�t�F�N�g�̕`��
	DrawEffect();

	// �^�[�Q�b�g�̕`��
	DrawTarget();

	if (s_game.gameState != GAMESTATE_START)
	{
		// ���̕`��
		DrawNumber(USE_GAME);

		// ���[���̕`��
		DrawRule();
	}

	if (s_game.gameState == GAMESTATE_START)
	{
		// ���̕`��
		DrawNumber(USE_START);
	}

	if (s_game.gameState == GAMESTATE_NORMAL ||
		s_game.gameState == GAMESTATE_COUNTDOWN ||
		s_game.gameState == GAMESTATE_END)
	{
		// ���̕`��
		DrawNumber(USE_GAME_ONLY);
	}

	if (s_game.gameState == GAMESTATE_RESULT)
	{
		// �r���{�[�h�̕`��
		DrawBillboard(true);

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

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	s_game.gameState = state;

	if (state == GAMESTATE_END)
	{
		// ���U���g�̏�����
		InitResult();
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Game GetGame(void)
{
	return s_game;
}

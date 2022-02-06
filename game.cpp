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
#include "field.h"
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

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �e�̏�����
	InitShadow();

	// ���̏�����
	//InitLine();

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

	// ���U���g�̐ݒ�
	SetResult(RESULT_NONE);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitGame(void)
{
	// �|���S���̏I��
	UninitPolygon();

	// �t�B�[���h�̏I��
	UninitField();

	// ���b�V���t�B�[���h�̏I��
	UninitMeshField();

	// ���b�V���~���̏I��
	//UninitMeshCylinder();

	// ���b�V�����̏I��
	//UninitMeshSphere();

	// ���b�V����̏I��
	//UninitMeshSky();

	// �ǂ̏I��
	UninitWall();

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
			if (!s_game.bPause)
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

				// ���b�V���~���̍X�V
				//UpdateMeshCylinder();

				// ���b�V�����̍X�V
				//UpdateMeshSphere();

				// ���b�V����̍X�V
				//UpdateMeshSky();

				switch (GetTitle())
				{// �ǂ̃Q�[���H
				case MENU_WALKING:		// �E�H�[�L���O
				case MENU_SLOPE:		// ��

					// �r���{�[�h�̍X�V
					UpdateBillboard();

					break;

				case MENU_STOP:			// �~�߂�

					/* �����Ȃ� */

					break;

				default:
					assert(false);
					break;
				}

				// ���f���̍X�V
				UpdateModel();

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
	// �J�����̐ݒ�
	SetCamera();

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

	// ���b�V����̕`��
	//DrawMeshSky();

	// ���f���̕`��
	DrawModel();

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

	switch (GetTitle())
	{// �ǂ̃Q�[���H
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_SLOPE:		// ��

		// �r���{�[�h�̕`��
		DrawBillboard(false);

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
		switch (GetTitle())
		{// �ǂ̃Q�[���H
		case MENU_WALKING:		// �E�H�[�L���O
		case MENU_SLOPE:		// ��

			// �r���{�[�h�̕`��
			DrawBillboard(true);

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

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	s_game.gameState = state;
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Game GetGame(void)
{
	return s_game;
}

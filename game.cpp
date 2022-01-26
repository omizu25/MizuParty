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
#include "effect.h"
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
#include "shadow.h"
#include "target.h"
#include "time.h"
#include "wall.h"

#include <assert.h>
//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Game		s_game;		// �Q�[�����

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitGame(HWND hWnd)
{
	// �|���S���̏�����
	//InitPolygon();

	// ���b�V���t�B�[���h�̏�����
	InitMeshField();

	// ���b�V���t�B�[���h�̐ݒ�
	SetMeshField();

	// ���b�V���~���̏�����
	InitMeshCylinder();

	// ���b�V���~���̐ݒ�
	SetMeshCylinder();

	// ���b�V�����̏�����
	InitMeshSphere();

	// ���b�V�����̐ݒ�
	SetMeshSphere();

	// ���b�V����̏�����
	InitMeshSky();

	// ���b�V����̐ݒ�
	SetMeshSky();

	// �ǂ̏�����
	InitWall();

	// �ǂ̐ݒu
	InstallationWall();

	// �r���{�[�h�̏�����
	InitBillboard();

	// �r���{�[�h�̓ǂݍ���
	LoadBillboard(hWnd);

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �e�̏�����
	InitShadow();

	// ���̏�����
	InitLine();

	// ���f���̏�����
	InitModel();

	// �v���C���[�̓ǂݍ���
	LoadPlayer(hWnd);

	// �v���C���[�̏�����
	InitPlayer();

	// �e�̏�����
	InitBullet();

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

	s_game.gameState = GAMESTATE_NORMAL;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitGame(void)
{
	// �|���S���̏I��
	//UninitPolygon();

	// ���b�V���t�B�[���h�̏I��
	UninitMeshField();

	// ���b�V���~���̏I��
	UninitMeshCylinder();

	// ���b�V�����̏I��
	UninitMeshSphere();

	// ���b�V����̏I��
	UninitMeshSky();

	// �ǂ̏I��
	UninitWall();

	// �r���{�[�h�̏I��
	UninitBillboard();

	// �G�t�F�N�g�̏I��
	UninitEffect();

	// �e�̏I��
	UninitShadow();

	// ���̏I��
	UninitLine();

	// ���f���̏I��
	UninitModel();

	// �v���C���[�̏I��
	UninitPlayer();

	// �e�̏I��
	UninitBullet();

	// �J�����̏I��
	UninitCamera();

	// ���C�g�̏I��
	UninitLight();

	// ���̏I��
	UninitNumber();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateGame(void)
{
	switch (s_game.gameState)
	{
	case GAMESTATE_START:		// �J�n���(�Q�[���J�n��)
						 
		break;

	case GAMESTATE_NORMAL:		// �ʏ���(�Q�[���i�s��)

		if (!s_game.bPause)
		{
			// �|���S���̍X�V
			//UpdatePolygon();

			// ���b�V���t�B�[���h�̍X�V
			UpdateMeshField();

			// ���b�V���~���̍X�V
			UpdateMeshCylinder();

			// ���b�V�����̍X�V
			UpdateMeshSphere();

			// ���b�V����̍X�V
			UpdateMeshSky();

			// �ǂ̍X�V
			UpdateWall();

			// �r���{�[�h�̍X�V
			UpdateBillboard();

			// �p�[�e�B�N���̍X�V
			UpdateParticle();

			// �G�t�F�N�g�̍X�V
			UpdateEffect();

			// ���f���̍X�V
			UpdateModel();

			// �v���C���[�̍X�V
			UpdatePlayer();

			// ���̍X�V
			UpdateLine();

			// �e�̍X�V
			UpdateBullet();

			// �e�̍X�V
			UpdateShadow();

			// �J�����̍X�V
			UpdateCamera();

			// ���C�g�̍X�V
			UpdateLight();
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
		
		// �^�C���̌��Z
		SubTime(1);

		// ���̍X�V
		UpdateNumber();

		break;

	case GAMESTATE_END:		// �I�����(�Q�[���I����)

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
	// �J�����̐ݒ�
	SetCamera();

	// �|���S���̕`��
	//DrawPolygon();

	// ���b�V����̕`��
	DrawMeshSky();

	// ���b�V���t�B�[���h�̕`��
	DrawMeshField();

	// ���f���̕`��
	DrawModel();

	// �v���C���[�̕`��
	DrawPlayer();

	// ���̕`��
	DrawLine();

	// �e�̕`��
	DrawShadow();

	// ���b�V�����̕`��
	//DrawMeshSphere();

	// ���b�V���~���̕`��
	//DrawMeshCylinder();

	// �r���{�[�h�̕`��
	DrawBillboard(false);

	// �ǂ̕`��
	DrawWall();

	// �G�t�F�N�g�̕`��
	DrawEffect();

	// �r���{�[�h�̕`��
	DrawBillboard(true);

	// ���̕`��
	DrawNumber();
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

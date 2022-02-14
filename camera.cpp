//==================================================
// 
// 3D�Q�[������ ( camera.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "countdown.h"
#include "field.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "result.h"
#include "setup.h"
#include "shadow.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_CAMERA			(2)				// �J�����̍ő吔
#define MAX_NEAR			(10.0f)			// �j�A�̍ő�l
#define MAX_FAR				(2500.0f)		// �t�@�[�̍ő�
#define MAX_ROTATION		(0.035f)		// ��]�̍ő�l
#define MAX_DISTANCE		(50.0f)			// �����̍ő�l
#define MIN_DISTANCE		(0.0f)			// �����̍ŏ��l
#define START_DISTANCE		(30.0f)			// �����̍ŏ��̒l
#define MAX_POS_FACTOR		(0.05f)			// �ʒu�̌����W��
#define MAX_ROT_FACTOR		(0.2f)			// �����̌����W��
#define START_WALKING_Y		(100.0f)		// Y�̈ʒu�̍ŏ��̒l
#define START_WALKING_Z		(-300.0f)		// Z�̈ʒu�̍ŏ��̒l
#define START_STOP_Y		(180.0f)		// Y�̈ʒu�̍ŏ��̒l
#define START_STOP_Z		(-500.0f)		// Z�̈ʒu�̍ŏ��̒l
#define START_SLOPE_Y		(250.0f)		// Y�̈ʒu�̍ŏ��̒l
#define START_SLOPE_Z		(-1300.0f)		// Z�̈ʒu�̍ŏ��̒l
#define MOVE_Y				(5.0f)			// Y�̈ړ���
#define MOVE_Z				(-3.0f)			// Z�̈ړ���
#define STOP_POS_Y			(100.0f)		// Y�̈ʒu�̎~�܂�ꏊ

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Camera		s_camera[MAX_CAMERA];		// �J�����̏��
static bool			s_bOverlap;					// �v���C���[�Əd�Ȃ������ǂ���
static bool			s_bStop;					// �~�܂邩�ǂ���

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void InitTitleMode(void);
static void InitGameMode(void);
static void UpdateGameMode(void);
static void FollowMove(void);
static void ResultMove(void);
static void Overlap(float fPosX);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitCamera(void)
{
	// �N���A
	memset(s_camera, 0, sizeof(s_camera));

	switch (GetMode())
	{
	case MODE_TITLE:		// �^�C�g��

		// �^�C�g���̎��̏�����
		InitTitleMode();

		break;

	case MODE_GAME:			// �Q�[��

		// �Q�[���̎��̏�����
		InitGameMode();

		break;

	default:
		assert(false);
		break;
	}

	// ���_�E�����_�E������E�����E������ݒ肷��
	s_camera[0].posVDest = s_camera[0].posV;
	s_camera[0].posRDest = s_camera[0].posR;
	s_camera[0].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// �Œ�ł���
	s_camera[0].rotDest = s_camera[0].rot;

	float fDisX, fDisZ;

	fDisX = s_camera[0].posV.x - s_camera[0].posR.x;
	fDisZ = s_camera[0].posV.z - s_camera[0].posR.z;

	s_camera[0].fDistance = sqrtf((fDisX * fDisX) + (fDisZ * fDisZ));
	s_camera[0].fDisPlayer = START_DISTANCE;

	s_camera[0].viewport.X = (DWORD)0.0f;
	s_camera[0].viewport.Y = (DWORD)0.0f;
	s_camera[0].viewport.Width = (DWORD)SCREEN_WIDTH;
	s_camera[0].viewport.Height = (DWORD)SCREEN_HEIGHT;
	s_camera[0].viewport.MinZ = 0.0f;
	s_camera[0].viewport.MaxZ = 1.0f;

	if (GetTitle() == MENU_SLOPE)
	{
		s_camera[1].posV = D3DXVECTOR3(GetField()->pos.x * 0.35f, 1200.0f, -1500.0f);
		s_camera[1].posR = D3DXVECTOR3(GetField()->pos.x * 0.35f, 350.0f, 0.0f);
		s_camera[1].rot = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);
		s_camera[1].posVDest = s_camera[1].posV;
		s_camera[1].posRDest = s_camera[1].posR;
		s_camera[1].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// �Œ�ł���
		s_camera[1].rotDest = s_camera[1].rot;

		fDisX = s_camera[1].posV.x - s_camera[1].posR.x;
		fDisZ = s_camera[1].posV.z - s_camera[1].posR.z;

		s_camera[1].fDistance = sqrtf((fDisX * fDisX) + (fDisZ * fDisZ));
		s_camera[1].fDisPlayer = START_DISTANCE;

		s_camera[1].viewport.X = (DWORD)0.0f;
		s_camera[1].viewport.Y = (DWORD)(SCREEN_HEIGHT * 0.65f);
		s_camera[1].viewport.Width = (DWORD)(SCREEN_WIDTH * 0.35f);
		s_camera[1].viewport.Height = (DWORD)(SCREEN_HEIGHT * 0.35f);
		s_camera[1].viewport.MinZ = 0.0f;
		s_camera[1].viewport.MaxZ = 1.0f;
	}

	if (GetTitle() == MENU_WALKING)
	{
		s_bOverlap = false;
	}
	else
	{
		s_bOverlap = true;;
	}
	
	s_bStop = false;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitCamera(void)
{
	
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateCamera(void)
{
	switch (GetMode())
	{
	case MODE_TITLE:		// �^�C�g��

		/* �����Ȃ� */

		break;

	case MODE_GAME:			// �Q�[��

		// �Q�[���̎��̍X�V
		UpdateGameMode();

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetCamera(int nData)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Camera *pCamara = &s_camera[nData];

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&pCamara->mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(
		&pCamara->mtxView,
		&pCamara->posV,
		&pCamara->posR,
		&pCamara->vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &pCamara->mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&pCamara->mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(
		&pCamara->mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		MAX_NEAR,
		MAX_FAR);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &pCamara->mtxProjection);
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Camera *GetCamera(int nData)
{
	return &s_camera[nData];
}

//--------------------------------------------------
// �d�Ȃ��Ă邩�ǂ������擾
//--------------------------------------------------
bool GetOverlap(void)
{
	return s_bOverlap;
}

//--------------------------------------------------
// �^�C�g���̎��̏�����
//--------------------------------------------------
static void InitTitleMode(void)
{
	s_camera[0].posV = D3DXVECTOR3(0.0f, 450.0f, -0.1f);
	s_camera[0].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_camera[0].rot = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);
}

//--------------------------------------------------
// �Q�[���̎��̏�����
//--------------------------------------------------
static void InitGameMode(void)
{
	float fPosX = (GetField()->pos.x + GetField()->vtxMax.x) * 0.75f;

	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O

		s_camera[0].posV = D3DXVECTOR3(0.0f, START_WALKING_Y, START_WALKING_Z);
		s_camera[0].posR = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
		s_camera[0].rot = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);

		break;

	case MENU_STOP:			// �~�߂�

		s_camera[0].posV = D3DXVECTOR3(0.0f, START_STOP_Y, START_STOP_Z);
		s_camera[0].posR = D3DXVECTOR3(0.0f, START_STOP_Y, 0.0f);
		s_camera[0].rot = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);

		break;

	case MENU_SLOPE:		// ��

		s_camera[0].posV = D3DXVECTOR3(fPosX, START_SLOPE_Y, START_SLOPE_Z);
		s_camera[0].posR = D3DXVECTOR3(fPosX, START_SLOPE_Y, 0.0f);
		s_camera[0].rot = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �Q�[���̎��̍X�V
//--------------------------------------------------
static void UpdateGameMode(void)
{
	switch (GetTitle())
	{
	case MENU_STOP:			// �~�߂�

		if (GetCountdown())
		{
			if (!GetStop())
			{
				if (!s_bStop)
				{// �~�܂�Ȃ�
					if (GetGame() == GAMESTATE_NORMAL)
					{
						s_camera[0].posV.z += MOVE_Y;
						s_camera[0].posR.z += MOVE_Y;
						s_camera[0].posV.y += MOVE_Z;
						s_camera[0].posR.y += MOVE_Z;
					}

					if (s_camera[0].posV.y <= STOP_POS_Y)
					{
						s_bStop = true;
					}
				}
			}
		}

		break;

	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_SLOPE:		// ��

		switch (GetGame())
		{
		case GAMESTATE_NORMAL:			// �Q�[����
		case GAMESTATE_END:				// �I���
		case GAMESTATE_COUNTDOWN:		// �J�E���g�_�E��

			// �Ǐ]�̈ړ�
			FollowMove();

			if (GetTitle() == MENU_SLOPE)
			{
				if (GetPlayer()->pos.y >= 0.0f)
				{
					s_camera[0].posV.y = GetPlayer()->pos.y + START_SLOPE_Y;
				}
			}

			break;

		case GAMESTATE_RESULT:			// ���U���g

			// ���U���g�̈ړ�
			ResultMove();
			break;

		case GAMESTATE_NONE:			// �����Ȃ�
		case GAMESTATE_START:			// �n�܂�

			/* �����Ȃ� */

			break;

		default:
			assert(false);
			break;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �Ǐ]�̈ړ�
//--------------------------------------------------
static void FollowMove(void)
{
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��

	float fAngle = s_camera[0].rot.y - pPlayer->rot.y;

	// �p�x�̐��K��
	NormalizeRot(&fAngle);

	// �ړI�̒����_
	s_camera[0].posRDest.x = pPlayer->pos.x + sinf(fAngle) * s_camera[0].fDisPlayer;
	s_camera[0].posRDest.z = pPlayer->pos.z - cosf(fAngle) * s_camera[0].fDisPlayer;

	// �ړI�̎��_
	s_camera[0].posVDest.x = pPlayer->pos.x + sinf(s_camera[0].rot.y) * s_camera[0].fDistance;
	s_camera[0].posVDest.z = pPlayer->pos.z - cosf(s_camera[0].rot.y) * s_camera[0].fDistance;

	// �����_�̈ړ�
	s_camera[0].posR.x += (s_camera[0].posRDest.x - s_camera[0].posR.x) * MAX_POS_FACTOR;
	s_camera[0].posR.z += (s_camera[0].posRDest.z - s_camera[0].posR.z) * MAX_POS_FACTOR;

	// ���_�̈ړ�
	s_camera[0].posV.x += (s_camera[0].posVDest.x - s_camera[0].posV.x) * MAX_POS_FACTOR;
	s_camera[0].posV.z += (s_camera[0].posVDest.z - s_camera[0].posV.z) * MAX_POS_FACTOR;
}

//--------------------------------------------------
// ���U���g�̈ړ�
//--------------------------------------------------
static void ResultMove(void)
{
	if (!s_bOverlap)
	{// �d�Ȃ��ĂȂ�
		D3DXVECTOR3 pos = GetPlayer()->pos;
		float fMove = GetPlayer()->fMove * 2.5f;
		bool bDirection = true;		// true : �E����  false : ������

		if (pos.x <= 0.0f)
		{
			fMove *= -1.0f;
			bDirection = false;
		}

		// �ړI�̒����_
		s_camera[0].posRDest.x += fMove;

		// �ړI�̎��_
		s_camera[0].posVDest.x += fMove;

		// �����_�̈ړ�
		s_camera[0].posR.x += (s_camera[0].posRDest.x - s_camera[0].posR.x) * MAX_POS_FACTOR;

		// ���_�̈ړ�
		s_camera[0].posV.x += (s_camera[0].posVDest.x - s_camera[0].posV.x) * MAX_POS_FACTOR;

		if (bDirection)
		{// �E����
			if (pos.x <= s_camera[0].posR.x)
			{
				// �d�˂�
				Overlap(pos.x);
			}
		}
		else
		{// ������
			if (pos.x >= s_camera[0].posR.x)
			{
				// �d�˂�
				Overlap(pos.x);
			}
		}
	}
}

//--------------------------------------------------
// �d�˂�
//--------------------------------------------------
static void Overlap(float fPosX)
{
	// �ړI�̒����_
	s_camera[0].posRDest.x = fPosX;

	// �ړI�̎��_
	s_camera[0].posVDest.x = fPosX;

	// �����_�̈ړ�
	s_camera[0].posR.x = fPosX;

	// ���_�̈ړ�
	s_camera[0].posV.x = fPosX;

	s_bOverlap = true;

	// ���U���g�̏�����
	InitResult();
}
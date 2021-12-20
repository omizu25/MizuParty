//==================================================
// 
// 3D�Q�[������ ( camera.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "camera.h"
#include "input.h"
#include "player.h"
#include "setup.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_NEAR			(10.0f)			// �j�A�̍ő�l
#define MAX_FAR				(1500.0f)		// �t�@�[�̍ő�l
#define MAX_MOVE			(2.0f)			// �ړ��ʂ̍ő�l
#define MAX_ROTATION		(0.035f)		// ��]�̍ő�l
#define MAX_DISTANCE		(50.0f)			// �����̍ő�l
#define MIN_DISTANCE		(0.0f)			// �����̍ŏ��l
#define START_DISTANCE		(20.0f)			// �����̍ŏ��̒l
#define MAX_POS_FACTOR		(0.05f)			// �ʒu�̌����W��
#define MAX_ROT_FACTOR		(0.2f)			// �����̌����W��
#define START_POS_Y			(100.0f)		// Y�̈ʒu�̍ŏ��̒l
#define START_POS_Z			(-200.0f)		// Z�̈ʒu�̍ŏ��̒l
#define STOP_TIME			(120)			// �~�܂��Ă��鎞��

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Camera		s_camera;			// �J�����̏��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void FollowMove(void);
static void FollowWrap(void);
static void Move(void);
static void Rot(void);
static void ResetCamera(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitCamera(void)
{
	// ���_�E�����_�E������E�����E������ݒ肷��
	s_camera.posV = D3DXVECTOR3(0.0f, START_POS_Y, START_POS_Z);
	s_camera.posR = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
	s_camera.posVDest = s_camera.posV;
	s_camera.posRDest = s_camera.posR;
	s_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// �Œ�ł���
	s_camera.rot  = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);
	s_camera.rotDest = s_camera.rot;

	float fDisX, fDisZ;

	fDisX = s_camera.posV.x - s_camera.posR.x;
	fDisZ = s_camera.posV.z - s_camera.posR.z;

	s_camera.fDistance = sqrtf((fDisX * fDisX) + (fDisZ * fDisZ));
	s_camera.fDisPlayer = START_DISTANCE;

	s_camera.bFollow = true;
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
	if (GetKeyboardTrigger(DIK_F6))
	{// F6�L�[�������ꂽ
		s_camera.bFollow = !s_camera.bFollow;
	
		// ���Z�b�g
		ResetCamera();
	}

	if (s_camera.bFollow)
	{// �Ǐ]����
		if (GetKeyboardPress(DIK_1))
		{// 1�L�[�������ꂽ
			s_camera.fDisPlayer -= 1.0f;
		}
		else if (GetKeyboardPress(DIK_2))
		{// 2�L�[�������ꂽ
			s_camera.fDisPlayer += 1.0f;
		}

		// �w��̒l�ȏ�E�ȉ�
		Specified(&s_camera.fDisPlayer, MAX_DISTANCE, MIN_DISTANCE);

		// �Ǐ]�̈ړ�
		FollowMove();

		// �Ǐ]�̉�荞��
		//FollowWrap();
	}
	else
	{// �Ǐ]���Ȃ�
		// �ړ�
		Move();

		// ��]
		Rot();

		/* ������̈ړ��� */

		if (GetKeyboardPress(DIK_Z) || GetKeyboardPress(DIK_C) ||
			GetKeyboardPress(DIK_U) || GetKeyboardPress(DIK_J))
		{// Z, C, U, J�L�[�������ꂽ
		 // ���_�̈ړ�
			s_camera.posV.x = s_camera.posR.x - sinf(s_camera.rot.y) * s_camera.fDistance;
			s_camera.posV.z = s_camera.posR.z - cosf(s_camera.rot.y) * s_camera.fDistance;
		}

		// �����_�̈ړ�
		s_camera.posR.x = s_camera.posV.x + sinf(s_camera.rot.y) * s_camera.fDistance;
		s_camera.posR.z = s_camera.posV.z + cosf(s_camera.rot.y) * s_camera.fDistance;
		s_camera.posR.y = s_camera.posV.y + tanf(-s_camera.rot.x + (D3DX_PI * 0.5f)) * s_camera.fDistance;
	}
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetCamera(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(
		&s_camera.mtxView,
		&s_camera.posV,
		&s_camera.posR,
		&s_camera.vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &s_camera.mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(
		&s_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		MAX_NEAR,
		MAX_FAR);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &s_camera.mtxProjection);
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Camera *GetCamera(void)
{
	return &s_camera;
}

//--------------------------------------------------
// �Ǐ]�̈ړ�
//--------------------------------------------------
static void FollowMove(void)
{
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��

	float fAngle = s_camera.rot.y - pPlayer->rot.y;

	// �p�x�̐��K��
	NormalizeRot(&fAngle);

	// �ړI�̒����_
	s_camera.posRDest.x = pPlayer->pos.x + sinf(fAngle) * s_camera.fDisPlayer;
	s_camera.posRDest.z = pPlayer->pos.z - cosf(fAngle) * s_camera.fDisPlayer;

	// �ړI�̎��_
	s_camera.posVDest.x = pPlayer->pos.x + sinf(s_camera.rot.y) * s_camera.fDistance;
	s_camera.posVDest.z = pPlayer->pos.z - cosf(s_camera.rot.y) * s_camera.fDistance;

	// �����_�̈ړ�
	s_camera.posR.x += (s_camera.posRDest.x - s_camera.posR.x) * MAX_POS_FACTOR;
	s_camera.posR.z += (s_camera.posRDest.z - s_camera.posR.z) * MAX_POS_FACTOR;

	// ���_�̈ړ�
	s_camera.posV.x += (s_camera.posVDest.x - s_camera.posV.x) * MAX_POS_FACTOR;
	s_camera.posV.z += (s_camera.posVDest.z - s_camera.posV.z) * MAX_POS_FACTOR;
}

//--------------------------------------------------
// �Ǐ]�̉�荞��
//--------------------------------------------------
static void FollowWrap(void)
{
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��

	if (pPlayer->nStopTime >= STOP_TIME)
	{// �w��̒l�ȏ�
		float fAngle = pPlayer->rot.y - s_camera.rot.y;

		// �p�x�̐��K��
		NormalizeRot(&fAngle);

		s_camera.rotDest.y = cosf(fAngle + (D3DX_PI * 0.5f));

		s_camera.rot.y += (s_camera.rotDest.y) * MAX_ROT_FACTOR;
	}
}

//--------------------------------------------------
// �ړ�
//--------------------------------------------------
static void Move(void)
{
	float fRot = 0.0f;

	if (GetDebug() != DEBUG_MESH)
	{// �f�o�b�O�\�������b�V���ł͂Ȃ���

		/* �����_�̈ړ��� */

		if (GetKeyboardPress(DIK_A))
		{// A�L�[�������ꂽ
			if (GetKeyboardPress(DIK_W))
			{// W�L�[�������ꂽ
				fRot = s_camera.rot.y + (-D3DX_PI * 0.25f);
			}
			else if (GetKeyboardPress(DIK_S))
			{// S�L�[�������ꂽ
				fRot = s_camera.rot.y + (-D3DX_PI * 0.75f);
			}
			else
			{
				fRot = s_camera.rot.y + (-D3DX_PI * 0.5f);
			}

			s_camera.posV.x += sinf(fRot) * MAX_MOVE;
			s_camera.posV.z += cosf(fRot) * MAX_MOVE;
		}
		else if (GetKeyboardPress(DIK_D))
		{// D�L�[�������ꂽ
			if (GetKeyboardPress(DIK_W))
			{// W�L�[�������ꂽ
				fRot = s_camera.rot.y + (D3DX_PI * 0.25f);
			}
			else if (GetKeyboardPress(DIK_S))
			{// S�L�[�������ꂽ
				fRot = s_camera.rot.y + (D3DX_PI * 0.75f);
			}
			else
			{
				fRot = s_camera.rot.y + (D3DX_PI * 0.5f);
			}

			s_camera.posV.x += sinf(fRot) * MAX_MOVE;
			s_camera.posV.z += cosf(fRot) * MAX_MOVE;
		}
		else if (GetKeyboardPress(DIK_W))
		{// W�L�[�������ꂽ
			fRot = s_camera.rot.y;

			s_camera.posV.x += sinf(fRot) * MAX_MOVE;
			s_camera.posV.z += cosf(fRot) * MAX_MOVE;
		}
		else if (GetKeyboardPress(DIK_S))
		{// S�L�[�������ꂽ
			fRot = s_camera.rot.y + (-D3DX_PI);

			s_camera.posV.x += sinf(fRot) * MAX_MOVE;
			s_camera.posV.z += cosf(fRot) * MAX_MOVE;
		}

		if (GetKeyboardPress(DIK_T))
		{// T�L�[�������ꂽ
			fRot = D3DX_PI * 0.5f;

			s_camera.posV.y += sinf(fRot) * MAX_MOVE;
		}
		else if (GetKeyboardPress(DIK_G))
		{// G�L�[�������ꂽ
			fRot = -D3DX_PI * 0.5f;

			s_camera.posV.y += sinf(fRot) * MAX_MOVE;
		}

		/* �����_�ƒ����_�̋����� */

		if (GetKeyboardPress(DIK_U))
		{// U�L�[�������ꂽ
			s_camera.fDistance += -MAX_MOVE;
		}
		else if (GetKeyboardPress(DIK_J))
		{// J�L�[�������ꂽ
			s_camera.fDistance += MAX_MOVE;
		}

		// �w��̒l�ȏ�E�ȉ�
		Specified(&s_camera.fDistance, MAX_FAR, MAX_NEAR);
	}
}

//--------------------------------------------------
// ��]
//--------------------------------------------------
static void Rot(void)
{
	if (GetDebug() != DEBUG_MESH)
	{// �f�o�b�O�\�������b�V���ł͂Ȃ���

		/* �����_�E�����_�̐��� */

		if (GetKeyboardPress(DIK_Z) || GetKeyboardPress(DIK_E))
		{// Z, E�L�[�������ꂽ
			s_camera.rot.y += MAX_ROTATION;
		}

		if (GetKeyboardPress(DIK_C) || GetKeyboardPress(DIK_Q))
		{// C, Q�L�[�������ꂽ
			s_camera.rot.y += -MAX_ROTATION;
		}

		// �p�x�̐��K��
		NormalizeRot(&s_camera.rot.y);

		/* �������_�̏㉺�� */

		if (GetKeyboardPress(DIK_Y))
		{// Y�L�[�������ꂽ
			s_camera.rot.x += -MAX_ROTATION;
		}
		else if (GetKeyboardPress(DIK_H))
		{// H�L�[�������ꂽ
			s_camera.rot.x += MAX_ROTATION;
		}

		// �w��̒l�ȏ�E�ȉ�
		Specified(&s_camera.rot.x, 3.0f, 0.1f);
	}
}

//--------------------------------------------------
// ���Z�b�g
//--------------------------------------------------
static void ResetCamera(void)
{
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��

	s_camera.posV.x = pPlayer->pos.x;
	s_camera.posV.y = pPlayer->pos.y + START_POS_Y;
	s_camera.posV.z = pPlayer->pos.z + START_POS_Z;
	s_camera.posR = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
	s_camera.posVDest = s_camera.posV;
	s_camera.posRDest = s_camera.posR;
	s_camera.rot = D3DXVECTOR3((D3DX_PI * 0.6f), 0.0f, 0.0f);
	s_camera.rotDest = s_camera.rot;
}

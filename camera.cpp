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
#include "game.h"
#include "input.h"
#include "player.h"
#include "setup.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_NEAR			(10.0f)			// �j�A�̍ő�l
#define MAX_FAR				(1500.0f)		// �t�@�[�̍ő�
#define MAX_ROTATION		(0.035f)		// ��]�̍ő�l
#define MAX_DISTANCE		(50.0f)			// �����̍ő�l
#define MIN_DISTANCE		(0.0f)			// �����̍ŏ��l
#define START_DISTANCE		(30.0f)			// �����̍ŏ��̒l
#define MAX_POS_FACTOR		(0.05f)			// �ʒu�̌����W��
#define MAX_ROT_FACTOR		(0.2f)			// �����̌����W��
#define START_POS_Y			(100.0f)		// Y�̈ʒu�̍ŏ��̒l
#define START_POS_Z			(-300.0f)		// Z�̈ʒu�̍ŏ��̒l
#define STOP_TIME			(120)			// �~�܂��Ă��鎞��

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Camera		s_camera;			// �J�����̏��
static bool			s_bOverlap;			// �v���C���[�Əd�Ȃ������ǂ���

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void FollowMove(void);
static void ResultMove(void);
static void Overlap(float fPosX);

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

	s_camera.viewport.X = (DWORD)0.0f;
	s_camera.viewport.Y = (DWORD)0.0f;
	s_camera.viewport.Width = SCREEN_WIDTH;
	s_camera.viewport.Height = SCREEN_HEIGHT;
	s_camera.viewport.MinZ = 0.0f;
	s_camera.viewport.MaxZ = 1.0f;

	s_bOverlap = false;
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
	switch (GetGame().gameState)
	{
	case GAMESTATE_NORMAL:		// �Q�[����
	case GAMESTATE_END:			// �I���

		// �Ǐ]�̈ړ�
		FollowMove();

		break;

	case GAMESTATE_RESULT:			// ���U���g
		
		// ���U���g�̈ړ�
		ResultMove();
		break;

	case GAMESTATE_NONE:			// �����Ȃ�
	case GAMESTATE_START:			// �n�܂�
	case GAMESTATE_COUNTDOWN:		// �J�E���g�_�E��

		/* �����Ȃ� */

		break;

	default:
		assert(false);
		break;
	}

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
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawCamera(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// �K����������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

	// ���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�o�b�t�@�̒l�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// �V�K�[�x�l <= Z�o�b�t�@�[�x�l (�����ݒ�)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
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
// �d�Ȃ��Ă邩�ǂ������擾
//--------------------------------------------------
bool GetOverlap(void)
{
	return s_bOverlap;
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
// ���U���g�̈ړ�
//--------------------------------------------------
static void ResultMove(void)
{
	if (!s_bOverlap)
	{// �d�Ȃ��ĂȂ�
		D3DXVECTOR3 pos = GetPlayer()->pos;
		float fMove = GetPlayer()->fMove;
		bool bDirection = true;		// true : �E����  false : ������

		if (pos.x <= 0.0f)
		{
			fMove *= -1.0f;
			bDirection = false;
		}

		// �ړI�̒����_
		s_camera.posRDest.x += fMove;

		// �ړI�̎��_
		s_camera.posVDest.x += fMove;

		// �����_�̈ړ�
		s_camera.posR.x += (s_camera.posRDest.x - s_camera.posR.x) * MAX_POS_FACTOR;

		// ���_�̈ړ�
		s_camera.posV.x += (s_camera.posVDest.x - s_camera.posV.x) * MAX_POS_FACTOR;

		if (bDirection)
		{// �E����
			if (pos.x <= s_camera.posR.x)
			{
				// �d�˂�
				Overlap(pos.x);
			}
		}
		else
		{// ������
			if (pos.x >= s_camera.posR.x)
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
	s_camera.posRDest.x = fPosX;

	// �ړI�̎��_
	s_camera.posVDest.x = fPosX;

	// �����_�̈ړ�
	s_camera.posR.x = fPosX;

	// ���_�̈ړ�
	s_camera.posV.x = fPosX;

	s_bOverlap = true;
}
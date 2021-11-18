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
#include "main.h"

//--------------------------------------------------
// �J�����̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		posV;				// ���_
	D3DXVECTOR3		posR;				// �����_
	D3DXVECTOR3		vecU;				// ������x�N�g��
	D3DXVECTOR3		rot;				// ����
	D3DXMATRIX		mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX		mtxView;			// �r���[�}�g���b�N�X
}Camera;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Camera		s_camera;		// �J�����̏��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void UpdateInput(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitCamera(void)
{
	// ���_�E�����_�E�������ݒ肷��
	s_camera.posV = D3DXVECTOR3(0.0f, 50.0f, -100.0f);
	s_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// �Œ�ł���
	s_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	// ����
	//UpdateInput();
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
		10.0f,
		1000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &s_camera.mtxProjection);
}

//--------------------------------------------------
// ����
//--------------------------------------------------
static void UpdateInput(void)
{
	
	if (GetKeyboardPress(DIK_A))
	{// A�L�[�������ꂽ
		s_camera.posV.x += sinf(-D3DX_PI * 0.5f);
		s_camera.posR.x += sinf(-D3DX_PI * 0.5f);
	}
	else if (GetKeyboardPress(DIK_D))
	{// D�L�[�������ꂽ
		s_camera.posV.x += sinf(D3DX_PI * 0.5f);
		s_camera.posR.x += sinf(D3DX_PI * 0.5f);
	}

	if (GetKeyboardPress(DIK_W))
	{// W�L�[�������ꂽ
		s_camera.posV.z += sinf(-D3DX_PI * 0.5f);
		s_camera.posR.z += sinf(-D3DX_PI * 0.5f);
	}
	else if (GetKeyboardPress(DIK_S))
	{// S�L�[�������ꂽ
		s_camera.posV.z += -1.0f;
		s_camera.posR.z += -1.0f;
	}
	
	if (GetKeyboardPress(DIK_Y))
	{// Y�L�[�������ꂽ
		s_camera.posR.z += 1.0f;
		s_camera.rot.z += D3DX_PI * 0.1f;
	}
	else if (GetKeyboardPress(DIK_N))
	{// N�L�[�������ꂽ
		s_camera.posR.z += -1.0f;
	}

	if (GetKeyboardPress(DIK_T))
	{// T�L�[�������ꂽ
		//s_camera.posR.y += -5.0f;
		s_camera.posV.y += 1.0f;
	}
	else if (GetKeyboardPress(DIK_B))
	{// B�L�[�������ꂽ
		//s_camera.posR.y += 5.0f;
		s_camera.posV.y += -1.0f;
	}
}
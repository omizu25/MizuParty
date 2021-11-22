//==================================================
// 
// 3D�Q�[������ ( model.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "camera.h"
#include "input.h"
#include "main.h"
#include "model.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_MOVE		(1.0f)		//�ړ��ʂ̍ő�l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPD3DXMESH		s_pMesh = NULL;			// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER		s_pBuffMat = NULL;		// �}�e���A�����ւ̃|�C���^
static DWORD			s_nNumMat = 0;			// �}�e���A�����̐�
static Model			s_model;				// ���f���̏��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void Move(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitModel(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		/*"data\\MODEL\\������.x",*/
		"data\\MODEL\\�Ă�Ă�S����.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

	s_model.pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	s_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitModel(void)
{
	if (s_pMesh != NULL)
	{// ���b�V���̔j��
		s_pMesh->Release();
		s_pMesh = NULL;
	}

	if (s_pBuffMat != NULL)
	{// �}�e���A���̔j��
		s_pBuffMat->Release();
		s_pBuffMat = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateModel(void)
{
	// �ړ�
	Move();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawModel(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_model.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_model.rot.y, s_model.rot.x, s_model.rot.z);
	D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_model.pos.x, s_model.pos.y, s_model.pos.z);
	D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_model.mtxWorld);

	// ���݂̃}�e���A���ێ�
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// ���f���p�[�c�̕`��
		s_pMesh->DrawSubset(i);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Model *GetModel(void)
{
	return &s_model;
}

//--------------------------------------------------
// �ړ�
//--------------------------------------------------
static void Move(void)
{ 
	Camera *pCamera = GetCamera();		//�J�����̎擾
	float fRot = 0.0f;

	if (GetKeyboardPress(DIK_LEFT))
	{// ���L�[�������ꂽ
		if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + (-D3DX_PI * 0.25f);

			s_model.rot.y = pCamera->rot.y + (D3DX_PI * 0.75f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + (-D3DX_PI * 0.75f);

			s_model.rot.y = pCamera->rot.y + (D3DX_PI * 0.25f);
		}
		else
		{
			fRot = pCamera->rot.y + (-D3DX_PI * 0.5f);

			s_model.rot.y = pCamera->rot.y + (D3DX_PI * 0.5f);
		}

		s_model.pos.x += sinf(fRot) * MAX_MOVE;
		s_model.pos.z += cosf(fRot) * MAX_MOVE;
	}
	else if (GetKeyboardPress(DIK_RIGHT))
	{// ���L�[�������ꂽ
		if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + (D3DX_PI * 0.25f);

			s_model.rot.y = pCamera->rot.y + (-D3DX_PI * 0.75f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + (D3DX_PI * 0.75f);

			s_model.rot.y = pCamera->rot.y + (-D3DX_PI * 0.25f);
		}
		else
		{
			fRot = pCamera->rot.y + (D3DX_PI * 0.5f);

			s_model.rot.y = pCamera->rot.y + (-D3DX_PI * 0.5f);
		}

		s_model.pos.x += sinf(fRot) * MAX_MOVE;
		s_model.pos.z += cosf(fRot) * MAX_MOVE;
	}
	else if (GetKeyboardPress(DIK_UP))
	{// ���L�[�������ꂽ
		fRot = pCamera->rot.y;

		s_model.pos.x += sinf(fRot) * MAX_MOVE;
		s_model.pos.z += cosf(fRot) * MAX_MOVE;

		s_model.rot.y = pCamera->rot.y + D3DX_PI;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{// ���L�[�������ꂽ
		fRot = pCamera->rot.y + D3DX_PI;

		s_model.pos.x += sinf(fRot) * MAX_MOVE;
		s_model.pos.z += cosf(fRot) * MAX_MOVE;

		s_model.rot.y = pCamera->rot.y;
	}
}
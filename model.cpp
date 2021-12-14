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
#include "setup.h"
#include "shadow.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_MOVE			(1.0f)			//�ړ��ʂ̍ő�l
#define MAX_ROTATION		(0.035f)		//��]�̍ő�l
#define MAX_ATTENUATION		(0.1f)			//�����W���̍ő�l
#define MAX_HEIGHT			(80.0f)			//�����̍ő�l
#define MIN_HEIGHT			(10.0f)			//�����̍ŏ��l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPD3DXMESH				s_pMesh = NULL;			// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER				s_pBuffMat = NULL;		// �}�e���A�����ւ̃|�C���^
static LPDIRECT3DTEXTURE9		*s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static DWORD					s_nNumMat = 0;			// �}�e���A�����̐�
static Model					s_model;				// ���f���̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitModel(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data\\MODEL\\������.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

	// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
	s_pTexture = new LPDIRECT3DTEXTURE9[s_nNumMat];

	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();

	// �e���b�V���̃}�e���A�������擾����
	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		s_pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_pTexture[i]);
		}
		else
		{
			s_pTexture[i] = NULL;
		}
	}

	s_model.pos = D3DXVECTOR3(50.0f, 20.0f, 0.0f);
	s_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_model.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�̐ݒ�
	s_model.nIdxShadow = SetShadow(s_model.pos, s_model.rot);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitModel(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		for (int i = 0; i < (int)s_nNumMat; i++)
		{
			if (s_pTexture[i] != NULL)
			{
				s_pTexture[i]->Release();
				s_pTexture[i] = NULL;
			}
		}

		delete[](s_pTexture);
		s_pTexture = NULL;
	}

	if (s_pMesh != NULL)
	{// ���b�V���̉��
		s_pMesh->Release();
		s_pMesh = NULL;
	}

	if (s_pBuffMat != NULL)
	{// �}�e���A���̉��
		s_pBuffMat->Release();
		s_pBuffMat = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateModel(void)
{

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

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[i]);

		// ���f���p�[�c�̕`��
		s_pMesh->DrawSubset(i);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Model *GetModel(void)
{
	return &s_model;
}

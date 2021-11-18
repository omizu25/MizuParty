//==================================================
// 
// 3D�Q�[������ ( model.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "model.h"

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPD3DXMESH		s_pMesh = NULL;			// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER		s_pBuffMat = NULL;		// �}�e���A�����ւ̃|�C���^
static DWORD			s_nNumMat = 0;			// �}�e���A�����̐�
static D3DXVECTOR3		s_pos;					// �ʒu
static D3DXVECTOR3		s_rot;					// ����
static D3DXMATRIX		s_mtxWorld;				// ���[���h�}�g���b�N�X

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitModel(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data\\������.x",
		/*"data\\�Ă�Ă�S����.x",*/
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

		s_pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		s_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	D3DXMatrixIdentity(&s_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_rot.y, s_rot.x, s_rot.z);
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_pos.x, s_pos.y, s_pos.z);
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_mtxWorld);

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

	// �ۑ����Ă����}�e���A����
	pDevice->SetMaterial(&matDef);
}
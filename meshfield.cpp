//==================================================
// 
// 3D�Q�[������ ( meshfield.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "meshfield.h"
#include "setup.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_WIDTH		(200.0f)		//���̍ő�l
#define MAX_HEIGHT		(0.0f)			//�����̍ő�l
#define MAX_DEPTH		(200.0f)		//���s���̍ő�l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�̃|�C���^
static MeshField					s_meshfield;			// ���b�V���t�B�[���h�̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitMeshField(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko000.jpg",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_meshfield.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_meshfield.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_meshfield.fWidth = MAX_WIDTH;
	s_meshfield.fHeight = MAX_HEIGHT;
	s_meshfield.fDepth = MAX_DEPTH;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, MAX_DEPTH);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, MAX_DEPTH);
	pVtx[4].pos = D3DXVECTOR3(MAX_WIDTH, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(MAX_WIDTH, 0.0f, MAX_DEPTH);
	pVtx[6].pos = D3DXVECTOR3(MAX_WIDTH, 0.0f, MAX_DEPTH);
	pVtx[7].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, -MAX_DEPTH);
	pVtx[8].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, -MAX_DEPTH);
	pVtx[9].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(0.0f, 0.0f, -MAX_DEPTH);
	pVtx[11].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[12].pos = D3DXVECTOR3(MAX_WIDTH, 0.0f, -MAX_DEPTH);
	pVtx[13].pos = D3DXVECTOR3(MAX_WIDTH, 0.0f, 0.0f);

	for (int i = 0; i < 14; i++)
	{
		// �e���_�̖@���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitMeshField(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMeshField(void)
{

}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawMeshField(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_meshfield.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_meshfield.rot.y, s_meshfield.rot.x, s_meshfield.rot.z);
	D3DXMatrixMultiply(&s_meshfield.mtxWorld, &s_meshfield.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_meshfield.pos.x, s_meshfield.pos.y, s_meshfield.pos.z);
	D3DXMatrixMultiply(&s_meshfield.mtxWorld, &s_meshfield.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_meshfield.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		12);						// �v���~�e�B�u(�|���S��)��

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
MeshField *GetMeshField(void)
{
	return &s_meshfield;
}
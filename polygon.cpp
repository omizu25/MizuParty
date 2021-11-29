//==================================================
// 
// 3D�Q�[������ ( polygon.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "polygon.h"
#include "setup.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_WIDTH		(100.0f)		//���̍ő�l
#define MAX_HEIGHT		(0.0f)			//�����̍ő�l
#define MAX_DEPTH		(100.0f)		//���s���̍ő�l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�̃|�C���^
static D3DXVECTOR3					s_pos;					// �ʒu
static D3DXVECTOR3					s_rot;					// ����
static D3DXMATRIX					s_mtxWorld;				// ���[���h�}�g���b�N�X

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitPolygon(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\�����f������.jpg",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_���W�̐ݒ�
	Setpos3D(pVtx, pos, MAX_WIDTH, MAX_HEIGHT, MAX_DEPTH);

	// �e���_�̖@���̐ݒ�
	Initnor3D(pVtx);

	// ���_�J���[�̐ݒ�
	Initcol3D(pVtx);

	// �e�N�X�`�����W�̐ݒ�
	Inittex3D(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitPolygon(void)
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
void UpdatePolygon(void)
{

}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawPolygon(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

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

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	// �|���S���̕`�� �l�p
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);
}
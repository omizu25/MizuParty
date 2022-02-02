//==================================================
// 
// 3D�Q�[������ ( number.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "setup.h"
#include "number.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_NUMBER		(256)		// ���̍ő吔
#define MAX_TEX			(10)		// tex�̍ő吔

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	float			fWidth;			// ��
	float			fHeight;		// ����
	int				nNumber;		// ��
	int				nDigit;			// ����
	USE				use;			// �g�p��
	bool			bUse;			// �g�p���Ă��邩�ǂ���
}Number;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�̃|�C���^
static Number						s_Number[MAX_NUMBER];		// ���̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitNumber(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\number000.png",
		&s_pTexture);

	// �������̃N���A
	memset(&s_Number, NULL, sizeof(s_Number));

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		// �S�Ă̏�����
		InitAll(pVtx);

		pVtx += 4;		// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitNumber(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̔j��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateNumber(void)
{
	
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawNumber(USE use)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		if (s_Number[i].bUse && s_Number[i].use == use)
		{// �����g�p����Ă���
			// �|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				i * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);							// �v���~�e�B�u(�|���S��)��
		}
	}
}

//--------------------------------------------------
// �ݒ� (�E�ɒ��S)
// 1.�ʒu 2.�� 3.���� 4.�� 5.���� 6.�g�p��
//--------------------------------------------------
void SetRightNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (pNumber->bUse)
		{// �����g�p����Ă���
			continue;
		}

		/*�� �����g�p����Ă��Ȃ� ��*/

		pNumber->pos = pos;
		pNumber->fWidth = fWidth;
		pNumber->fHeight = fHeight;
		pNumber->nNumber = nNumber;
		pNumber->nDigit = nDigit;
		pNumber->use = use;
		pNumber->bUse = true;

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// �Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos(pVtx, pos, fWidth, fHeight, SETPOS_RIGHT);

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;
		
		// �e�N�X�`�����W�̐ݒ�
		Settex(pVtx, 0.0f + fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// �ݒ� (�^�񒆂ɒ��S)
// 1.�ʒu 2.�� 3.���� 4.�� 5.���� 6.����
//--------------------------------------------------
void SetMiddleNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (pNumber->bUse)
		{// �����g�p����Ă���
			continue;
		}

		/*�� �����g�p����Ă��Ȃ� ��*/

		pNumber->pos = pos;
		pNumber->fWidth = fWidth;
		pNumber->fHeight = fHeight;
		pNumber->nNumber = nNumber;
		pNumber->nDigit = nDigit;
		pNumber->use = use;
		pNumber->bUse = true;

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// �Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

		// ���_�J���[�̐ݒ�
		Setcol(pVtx, D3DXCOLOR(0.25f, 1.0f, 0.5f, 1.0f));

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		// �e�N�X�`�����W�̐ݒ�
		Settex(pVtx, fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// �e�N�X�`��
//--------------------------------------------------
void TexNumber(int nNumber, int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (!pNumber->bUse || pNumber->nDigit != nDigit || pNumber->use != use)
		{// �����g�p����Ă��Ȃ��A�������Ⴄ
			continue;
		}

		/*�� �����g�p����Ă���A���������� ��*/

		pNumber->nNumber = nNumber;

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// �Y���̈ʒu�܂Ői�߂�

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		// �e�N�X�`�����W�̐ݒ�
		Settex(pVtx, fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
//���̎g�p����
//--------------------------------------------------
void UseNumber(int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_Number[i];

		if (!pNumber->bUse || pNumber->nDigit != nDigit || pNumber->use != use)
		{//�����g�p����Ă��Ȃ��A�������Ⴄ
			continue;
		}

		//�����g�p����Ă���A����������
		pNumber->bUse = false;

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// �Y���̈ʒu�܂Ői�߂�

		// ���_�J���[�̏�����
		Initcol(pVtx);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

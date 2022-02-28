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
}Number2D;

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	float			fWidth;			// ��
	float			fHeight;		// ����
	int				nNumber;		// ��
	int				nDigit;			// ����
	bool			bUse;			// �g�p���Ă��邩�ǂ���
}Number3D;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff2D = NULL;		// ���_�o�b�t�@�̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff3D = NULL;		// ���_�o�b�t�@�̃|�C���^
static Number2D						s_Number2D[MAX_NUMBER];		// ���̏�� [2D]
static Number3D						s_Number3D[MAX_NUMBER];		// ���̏�� [3D]

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void Init2D(void);
static void Init3D(void);

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
	memset(&s_Number2D, NULL, sizeof(s_Number2D));

	// �������̃N���A
	memset(&s_Number3D, NULL, sizeof(s_Number3D));

	// ������ [2D]
	Init2D();

	// ������ [3D]
	Init3D();
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

	if (s_pVtxBuff2D != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuff2D->Release();
		s_pVtxBuff2D = NULL;
	}

	if (s_pVtxBuff3D != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuff3D->Release();
		s_pVtxBuff3D = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateNumber(void)
{
	
}

//--------------------------------------------------
// �`�� [2D]
//--------------------------------------------------
void DrawNumber2D(USE use)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff2D, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		if (s_Number2D[i].bUse && s_Number2D[i].use == use)
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
// �`�� [3D]
//--------------------------------------------------
void DrawNumber3D(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff3D, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number3D *pNumber3D = &s_Number3D[i];

		if (!pNumber3D->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pNumber3D->mtxWorld);
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �J�����̋t�s���ݒ�
		pNumber3D->mtxWorld._11 = mtxView._11;
		pNumber3D->mtxWorld._12 = mtxView._21;
		pNumber3D->mtxWorld._13 = mtxView._31;
		pNumber3D->mtxWorld._21 = mtxView._12;
		pNumber3D->mtxWorld._22 = mtxView._22;
		pNumber3D->mtxWorld._23 = mtxView._32;
		pNumber3D->mtxWorld._31 = mtxView._13;
		pNumber3D->mtxWorld._32 = mtxView._23;
		pNumber3D->mtxWorld._33 = mtxView._33;

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pNumber3D->pos.x, pNumber3D->pos.y, pNumber3D->pos.z);
		D3DXMatrixMultiply(&pNumber3D->mtxWorld, &pNumber3D->mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pNumber3D->mtxWorld);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			i * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��
	}

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
// �ݒ� (�E�ɒ��S)
// 1.�ʒu 2.�� 3.���� 4.�� 5.����
//--------------------------------------------------
void SetNumber3D(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit)
{
	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number3D *pNumber3D = &s_Number3D[i];

		if (pNumber3D->bUse)
		{// �����g�p����Ă���
			continue;
		}

		/*�� �����g�p����Ă��Ȃ� ��*/

		pNumber3D->pos = pos;
		pNumber3D->fWidth = fWidth;
		pNumber3D->fHeight = fHeight;
		pNumber3D->nNumber = nNumber;
		pNumber3D->nDigit = nDigit;
		pNumber3D->bUse = true;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff3D->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// �Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f, SETPOS_MIDDLE);

		// ���_�̖@���̐ݒ�
		Setnor(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		// �e�N�X�`�����W�̐ݒ�
		Settex(pVtx, fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff3D->Unlock();

		break;
	}
}

//--------------------------------------------------
// �ݒ� (�E�ɒ��S)
// 1.�ʒu 2.�F 3.�� 4.���� 5.�� 6.���� 7.�g�p��
//--------------------------------------------------
void SetRightNumber(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber, int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number2D *pNumber2D = &s_Number2D[i];

		if (pNumber2D->bUse)
		{// �����g�p����Ă���
			continue;
		}

		/*�� �����g�p����Ă��Ȃ� ��*/

		pNumber2D->pos = pos;
		pNumber2D->fWidth = fWidth;
		pNumber2D->fHeight = fHeight;
		pNumber2D->nNumber = nNumber;
		pNumber2D->nDigit = nDigit;
		pNumber2D->use = use;
		pNumber2D->bUse = true;

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff2D->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// �Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos(pVtx, pos, fWidth, fHeight, SETPOS_RIGHT);

		// ���_�J���[�̐ݒ�
		Setcol(pVtx, col);

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;
		
		// �e�N�X�`�����W�̐ݒ�
		Settex(pVtx, 0.0f + fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff2D->Unlock();

		break;
	}
}

//--------------------------------------------------
// �ݒ� (�^�񒆂ɒ��S)
// 1.�ʒu 2.�F 3.�� 4.���� 5.�� 6.���� 7.����
//--------------------------------------------------
void SetMiddleNumber(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber, int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number2D *pNumber2D = &s_Number2D[i];

		if (pNumber2D->bUse)
		{// �����g�p����Ă���
			continue;
		}

		/*�� �����g�p����Ă��Ȃ� ��*/

		pNumber2D->pos = pos;
		pNumber2D->fWidth = fWidth;
		pNumber2D->fHeight = fHeight;
		pNumber2D->nNumber = nNumber;
		pNumber2D->nDigit = nDigit;
		pNumber2D->use = use;
		pNumber2D->bUse = true;

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff2D->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// �Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

		// ���_�J���[�̐ݒ�
		Setcol(pVtx, col);

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		// �e�N�X�`�����W�̐ݒ�
		Settex(pVtx, fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff2D->Unlock();

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
		Number2D *pNumber2D = &s_Number2D[i];

		if (!pNumber2D->bUse || pNumber2D->nDigit != nDigit || pNumber2D->use != use)
		{// �����g�p����Ă��Ȃ��A�������Ⴄ
			continue;
		}

		/*�� �����g�p����Ă���A���������� ��*/

		pNumber2D->nNumber = nNumber;

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff2D->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// �Y���̈ʒu�܂Ői�߂�

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		// �e�N�X�`�����W�̐ݒ�
		Settex(pVtx, fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff2D->Unlock();

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
		Number2D *pNumber2D = &s_Number2D[i];

		if (!pNumber2D->bUse || pNumber2D->nDigit != nDigit || pNumber2D->use != use)
		{//�����g�p����Ă��Ȃ��A�������Ⴄ
			continue;
		}

		//�����g�p����Ă���A����������
		pNumber2D->bUse = false;

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff2D->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// �Y���̈ʒu�܂Ői�߂�

		// ���_�J���[�̏�����
		Initcol(pVtx);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff2D->Unlock();
	}
}

//--------------------------------------------------
// ������ [2D]
//--------------------------------------------------
static void Init2D(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff2D,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		// �S�Ă̏�����
		InitAll(pVtx);

		pVtx += 4;		// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff2D->Unlock();
}

//--------------------------------------------------
// ������ [3D]
//--------------------------------------------------
static void Init3D(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff3D,
		NULL);

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff3D->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		//�S�Ă̏���������
		InitAll(pVtx);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff3D->Unlock();
}

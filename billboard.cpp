//==================================================
// 
// 3D�Q�[������ ( billboard.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "billboard.h"
#include "camera.h"
#include "main.h"
#include "setup.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define FILE_NAME		"data\\TXT\\billboard.txt"		//�t�@�C���̖��O
#define MAX_WALL				(256)					//�r���{�[�h�̍ő吔
#define MAX_TEXTURE				(256)					//�e�N�X�`���̍ő吔

//--------------------------------------------------
// �r���{�[�h�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	int				nIdxTex;		// �e�N�X�`���ԍ�
	bool			bUse;			// �g�p���Ă��邩�ǂ���
}Billboard;

//--------------------------------------------------
// �ǂݍ��ޓ��e�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;				// �ʒu
	float			fWidth;				// ��
	float			fHeight;			// ����
	int				nIdxTex;			// �e�N�X�`���ԍ�
}File;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			*s_pTexture;				// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�̃|�C���^
static Billboard					s_billboard[MAX_WALL];		// �r���{�[�h�̏��
static int							s_nMaxTex;					// �e�N�X�`���̍ő吔

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void SetFileBillboard(File *pFile);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitBillboard(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//�������̃N���A
	memset(s_billboard, NULL, sizeof(s_billboard));

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_WALL; i++)
	{
		//�S�Ă̏���������
		InitAll3D(pVtx);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitBillboard(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		for (int i = 0; i < s_nMaxTex; i++)
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

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateBillboard(void)
{
	
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawBillboard(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < MAX_WALL; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (!pBillboard->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pBillboard->mtxWorld);
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �J�����̋t�s���ݒ�
		pBillboard->mtxWorld._11 = mtxView._11;
		pBillboard->mtxWorld._12 = mtxView._21;
		pBillboard->mtxWorld._13 = mtxView._31;
		pBillboard->mtxWorld._21 = mtxView._12;
		pBillboard->mtxWorld._22 = mtxView._22;
		pBillboard->mtxWorld._23 = mtxView._32;
		pBillboard->mtxWorld._31 = mtxView._13;
		pBillboard->mtxWorld._32 = mtxView._23;
		pBillboard->mtxWorld._33 = mtxView._33;

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);
		D3DXMatrixMultiply(&pBillboard->mtxWorld, &pBillboard->mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pBillboard->mtxWorld);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[pBillboard->nIdxTex]);

		// �|���S���̕`�� �l�p
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			i * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��
	}

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

	// ���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	for (int i = 0; i < MAX_WALL; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (pBillboard->bUse)
		{//�g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		pBillboard->pos = pos;
		pBillboard->bUse = true;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		// ���_�̖@���̐ݒ�
		Setnor3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// �ݒu
//--------------------------------------------------
void InstallationBillboard(void)
{
	float fWidth = 10.0f;
	float fHeight = 10.0f;

	// �r���{�[�h�̐ݒ�
	SetBillboard(D3DXVECTOR3(25.0f, fHeight, 0.0f), fWidth, fHeight);
	SetBillboard(D3DXVECTOR3(-25.0f, fHeight, 0.0f), fWidth, fHeight);
	SetBillboard(D3DXVECTOR3(75.0f, fHeight, 0.0f), fWidth, fHeight);
	SetBillboard(D3DXVECTOR3(-75.0f, fHeight, 0.0f), fWidth, fHeight);
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void LoadBillboard(void)
{
	FILE *pFile;		// �t�@�C���|�C���^��錾
	File *file;

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fscanf(pFile, "%d", &s_nMaxTex);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}

	// txt�ɏ����Ă�ő吔���̓ǂݍ��ݗp�̔z���p�ӂ���
	file = new File[s_nMaxTex];

	char aTexture[MAX_TEXTURE][1024];

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fscanf(pFile, "%d", &s_nMaxTex);

		for (int i = 0; i < s_nMaxTex; i++)
		{
			fscanf(pFile, "%s", aTexture[i]);
		}

		for (int i = 0; i < s_nMaxTex; i++)
		{
			fscanf(pFile, "%d", &file[i].nIdxTex);
			fscanf(pFile, "%f", &file[i].fWidth);
			fscanf(pFile, "%f", &file[i].fHeight);
			fscanf(pFile, "%f", &file[i].pos.x);
			fscanf(pFile, "%f", &file[i].pos.y);
			fscanf(pFile, "%f", &file[i].pos.z);
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}

	// txt�ɏ����Ă�ő吔���̃e�N�X�`���̔z���p�ӂ���
	s_pTexture = new LPDIRECT3DTEXTURE9[s_nMaxTex];

	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < s_nMaxTex; i++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			&aTexture[i][0],
			&s_pTexture[i]);
	}

	for (int i = 0; i < s_nMaxTex; i++)
	{
		SetFileBillboard(&file[i]);
	}
}

//--------------------------------------------------
// �ǂݍ��ݐݒ�
//--------------------------------------------------
static void SetFileBillboard(File *pFile)
{
	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	for (int i = 0; i < MAX_WALL; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (pBillboard->bUse)
		{//�g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		pBillboard->pos = pFile->pos;
		pBillboard->nIdxTex = pFile->nIdxTex;
		pBillboard->bUse = true;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pFile->fWidth, pFile->fHeight, 0.0f);

		// ���_�̖@���̐ݒ�
		Setnor3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}
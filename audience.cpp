//==================================================
// 
// 3D�Q�[������ ( audience.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "game.h"
#include "input.h"
#include "audience.h"
#include "setup.h"
#include "shadow.h"
#include "title.h"

#include <stdio.h>
#include <assert.h>
#include <time.h>
//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
#define MAX_RANDOM		(3)		// �ϋq�̍ő吔
#define MIN_RANDOM		(1)		// �ϋq�̍ŏ���

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	LPD3DXMESH			pMesh;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		pBuffMat;				// �}�e���A�����ւ̃|�C���^
	DWORD				nNumMat;				// �}�e���A�����̐�
	char				aText[MAX_TEXT];		// ���f���t�@�C����
	bool				bLoad;					// �ǂݍ��݂��������ǂ���
}Text;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Audience		*s_pAudience;		// �ϋq�̏��
static Text			*s_pText;			// �e�L�X�g�̏��
static int			s_nNumAudience;		// �ϋq�̐�
static int			s_nNumFileName;		// �t�@�C�����̐�
static int			s_nLeft;			// �����̊ϋq�̐�
static int			s_nRight;			// �E���̊ϋq�̐�

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void System(char *pFileName);
static void LoadNew(char *pFileName);
static void LoadAudience(char *pFileName);
static void InitRandom(void);
static void RandomNum(void);
static void RandomUse(void);
static void RandomPos(void);
static void Collision(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitAudience(void)
{
	// �����_��
	InitRandom();

	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < s_nNumAudience; i++)
	{
		Audience *pAudience = &s_pAudience[i];

		int nNumVtx;		// ���_��
		DWORD SizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVexBuff;		// ���_�o�b�t�@�ւ̃|�C���^

		pAudience->vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		pAudience->vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		// ���_�����擾
		nNumVtx = pAudience->pMesh->GetNumVertices();

		// �t�H�[�}�b�g�̃T�C�Y���擾
		SizeFVF = D3DXGetFVFVertexSize(pAudience->pMesh->GetFVF());

		// ���_�o�b�t�@�̃��b�N
		pAudience->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

		for (int j = 0; j < nNumVtx; j++)
		{
			// ���_���̑��
			D3DXVECTOR3 pos = *(D3DXVECTOR3*)pVexBuff;

			// �������E�傫�� [x]
			VtxSmallBig(&pAudience->vtxMin.x, &pAudience->vtxMax.x, pos.x);

			// �������E�傫�� [y]
			VtxSmallBig(&pAudience->vtxMin.y, &pAudience->vtxMax.y, pos.y);

			// �������E�傫�� [z]
			VtxSmallBig(&pAudience->vtxMin.z, &pAudience->vtxMax.z, pos.z);

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVexBuff += SizeFVF;
		}

		// ���_�o�b�t�@�̃A�����b�N
		pAudience->pMesh->UnlockVertexBuffer();

		// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
		pAudience->pTexture = new LPDIRECT3DTEXTURE9[pAudience->nNumMat];

		// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)pAudience->pBuffMat->GetBufferPointer();

		// �e���b�V���̃}�e���A�������擾����
		for (int j = 0; j < (int)pAudience->nNumMat; j++)
		{
			pAudience->pTexture[j] = NULL;

			if (pMat[j].pTextureFilename != NULL)
			{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
				D3DXCreateTextureFromFileA(pDevice,
					pMat[j].pTextureFilename,
					&pAudience->pTexture[j]);
			}
			else
			{// �g�p����ĂȂ�
				pAudience->pTexture[j] = NULL;
			}
		}
		pAudience->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �e�̐ݒ�
		pAudience->nIdxShadow = SetShadow(pAudience->pos, pAudience->rot, pAudience->vtxMax.x);
	}

	// �����蔻��
	Collision();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitAudience(void)
{
	for (int i = 0; i < s_nNumAudience; i++)
	{
		Audience *pAudience = &s_pAudience[i];

		if (pAudience->pTexture != NULL)
		{// �e�N�X�`���̉��
			for (int j = 0; j < (int)pAudience->nNumMat; j++)
			{
				if (pAudience->pTexture[j] != NULL)
				{
					pAudience->pTexture[j]->Release();
					pAudience->pTexture[j] = NULL;
				}
			}

			delete[] pAudience->pTexture;
			pAudience->pTexture = NULL;
		}

		if (pAudience->pMesh != NULL)
		{// ���b�V���̉��
			pAudience->pMesh = NULL;
		}

		if (pAudience->pBuffMat != NULL)
		{// �}�e���A���̉��
			pAudience->pBuffMat = NULL;
		}
	}

	for (int i = 0; i < s_nNumFileName; i++)
	{
		Text *pText = &s_pText[i];

		if (pText->pMesh != NULL)
		{// ���b�V���̉��
			pText->pMesh->Release();
			pText->pMesh = NULL;
		}

		if (pText->pBuffMat != NULL)
		{// �}�e���A���̉��
			pText->pBuffMat->Release();
			pText->pBuffMat = NULL;
		}
	}

	// �ϋq�̊J��
	delete[] s_pAudience;
	s_pAudience = NULL;

	// �e�L�X�g�̊J��
	delete[] s_pText;
	s_pText = NULL;
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateAudience(void)
{
	
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawAudience(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

	for (int i = 0; i < s_nNumAudience; i++)
	{
		Audience *pAudience = &s_pAudience[i];

		if (!pAudience->bDraw)
		{// �`�悵�Ȃ�
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pAudience->mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pAudience->rot.y, pAudience->rot.x, pAudience->rot.z);
		D3DXMatrixMultiply(&pAudience->mtxWorld, &pAudience->mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pAudience->pos.x, pAudience->pos.y, pAudience->pos.z);
		D3DXMatrixMultiply(&pAudience->mtxWorld, &pAudience->mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pAudience->mtxWorld);

		// ���݂̃}�e���A���ێ�
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)pAudience->pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)pAudience->nNumMat; j++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[j].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pAudience->pTexture[j]);

			// ���f���p�[�c�̕`��
			pAudience->pMesh->DrawSubset(j);
		}

		// �ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);
	}
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void LoadAudience(void)
{
	// �V�X�e��
	char aFileName[MAX_TEXT];

	System(&aFileName[0]);

	// new�p�̓ǂݍ���
	LoadNew(&aFileName[0]);

	// �ϋq�̓ǂݍ���
	LoadAudience(&aFileName[0]);
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Audience *GetAudience(int nAudience)
{
	return &s_pAudience[nAudience];
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
int GetNumAudience(void)
{
	return s_nNumAudience;
}

//--------------------------------------------------
// �V�X�e��
//--------------------------------------------------
static void System(char *pFileName)
{
	FILE *pFile;		// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[MAX_TEXT] = {};
		
		while (strcmp(&aRead[0], "SCRIPT") != 0)
		{// �n�܂肪����܂ŌJ��Ԃ�
			fscanf(pFile, "%s", &aRead);
		}

		while (strcmp(&aRead[0], "END_SCRIPT") != 0)
		{// �I��肪����܂ŌJ��Ԃ�
			fscanf(pFile, "%s", &aRead);

			if (strncmp(&aRead[0], "#-", 2) == 0)
			{// �R�����g
				continue;
			}
			else if (strncmp(&aRead[0], "#", 1) == 0)
			{// �R�����g
				fgets(aRead, MAX_TEXT, pFile);
				continue;
			}

			if (strcmp(&aRead[0], "AUDIENCE_FILENAME") == 0)
			{// �v���C���[�̃t�@�C����
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", pFileName);
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//--------------------------------------------------
// new�p�̓ǂݍ���
//--------------------------------------------------
static void LoadNew(char *pFileName)
{
	FILE *pFile;		// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[MAX_TEXT] = {};
		s_nNumFileName = 0;

		while (strncmp(&aRead[0], "END_SCRIPT", 10) != 0)
		{// �I��肪����܂ŌJ��Ԃ�
			fgets(aRead, MAX_TEXT, pFile);

			if (strncmp(&aRead[0], "MODEL_FILENAME", 14) == 0)
			{// ���f���̏��
				s_nNumFileName++;
			}
		}

		// �t�@�C�������
		fclose(pFile);

		// �K�v�����̔z���p�ӂ���
		s_pText = new Text[s_nNumFileName];

		for (int i = 0; i < s_nNumFileName; i++)
		{// �e�L�X�g�̏��̏�����
			Text *pText = &s_pText[i];

			pText->pBuffMat = NULL;
			pText->pMesh = NULL;
			pText->nNumMat = 0;
			pText->bLoad = false;
		}
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//--------------------------------------------------
// �ϋq�̓ǂݍ���
//--------------------------------------------------
static void LoadAudience(char *pFileName)
{
	FILE *pFile;		// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[MAX_TEXT] = {};
		int nFileName = 0;

		while (strcmp(&aRead[0], "SCRIPT") != 0)
		{// �n�܂肪����܂ŌJ��Ԃ�
			fscanf(pFile, "%s", &aRead);
		}

		while (strcmp(&aRead[0], "END_SCRIPT") != 0)
		{// �I��肪����܂ŌJ��Ԃ�
			fscanf(pFile, "%s", &aRead);

			if (strncmp(&aRead[0], "#-", 2) == 0)
			{// �R�����g
				continue;
			}
			else if (strncmp(&aRead[0], "#", 1) == 0)
			{// �R�����g
				fgets(aRead, MAX_TEXT, pFile);
				continue;
			}
			
			if (strcmp(&aRead[0], "MODEL_FILENAME") == 0)
			{// ���f���t�@�C����
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", &s_pText[nFileName].aText);

				nFileName++;
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//--------------------------------------------------
// �����_��
//--------------------------------------------------
static void InitRandom(void)
{
	//���E�̎�q�̏�����
	srand((unsigned)time(NULL));

	// �g����
	RandomNum();

	// �g���ϋq
	RandomUse();

	// �ʒu
	RandomPos();
}

//--------------------------------------------------
// �g����
//--------------------------------------------------
static void RandomNum(void)
{
	s_nLeft = 0;
	s_nRight = 0;
	s_nNumAudience = 0;

	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_STOP:			// �~�߂�
		s_nLeft = (rand() % MAX_RANDOM) + MIN_RANDOM;
		s_nRight = (rand() % MAX_RANDOM) + MIN_RANDOM;

		s_nNumAudience = s_nLeft + s_nRight;

		break;

	case MENU_SLOPE:		// ��

		s_nNumAudience = (rand() % MAX_RANDOM) + MIN_RANDOM;

		break;

	default:
		assert(false);
		break;
	}

	// �K�v�����̔z���p�ӂ���
	s_pAudience = new Audience[s_nNumAudience];
}

//--------------------------------------------------
// �g���ϋq
//--------------------------------------------------
static void RandomUse(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < s_nNumAudience; i++)
	{
		Audience *pAudience = &s_pAudience[i];

		int nUse = (rand() % s_nNumFileName);

		Text *pText = &s_pText[nUse];

		if (!pText->bLoad)
		{// �ǂݍ��݂��܂����ĂȂ�
			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(
				pText->aText,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&pText->pBuffMat,
				NULL,
				&pText->nNumMat,
				&pText->pMesh);

			pText->bLoad = true;
		}

		// �g�p����ϋq�̏����擾
		pAudience->pBuffMat = pText->pBuffMat;
		pAudience->nNumMat = pText->nNumMat;
		pAudience->pMesh = pText->pMesh;

		pAudience->bDraw = true;
	}
}

//--------------------------------------------------
// �ʒu
//--------------------------------------------------
static void RandomPos(void)
{
	int nPosX = 0;
	float fRand = 0.0f;

	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
		
		for (int i = 0; i < s_nLeft; i++)
		{
			Audience *pAudience = &s_pAudience[i];

			nPosX = (30 * 75) / s_nLeft;
			fRand = (float)((rand() % nPosX) + (nPosX * i));
			fRand += (30.0f * 10.0f);

			pAudience->pos = D3DXVECTOR3(-fRand, 0.0f, 0.0f);
		}

		for (int i = 0; i < s_nRight; i++)
		{
			Audience *pAudience = &s_pAudience[i + s_nLeft];

			nPosX = (30 * 75) / s_nRight;
			fRand = (float)((rand() % nPosX) + (nPosX * i));
			fRand += (30.0f * 10.0f);

			pAudience->pos = D3DXVECTOR3(fRand, 0.0f, 0.0f);
		}

		break;

	case MENU_STOP:			// �~�߂�

		break;

	case MENU_SLOPE:		// ��

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �����蔻��
//--------------------------------------------------
static void Collision(void)
{
	for (int i = 0; i < (s_nNumAudience - 1); i++)
	{
		Audience *pAudience = &s_pAudience[i];

		float fAudienceLeft = pAudience->pos.x + pAudience->vtxMin.x;
		float fAudienceRight = pAudience->pos.x + pAudience->vtxMax.x;

		Audience *pNext = &s_pAudience[i + 1];

		float fNextLeft = pNext->pos.x + pNext->vtxMin.x;
		float fNextRight = pNext->pos.x + pNext->vtxMax.x;

		if (fAudienceLeft <= fNextRight &&
			fAudienceRight >= fNextLeft)
		{// �d�Ȃ��Ă�
			float fPosX = fAudienceRight + pNext->vtxMin.x;
			pNext->pos.x = fPosX + pAudience->vtxMax.x;
		}
	}
}

//==================================================
// 
// 3D�Q�[������ ( field.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "game.h"
#include "input.h"
#include "line.h"
#include "field.h"
#include "setup.h"
#include "shadow.h"
#include "particle.h"
#include "result.h"
#include "title.h"

#include <time.h>
//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define START_POS_Y		(300.0f)		// �X�^�[�g�̍���
#define START_POS_Z		(-15.0f)		// �X�^�[�g�̉��s��
#define MAX_MOVE		(5.0f)			// �ړ���
#define MAX_RANDOM		(10)			// �����_���̍ő�l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Field		s_field;			// ���f���̏��
static int			s_nRand;			// �����_��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitField(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;		// ���_��
	DWORD SizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVexBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data\\MODEL\\field.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_field.pBuffMat,
		NULL,
		&s_field.nNumMat,
		&s_field.pMesh);

	s_field.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_field.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// ���_�����擾
	nNumVtx = s_field.pMesh->GetNumVertices();

	// �t�H�[�}�b�g�̃T�C�Y���擾
	SizeFVF = D3DXGetFVFVertexSize(s_field.pMesh->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	s_field.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

	for (int i = 0; i < nNumVtx; i++)
	{
		// ���_���̑��
		D3DXVECTOR3 pos = *(D3DXVECTOR3*)pVexBuff;

		// �������E�傫�� [x]
		VtxSmallBig(&s_field.vtxMin.x, &s_field.vtxMax.x, pos.x);

		// �������E�傫�� [y]
		VtxSmallBig(&s_field.vtxMin.y, &s_field.vtxMax.y, pos.y);

		// �������E�傫�� [z]
		VtxSmallBig(&s_field.vtxMin.z, &s_field.vtxMax.z, pos.z);

		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVexBuff += SizeFVF;
	}

	// ���_�o�b�t�@�̃A�����b�N
	s_field.pMesh->UnlockVertexBuffer();

	// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
	s_field.pTexture = new LPDIRECT3DTEXTURE9[s_field.nNumMat];

	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_field.pBuffMat->GetBufferPointer();

	// �e���b�V���̃}�e���A�������擾����
	for (int i = 0; i < (int)s_field.nNumMat; i++)
	{
		s_field.pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_field.pTexture[i]);
		}
		else
		{
			s_field.pTexture[i] = NULL;
		}
	}

	s_field.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_field.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_field.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���E�̎�q�̏�����
	srand((unsigned)time(NULL));

	int nRand = (rand() % MAX_RANDOM);

	s_field.fMove = MAX_MOVE + (nRand * 0.1f);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitField(void)
{
	if (s_field.pTexture != NULL)
	{// �e�N�X�`���̉��
		for (int i = 0; i < (int)s_field.nNumMat; i++)
		{
			if (s_field.pTexture[i] != NULL)
			{
				s_field.pTexture[i]->Release();
				s_field.pTexture[i] = NULL;
			}
		}

		delete[](s_field.pTexture);
		s_field.pTexture = NULL;
	}

	if (s_field.pMesh != NULL)
	{// ���b�V���̉��
		s_field.pMesh->Release();
		s_field.pMesh = NULL;
	}

	if (s_field.pBuffMat != NULL)
	{// �}�e���A���̉��
		s_field.pBuffMat->Release();
		s_field.pBuffMat = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateField(void)
{
	
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawField(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_field.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_field.rot.y, s_field.rot.x, s_field.rot.z);
	D3DXMatrixMultiply(&s_field.mtxWorld, &s_field.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_field.pos.x, s_field.pos.y, s_field.pos.z);
	D3DXMatrixMultiply(&s_field.mtxWorld, &s_field.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_field.mtxWorld);

	// ���݂̃}�e���A���ێ�
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)s_field.pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)s_field.nNumMat; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_field.pTexture[i]);

		// ���f���p�[�c�̕`��
		s_field.pMesh->DrawSubset(i);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// �擾 (���f�����)
//--------------------------------------------------
Field *GetField(void)
{
	return &s_field;
}

//--------------------------------------------------
// �����蔻��
//--------------------------------------------------
void CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size)
{
	float fLeft = s_field.pos.x + s_field.vtxMin.x;
	float fRight = s_field.pos.x + s_field.vtxMax.x;
	float fBottom = s_field.pos.y + s_field.vtxMin.y;
	float fTop = s_field.pos.y + s_field.vtxMax.y;
	float fFront = s_field.pos.z + s_field.vtxMin.z;
	float fBack = s_field.pos.z + s_field.vtxMax.z;

	if ((pPos->x + size.x > fLeft) && (pPos->x - size.x < fRight) &&
		(pPos->y + size.y > fBottom) && (pPos->y < fTop))
	{// x, y���͈͓�
		if ((pPosOld->z + size.z <= fFront) && (pPos->z + size.z > fFront))
		{// �O
			pPos->z = fFront - size.z;
		}
		else if ((pPosOld->z - size.z >= fBack) && (pPos->z - size.z < fBack))
		{// ��
			pPos->z = fBack + size.z;
		}
	}

	if ((pPos->z + size.z > fFront) && (pPos->z - size.z < fBack) &&
		(pPos->y + size.y > fBottom) && (pPos->y < fTop))
	{// z, y���͈͓�
		if ((pPosOld->x + size.x <= fLeft) && (pPos->x + size.x > fLeft))
		{// ��
			pPos->x = fLeft - size.x;
		}
		else if ((pPosOld->x - size.x >= fRight) && (pPos->x - size.x < fRight))
		{// �E
			pPos->x = fRight + size.x;
		}
	}

	if ((pPos->x + size.x > fLeft) && (pPos->x - size.x < fRight) &&
		(pPos->z + size.z > fFront) && (pPos->z - size.z < fBack))
	{// x, z���͈͓�
		if ((pPosOld->y + size.y <= fBottom) && (pPos->y + size.y > fBottom))
		{// ��
			pPos->y = fBottom - size.y;
		}
		if ((pPosOld->y >= fTop) && (pPos->y < fTop))
		{// ��
			pPos->y = fTop;
		}
	}
}

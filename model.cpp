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
#include "camera.h"
#include "input.h"
#include "line.h"
#include "model.h"
#include "setup.h"
#include "shadow.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define START_POS_Y		(400.0f)		// �X�^�[�g�̍���	
#define MAX_MOVE		(3.0f)			// �ړ���

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Model		s_model;		// ���f���̏��
static int			s_nTime;		// �^�C��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitModel(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nTime = 0;

	int nNumVtx;		// ���_��
	DWORD SizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVexBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data\\MODEL\\�v����.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_model.pBuffMat,
		NULL,
		&s_model.nNumMat,
		&s_model.pMesh);

	s_model.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_model.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// ���_�����擾
	nNumVtx = s_model.pMesh->GetNumVertices();

	// �t�H�[�}�b�g�̃T�C�Y���擾
	SizeFVF = D3DXGetFVFVertexSize(s_model.pMesh->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	s_model.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

	for (int i = 0; i < nNumVtx; i++)
	{
		// ���_���̑��
		D3DXVECTOR3 pos = *(D3DXVECTOR3*)pVexBuff;

		// �������E�傫�� [x]
		VtxSmallBig(&s_model.vtxMin.x, &s_model.vtxMax.x, pos.x);

		// �������E�傫�� [y]
		VtxSmallBig(&s_model.vtxMin.y, &s_model.vtxMax.y, pos.y);

		// �������E�傫�� [z]
		VtxSmallBig(&s_model.vtxMin.z, &s_model.vtxMax.z, pos.z);

		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVexBuff += SizeFVF;
	}

	// ���_�o�b�t�@�̃A�����b�N
	s_model.pMesh->UnlockVertexBuffer();

	// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
	s_model.pTexture = new LPDIRECT3DTEXTURE9[s_model.nNumMat];

	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_model.pBuffMat->GetBufferPointer();

	// �e���b�V���̃}�e���A�������擾����
	for (int i = 0; i < (int)s_model.nNumMat; i++)
	{
		s_model.pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_model.pTexture[i]);
		}
		else
		{
			s_model.pTexture[i] = NULL;
		}
	}

	s_model.pos = D3DXVECTOR3(0.0f, START_POS_Y, 0.0f);
	s_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_model.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�̐ݒ�
	s_model.nIdxShadow = SetShadow(s_model.pos, s_model.rot, s_model.vtxMax.x);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitModel(void)
{
	if (s_model.pTexture != NULL)
	{// �e�N�X�`���̉��
		for (int i = 0; i < (int)s_model.nNumMat; i++)
		{
			if (s_model.pTexture[i] != NULL)
			{
				s_model.pTexture[i]->Release();
				s_model.pTexture[i] = NULL;
			}
		}

		delete[](s_model.pTexture);
		s_model.pTexture = NULL;
	}

	if (s_model.pMesh != NULL)
	{// ���b�V���̉��
		s_model.pMesh->Release();
		s_model.pMesh = NULL;
	}

	if (s_model.pBuffMat != NULL)
	{// �}�e���A���̉��
		s_model.pBuffMat->Release();
		s_model.pBuffMat = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateModel(void)
{
	s_model.pos.y -= MAX_MOVE;
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
	pMat = (D3DXMATERIAL*)s_model.pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)s_model.nNumMat; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_model.pTexture[i]);

		// ���f���p�[�c�̕`��
		s_model.pMesh->DrawSubset(i);
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

//--------------------------------------------------
// �����蔻��
//--------------------------------------------------
void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size)
{
	float fLeft = s_model.pos.x + s_model.vtxMin.x;
	float fRight = s_model.pos.x + s_model.vtxMax.x;
	float fBottom = s_model.pos.y + s_model.vtxMin.y;
	float fTop = s_model.pos.y + s_model.vtxMax.y;
	float fFront = s_model.pos.z + s_model.vtxMin.z;
	float fBack = s_model.pos.z + s_model.vtxMax.z;

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

//==================================================
// 
// 3D�Q�[������ ( player.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "camera.h"
#include "input.h"
#include "line.h"
#include "player.h"
#include "setup.h"
#include "shadow.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define FILE_NAME		"data\\TXT\\model.txt"		// �t�@�C���̖��O
#define MAX_MOVE				(1.0f)				//�ړ��ʂ̍ő�l
#define MAX_ROTATION			(0.035f)			//��]�̍ő�l
#define MAX_ATTENUATION			(0.1f)				//�����W���̍ő�l
#define MAX_HEIGHT				(80.0f)				//�����̍ő�l
#define MIN_HEIGHT				(0.0f)				//�����̍ŏ��l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Player		s_player;			// ���f���̏��
static int			s_nUsePlayer;		// �g�p����v���C���[�̐�

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void FollowMove(void);
static void Move(void);
static void Rot(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitPlayer(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;		// ���_��
	DWORD SizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVexBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data/MODEL/�Ă�Ă�S����.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_player.pBuffMat,
		NULL,
		&s_player.nNumMat,
		&s_player.pMesh);

	s_player.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_player.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// ���_�����擾
	nNumVtx = s_player.pMesh->GetNumVertices();

	// �t�H�[�}�b�g�̃T�C�Y���擾
	SizeFVF = D3DXGetFVFVertexSize(s_player.pMesh->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	s_player.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

	for (int i = 0; i < nNumVtx; i++)
	{
		// ���_���̑��
		D3DXVECTOR3 pos = *(D3DXVECTOR3*)pVexBuff;

		// �������E�傫�� [x]
		VtxSmallBig(&s_player.vtxMin.x, &s_player.vtxMax.x, pos.x);

		// �������E�傫�� [y]
		VtxSmallBig(&s_player.vtxMin.y, &s_player.vtxMax.y, pos.y);

		// �������E�傫�� [z]
		VtxSmallBig(&s_player.vtxMin.z, &s_player.vtxMax.z, pos.z);

		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVexBuff += SizeFVF;
	}

	// ���_�o�b�t�@�̃A�����b�N
	s_player.pMesh->UnlockVertexBuffer();

	// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
	s_player.pTexture = new LPDIRECT3DTEXTURE9[s_player.nNumMat];

	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_player.pBuffMat->GetBufferPointer();

	// �e���b�V���̃}�e���A�������擾����
	for (int i = 0; i < (int)s_player.nNumMat; i++)
	{
		s_player.pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_player.pTexture[i]);
		}
		else
		{
			s_player.pTexture[i] = NULL;
		}
	}

	s_player.pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	s_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_player.rotDest = s_player.rot;
	s_player.nStopTime = 0;

	// �e�̐ݒ�
	s_player.nIdxShadow = SetShadow(s_player.pos, s_player.rot);

	D3DXCOLOR col = D3DXCOLOR(0.615f, 0.215f, 0.341f, 1.0f);

	/*�� Y�_ ��*/

	D3DXVECTOR3 start = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMin.z);
	D3DXVECTOR3 end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMin.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMin.z);
	end = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMin.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMax.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMax.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);

	/*�� X�_ ��*/

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMax.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMin.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMin.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMax.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMin.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMin.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);

	/*�� Z�_ ��*/

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMin.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMin.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMin.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMin.z);

	// ���̐ݒ�
	SetLine(s_player.pos, s_player.rot, start, end, col);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitPlayer(void)
{
	if (s_player.pTexture != NULL)
	{// �e�N�X�`���̉��
		for (int i = 0; i < (int)s_player.nNumMat; i++)
		{
			if (s_player.pTexture[i] != NULL)
			{
				s_player.pTexture[i]->Release();
				s_player.pTexture[i] = NULL;
			}
		}

		delete[](s_player.pTexture);
		s_player.pTexture = NULL;
	}

	if (s_player.pMesh != NULL)
	{// ���b�V���̉��
		s_player.pMesh->Release();
		s_player.pMesh = NULL;
	}

	if (s_player.pBuffMat != NULL)
	{// �}�e���A���̉��
		s_player.pBuffMat->Release();
		s_player.pBuffMat = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdatePlayer(void)
{
	s_player.nStopTime++;

	Camera *pCamera = GetCamera();

	if (pCamera->bFollow)
	{// �Ǐ]����
		// �Ǐ]�̈ړ�
		FollowMove();
	}
	else
	{// �Ǐ]���Ȃ�
		// �ړ�
		Move();

		// ��]
		Rot();
	}

	float fAngle = s_player.rotDest.y - s_player.rot.y;

	// �p�x�̐��K��
	NormalizeRot(&fAngle);

	//�����E�������X�V (����������)
	s_player.rot.y += fAngle * MAX_ATTENUATION;

	// �p�x�̐��K��
	NormalizeRot(&s_player.rot.y);

	// �e�̈ʒu�̐ݒ�
	SetPosShadow(s_player.nIdxShadow, s_player.pos);
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawPlayer(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_player.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_player.rot.y, s_player.rot.x, s_player.rot.z);
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_player.mtxWorld);

	// ���݂̃}�e���A���ێ�
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)s_player.pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)s_player.nNumMat; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_player.pTexture[i]);

		// ���f���p�[�c�̕`��
		s_player.pMesh->DrawSubset(i);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player;
}

////--------------------------------------------------
//// �u���b�N�̓����蔻�菈��
////--------------------------------------------------
//void CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
//{
//		float fLeft = pBlock->pos.x - pBlock->fWidth;
//		float fRight = pBlock->pos.x + pBlock->fWidth;
//		float fTop = pBlock->pos.y - pBlock->fHeight;
//		float fBottom = pBlock->pos.y + pBlock->fHeight;
//
//		if ((pPos->x + fWidth > fLeft) && (pPos->x - fWidth < fRight))
//		{// X���u���b�N�͈͓̔�
//			if ((pPosOld->y <= fTop) && (pPos->y >= fTop))
//			{// �u���b�N�̉������
//				pPos->y = fTop;
//			}
//			else if ((pPosOld->y - fHeight >= fBottom) && (pPos->y - fHeight < fBottom))
//			{// �u���b�N�̏ォ�牺
//				pPos->y = fBottom + fHeight;
//				pMove->y = 0.0f;
//			}
//		}
//
//		if ((pPosOld->y > fTop) && (pPosOld->y - fHeight < fBottom))
//		{//�O���Y���u���b�N�͈͓̔�
//			if ((pPosOld->x + fWidth <= fLeft) && (pPos->x + fWidth > fLeft))
//			{//�u���b�N�̍��[
//				pPos->x = fLeft - fWidth;
//				pMove->x *= 0.5f;
//			}
//			else if ((pPosOld->x - fWidth >= fRight) && (pPos->x - fWidth < fRight))
//			{//�u���b�N�̉E�[
//				pPos->x = fRight + fWidth;
//				pMove->x *= 0.5f;
//			}
//		}
//	}
//}

//--------------------------------------------------
// �Ǐ]�̈ړ�
//--------------------------------------------------
static void FollowMove(void)
{
	if (GetDebug() != DEBUG_MESH)
	{// �f�o�b�O�\�������b�V���ł͂Ȃ���
		float fRot = 0.0f;

		/* �����f���̈ړ��� */

		if (GetKeyboardPress(DIK_LEFT))
		{// ���L�[�������ꂽ
			if (GetKeyboardPress(DIK_UP))
			{// ���L�[�������ꂽ
				fRot = -D3DX_PI * 0.25f;

				s_player.rotDest.y = D3DX_PI * 0.75f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = -D3DX_PI * 0.75f;

				s_player.rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{
				fRot = -D3DX_PI * 0.5f;

				s_player.rotDest.y = D3DX_PI * 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_RIGHT))
		{// ���L�[�������ꂽ
			if (GetKeyboardPress(DIK_UP))
			{// ���L�[�������ꂽ
				fRot = D3DX_PI * 0.25f;

				s_player.rotDest.y = -D3DX_PI * 0.75f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = D3DX_PI * 0.75f;

				s_player.rotDest.y = -D3DX_PI * 0.25f;
			}
			else
			{
				fRot = D3DX_PI * 0.5f;

				s_player.rotDest.y = -D3DX_PI * 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = 0.0f;

			s_player.rotDest.y = D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
			fRot = D3DX_PI;

			s_player.rotDest.y = 0.0f;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// ��, ��, ��, ���L�[�������ꂽ
			s_player.pos.x += sinf(fRot) * MAX_MOVE;
			s_player.pos.z += cosf(fRot) * MAX_MOVE;

			s_player.nStopTime = 0;
		}

		if (GetKeyboardPress(DIK_I))
		{// I�L�[�������ꂽ
			s_player.pos.y += sinf(D3DX_PI * 0.5f) * MAX_MOVE;

			s_player.nStopTime = 0;
		}
		else if (GetKeyboardPress(DIK_K))
		{// K�L�[�������ꂽ
			s_player.pos.y += sinf(-D3DX_PI * 0.5f) * MAX_MOVE;

			s_player.nStopTime = 0;
		}

		// �w��̒l�ȏ�E�ȉ�
		Specified(&s_player.pos.y, MAX_HEIGHT, MIN_HEIGHT);
	}
}

//--------------------------------------------------
// �ړ�
//--------------------------------------------------
static void Move(void)
{
	if (GetDebug() != DEBUG_MESH)
	{// �f�o�b�O�\�������b�V���ł͂Ȃ���

		Camera *pCamera = GetCamera();		//�J�����̎擾
		float fRot = 0.0f;

		/* �����f���̈ړ��� */

		if (GetKeyboardPress(DIK_LEFT))
		{// ���L�[�������ꂽ
			if (GetKeyboardPress(DIK_UP))
			{// ���L�[�������ꂽ
				fRot = pCamera->rot.y + (-D3DX_PI * 0.25f);

				s_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = pCamera->rot.y + (-D3DX_PI * 0.75f);

				s_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.25f);
			}
			else
			{
				fRot = pCamera->rot.y + (-D3DX_PI * 0.5f);

				s_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_RIGHT))
		{// ���L�[�������ꂽ
			if (GetKeyboardPress(DIK_UP))
			{// ���L�[�������ꂽ
				fRot = pCamera->rot.y + (D3DX_PI * 0.25f);

				s_player.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = pCamera->rot.y + (D3DX_PI * 0.75f);

				s_player.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.25f);
			}
			else
			{
				fRot = pCamera->rot.y + (D3DX_PI * 0.5f);

				s_player.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y;

			s_player.rotDest.y = pCamera->rot.y + D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + D3DX_PI;

			s_player.rotDest.y = pCamera->rot.y;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// ��, ��, ��, ���L�[�������ꂽ
			s_player.pos.x += sinf(fRot) * MAX_MOVE;
			s_player.pos.z += cosf(fRot) * MAX_MOVE;
		}

		if (GetKeyboardPress(DIK_I))
		{// I�L�[�������ꂽ
			s_player.pos.y += sinf(D3DX_PI * 0.5f) * MAX_MOVE;
		}
		else if (GetKeyboardPress(DIK_K))
		{// K�L�[�������ꂽ
			s_player.pos.y += sinf(-D3DX_PI * 0.5f) * MAX_MOVE;
		}

		// �w��̒l�ȏ�E�ȉ�
		Specified(&s_player.pos.y, MAX_HEIGHT, MIN_HEIGHT);
	}
}

//--------------------------------------------------
// ��]
//--------------------------------------------------
static void Rot(void)
{
	if (GetDebug() != DEBUG_MESH)
	{// �f�o�b�O�\�������b�V���ł͂Ȃ���

		/* �����f���̉�]�� */

		if (GetKeyboardPress(DIK_LSHIFT))
		{// ���V�t�g�L�[�������ꂽ
			s_player.rotDest.y += -MAX_ROTATION;
		}
		else if (GetKeyboardPress(DIK_RSHIFT))
		{// �E�V�t�g�L�[�������ꂽ
			s_player.rotDest.y += MAX_ROTATION;
		}
	}
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void LoadPlayer(HWND hWnd)
{
	//FILE *pFile;			// �t�@�C���|�C���^��錾
	//int nUseText = 0;		// �e�L�X�g�œǂݍ��񂾃r���{�[�h�̎g�p��

	//char aTexture[MAX_MODEL][1024];

	//// �t�@�C�����J��
	//pFile = fopen(FILE_NAME, "r");

	//if (pFile != NULL)
	//{// �t�@�C�����J�����ꍇ
	//	char aRead[256] = {};
	//	int nTex = 0, nText = 0;

	//	while (strcmp(&aRead[0], "SCRIPT") != 0)
	//	{// �n�܂肪����܂ŌJ��Ԃ�
	//		fscanf(pFile, "%s", &aRead);
	//	}

	//	while (strcmp(&aRead[0], "END_SCRIPT") != 0)
	//	{// �I��肪����܂ŌJ��Ԃ�
	//		fscanf(pFile, "%s", &aRead);

	//		if (strncmp(&aRead[0], "#-", 2) == 0)
	//		{// �R�����g
	//			continue;
	//		}
	//		else if (strncmp(&aRead[0], "#", 1) == 0)
	//		{// �R�����g
	//			fscanf(pFile, "%s", &aRead);
	//			continue;
	//		}

	//		if (strcmp(&aRead[0], "NUM_MODEL") == 0)
	//		{// �e�N�X�`���̎g�p��
	//			fscanf(pFile, "%d", &s_nUsePlayer);
	//		}
	//		else if (strcmp(&aRead[0], "MODEL_FILENAME") == 0)
	//		{// �e�N�X�`���̏��
	//			fscanf(pFile, "%s", aTexture[nTex]);
	//			nTex++;
	//		}
	//		else if (strcmp(&aRead[0], "NUM_MODEL") == 0)
	//		{// �r���{�[�h�̎g�p��
	//			fscanf(pFile, "%d", &nUseText);

	//			// txt�ɏ����Ă�ő吔���̓ǂݍ��ݗp�̔z���p�ӂ���
	//			//s_player = new s_player[nUseText];
	//		}
	//		else if (strcmp(&aRead[0], "BILLBOARD_SET") == 0)
	//		{// �r���{�[�h�̏��
	//			while (strcmp(&aRead[0], "END_BILLBOARD_SET") != 0)
	//			{// �I��肪����܂ŌJ��Ԃ�
	//				fscanf(pFile, "%s", &aRead);

	//				if (strncmp(&aRead[0], "#", 1) == 0)
	//				{// �R�����g
	//					fscanf(pFile, "%s", &aRead);
	//					continue;
	//				}

	//				if (strcmp(&aRead[0], "INDEX") == 0)
	//				{// ���f���ԍ�
	//					fscanf(pFile, "%d", &s_player[nText].nIdxModel);
	//				}
	//				else if (strcmp(&aRead[0], "POS") == 0)
	//				{// �ʒu
	//					fscanf(pFile, "%f", &s_player[nText].pos.x);
	//					fscanf(pFile, "%f", &s_player[nText].pos.y);
	//					fscanf(pFile, "%f", &s_player[nText].pos.z);
	//				}
	//				else if (strcmp(&aRead[0], "ROT") == 0)
	//				{// ����
	//					fscanf(pFile, "%f", &s_player[nText].rot.x);
	//					fscanf(pFile, "%f", &s_player[nText].rot.y);
	//					fscanf(pFile, "%f", &s_player[nText].rot.z);
	//				}
	//			}
	//			nText++;
	//		}
	//	}

	//	// �t�@�C�������
	//	fclose(pFile);
	//}
	//else
	//{// �t�@�C�����J���Ȃ��ꍇ
	//	MessageBox(hWnd, "�e�L�X�g�t�@�C���̓ǂݍ��݂Ɏ��s�I\n�G���[�ꏊ  : [ �r���{�[�h ]", "�x���I", MB_ICONWARNING);
	//	assert(false);
	//}

	//for (int i = 0; i < nUseText; i++)
	//{
	//	if (s_player[i].nIdxModel >= s_nUsePlayer)
	//	{// �Y�����Ȃ��e�N�X�`���ԍ�
	//		MessageBox(hWnd, "�Y�����Ȃ��e�N�X�`���ԍ��ł��I\n�G���[�ꏊ  : [ �r���{�[�h ]", "�x���I", MB_ICONWARNING);
	//		assert(false);
	//	}
	//}

	//// �f�o�C�X�ւ̃|�C���^�̎擾
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//for (int i = 0; i < s_nUsePlayer; i++)
	//{
	//	// �e�N�X�`���̓ǂݍ���
	//	D3DXCreateTextureFromFile(
	//		pDevice,
	//		&aTexture[i][0],
	//		&s_pTexture[i]);
	//}

	//for (int i = 0; i < nUseText; i++)
	//{
	//	pText[i].pTexture = s_pTexture[pText[i].nTexIdx];

	//	bool bYRot = true;

	//	if (pText[i].nYRot == DO_NOT_ROT_Y)
	//	{// Y����]�����Ȃ����l�̎�
	//		bYRot = false;
	//	}

	//	// �ݒ�
	//	SetBillboard(pText[i].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pText[i].fWidth, pText[i].fHeight, bYRot, &pText[i].pTexture);
	//}
}

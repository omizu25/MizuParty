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
#include "model.h"
#include "billboard.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "setup.h"
#include "shadow.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define FILE_NAME		"data\\TXT\\model.txt"		// �t�@�C���̖��O
#define MAX_MOVE				(1.0f)				// �ړ��ʂ̍ő�l
#define MAX_ROTATION			(0.035f)			// ��]�̍ő�l
#define MAX_ATTENUATION			(0.1f)				// �����W���̍ő�l
#define MAX_HEIGHT				(80.0f)				// �����̍ő�l
#define MIN_HEIGHT				(0.0f)				// �����̍ŏ��l

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
		"data/MODEL/�Ă�Ă��.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_player.parts[0].pBuffMat,
		NULL,
		&s_player.parts[0].nNumMat,
		&s_player.parts[0].pMesh);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data/MODEL/�Ă�Ă铪.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_player.parts[1].pBuffMat,
		NULL,
		&s_player.parts[1].nNumMat,
		&s_player.parts[1].pMesh);

	s_player.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_player.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// ���_�����擾
	nNumVtx = s_player.parts[0].pMesh->GetNumVertices();

	// �t�H�[�}�b�g�̃T�C�Y���擾
	SizeFVF = D3DXGetFVFVertexSize(s_player.parts[0].pMesh->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	s_player.parts[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

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
	s_player.parts[0].pMesh->UnlockVertexBuffer();

	for (int i = 0; i < MAX_PARTS; i++)
	{
		// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
		s_player.parts[i].pTexture = new LPDIRECT3DTEXTURE9[s_player.parts[i].nNumMat];

		// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_player.parts[i].pBuffMat->GetBufferPointer();

		// �e���b�V���̃}�e���A�������擾����
		for (int j = 0; j < (int)s_player.parts[i].nNumMat; j++)
		{
			s_player.parts[i].pTexture[j] = NULL;

			if (pMat[j].pTextureFilename != NULL)
			{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
				D3DXCreateTextureFromFileA(pDevice,
					pMat[j].pTextureFilename,
					&s_player.parts[i].pTexture[j]);
			}
			else
			{
				s_player.parts[i].pTexture[j] = NULL;
			}
		}

		s_player.pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		s_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_player.rotDest = s_player.rot;
		s_player.nStopTime = 0;

		// �e�̐ݒ�
		s_player.nIdxShadow = SetShadow(s_player.pos, s_player.rot);
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitPlayer(void)
{
	for (int i = 0; i < MAX_PARTS; i++)
	{
		if (s_player.parts[i].pTexture != NULL)
		{// �e�N�X�`���̉��
			for (int j = 0; j < (int)s_player.parts[i].nNumMat; j++)
			{
				if (s_player.parts[i].pTexture[j] != NULL)
				{
					s_player.parts[i].pTexture[j]->Release();
					s_player.parts[i].pTexture[j] = NULL;
				}
			}

			delete[](s_player.parts[i].pTexture);
			s_player.parts[i].pTexture = NULL;
		}
	}

	for (int i = 0; i < MAX_PARTS; i++)
	{
		if (s_player.parts[i].pMesh != NULL)
		{// ���b�V���̉��
			s_player.parts[i].pMesh->Release();
			s_player.parts[i].pMesh = NULL;
		}
	}

	for (int i = 0; i < MAX_PARTS; i++)
	{
		if (s_player.parts[i].pBuffMat != NULL)
		{// �}�e���A���̉��
			s_player.parts[i].pBuffMat->Release();
			s_player.parts[i].pBuffMat = NULL;
		}
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdatePlayer(void)
{
	s_player.nStopTime++;

	Camera *pCamera = GetCamera();

	// �O��̈ʒu��ۑ�
	s_player.posOld = s_player.pos;

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

	// ���f���Ƃ̓����蔻��
	CollisionModel(&s_player.pos, &s_player.posOld, s_player.vtxMin.z, s_player.vtxMin.z);

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

	for (int i = 0; i < MAX_PARTS; i++)
	{
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
		pMat = (D3DXMATERIAL*)s_player.parts[i].pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)s_player.parts[i].nNumMat; j++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[j].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_player.parts[i].pTexture[j]);

			// ���f���p�[�c�̕`��
			s_player.parts[i].pMesh->DrawSubset(j);
		}

		// �ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player;
}

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

	//	while (strcmp(&aRead, "SCRIPT") != 0)
	//	{// �n�܂肪����܂ŌJ��Ԃ�
	//		fscanf(pFile, "%s", &aRead);
	//	}

	//	while (strcmp(&aRead, "END_SCRIPT") != 0)
	//	{// �I��肪����܂ŌJ��Ԃ�
	//		fscanf(pFile, "%s", &aRead);

	//		if (strncmp(&aRead, "#-", 2) == 0)
	//		{// �R�����g
	//			continue;
	//		}
	//		else if (strncmp(&aRead, "#", 1) == 0)
	//		{// �R�����g
	//			fscanf(pFile, "%s", &aRead);
	//			continue;
	//		}

	//		if (strcmp(&aRead, "NUM_MODEL") == 0)
	//		{// �e�N�X�`���̎g�p��
	//			fscanf(pFile, "%d", &s_nUsePlayer);
	//		}
	//		else if (strcmp(&aRead, "MODEL_FILENAME") == 0)
	//		{// �e�N�X�`���̏��
	//			fscanf(pFile, "%s", aTexture[nTex]);
	//			nTex++;
	//		}
	//		else if (strcmp(&aRead, "NUM_MODEL") == 0)
	//		{// �r���{�[�h�̎g�p��
	//			fscanf(pFile, "%d", &nUseText);

	//			// txt�ɏ����Ă�ő吔���̓ǂݍ��ݗp�̔z���p�ӂ���
	//			//s_player = new s_player[nUseText];
	//		}
	//		else if (strcmp(&aRead, "BILLBOARD_SET") == 0)
	//		{// �r���{�[�h�̏��
	//			while (strcmp(&aRead, "END_BILLBOARD_SET") != 0)
	//			{// �I��肪����܂ŌJ��Ԃ�
	//				fscanf(pFile, "%s", &aRead);

	//				if (strncmp(&aRead, "#", 1) == 0)
	//				{// �R�����g
	//					fscanf(pFile, "%s", &aRead);
	//					continue;
	//				}

	//				if (strcmp(&aRead, "INDEX") == 0)
	//				{// ���f���ԍ�
	//					fscanf(pFile, "%d", &s_player[nText].nIdxModel);
	//				}
	//				else if (strcmp(&aRead, "POS") == 0)
	//				{// �ʒu
	//					fscanf(pFile, "%f", &s_player[nText].pos.x);
	//					fscanf(pFile, "%f", &s_player[nText].pos.y);
	//					fscanf(pFile, "%f", &s_player[nText].pos.z);
	//				}
	//				else if (strcmp(&aRead, "ROT") == 0)
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
	//		&aTexture[i],
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

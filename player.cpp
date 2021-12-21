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
#define MAX_MOVE			(1.0f)			// �ړ��ʂ̍ő�l
#define MAX_ROTATION		(0.035f)		// ��]�̍ő�l
#define MAX_ATTENUATION		(0.1f)			// �����W���̍ő�l
#define MAX_HEIGHT			(80.0f)			// �����̍ő�l
#define MIN_HEIGHT			(0.0f)			// �����̍ŏ��l
#define IDX_PARENT			(-1)			// �e�̔ԍ�

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Player		*s_player;				// ���f���̏��
static int			s_nUsePlayer;			// �g�p����v���C���[�̐�
static int			s_nSelectPlayer;		// �I�΂�Ă���v���C���[
static int			s_nSelectParts;			// �I�΂�Ă���p�[�c

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void FollowMove(Player *pPlayer);
static void Move(Player *pPlayer);
static void Rot(Player *pPlayer);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitPlayer(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < s_nUsePlayer; i++)
	{
		Player *pPlayer = &s_player[i];

		pPlayer->vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		pPlayer->vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		int nParent = 0;		// �e�̔ԍ�

		for (int j = 0; j < pPlayer->nUseParts; j++)
		{
			if (pPlayer->parts[j].nIdxParent == IDX_PARENT)
			{// �e
				nParent = j;
			}
		}

		int nNumVtx;		// ���_��
		DWORD SizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVexBuff;		// ���_�o�b�t�@�ւ̃|�C���^

		// ���_�����擾
		nNumVtx = pPlayer->parts[nParent].pMesh->GetNumVertices();

		// �t�H�[�}�b�g�̃T�C�Y���擾
		SizeFVF = D3DXGetFVFVertexSize(pPlayer->parts[nParent].pMesh->GetFVF());

		// ���_�o�b�t�@�̃��b�N
		pPlayer->parts[nParent].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

		for (int j = 0; j < nNumVtx; j++)
		{
			// ���_���̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVexBuff;

			// �������E�傫�� [x]
			VtxSmallBig(&pPlayer->vtxMin.x, &pPlayer->vtxMax.x, vtx.x);

			// �������E�傫�� [y]
			VtxSmallBig(&pPlayer->vtxMin.y, &pPlayer->vtxMax.y, vtx.y);

			// �������E�傫�� [z]
			VtxSmallBig(&pPlayer->vtxMin.z, &pPlayer->vtxMax.z, vtx.z);

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVexBuff += SizeFVF;
		}

		// ���_�o�b�t�@�̃A�����b�N
		pPlayer->parts[nParent].pMesh->UnlockVertexBuffer();

		for (int j = 0; j < pPlayer->nUseParts; j++)
		{
			PlayerParts *pParts = &pPlayer->parts[j];

			// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
			pParts->pTexture = new LPDIRECT3DTEXTURE9[pParts->nNumMat];

			// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
			D3DXMATERIAL *pMat = (D3DXMATERIAL*)pParts->pBuffMat->GetBufferPointer();

			// �e���b�V���̃}�e���A�������擾����
			for (int k = 0; k < (int)pParts->nNumMat; k++)
			{
				pParts->pTexture[k] = NULL;

				if (pMat[k].pTextureFilename != NULL)
				{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
					D3DXCreateTextureFromFileA(pDevice,
						pMat[k].pTextureFilename,
						&pParts->pTexture[k]);
				}
				else
				{// �g�p����ĂȂ�
					pParts->pTexture[k] = NULL;
				}
			}
		}

		pPlayer->posOld = pPlayer->pos;
		pPlayer->rotDest = pPlayer->rot;
		pPlayer->nStopTime = 0;

		// �e�̐ݒ�
		pPlayer->nIdxShadow = SetShadow(pPlayer->pos, pPlayer->rot);
	}

	s_nSelectPlayer = 0;
	s_nSelectParts = 0;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitPlayer(void)
{
	for (int i = 0; i < s_nUsePlayer; i++)
	{
		for (int j = 0; j < s_player[i].nUseParts; j++)
		{
			PlayerParts *pParts = &s_player[i].parts[j];

			if (pParts->pTexture != NULL)
			{// �e�N�X�`���̉��
				for (int k = 0; k < (int)pParts->nNumMat; k++)
				{
					if (pParts->pTexture[k] != NULL)
					{
						pParts->pTexture[k]->Release();
						pParts->pTexture[k] = NULL;
					}
				}

				delete[](pParts->pTexture);
				pParts->pTexture = NULL;
			}
		}
	}

	for (int i = 0; i < s_nUsePlayer; i++)
	{
		for (int j = 0; j < s_player[i].nUseParts; j++)
		{
			PlayerParts *pParts = &s_player[i].parts[j];

			if (pParts->pMesh != NULL)
			{// ���b�V���̉��
				pParts->pMesh->Release();
				pParts->pMesh = NULL;
			}

			if (pParts->pBuffMat != NULL)
			{// �}�e���A���̉��
				pParts->pBuffMat->Release();
				pParts->pBuffMat = NULL;
			}
		}
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdatePlayer(void)
{
	Player *pPlayer = &s_player[s_nSelectPlayer];

	pPlayer->nStopTime++;

	Camera *pCamera = GetCamera();

	// �O��̈ʒu��ۑ�
	pPlayer->posOld = pPlayer->pos;

	if (pCamera->bFollow)
	{// �Ǐ]����
		// �Ǐ]�̈ړ�
		FollowMove(pPlayer);
	}
	else
	{// �Ǐ]���Ȃ�
		// �ړ�
		Move(pPlayer);

		// ��]
		Rot(pPlayer);
	}

	float fAngle = pPlayer->rotDest.y - pPlayer->rot.y;

	// �p�x�̐��K��
	NormalizeRot(&fAngle);

	//�����E�������X�V (����������)
	pPlayer->rot.y += fAngle * MAX_ATTENUATION;

	// �p�x�̐��K��
	NormalizeRot(&pPlayer->rot.y);

	// ���f���Ƃ̓����蔻��
	CollisionModel(&pPlayer->pos, &pPlayer->posOld, pPlayer->vtxMin.z, pPlayer->vtxMin.z);

	// �e�̈ʒu�̐ݒ�
	SetPosShadow(pPlayer->nIdxShadow, pPlayer->pos);
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

	for (int i = 0; i < s_nUsePlayer; i++)
	{
		Player *pPlayer = &s_player[i];

		for (int j = 0; j < pPlayer->nUseParts; j++)
		{
			PlayerParts *pParts = &pPlayer->parts[j];

			// �p�[�c�̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pParts->mtxWorld);

			// �p�[�c�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rot.y, pParts->rot.x, pParts->rot.z);
			D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxRot);

			// �p�[�c�̈ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pParts->pos.x, pParts->pos.y, pParts->pos.z);
			D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxTrans);

			D3DXMATRIX mtxParent;

			if (pParts->nIdxParent == IDX_PARENT)
			{// �e
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&pPlayer->mtxWorld);

				// �����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, pPlayer->rot.y, pPlayer->rot.x, pPlayer->rot.z);
				D3DXMatrixMultiply(&pPlayer->mtxWorld, &pPlayer->mtxWorld, &mtxRot);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
				D3DXMatrixMultiply(&pPlayer->mtxWorld, &pPlayer->mtxWorld, &mtxTrans);

				mtxParent = pPlayer->mtxWorld;
			}
			else
			{// �q
				mtxParent = pPlayer->parts[pParts->nIdxParent].mtxWorld;
			}

			// �e���f���Ƃ̃}�g���b�N�X�̊|���Z
			D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxParent);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pParts->mtxWorld);

			// ���݂̃}�e���A���ێ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)pParts->pBuffMat->GetBufferPointer();

			for (int k = 0; k < (int)pParts->nNumMat; k++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[k].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, pParts->pTexture[k]);

				// �p�[�c�̕`��
				pParts->pMesh->DrawSubset(k);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);
		}
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Player *GetPlayer(void)
{
	return s_player;
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void LoadPlayer(HWND hWnd)
{
	FILE *pFile;		// �t�@�C���|�C���^��錾

	char aText[1024];
	int nPlayer = 0;

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[256] = {};

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
				fscanf(pFile, "%s", &aRead);
				continue;
			}

			if (strcmp(&aRead[0], "NUM_PLAYER") == 0)
			{// �p�[�c�̎g�p��
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%d", &s_nUsePlayer);

				// txt�ɏ����Ă�ő吔���̃v���C���[�̔z���p�ӂ���
				s_player = new Player[s_nUsePlayer];
			}
			else if (strcmp(&aRead[0], "PLAYER_SET") == 0)
			{// ���f���̏��
				while (strcmp(&aRead[0], "END_PLAYER_SET") != 0)
				{// �I��肪����܂ŌJ��Ԃ�
					fscanf(pFile, "%s", &aRead);

					if (strncmp(&aRead[0], "#", 1) == 0)
					{// �R�����g
						fscanf(pFile, "%s", &aRead);
						continue;
					}

					if (strcmp(&aRead[0], "PLAYER_FILENAME") == 0)
					{// �v���C���[�̃t�@�C����
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%s", aText);
					}
					else if (strcmp(&aRead[0], "POS") == 0)
					{// �ʒu
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &s_player[nPlayer].pos.x);
						fscanf(pFile, "%f", &s_player[nPlayer].pos.y);
						fscanf(pFile, "%f", &s_player[nPlayer].pos.z);
					}
					else if (strcmp(&aRead[0], "ROT") == 0)
					{// ����
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &s_player[nPlayer].rot.x);
						fscanf(pFile, "%f", &s_player[nPlayer].rot.y);
						fscanf(pFile, "%f", &s_player[nPlayer].rot.z);
					}
				}
				nPlayer++;
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(hWnd, "�V�X�e���t�@�C���̓ǂݍ��݂Ɏ��s�I\n�G���[�ꏊ  : [ ���f�� ]", "�x���I", MB_ICONWARNING);
		assert(false);
	}

	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < nPlayer; i++)
	{
		// �t�@�C�����J��
		pFile = fopen(aText, "r");

		Player *pPlayer = &s_player[i];

		if (pFile != NULL)
		{// �t�@�C�����J�����ꍇ
			char aRead[256] = {};
			int nParts = 0, nFileName = 0;

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
					fscanf(pFile, "%s", &aRead);
					continue;
				}

				if (strcmp(&aRead[0], "NUM_MODEL") == 0)
				{// �p�[�c�̎g�p��
					fscanf(pFile, "%s", &aRead);
					fscanf(pFile, "%d", &pPlayer->nUseParts);

					// txt�ɏ����Ă�ő吔���̃p�[�c�̔z���p�ӂ���
					pPlayer->parts = new PlayerParts[pPlayer->nUseParts];
				}
				else if (strcmp(&aRead[0], "MODEL_FILENAME") == 0)
				{// ���f���t�@�C����
					char aParts[1024];

					fscanf(pFile, "%s", &aRead);
					fscanf(pFile, "%s", aParts);

					// X�t�@�C���̓ǂݍ���
					D3DXLoadMeshFromX(
						aParts,
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&pPlayer->parts[nFileName].pBuffMat,
						NULL,
						&pPlayer->parts[nFileName].nNumMat,
						&pPlayer->parts[nFileName].pMesh);

					nFileName++;
				}
				else if (strcmp(&aRead[0], "MODEL_SET") == 0)
				{// ���f���̏��
					while (strcmp(&aRead[0], "END_MODEL_SET") != 0)
					{// �I��肪����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aRead);

						if (strncmp(&aRead[0], "#", 1) == 0)
						{// �R�����g
							fscanf(pFile, "%s", &aRead);
							continue;
						}

						PlayerParts *pParts = &pPlayer->parts[nParts];

						if (strcmp(&aRead[0], "INDEX") == 0)
						{// �e�̔ԍ�
							fscanf(pFile, "%s", &aRead);
							fscanf(pFile, "%d", &pParts->nIdxParent);
						}
						else if (strcmp(&aRead[0], "POS") == 0)
						{// �ʒu
							fscanf(pFile, "%s", &aRead);
							fscanf(pFile, "%f", &pParts->pos.x);
							fscanf(pFile, "%f", &pParts->pos.y);
							fscanf(pFile, "%f", &pParts->pos.z);
						}
						else if (strcmp(&aRead[0], "ROT") == 0)
						{// ����
							fscanf(pFile, "%s", &aRead);
							fscanf(pFile, "%f", &pParts->rot.x);
							fscanf(pFile, "%f", &pParts->rot.y);
							fscanf(pFile, "%f", &pParts->rot.z);
						}
					}
					nParts++;
				}
			}

			// �t�@�C�������
			fclose(pFile);

			if (nParts != pPlayer->nUseParts)
			{// ���f�����ƃ��f���̏��̐����Ⴄ
				MessageBox(hWnd, "[ ���f���� ] �� [ ���f���̏�� ] �̐��������ĂȂ���I�I\n�G���[�ꏊ  : [ ���f�� ]", "�x���I", MB_ICONWARNING);
			}

			if (nFileName != pPlayer->nUseParts)
			{// ���f�����ƃ��f���̃t�@�C�����̐����Ⴄ
				MessageBox(hWnd, "[ ���f���� ] �� [ ���f���̃t�@�C���� ] �̐��������ĂȂ���I�I\n�G���[�ꏊ  : [ ���f�� ]", "�x���I", MB_ICONWARNING);
			}

			bool bParent = false;		// �e�����邩�ǂ���

			for (int j = 0; j < pPlayer->nUseParts; j++)
			{
				if (pPlayer->parts[j].nIdxParent == IDX_PARENT)
				{// �e����
					bParent = true;
				}
			}

			if (!bParent)
			{// �e�����Ȃ�
				MessageBox(hWnd, "�e�����Ȃ��A[ -1 ] �������Ă�I�I\n�G���[�ꏊ  : [ ���f�� ]", "�x���I", MB_ICONWARNING);
			}

			bParent = true;

			for (int j = 0; j < pPlayer->nUseParts; j++)
			{
				if (pPlayer->parts[j].nIdxParent >= pPlayer->nUseParts - 1)
				{// ����Ȑe�̔ԍ��͑��݂��Ȃ�
					bParent = false;
				}
			}

			if (!bParent)
			{// ����Ȑe�̔ԍ��͑��݂��Ȃ�
				MessageBox(hWnd, "����Ȑe�̔ԍ��͑��݂��Ȃ��I�I\n�G���[�ꏊ  : [ ���f�� ]", "�x���I", MB_ICONWARNING);
			}
		}
		else
		{// �t�@�C�����J���Ȃ��ꍇ
			MessageBox(hWnd, "�e�L�X�g�t�@�C���̓ǂݍ��݂Ɏ��s�I\n�G���[�ꏊ  : [ ���f�� ]", "�x���I", MB_ICONWARNING);
			assert(false);
		}
	}
}

//--------------------------------------------------
// �Ǐ]�̈ړ�
//--------------------------------------------------
static void FollowMove(Player *pPlayer)
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

				pPlayer->rotDest.y = D3DX_PI * 0.75f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = -D3DX_PI * 0.75f;

				pPlayer->rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{
				fRot = -D3DX_PI * 0.5f;

				pPlayer->rotDest.y = D3DX_PI * 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_RIGHT))
		{// ���L�[�������ꂽ
			if (GetKeyboardPress(DIK_UP))
			{// ���L�[�������ꂽ
				fRot = D3DX_PI * 0.25f;

				pPlayer->rotDest.y = -D3DX_PI * 0.75f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = D3DX_PI * 0.75f;

				pPlayer->rotDest.y = -D3DX_PI * 0.25f;
			}
			else
			{
				fRot = D3DX_PI * 0.5f;

				pPlayer->rotDest.y = -D3DX_PI * 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = 0.0f;

			pPlayer->rotDest.y = D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
			fRot = D3DX_PI;

			pPlayer->rotDest.y = 0.0f;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// ��, ��, ��, ���L�[�������ꂽ
			pPlayer->pos.x += sinf(fRot) * MAX_MOVE;
			pPlayer->pos.z += cosf(fRot) * MAX_MOVE;

			pPlayer->nStopTime = 0;
		}

		if (GetKeyboardPress(DIK_I))
		{// I�L�[�������ꂽ
			pPlayer->pos.y += sinf(D3DX_PI * 0.5f) * MAX_MOVE;

			pPlayer->nStopTime = 0;
		}
		else if (GetKeyboardPress(DIK_K))
		{// K�L�[�������ꂽ
			pPlayer->pos.y += sinf(-D3DX_PI * 0.5f) * MAX_MOVE;

			pPlayer->nStopTime = 0;
		}

		// �w��̒l�ȏ�E�ȉ�
		Specified(&pPlayer->pos.y, MAX_HEIGHT, MIN_HEIGHT);
	}
}

//--------------------------------------------------
// �ړ�
//--------------------------------------------------
static void Move(Player *pPlayer)
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

				pPlayer->rotDest.y = pCamera->rot.y + (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = pCamera->rot.y + (-D3DX_PI * 0.75f);

				pPlayer->rotDest.y = pCamera->rot.y + (D3DX_PI * 0.25f);
			}
			else
			{
				fRot = pCamera->rot.y + (-D3DX_PI * 0.5f);

				pPlayer->rotDest.y = pCamera->rot.y + (D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_RIGHT))
		{// ���L�[�������ꂽ
			if (GetKeyboardPress(DIK_UP))
			{// ���L�[�������ꂽ
				fRot = pCamera->rot.y + (D3DX_PI * 0.25f);

				pPlayer->rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = pCamera->rot.y + (D3DX_PI * 0.75f);

				pPlayer->rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.25f);
			}
			else
			{
				fRot = pCamera->rot.y + (D3DX_PI * 0.5f);

				pPlayer->rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y;

			pPlayer->rotDest.y = pCamera->rot.y + D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + D3DX_PI;

			pPlayer->rotDest.y = pCamera->rot.y;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// ��, ��, ��, ���L�[�������ꂽ
			pPlayer->pos.x += sinf(fRot) * MAX_MOVE;
			pPlayer->pos.z += cosf(fRot) * MAX_MOVE;
		}

		if (GetKeyboardPress(DIK_I))
		{// I�L�[�������ꂽ
			pPlayer->pos.y += sinf(D3DX_PI * 0.5f) * MAX_MOVE;
		}
		else if (GetKeyboardPress(DIK_K))
		{// K�L�[�������ꂽ
			pPlayer->pos.y += sinf(-D3DX_PI * 0.5f) * MAX_MOVE;
		}

		// �w��̒l�ȏ�E�ȉ�
		Specified(&pPlayer->pos.y, MAX_HEIGHT, MIN_HEIGHT);
	}
}

//--------------------------------------------------
// ��]
//--------------------------------------------------
static void Rot(Player *pPlayer)
{
	if (GetDebug() != DEBUG_MESH)
	{// �f�o�b�O�\�������b�V���ł͂Ȃ���

		/* �����f���̉�]�� */

		if (GetKeyboardPress(DIK_LSHIFT))
		{// ���V�t�g�L�[�������ꂽ
			pPlayer->rotDest.y += -MAX_ROTATION;
		}
		else if (GetKeyboardPress(DIK_RSHIFT))
		{// �E�V�t�g�L�[�������ꂽ
			pPlayer->rotDest.y += MAX_ROTATION;
		}
	}
}

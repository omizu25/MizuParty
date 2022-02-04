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
#include "game.h"
#include "input.h"
#include "mesh_field.h"
#include "model.h"
#include "player.h"
#include "setup.h"
#include "shadow.h"
#include "wall.h"
#include "title.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_ROTATION		(0.035f)		// ��]�̍ő�l
#define MAX_ATTENUATION		(0.1f)			// �����W���̍ő�l
#define MAX_HEIGHT			(80.0f)			// �����̍ő�l
#define MIN_HEIGHT			(-80.0f)		// �����̍ŏ��l
#define IDX_PARENT			(-1)			// �e�̔ԍ�
#define MAX_BLEND			(30)			// �u�����h�̍ő�l

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	LPD3DXMESH			pMesh;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		pBuffMat;				// �}�e���A�����ւ̃|�C���^
	DWORD				nNumMat;				// �}�e���A�����̐�
	char				aParts[MAX_TEXT];		// ���f���t�@�C����
}File;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Player		*s_player;				// ���f���̏��
static File			**s_file;				// �t�@�C���̏��
static MOTION		s_IdxMotion;			// ���[�V�����ԍ�
static int			s_nNumPlayer;			// �v���C���[�̐�
static int			s_nSelectPlayer;		// �I�΂�Ă���v���C���[
static int			s_nSelectParts;			// �I�΂�Ă���p�[�c
static int			s_nSelectMotion;		// �I�΂�Ă��郂�[�V����
static int			s_nFrame;				// �t���[����
static int			s_nIdxKey;				// �L�[�ԍ�
static bool			s_bMotionBlend;			// ���[�V�����u�����h
static bool			s_bMotionLoop;			// ���[�V�������[�v

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void System(void);
static void LoadNew(int nCnt);
static void LoadParts(int nCnt);
static void LoadMotion(int nCnt);
static void Move(Player *pPlayer);
static void Motion(Player *pPlayer);
static void SetMotion(Player *pPlayer);
static void MotionBlend(Player *pPlayer);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitPlayer(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	switch (GetTitle())
	{// �ǂ̃Q�[���H
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_STOP:			// �~�߂�

		s_nSelectPlayer = 0;

		break;

	case MENU_SLOPE:		// ��

		s_nSelectPlayer = 1;

		break;

	default:
		assert(false);
		break;
	}

	s_nSelectParts = 0;
	s_nSelectMotion = 0;
	s_nFrame = 0;
	s_IdxMotion = MOTION_NEUTRAL;
	s_nIdxKey = 0;
	s_bMotionBlend = true;
	s_bMotionLoop = false;

	for (int i = 0; i < s_nNumPlayer; i++)
	{
		Player *pPlayer = &s_player[i];

		for (int j = 0; j < pPlayer->nNumParts; j++)
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

			pPlayer->parts[j].posSet = pPlayer->parts[j].pos;
			pPlayer->parts[j].rotSet = pPlayer->parts[j].rot;
			pPlayer->parts[j].posOld = pPlayer->parts[j].pos;
			pPlayer->parts[j].rotOld = pPlayer->parts[j].rot;
		}

		pPlayer->posOld = pPlayer->pos;
		pPlayer->rotDest = pPlayer->rot;
		pPlayer->nStopTime = 0;
	}

	Player *pPlayer = &s_player[s_nSelectPlayer];

	// �e�̐ݒ�
	pPlayer->nIdxShadow = SetShadow(pPlayer->pos, pPlayer->rot, pPlayer->fSize);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitPlayer(void)
{
	for (int i = 0; i < s_nNumPlayer; i++)
	{
		for (int j = 0; j < s_player[i].nNumParts; j++)
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

				delete[] pParts->pTexture;
				pParts->pTexture = NULL;
			}
		}
	}

	for (int i = 0; i < s_nNumPlayer; i++)
	{
		for (int j = 0; j < s_player[i].nNumParts; j++)
		{
			PlayerParts *pParts = &s_player[i].parts[j];

			if (pParts->pMesh != NULL)
			{// ���b�V���̉��
				pParts->pMesh = NULL;
			}

			if (pParts->pBuffMat != NULL)
			{// �}�e���A���̉��
				pParts->pBuffMat = NULL;
			}
		}

		for (int j = 0; j <  s_player[i].nNumName; j++)
		{
			if (s_file[i][j].pMesh != NULL)
			{// ���b�V���̉��
				s_file[i][j].pMesh->Release();
				s_file[i][j].pMesh = NULL;
			}

			if (s_file[i][j].pBuffMat != NULL)
			{// �}�e���A���̉��
				s_file[i][j].pBuffMat->Release();
				s_file[i][j].pBuffMat = NULL;
			}
		}

		// ���f���̊J��
		delete[] s_file[i];
		s_file[i] = NULL;
	}
	
	// ���f���̊J��
	delete[] s_file;
	s_file = NULL;

	for (int i = 0; i < s_nNumPlayer; i++)
	{// �v���C���[��
		for (int j = 0; j < s_player[i].nNumMotion; j++)
		{// ���[�V������
			for (int k = 0; k < s_player[i].Motion[j].nNumKey; k++)
			{// �L�[�Z�b�g��
				delete[] s_player[i].Motion[j].keySet[k].key;
				s_player[i].Motion[j].keySet[k].key = NULL;
			}

			delete[] s_player[i].Motion[j].keySet;
			s_player[i].Motion[j].keySet = NULL;
		}

		delete[] s_player[i].Motion;
		s_player[i].Motion = NULL;
	}

	for (int i = 0; i < s_nNumPlayer; i++)
	{// �p�[�c�̊J��
		delete[] s_player[i].parts;
		s_player[i].parts = NULL;
	}

	// �v���C���[�̊J��
	delete[] s_player;
	s_player = NULL; 
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdatePlayer(void)
{
	Player *pPlayer = &s_player[s_nSelectPlayer];

	pPlayer->nStopTime++;

	// �O��̈ʒu��ۑ�
	pPlayer->posOld = pPlayer->pos;

	switch (GetTitle())
	{// �ǂ̃Q�[���H
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_SLOPE:		// ��

		switch (GetGame().gameState)
		{
		case GAMESTATE_NONE:			// �������Ă��Ȃ����
		case GAMESTATE_START:			// �J�n��� (�Q�[���J�n�O)
		case GAMESTATE_COUNTDOWN:		// �J�E���g�_�E����� (�Q�[���J�n��)
		case GAMESTATE_RESULT:			// ���U���g��� (�Q�[���I����)

			/* �����Ȃ� */

			break;

		case GAMESTATE_NORMAL:		// �ʏ��� (�Q�[���i�s��)

			// �ړ�
			Move(pPlayer);

			break;

		case GAMESTATE_END:			// �I����� (�Q�[���I����)

			pPlayer->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;

		default:
			assert(false);
			break;
		}

		break;

	case MENU_STOP:			// �~�߂�

		/* �ړ����Ȃ� */

		break;

	default:
		assert(false);
		break;
	}

	float fAngle = pPlayer->rotDest.y - pPlayer->rot.y;

	// �p�x�̐��K��
	NormalizeRot(&fAngle);

	//�����E�������X�V (����������)
	pPlayer->rot.y += fAngle * MAX_ATTENUATION;

	// �p�x�̐��K��
	NormalizeRot(&pPlayer->rot.y);

	D3DXVECTOR3 size = D3DXVECTOR3(pPlayer->fSize, pPlayer->fHeight, pPlayer->fSize);

	// ���f���Ƃ̓����蔻��
	CollisionModel(&pPlayer->pos, &pPlayer->posOld, size);

	if (!GetCollision())
	{
		// �~�߂�̓����蔻��
		CollisionStop(&pPlayer->pos, size);
	}

	// �ǂƂ̓����蔻��
	//CollisionWall(&pPlayer->pos, &pPlayer->posOld, size);

	// ���b�V���t�B�[���h�Ƃ̓����蔻��
	//CollisionMeshField(&pPlayer->pos);

	switch (GetTitle())
	{// �ǂ̃Q�[���H
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_SLOPE:		// ��

		// ���[�V����
		Motion(pPlayer);

		break;

	case MENU_STOP:			// �~�߂�

		if (s_bMotionBlend)
		{// ���[�V�����u�����h��
			s_nFrame++;

			// ���[�V�����u�����h
			MotionBlend(pPlayer);
		}

		break;

	default:
		assert(false);
		break;
	}

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

	Player *pPlayer = &s_player[s_nSelectPlayer];

	for (int j = 0; j < pPlayer->nNumParts; j++)
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
void LoadPlayer(void)
{
	// �V�X�e��
	System();

	for (int i = 0; i < s_nNumPlayer; i++)
	{
		// new�p�̓ǂݍ���
		LoadNew(i);

		// �p�[�c�̓ǂݍ���
		LoadParts(i);

		// ���[�V�����̓ǂݍ���
		LoadMotion(i);
	}
}

//--------------------------------------------------
// ���̃��[�V����
//--------------------------------------------------
void NextMotion(MOTION motion)
{
	s_nFrame = 0;
	s_nIdxKey = 0;

	// ���[�V�����Z�b�g
	SetMotion(&s_player[s_nSelectPlayer]);

	s_IdxMotion = motion;
}

//--------------------------------------------------
// �V�X�e��
//--------------------------------------------------
static void System(void)
{
	FILE *pFile;		// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[MAX_TEXT] = {};
		s_nNumPlayer = 0;

		while (strncmp(&aRead[0], "END_SCRIPT", 10) != 0)
		{// �I��肪����܂ŌJ��Ԃ�
			fgets(aRead, MAX_TEXT, pFile);

			if (strncmp(&aRead[0], "PLAYER_SET", 10) == 0)
			{// ���f���̏��
				s_nNumPlayer++;
			}
		}

		// �t�@�C�������
		fclose(pFile);

		// txt�ɏ����Ă�ő吔���̃v���C���[�̔z���p�ӂ���
		s_player = new Player[s_nNumPlayer];
		s_file = new File*[s_nNumPlayer];
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[MAX_TEXT] = {};
		int nFileName = 0, nPlayer = 0;

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

			if (strcmp(&aRead[0], "PLAYER_SET") == 0)
			{// ���f���̏��
				while (strcmp(&aRead[0], "END_PLAYER_SET") != 0)
				{// �I��肪����܂ŌJ��Ԃ�
					fscanf(pFile, "%s", &aRead);

					if (strncmp(&aRead[0], "#", 1) == 0)
					{// �R�����g
						fgets(aRead, MAX_TEXT, pFile);
						continue;
					}

					if (strcmp(&aRead[0], "PLAYER_FILENAME") == 0)
					{// �v���C���[�̃t�@�C����
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%s", s_player[nFileName].aText);
						nFileName++;
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
		assert(false);
	}
}

//--------------------------------------------------
// new�p�̓ǂݍ���
//--------------------------------------------------
static void LoadNew(int nCnt)
{
	FILE *pFile;		// �t�@�C���|�C���^��錾

	Player *pPlayer = &s_player[nCnt];

	// �t�@�C�����J��
	pFile = fopen(pPlayer->aText, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[MAX_TEXT] = {};
		pPlayer->nNumMotion = 0;
		pPlayer->nNumName = 0;

		while (strncmp(&aRead[0], "END_SCRIPT", 10) != 0)
		{// �I��肪����܂ŌJ��Ԃ�
			fgets(aRead, MAX_TEXT, pFile);

			if (strncmp(&aRead[0], "MOTIONSET", 9) == 0)
			{// ���[�V�����̏��
				pPlayer->nNumMotion++;
			}
			else if (strncmp(&aRead[0], "MODEL_FILENAME", 14) == 0)
			{// ���[�V�����̏��
				pPlayer->nNumName++;
			}
		}

		// �t�@�C�������
		fclose(pFile);

		// txt�ɏ����Ă�ő吔���̃��[�V�����̔z���p�ӂ���
		pPlayer->Motion = new MotionSet[pPlayer->nNumMotion];

		s_file[nCnt] = new File[pPlayer->nNumName];
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//--------------------------------------------------
// �p�[�c�̓ǂݍ���
//--------------------------------------------------
static void LoadParts(int nCnt)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE *pFile;		// �t�@�C���|�C���^��錾

	Player *pPlayer = &s_player[nCnt];

	// �t�@�C�����J��
	pFile = fopen(pPlayer->aText, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[MAX_TEXT] = {};
		int nParts = 0, nFileName = 0, nNumModel = 0;

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

			if (strcmp(&aRead[0], "NUM_MODEL") == 0)
			{// ���f���̎g�p��
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%d", &nNumModel);
			}
			else if (strcmp(&aRead[0], "MODEL_FILENAME") == 0)
			{// ���f���t�@�C����
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", &s_file[nCnt][nFileName].aParts);

				nFileName++;
			}
			else if (strcmp(&aRead[0], "CHARACTERSET") == 0)
			{// �L�����N�^�[�̏��
				nParts = 0;

				while (strcmp(&aRead[0], "END_CHARACTERSET") != 0)
				{// �I��肪����܂ŌJ��Ԃ�
					fscanf(pFile, "%s", &aRead);

					if (strncmp(&aRead[0], "#", 1) == 0)
					{// �R�����g
						fgets(aRead, MAX_TEXT, pFile);
						continue;
					}

					PlayerParts *pParts = &pPlayer->parts[nParts];

					if (strcmp(&aRead[0], "NUM_PARTS") == 0)
					{// �p�[�c�̎g�p��
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pPlayer->nNumParts);

						// txt�ɏ����Ă�ő吔���̔z���p�ӂ���
						pPlayer->parts = new PlayerParts[pPlayer->nNumParts];
					}
					else if (strcmp(&aRead[0], "MOVE") == 0)
					{// �ړ���
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pPlayer->fMove);
					}
					else if (strcmp(&aRead[0], "JUMP") == 0)
					{// �W�����v��
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pPlayer->fJump);
					}
					else if (strcmp(&aRead[0], "RADIUS") == 0)
					{// �T�C�Y
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pPlayer->fSize);
					}
					else if (strcmp(&aRead[0], "HEIGHT") == 0)
					{// ����
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pPlayer->fHeight);
					}
					else if (strcmp(&aRead[0], "PARTSSET") == 0)
					{// �L�����N�^�[�̏��
						while (strcmp(&aRead[0], "END_PARTSSET") != 0)
						{// �I��肪����܂ŌJ��Ԃ�
							fscanf(pFile, "%s", &aRead);

							if (strncmp(&aRead[0], "#", 1) == 0)
							{// �R�����g
								fgets(aRead, MAX_TEXT, pFile);
								continue;
							}

							if (strcmp(&aRead[0], "INDEX") == 0)
							{// �g�p���郂�f���̔ԍ�
								fscanf(pFile, "%s", &aRead);
								fscanf(pFile, "%d", &pParts->nIdxModel);
							}
							else if (strcmp(&aRead[0], "PARENT") == 0)
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

								pParts->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}
						nParts++;
					}
				}
			}
		}

		// �t�@�C�������
		fclose(pFile);

		for (int j = 0; j < pPlayer->nNumName; j++)
		{// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(
				s_file[nCnt][j].aParts,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&s_file[nCnt][j].pBuffMat,
				NULL,
				&s_file[nCnt][j].nNumMat,
				&s_file[nCnt][j].pMesh);
		}

		for (int j = 0; j < pPlayer->nNumParts; j++)
		{// �g�p���郂�f���̏����擾
			PlayerParts *Parts = &pPlayer->parts[j];

			Parts->pBuffMat = s_file[nCnt][Parts->nIdxModel].pBuffMat;
			Parts->nNumMat = s_file[nCnt][Parts->nIdxModel].nNumMat;
			Parts->pMesh = s_file[nCnt][Parts->nIdxModel].pMesh;
		}
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//--------------------------------------------------
// ���[�V�����̓ǂݍ���
//--------------------------------------------------
static void LoadMotion(int nCnt)
{
	FILE *pFile;		// �t�@�C���|�C���^��錾

	Player *pPlayer = &s_player[nCnt];

	// �t�@�C�����J��
	pFile = fopen(pPlayer->aText, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[MAX_TEXT] = {};
		int nNumMotion = 0;

		while (strcmp(&aRead[0], "END_CHARACTERSET") != 0)
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

			if (strcmp(&aRead[0], "MOTIONSET") == 0)
			{// ���[�V�����̏��
				int nNumKeySet = 0;

				while (strcmp(&aRead[0], "END_MOTIONSET") != 0)
				{// �I��肪����܂ŌJ��Ԃ�
					fscanf(pFile, "%s", &aRead);

					if (strncmp(&aRead[0], "#", 1) == 0)
					{// �R�����g
						fgets(aRead, MAX_TEXT, pFile);
						continue;
					}

					MotionSet *pMotion = &pPlayer->Motion[nNumMotion];

					if (strcmp(&aRead[0], "LOOP") == 0)
					{// ���[�v
						int nLoop = 0;
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &nLoop);

						if (nLoop == 0)
						{// ���[�v���Ȃ�
							pMotion->bLoop = false;
						}
						else
						{// ���[�v����
							pMotion->bLoop = true;
						}
					}
					else if (strcmp(&aRead[0], "NUM_KEY") == 0)
					{// �L�[��
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pMotion->nNumKey);

						// txt�ɏ����Ă�ő吔���̃L�[�ݒ�̔z���p�ӂ���
						pMotion->keySet = new KeySet[pMotion->nNumKey];

					}
					else if (strcmp(&aRead[0], "KEYSET") == 0)
					{// �L�[�ݒ�
						int nNumKey = 0;

						while (strcmp(&aRead[0], "END_KEYSET") != 0)
						{// �I��肪����܂ŌJ��Ԃ�
							fscanf(pFile, "%s", &aRead);

							if (strncmp(&aRead[0], "#", 1) == 0)
							{// �R�����g
								fgets(aRead, MAX_TEXT, pFile);
								continue;
							}

							KeySet *pKeySet = &pMotion->keySet[nNumKeySet];

							if (strcmp(&aRead[0], "FRAME") == 0)
							{// �t���[����
								fscanf(pFile, "%s", &aRead);
								fscanf(pFile, "%d", &pKeySet->nFrame);

								// txt�ɏ����Ă�ő吔���̃L�[�̔z���p�ӂ���
								pKeySet->key = new Key[pPlayer->nNumParts];
							}
							else if (strcmp(&aRead[0], "KEY") == 0)
							{// �L�[
								while (strcmp(&aRead[0], "END_KEY") != 0)
								{// �I��肪����܂ŌJ��Ԃ�
									fscanf(pFile, "%s", &aRead);

									if (strncmp(&aRead[0], "#", 1) == 0)
									{// �R�����g
										fgets(aRead, MAX_TEXT, pFile);
										continue;
									}

									Key *pKey = &pKeySet->key[nNumKey];

									if (strcmp(&aRead[0], "POS") == 0)
									{// �ʒu
										fscanf(pFile, "%s", &aRead);
										fscanf(pFile, "%f", &pKey->pos.x);
										fscanf(pFile, "%f", &pKey->pos.y);
										fscanf(pFile, "%f", &pKey->pos.z);
									}
									else if (strcmp(&aRead[0], "ROT") == 0)
									{// ����
										fscanf(pFile, "%s", &aRead);
										fscanf(pFile, "%f", &pKey->rot.x);
										fscanf(pFile, "%f", &pKey->rot.y);
										fscanf(pFile, "%f", &pKey->rot.z);
									}
								}
								nNumKey++;
							}
						}
						nNumKeySet++;
					}
				}
				nNumMotion++;
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
// �ړ�
//--------------------------------------------------
static void Move(Player *pPlayer)
{
	float fRot = 0.0f;

	/* �����f���̈ړ��� */

	if (GetKeyboardPress(DIK_A))
	{// ���L�[�������ꂽ
		fRot = -D3DX_PI * 0.5f;

		pPlayer->rotDest.y = D3DX_PI * 0.5f;
	}
	else if (GetKeyboardPress(DIK_D))
	{// ���L�[�������ꂽ
		fRot = D3DX_PI * 0.5f;

		pPlayer->rotDest.y = -D3DX_PI * 0.5f;
	}

	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
	{// ��, ��, ��, ���L�[�������ꂽ
		pPlayer->pos.x += sinf(fRot) * pPlayer->fMove;
		pPlayer->pos.z += cosf(fRot) * pPlayer->fMove;
	}

	if (GetTitle() == MENU_SLOPE)
	{
		if (pPlayer->pos.x >= 0.0f)
		{// �ړ�����
			pPlayer->pos.x = 0.0f;
		}
	}
}

//--------------------------------------------------
// ���[�V����
//--------------------------------------------------
static void Motion(Player * pPlayer)
{
	s_nFrame++;

	if (GetGame().gameState == GAMESTATE_NORMAL)
	{// �ʏ��� (�Q�[���i�s��)
		switch (GetTitle())
		{// �ǂ̃Q�[���H
		case MENU_WALKING:		// �E�H�[�L���O
		case MENU_SLOPE:		// ��

			if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_D))
			{// ��, ��, ��, ���L�[�������ꂽ
				// ���̃��[�V����
				NextMotion(MOTION_MOVE);
			}

			break;

		case MENU_STOP:			// �~�߂�

			/* �����Ȃ� */

			break;

		default:
			assert(false);
			break;
		}
	}

	if (s_bMotionBlend)
	{// ���[�V�����u�����h��
		MotionBlend(pPlayer);
	}
	else
	{
		if (s_IdxMotion == MOTION_MOVE)
		{
			s_bMotionLoop = true;
		}
		else if (s_IdxMotion == MOTION_ATTACK)
		{
			s_bMotionLoop = false;
		}

		if (GetGame().gameState == GAMESTATE_NORMAL)
		{// �ʏ��� (�Q�[���i�s��)
			if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D) ||
				GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S))
			{// ��, ��, ��, ���L�[�������ꂽ
				s_bMotionLoop = false;
			}
		}

		if (s_bMotionLoop)
		{// ���[�V�������[�v����
			// ���̃��[�V����
			NextMotion(MOTION_NEUTRAL);

			s_bMotionLoop = false;
		}

		MotionSet *pMotion = &pPlayer->Motion[s_IdxMotion];

		if (s_nFrame >= pMotion->keySet[s_nIdxKey].nFrame)
		{// �t���[������������
			s_nIdxKey++;

			if (pMotion->bLoop)
			{// ���[�v����
				s_nIdxKey %= pMotion->nNumKey;
			}
			else
			{// ���[�v���Ȃ�
				if (s_nIdxKey >= pMotion->nNumKey)
				{// �L�[����������
					// ���̃��[�V����
					NextMotion(MOTION_NEUTRAL);
				}
			}

			s_nFrame = 0;
		}

		for (int i = 0; i < pPlayer->nNumParts; i++)
		{
			int nNext = (s_nIdxKey + 1) % pMotion->nNumKey;

			D3DXVECTOR3 pos = pMotion->keySet[nNext].key[i].pos - pMotion->keySet[s_nIdxKey].key[i].pos;
			D3DXVECTOR3 rot = pMotion->keySet[nNext].key[i].rot - pMotion->keySet[s_nIdxKey].key[i].rot;

			pos /= (float)pMotion->keySet[s_nIdxKey].nFrame;
			rot /= (float)pMotion->keySet[s_nIdxKey].nFrame;

			pPlayer->parts[i].pos += pos;
			pPlayer->parts[i].rot += rot;
		}
	}
}

//--------------------------------------------------
// ���[�V�����Z�b�g
//--------------------------------------------------
static void SetMotion(Player * pPlayer)
{
	for (int i = 0; i < pPlayer->nNumParts; i++)
	{
		pPlayer->parts[i].posOld = pPlayer->parts[i].pos;
		pPlayer->parts[i].rotOld = pPlayer->parts[i].rot;
	}
	
	s_bMotionBlend = true;
}

//--------------------------------------------------
// ���[�V�����u�����h
//--------------------------------------------------
static void MotionBlend(Player *pPlayer)
{
	MotionSet *pMotion = &pPlayer->Motion[s_IdxMotion];

	for (int i = 0; i < pPlayer->nNumParts; i++)
	{
		D3DXVECTOR3 posNew = pPlayer->parts[i].posSet + pMotion->keySet[s_nIdxKey].key[i].pos;
		D3DXVECTOR3 rotNew = pPlayer->parts[i].rotSet + pMotion->keySet[s_nIdxKey].key[i].rot;

		D3DXVECTOR3 pos = posNew - pPlayer->parts[i].posOld;
		D3DXVECTOR3 rot = rotNew - pPlayer->parts[i].rotOld;

		pos /= MAX_BLEND;
		rot /= MAX_BLEND;

		pPlayer->parts[i].pos += pos;
		pPlayer->parts[i].rot += rot;
	}

	if (s_nFrame >= MAX_BLEND)
	{// �t���[������������
		s_bMotionBlend = false;
		s_nFrame = 0;
	}
}

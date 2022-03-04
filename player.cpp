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
#include "fade.h"
#include "field.h"
#include "frame.h"
#include "game.h"
#include "input.h"
#include "mesh_field.h"
#include "model.h"
#include "particle.h"
#include "player.h"
#include "result.h"
#include "setup.h"
#include "shadow.h"
#include "wall.h"
#include "sound.h"
#include "title.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_ROTATION		(0.035f)		// ��]�̍ő�l
#define MAX_ATTENUATION		(0.1f)			// �����W���̍ő�l
#define MAX_INERTIA			(0.5f)			// �����̍ő�l
#define MAX_HEIGHT			(80.0f)			// �����̍ő�l
#define MIN_HEIGHT			(-80.0f)		// �����̍ŏ��l
#define IDX_PARENT			(-1)			// �e�̔ԍ�
#define MAX_BLEND			(20)			// �u�����h�̍ő�l
#define SLOPE_LIMIT			(90.0f)			// ��̈ړ�����
#define SLOPE_RESULT		(1500.0f)		// ��̃��U���g�ւ̔���
#define TITLE_WIDTH			(255.0f)		// �^�C�g���̈ړ�����
#define TITLE_DEPTH			(135.0f)		// �^�C�g���̈ړ�����
#define DEAD_ZONE			(0.1f)			// �X�e�B�b�N�̗V��
#define UP_SPEED			(0.75f)			// �オ�鑬�x
#define DOWN_SPEED			(1.15f)			// �����鑬�x

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
static float		s_fSlopeMove;			// ��̈ړ���
static int			s_nEndTime;				// �G���h�̎���
static bool			s_bSoundRun;			// ����T�E���h�𗬂�����
static bool			s_bSoundFall;			// ������T�E���h�𗬂�����
static bool			s_bKeyBoardWASD;		// WASD�̃L�[�{�[�h���͂����邩�ǂ���
static bool			s_bKeyBoardArrow;		// �₶�邵�̃L�[�{�[�h���͂����邩�ǂ���
static bool			s_bJoyPad;				// �W���C�p�b�h���͂����邩�ǂ���
static bool			s_bStickLeft;			// ���X�e�B�b�N���͂����邩�ǂ���
static bool			s_bStickRight;			// �E�X�e�B�b�N���͂����邩�ǂ���
static bool			s_bRot;					// ���� false : �E true : ��
static bool			s_bDraw;				// �`�悷�邩�ǂ���

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void System(void);
static void LoadNew(int nCnt);
static void LoadParts(int nCnt);
static void LoadMotion(int nCnt);
static void UpdateTitle(Player *pPlayer);
static void UpdateGame(Player *pPlayer);
static void TitleMove(Player *pPlayer);
static void Move(Player *pPlayer);
static void InputMove(void);
static void Rot(Player *pPlayer);
static void Motion(Player *pPlayer);
static void SetMotion(Player *pPlayer);
static void MotionBlend(Player *pPlayer);
static void MotionSlope(Player *pPlayer);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitPlayer(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	switch (GetMode())
	{
	case MODE_TITLE:		// �^�C�g��

		s_nSelectPlayer = 0;

		break;

	case MODE_GAME:			// �Q�[��

		switch (GetTitle())
		{// �ǂ̃Q�[���H
		case MENU_WALKING:		// �E�H�[�L���O
		case MENU_ROTATION:		// ��]
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
	s_fSlopeMove = 0.0f;
	s_nEndTime = 0;
	s_bSoundRun = false;
	s_bSoundFall = false;
	s_bKeyBoardWASD = false;
	s_bKeyBoardArrow = false;
	s_bJoyPad = false;
	s_bStickLeft = false;
	s_bStickRight = false;
	s_bRot = true;
	s_bDraw = true;

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

		pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	Player *pPlayer = &s_player[s_nSelectPlayer];

	D3DXVECTOR3 pos = D3DXVECTOR3(pPlayer->pos.x, 0.1f, pPlayer->pos.z);
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (s_nSelectPlayer)
	{
	case 0:		// �Ă�Ă�
		pos.y = 0.1f;
		size = D3DXVECTOR3(pPlayer->fSize, 0.0f, pPlayer->fSize);
		break;

	case 1:		// ���]��
		pos.y = pPlayer->pos.y + 0.1f;
		size = D3DXVECTOR3(15.0f, 0.0f, 140.0f);
		break;

	default:
		assert(false);
		break;
	}

	// �e�̐ݒ�
	pPlayer->nIdxShadow = SetShadow(pos, pPlayer->rot, size);
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

	switch (GetMode())
	{
	case MODE_TITLE:		// �^�C�g��

		// �^�C�g���̎��̍X�V
		UpdateTitle(pPlayer);

		break;

	case MODE_GAME:			// �Q�[��

		// �Q�[���̎��̍X�V
		UpdateGame(pPlayer);

		break;

	default:
		assert(false);
		break;
	}

	D3DXVECTOR3 pos = D3DXVECTOR3(pPlayer->pos.x, 0.1f, pPlayer->pos.z);
	
	switch (s_nSelectPlayer)
	{
	case 0:		// �Ă�Ă�
		pos.y = 0.1f;
		
		break;

	case 1:		// ���]��
		pos.y = pPlayer->pos.y + 0.1f;
		break;

	default:
		assert(false);
		break;
	}

	// �e�̈ʒu�̐ݒ�
	SetPosShadow(pPlayer->nIdxShadow, pos, pPlayer->rot);
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

	if (s_bDraw)
	{// �`�悷��
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
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player[s_nSelectPlayer];
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
// ��̐ݒ�
//--------------------------------------------------
void SetSlopePlayer(void)
{
	float fPosX = s_player[s_nSelectPlayer].pos.x * -0.02f;
	s_fSlopeMove = fPosX;
}

//--------------------------------------------------
// �`�悷�邩�̐ݒ�
//--------------------------------------------------
void SetDrawPlayer(bool bDraw)
{
	s_bDraw = bDraw;

	if (!bDraw)
	{
		// �e���g���̂��~�߂�
		UseStopShadow(s_player[s_nSelectPlayer].nIdxShadow);
	}
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
// �^�C�g���̎��̍X�V
//--------------------------------------------------
static void UpdateTitle(Player *pPlayer)
{
	if (GetFade() == FADE_NONE)
	{
		// �ړ�
		TitleMove(pPlayer);

		// ��]
		Rot(pPlayer);
	}

	// ���[�V����
	Motion(pPlayer);

	// �g�̓����蔻��
	CollisionFrame(&pPlayer->pos);
}

//--------------------------------------------------
// �Q�[���̎��̍X�V
//--------------------------------------------------
static void UpdateGame(Player *pPlayer)
{
	pPlayer->nStopTime++;

	// �O��̈ʒu��ۑ�
	pPlayer->posOld = pPlayer->pos;

	switch (GetTitle())
	{// �ړ��n
	case MENU_WALKING:		// �E�H�[�L���O

		switch (GetGame())
		{
		case GAMESTATE_NONE:			// �������Ă��Ȃ����
		case GAMESTATE_START:			// �J�n��� (�Q�[���J�n�O)
		case GAMESTATE_COUNTDOWN:		// �J�E���g�_�E����� (�Q�[���J�n��)
		case GAMESTATE_RESULT:			// ���U���g��� (�Q�[���I����)
		case GAMESTATE_LOOP:			// �J��Ԃ���� (���U���g�I����)

			/* �����Ȃ� */

			break;

		case GAMESTATE_NORMAL:		// �ʏ��� (�Q�[���i�s��)

			// �ړ�
			Move(pPlayer);

			break;

		case GAMESTATE_END:			// �I����� (�Q�[���I����)

			// �����̏�����
			pPlayer->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;

		default:
			assert(false);
			break;
		}

		break;

	case MENU_SLOPE:		// ��

		// �d�͂����Z
		pPlayer->pos.y += -10.8f;

		switch (GetGame())
		{
		case GAMESTATE_NONE:			// �������Ă��Ȃ����
		case GAMESTATE_START:			// �J�n��� (�Q�[���J�n�O)
		case GAMESTATE_COUNTDOWN:		// �J�E���g�_�E����� (�Q�[���J�n��)
		case GAMESTATE_RESULT:			// ���U���g��� (�Q�[���I����)
		case GAMESTATE_LOOP:			// �J��Ԃ���� (���U���g�I����)

			/* �����Ȃ� */

			break;

		case GAMESTATE_NORMAL:		// �ʏ��� (�Q�[���i�s��)

			// �ړ�
			Move(pPlayer);

			break;

		case GAMESTATE_END:			// �I����� (�Q�[���I����)

			s_nEndTime++;

			if (s_fSlopeMove != 0.0f)
			{
				pPlayer->move.x = 0.0f;

				if (s_nEndTime >= 120)
				{
					if (!s_bSoundRun)
					{
						s_bSoundRun = true;

						// �T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_RUN);
					}

					pPlayer->pos.x += s_fSlopeMove;

					pPlayer->move.x = s_fSlopeMove;
				}
			}
			else
			{
				if (s_nEndTime >= 120)
				{
					if (CollisionField(&pPlayer->pos, &pPlayer->posOld))
					{
						// ���U���g�̐ݒ�
						SetResult(RESULT_CLEAR);
					}
					else
					{
						// ���U���g�̐ݒ�
						SetResult(RESULT_GAMEOVER);
					}

					pPlayer->pos.y += -10.8f;

					// ���U���g�̏�����
					InitResult();

					// �Q�[���̐ݒ�
					SetGameState(GAMESTATE_RESULT);
				}
			}
			break;

		default:
			assert(false);
			break;
		}

		break;

	case MENU_ROTATION:		// ��]
	case MENU_STOP:			// �~�߂�

		/* �ړ����Ȃ� */

		break;

	default:
		assert(false);
		break;
	}

	// ��]
	Rot(pPlayer);

	D3DXVECTOR3 size = D3DXVECTOR3(pPlayer->fSize, pPlayer->fHeight, pPlayer->fSize);

	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_ROTATION:		// ��]
	case MENU_STOP:			// �~�߂�

		/* �����Ȃ� */

		break;

	case MENU_SLOPE:		// ��

		if (!CollisionField(&pPlayer->pos, &pPlayer->posOld))
		{// �t�B�[���h�Ƃ̓����蔻��
			if (!CollisionMeshField(&pPlayer->pos))
			{
				if (GetGame() == GAMESTATE_END ||
					GetGame() == GAMESTATE_RESULT)
				{// �����̉�]
					pPlayer->rotDest.x += -D3DX_PI * 0.25f;
					pPlayer->rot.x += -D3DX_PI * 0.25f;
					pPlayer->rotDest.y += -D3DX_PI * 0.25f;
					pPlayer->rot.y += -D3DX_PI * 0.25f;

					// �e���g���̂��~�߂�
					UseStopShadow(pPlayer->nIdxShadow);

					if (!s_bSoundFall)
					{
						s_bSoundFall = true;

						// �T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_FALL);
					}
				}
			}
		}

		break;

	default:
		assert(false);
		break;
	}

	if (GetTitle() == MENU_SLOPE)
	{
		// ���b�V���t�B�[���h�Ƃ̓����蔻��
		if (CollisionMeshField(&pPlayer->pos))
		{// �������Ă�
			if (GetGame() != GAMESTATE_END)
			{
				if (pPlayer->rot.y >= 0.0f)
				{// ������
					pPlayer->rotDest.x = D3DX_PI * 0.1f;
					pPlayer->rot.x = D3DX_PI * 0.1f;
				}
				else if (pPlayer->rot.y <= 0.0f)
				{// �E����
					pPlayer->rotDest.x = -D3DX_PI * 0.1f;
					pPlayer->rot.x = -D3DX_PI * 0.1f;
				}
			}
			else if (GetGame() == GAMESTATE_END)
			{
				pPlayer->rotDest.y = -D3DX_PI * 0.5f;
				pPlayer->rotDest.x = -D3DX_PI * 0.1f;
				pPlayer->rot.x = -D3DX_PI * 0.1f;

				if (s_nEndTime >= 120)
				{
					s_fSlopeMove += 0.1f;
				}
			}
		}
		else
		{// �������ĂȂ�
			pPlayer->rotDest.x = 0.0f;

			if (s_nEndTime >= 120)
			{
				float fField = GetField()->pos.x + GetField()->vtxMax.x;

				if (pPlayer->pos.x + SLOPE_LIMIT <= fField)
				{
					s_fSlopeMove -= 0.1f;

					if (s_fSlopeMove <= 0.0f)
					{
						s_fSlopeMove = 0.0f;

						s_nEndTime = 0;
					}
				}
				else
				{
					if (pPlayer->pos.y < -SLOPE_RESULT && pPlayer->posOld.y >= -SLOPE_RESULT)
					{
						// ���U���g�̐ݒ�
						SetResult(RESULT_GAMEOVER);

						// ���U���g�̏�����
						InitResult();

						// �Q�[���̐ݒ�
						SetGameState(GAMESTATE_RESULT);
					}
				}
			}
		}
	}

	switch (GetTitle())
	{// �ǂ̃Q�[���H
	case MENU_WALKING:		// �E�H�[�L���O

		// ���[�V����
		Motion(pPlayer);

		break;

	case MENU_ROTATION:		// ��]
	case MENU_STOP:			// �~�߂�

		if (s_bMotionBlend)
		{// ���[�V�����u�����h��
			s_nFrame++;

			// ���[�V�����u�����h
			MotionBlend(pPlayer);
		}

		break;

	case MENU_SLOPE:		// ��

		// ��̃��[�V����
		MotionSlope(pPlayer);

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
}

//--------------------------------------------------
// �^�C�g���̎��̈ړ�
//--------------------------------------------------
static void TitleMove(Player *pPlayer)
{
	// �ړ��̓���
	InputMove();

	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	/* �����f���̈ړ��� */

	if (s_bKeyBoardWASD)
	{// �L�[�{�[�h
		if (GetKeyboardPress(DIK_A))
		{// �L�[�������ꂽ
			vec.x -= 1.0f;
		}
		if (GetKeyboardPress(DIK_D))
		{// �L�[�������ꂽ
			vec.x += 1.0f;
		}
		if (GetKeyboardPress(DIK_W))
		{// �L�[�������ꂽ
			vec.z += 1.0f;
		}
		if (GetKeyboardPress(DIK_S))
		{// �L�[�������ꂽ
			vec.z -= 1.0f;
		}

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bKeyBoardArrow)
	{// �L�[�{�[�h
		if (GetKeyboardPress(DIK_LEFT))
		{// �L�[�������ꂽ
			vec.x -= 1.0f;
		}
		if (GetKeyboardPress(DIK_RIGHT))
		{// �L�[�������ꂽ
			vec.x += 1.0f;
		}
		if (GetKeyboardPress(DIK_UP))
		{// �L�[�������ꂽ
			vec.z += 1.0f;
		}
		if (GetKeyboardPress(DIK_DOWN))
		{// �L�[�������ꂽ
			vec.z -= 1.0f;
		}

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bJoyPad)
	{// �W���C�p�b�h
		if (GetJoypadPress(JOYKEY_LEFT))
		{// �{�^���������ꂽ
			vec.x -= 1.0f;
		}
		if (GetJoypadPress(JOYKEY_RIGHT))
		{// �{�^���������ꂽ
			vec.x += 1.0f;
		}
		else if (GetJoypadPress(JOYKEY_UP))
		{// �{�^���������ꂽ
			vec.z += 1.0f;
		}
		else if (GetJoypadPress(JOYKEY_DOWN))
		{// �{�^���������ꂽ
			vec.z -= 1.0f;
		}

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bStickLeft)
	{// ���X�e�B�b�N
		D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		stick.x = GetJoypadStick(JOYKEY_LEFT_STICK, 0).x;
		stick.z = -GetJoypadStick(JOYKEY_LEFT_STICK, 0).y;

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&stick, &stick);

		pPlayer->rotDest.y = atan2f(stick.x, stick.z) + D3DX_PI;
		pPlayer->move += stick * pPlayer->fMove;
	}
	else if (s_bStickRight)
	{// �E�X�e�B�b�N
		D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		stick.x = GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x;
		stick.z = -GetJoypadStick(JOYKEY_RIGHT_STICK, 0).y;

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&stick, &stick);

		pPlayer->rotDest.y = atan2f(stick.x, stick.z) + D3DX_PI;
		pPlayer->move += stick * pPlayer->fMove;
	}

	// �ړ�
	pPlayer->pos.x += pPlayer->move.x;
	pPlayer->pos.z += pPlayer->move.z;

	// �����E�ړ��ʂ��X�V (����������)
	pPlayer->move.x += (0.0f - pPlayer->move.x) * MAX_INERTIA;
	pPlayer->move.z += (0.0f - pPlayer->move.z) * MAX_INERTIA;

	if (pPlayer->pos.x <= -TITLE_WIDTH)
	{// �ړ�����
		pPlayer->pos.x = -TITLE_WIDTH;
	}
	else if (pPlayer->pos.x >= TITLE_WIDTH)
	{// �ړ�����
		pPlayer->pos.x = TITLE_WIDTH;
	}

	if (pPlayer->pos.z <= -TITLE_DEPTH)
	{// �ړ�����
		pPlayer->pos.z = -TITLE_DEPTH;
	}
	else if (pPlayer->pos.z >= TITLE_DEPTH)
	{// �ړ�����
		pPlayer->pos.z = TITLE_DEPTH;
	}
}

//--------------------------------------------------
// �ړ�
//--------------------------------------------------
static void Move(Player *pPlayer)
{
	// �ړ��̓���
	InputMove();

	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	/* �����f���̈ړ��� */

	if (s_bKeyBoardWASD)
	{// �L�[�{�[�h
		if (GetKeyboardPress(DIK_A))
		{// �L�[�������ꂽ
			vec.x -= 1.0f;
			s_bRot = true;
		}
		if (GetKeyboardPress(DIK_D))
		{// �L�[�������ꂽ
			vec.x += 1.0f;
			s_bRot = false;
		}

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bKeyBoardArrow)
	{// �L�[�{�[�h
		if (GetKeyboardPress(DIK_LEFT))
		{// �L�[�������ꂽ
			vec.x -= 1.0f;
			s_bRot = true;
		}
		if (GetKeyboardPress(DIK_RIGHT))
		{// �L�[�������ꂽ
			vec.x += 1.0f;
			s_bRot = false;
		}

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bJoyPad)
	{// �W���C�p�b�h
		if (GetJoypadPress(JOYKEY_LEFT))
		{// �{�^���������ꂽ
			vec.x -= 1.0f;

			s_bRot = true;
		}
		if (GetJoypadPress(JOYKEY_RIGHT))
		{// �{�^���������ꂽ
			vec.x += 1.0f;

			s_bRot = false;
		}

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bStickLeft)
	{// �X�e�B�b�N
		D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		stick.x = GetJoypadStick(JOYKEY_LEFT_STICK, 0).x;

		if (stick.x < 0.0f)
		{// ��
			s_bRot = true;
		}
		else
		{// �E
			s_bRot = false;
		}

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&stick, &stick);

		pPlayer->rotDest.y = atan2f(stick.x, stick.z) + D3DX_PI;
		pPlayer->move += stick * pPlayer->fMove;
	}
	else if (s_bStickRight)
	{// �X�e�B�b�N
		D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		stick.x = GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x;

		if (stick.x < 0.0f)
		{// ��
			s_bRot = true;
		}
		else
		{// �E
			s_bRot = false;
		}

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&stick, &stick);

		pPlayer->rotDest.y = atan2f(stick.x, stick.z) + D3DX_PI;
		pPlayer->move += stick * pPlayer->fMove;
	}

	if (GetTitle() == MENU_SLOPE)
	{// ��̎�
		if (s_bRot)
		{// ��
			pPlayer->move.x *= UP_SPEED;
		}
		else
		{// �E
			pPlayer->move.x *= DOWN_SPEED;
		}
	}

	// �ړ�
	pPlayer->pos.x += pPlayer->move.x;

	// �����E�ړ��ʂ��X�V (����������)
	pPlayer->move.x += (0.0f - pPlayer->move.x) * MAX_INERTIA;

	if (GetTitle() == MENU_SLOPE)
	{// ��
		float fWidth = GetMeshField()->fWidth;

		if (pPlayer->pos.x >= -SLOPE_LIMIT)
		{// �ړ�����
			pPlayer->pos.x = -SLOPE_LIMIT;
		}
		else if (pPlayer->pos.x <= fWidth + SLOPE_LIMIT)
		{// �ړ�����
			pPlayer->pos.x = fWidth + SLOPE_LIMIT;
		}
	}
}

//--------------------------------------------------
// �ړ��̓���
//--------------------------------------------------
static void InputMove(void)
{
	s_bKeyBoardWASD = false;
	s_bKeyBoardArrow = false;
	s_bJoyPad = false;
	s_bStickLeft = false;
	s_bStickRight = false;

	switch (GetMode())
	{
	case MODE_TITLE:		// �^�C�g��

		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D) ||
			GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S))
		{// �L�[�������ꂽ
			s_bKeyBoardWASD = true;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// �L�[�������ꂽ
			s_bKeyBoardArrow = true;
		}

		if (GetJoypadPress(JOYKEY_LEFT) || GetJoypadPress(JOYKEY_RIGHT) ||
			GetJoypadPress(JOYKEY_UP) || GetJoypadPress(JOYKEY_DOWN))
		{// �{�^���������ꂽ
			s_bJoyPad = true;
		}

		if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, 0).y > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, 0).y < -DEAD_ZONE)
		{// ���X�e�B�b�N���X����
			s_bStickLeft = true;
		}

		if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x < -DEAD_ZONE ||
			GetJoypadStick(JOYKEY_RIGHT_STICK, 0).y > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_RIGHT_STICK, 0).y < -DEAD_ZONE)
		{// �E�X�e�B�b�N���X����
			s_bStickRight = true;
		}
		
		break;

	case MODE_GAME:			// �Q�[��

		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
		{// �L�[�������ꂽ
			s_bKeyBoardWASD = true;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT))
		{// �L�[�������ꂽ
			s_bKeyBoardArrow = true;
		}

		if (GetJoypadPress(JOYKEY_LEFT) || GetJoypadPress(JOYKEY_RIGHT))
		{// �{�^���������ꂽ
			s_bJoyPad = true;
		}

		if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x > DEAD_ZONE || 
			GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -DEAD_ZONE)
		{// ���X�e�B�b�N���X����
			s_bStickLeft = true;
		}

		if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x < -DEAD_ZONE)
		{// �E�X�e�B�b�N���X����
			s_bStickRight = true;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// ��]
//--------------------------------------------------
static void Rot(Player *pPlayer)
{
	D3DXVECTOR3 angle;

	// �p�x�̐��K��
	NormalizeRot(&pPlayer->rotDest.x);
	NormalizeRot(&pPlayer->rotDest.y);

	angle.x = pPlayer->rotDest.x - pPlayer->rot.x;
	angle.y = pPlayer->rotDest.y - pPlayer->rot.y;

	// �p�x�̐��K��
	NormalizeRot(&angle.x);
	NormalizeRot(&angle.y);

	//�����E�������X�V (����������)
	pPlayer->rot.x += angle.x * MAX_ATTENUATION;
	pPlayer->rot.y += angle.y * MAX_ATTENUATION;

	// �p�x�̐��K��
	NormalizeRot(&pPlayer->rot.x);
	NormalizeRot(&pPlayer->rot.y);
}

//--------------------------------------------------
// ���[�V����
//--------------------------------------------------
static void Motion(Player * pPlayer)
{
	s_nFrame++;

	switch (GetMode())
	{
	case MODE_TITLE:		// �^�C�g��

		if (s_IdxMotion != MOTION_MOVE)
		{
			if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f ||
				pPlayer->move.z >= 0.1f || pPlayer->move.z <= -0.1f)
			{// ��, ��, ��, ���L�[�������ꂽ
				// ���̃��[�V����
				NextMotion(MOTION_MOVE);
			}
		}

		break;

	case MODE_GAME:			// �Q�[��

		if (GetGame() == GAMESTATE_NORMAL)
		{// �ʏ��� (�Q�[���i�s��)
			switch (GetTitle())
			{// �ǂ̃Q�[���H
			case MENU_WALKING:		// �E�H�[�L���O
			case MENU_SLOPE:		// ��

				if (s_IdxMotion != MOTION_MOVE)
				{
					if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f)
					{// ��, ��, ��, ���L�[�������ꂽ
					 // ���̃��[�V����
						NextMotion(MOTION_MOVE);
					}
				}

				break;

			case MENU_ROTATION:		// ��]
			case MENU_STOP:			// �~�߂�

				/* �����Ȃ� */

				break;

			default:
				assert(false);
				break;
			}
		}

		break;

	default:
		assert(false);
		break;
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

		switch (GetMode())
		{
		case MODE_TITLE:		// �^�C�g��

			if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f ||
				pPlayer->move.z >= 0.1f || pPlayer->move.z <= -0.1f)
			{// ��, ��, ��, ���L�[�������ꂽ
				s_bMotionLoop = false;
			}

			break;

		case MODE_GAME:			// �Q�[��

			if (GetGame() == GAMESTATE_NORMAL)
			{// �ʏ��� (�Q�[���i�s��)
				if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f)
				{// ��, ��, ��, ���L�[�������ꂽ
					s_bMotionLoop = false;
				}
			}

			break;

		default:
			assert(false);
			break;
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

//--------------------------------------------------
// ��̃��[�V����
//--------------------------------------------------
static void MotionSlope(Player *pPlayer)
{
	float fRot = 0.025f;

	switch (GetGame())
	{
	case GAMESTATE_NONE:			// �������Ă��Ȃ����
	case GAMESTATE_START:			// �J�n��� (�Q�[���J�n�O)
	case GAMESTATE_COUNTDOWN:		// �J�E���g�_�E����� (�Q�[���J�n��)
	case GAMESTATE_RESULT:			// ���U���g��� (�Q�[���I����)
	case GAMESTATE_LOOP:			// �J��Ԃ���� (���U���g�I����)

		/* �����Ȃ� */

		break;

	case GAMESTATE_NORMAL:		// �ʏ��� (�Q�[���i�s��)

		if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f)
		{// �ړ����Ă���

			if (GetTitle() == MENU_SLOPE)
			{// ��̎�
				if (s_bRot)
				{// ��
					fRot *= UP_SPEED;
				}
				else
				{// �E
					fRot *= DOWN_SPEED;
				}
			}

			pPlayer->parts[1].rot.x += -D3DX_PI * fRot;
			pPlayer->parts[2].rot.x += -D3DX_PI * fRot;
		}

		break;

	case GAMESTATE_END:			// �I����� (�Q�[���I����)

		if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f)
		{// �ړ����Ă���
			pPlayer->parts[1].rot.x += -D3DX_PI * (s_fSlopeMove * 0.005f);
			pPlayer->parts[2].rot.x += -D3DX_PI * (s_fSlopeMove * 0.005f);
		}

		break;

	default:
		assert(false);
		break;
	}
}

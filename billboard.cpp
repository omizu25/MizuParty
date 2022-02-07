//==================================================
// 
// 3D�Q�[������ ( billboard.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "effect.h"
#include "field.h"
#include "particle.h"
#include "player.h"
#include "setup.h"
#include "wall.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_BILLBOARD		(256)			// �r���{�[�h�̍ő吔
#define MAX_TEXTURE			(256)			// �e�N�X�`���̍ő吔
#define DO_NOT_ROT_Y		(0)				// Y����]�����Ȃ����l
#define DO_NOT_RESULT		(0)				// ���U���g�ŕ\�����Ȃ�
#define TARGET_WIDTH		(250.0f)		// ��
#define TARGET_HEIGHT		(600.0f)		// ����
#define CHEAT_WIDTH			(300.0f)		// ��
#define CHEAT_HEIGHT		(2000.0f)		// ����
#define PLAYER_WIDTH		(300.0f)		// ��
#define PLAYER_HEIGHT		(1500.0f)		// ����

//--------------------------------------------------
// �\����
//--------------------------------------------------

/*�� �r���{�[�h ��*/

typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	D3DXVECTOR3				move;			// �ړ���
	D3DXMATRIX				mtxWorld;		// ���[���h�}�g���b�N�X
	float					fWidth;			// ��
	float					fHeight;		// ����
	bool					bUse;			// �g�p���Ă��邩�ǂ���
	bool					bYRot;			// Y����]�����邩�ǂ���
	bool					bResult;		// ���U���g�����ŕ\������
	bool					bCamera;		// �J�������Ԗڂ�
	LPDIRECT3DTEXTURE9		pTexture;		// �e�N�X�`��
}Billboard;

/*�� �ǂݍ��ޓ��e ��*/

typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	float					fWidth;			// ��
	float					fHeight;		// ����
	int						nTexIdx;		// �e�N�X�`���ԍ�
	int						nYRot;			// Y����]�����邩�ǂ���
	int						nResult;		// ���U���g�ŕ\������
	int						nCamera;		// �J�������Ԗڂ�
	LPDIRECT3DTEXTURE9		pTexture;		// �e�N�X�`��
}Text;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			*s_pTexture;					// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureTarget;				// �ڕW�n�_�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureTargetCheat;			// ����������ڕW�n�_�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTexturePlayer;				// ���]�Ԃ̃e�N�X�`���ւ̃|�C���^
static Billboard					s_billboard[MAX_BILLBOARD];		// �r���{�[�h�̏��
static int							s_nUseTex;						// �e�N�X�`���̎g�p��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void System(FILE *pFile, char *aFile);
static void Load(FILE *pFile);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitBillboard(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//�������̃N���A
	memset(s_billboard, 0, sizeof(s_billboard));

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		//�S�Ă̏���������
		InitAll(pVtx);

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
		for (int i = 0; i < s_nUseTex; i++)
		{
			if (s_pTexture[i] != NULL)
			{
				s_pTexture[i]->Release();
				s_pTexture[i] = NULL;
			}
		}

		delete[] s_pTexture;
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pTextureTarget != NULL)
	{// �e�N�X�`���̉��
		s_pTextureTarget->Release();
		s_pTextureTarget = NULL;
	}

	if (s_pTextureTargetCheat != NULL)
	{// �e�N�X�`���̉��
		s_pTextureTargetCheat->Release();
		s_pTextureTargetCheat = NULL;
	}

	if (s_pTexturePlayer != NULL)
	{// �e�N�X�`���̉��
		s_pTexturePlayer->Release();
		s_pTexturePlayer = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateBillboard(void)
{
	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (!pBillboard->bUse || pBillboard->pTexture != s_pTexturePlayer)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		D3DXVECTOR3 pos = D3DXVECTOR3(GetPlayer()->pos.x, GetPlayer()->pos.y, 0.0f);

		pBillboard->pos = pos;
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawBillboard(bool bResult, bool bCamera)
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

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (!pBillboard->bUse || pBillboard->bResult != bResult || pBillboard->bCamera != bCamera)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pBillboard->mtxWorld);
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �J�����̋t�s���ݒ�
		if (pBillboard->bYRot)
		{// Y����]������
			pBillboard->mtxWorld._11 = mtxView._11;
			pBillboard->mtxWorld._12 = mtxView._21;
			pBillboard->mtxWorld._13 = mtxView._31;
			pBillboard->mtxWorld._21 = mtxView._12;
			pBillboard->mtxWorld._22 = mtxView._22;
			pBillboard->mtxWorld._23 = mtxView._32;
			pBillboard->mtxWorld._31 = mtxView._13;
			pBillboard->mtxWorld._32 = mtxView._23;
			pBillboard->mtxWorld._33 = mtxView._33;
		}
		else
		{// Y����]�����Ȃ�
			pBillboard->mtxWorld._11 = mtxView._11;
			pBillboard->mtxWorld._13 = mtxView._31;
			pBillboard->mtxWorld._31 = mtxView._13;
			pBillboard->mtxWorld._33 = mtxView._33;
		}

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);
		D3DXMatrixMultiply(&pBillboard->mtxWorld, &pBillboard->mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pBillboard->mtxWorld);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pBillboard->pTexture);

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
// �ݒ�
//--------------------------------------------------
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, bool bYRot, bool bResult, bool bCamera, LPDIRECT3DTEXTURE9 *pTexture)
{
	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (pBillboard->bUse)
		{//�g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		pBillboard->pos = pos;
		pBillboard->move = move;
		pBillboard->fWidth = fWidth;
		pBillboard->fHeight = fHeight;
		pBillboard->pTexture = *pTexture;
		pBillboard->bYRot = bYRot;
		pBillboard->bResult = bResult;
		pBillboard->bCamera = bCamera;
		pBillboard->bUse = true;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		// ���_�̖@���̐ݒ�
		Setnor(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void LoadBillboard(void)
{
	FILE *pFile;			// �t�@�C���|�C���^��錾

	char aFile[MAX_TEXT];

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	// �V�X�e��
	System(pFile, aFile);

	// �t�@�C�����J��
	pFile = fopen(aFile, "r");

	// �ǂݍ���
	Load(pFile);
}

//--------------------------------------------------
// �V�X�e��
//--------------------------------------------------
static void System(FILE *pFile, char *aFile)
{
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

			if (strcmp(&aRead[0], "BILLBOARD_FILENAME") == 0)
			{// ���f���̏��
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", aFile);
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
// �ǂݍ���
//--------------------------------------------------
static void Load(FILE *pFile)
{
	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[MAX_TEXT] = {};
		char aTexture[MAX_TEXT];
		int nTex = 0, nText = 0, nNumBillBoard = 0;
		Text *pText;

		//�������̃N���A
		memset(&pText, 0, sizeof(pText));

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

			if (strcmp(&aRead[0], "NUM_TEXTURE") == 0)
			{// �e�N�X�`���̎g�p��
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%d", &s_nUseTex);

				// txt�ɏ����Ă�ő吔���̃e�N�X�`���̔z���p�ӂ���
				s_pTexture = new LPDIRECT3DTEXTURE9[s_nUseTex];
			}
			else if (strcmp(&aRead[0], "TEXTURE_FILENAME") == 0)
			{// �e�N�X�`���̏��
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", aTexture);

				// �f�o�C�X�ւ̃|�C���^�̎擾
				LPDIRECT3DDEVICE9 pDevice = GetDevice();

				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(
					pDevice,
					&aTexture[0],
					&s_pTexture[nTex]);

				nTex++;
			}
			else if (strcmp(&aRead[0], "NUM_MODEL") == 0)
			{// �r���{�[�h�̎g�p��
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%d", &nNumBillBoard);

				// txt�ɏ����Ă�ő吔���̓ǂݍ��ݗp�̔z���p�ӂ���
				pText = new Text[nNumBillBoard];
			}
			else if (strcmp(&aRead[0], "BILLBOARD_SET") == 0)
			{// �r���{�[�h�̏��
				while (strcmp(&aRead[0], "END_BILLBOARD_SET") != 0)
				{// �I��肪����܂ŌJ��Ԃ�
					fscanf(pFile, "%s", &aRead);

					if (strncmp(&aRead[0], "#", 1) == 0)
					{// �R�����g
						fgets(aRead, MAX_TEXT, pFile);
						continue;
					}

					if (strcmp(&aRead[0], "TEXIDX") == 0)
					{// �e�N�X�`���ԍ�
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pText[nText].nTexIdx);
					}
					else if (strcmp(&aRead[0], "YROT") == 0)
					{// Y��]�����邩�ǂ���
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pText[nText].nYRot);
					}
					else if (strcmp(&aRead[0], "RESULT") == 0)
					{// ���U���g�ŕ\������
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pText[nText].nResult);
					}
					else if (strcmp(&aRead[0], "CAMERA") == 0)
					{// �J���������Ԗڂ�
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pText[nText].nCamera);
					}
					else if (strcmp(&aRead[0], "WIDTH") == 0)
					{// ��
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pText[nText].fWidth);
					}
					else if (strcmp(&aRead[0], "HEIGHT") == 0)
					{// ����
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pText[nText].fHeight);
					}
					else if (strcmp(&aRead[0], "POS") == 0)
					{// �ʒu
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pText[nText].pos.x);
						fscanf(pFile, "%f", &pText[nText].pos.y);
						fscanf(pFile, "%f", &pText[nText].pos.z);
					}
				}
				nText++;
			}
		}

		// �t�@�C�������
		fclose(pFile);

		for (int i = 0; i < nNumBillBoard; i++)
		{
			pText[i].pTexture = s_pTexture[pText[i].nTexIdx];

			bool bYRot = true;
			bool bResult = true;
			bool bCamera = true;

			if (pText[i].nYRot == DO_NOT_ROT_Y)
			{// Y����]�����Ȃ����l�̎�
				bYRot = false;
			}

			if (pText[i].nResult == DO_NOT_RESULT)
			{// Y����]�����Ȃ����l�̎�
				bResult = false;
			}

			if (pText[i].nCamera == 0)
			{// Y����]�����Ȃ����l�̎�
				bCamera = false;
			}

			// �ݒ�
			SetBillboard(pText[i].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pText[i].fWidth, pText[i].fHeight, bYRot, bResult, bCamera, &pText[i].pTexture);
		}

		delete[] pText;

		pText = NULL;
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//--------------------------------------------------
// ��
//--------------------------------------------------
void InitBillboardSlope(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/target000.png",
		&s_pTextureTarget);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/target001.png",
		&s_pTextureTargetCheat);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/player.png",
		&s_pTexturePlayer);

	float fWidth = TARGET_WIDTH * 0.5f;
	float fHeight = TARGET_HEIGHT * 0.5f;

	float fPosX = (GetField()->pos.x + GetField()->vtxMax.x);
	
	D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, fHeight + 100.0f, 30.0f);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, move, fWidth, fHeight, true, false, false, &s_pTextureTarget);

	fWidth = CHEAT_WIDTH * 0.5f;
	fHeight = CHEAT_HEIGHT * 0.5f;

	fPosX = (GetField()->pos.x + GetField()->vtxMax.x);

	pos = D3DXVECTOR3(fPosX, 0.0f, 0.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, move, fWidth, fHeight, true, false, true, &s_pTextureTargetCheat);
	
	fWidth = PLAYER_WIDTH * 0.5f;
	fHeight = PLAYER_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(GetPlayer()->pos.x * 0.5f, GetPlayer()->pos.y + 400.0f, 30.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, move, fWidth, fHeight, true, false, true, &s_pTexturePlayer);
}
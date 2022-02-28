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
#include "fade.h"
#include "field.h"
#include "frame.h"
#include "game.h"
#include "particle.h"
#include "player.h"
#include "setup.h"
#include "sound.h"
#include "title.h"
#include "wall.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_TEXTURE			(256)			// �e�N�X�`���̍ő吔
#define DO_NOT_ROT_Y		(0)				// Y����]�����Ȃ����l
#define DO_NOT_RESULT		(0)				// ���U���g�ŕ\�����Ȃ�
#define TARGET_WIDTH		(250.0f)		// �ڕW�n�_�̕�
#define TARGET_HEIGHT		(600.0f)		// �ڕW�n�_�̍���
#define CHEAT_WIDTH			(300.0f)		// ����������ڕW�n�_�̕�
#define CHEAT_HEIGHT		(2000.0f)		// ����������ڕW�n�_�̍���
#define PLAYER_WIDTH		(300.0f)		// ���]�Ԃ̕�
#define PLAYER_HEIGHT		(2000.0f)		// ���]�Ԃ̍���
#define TITLE_WIDTH			(130.0f)		// ���j���[�̕�
#define TITLE_HEIGHT		(120.0f)		// ���j���[�̍���
#define RULE_WIDTH			(180.0f)		// ���[���̕�
#define RULE_HEIGHT			(180.0f)		// ���[���̍���
#define MOVE_WIDTH			(180.0f)		// �ړ��̕�
#define MOVE_HEIGHT			(80.0f)			// �ړ��̍���
#define REMIX_WIDTH			(150.0f)		// ���~�b�N�X�̕�
#define REMIX_HEIGHT		(60.0f)			// ���~�b�N�X�̍���
#define MIZU_WIDTH			(150.0f)		// Mizu�̕�
#define MIZU_HEIGHT			(100.0f)		// Mizu�̍���
#define PARTY_WIDTH			(150.0f)		// Party�̕�
#define PARTY_HEIGHT		(100.0f)		// Party�̍���
#define LOGO_WIDTH			(100.0f)		// !!�̕�
#define LOGO_HEIGHT			(100.0f)		// !!�̍���
#define LOGO_CHANGE			(0.4f)			// ���S�̕ω�

//--------------------------------------------------
// �\����
//--------------------------------------------------
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
static LPDIRECT3DTEXTURE9			s_pTextureWalking;				// �E�H�[�L���O�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureRotation;				// ��]�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureStop;					// �~�߂�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureSlope;				// ��̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureRule;					// �����̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureMove;					// �ړ��̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureRemix;				// ���~�b�N�X�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureMizu;					// Mizu�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureParty;				// Party�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureLogo;					// !!�̃e�N�X�`���ւ̃|�C���^
static Billboard					s_billboard[MAX_BILLBOARD];		// �r���{�[�h�̏��
static int							s_nUseTex;						// �e�N�X�`���̎g�p��
static int							s_nTime;						// �^�C��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void System(FILE *pFile, char *aFile);
static void Load(FILE *pFile);
static void TitleMenu(void);
static void TitleRule(void);
static void TitleLogo(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitBillboard(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nTime = 0;

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
			{// �e�N�X�`���̉��
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

	if (s_pTextureWalking != NULL)
	{// �e�N�X�`���̉��
		s_pTextureWalking->Release();
		s_pTextureWalking = NULL;
	}

	if (s_pTextureRotation != NULL)
	{// �e�N�X�`���̉��
		s_pTextureRotation->Release();
		s_pTextureRotation = NULL;
	}

	if (s_pTextureStop != NULL)
	{// �e�N�X�`���̉��
		s_pTextureStop->Release();
		s_pTextureStop = NULL;
	}

	if (s_pTextureSlope != NULL)
	{// �e�N�X�`���̉��
		s_pTextureSlope->Release();
		s_pTextureSlope = NULL;
	}

	if (s_pTextureRule != NULL)
	{// �e�N�X�`���̉��
		s_pTextureRule->Release();
		s_pTextureRule = NULL;
	}

	if (s_pTextureMove != NULL)
	{// �e�N�X�`���̉��
		s_pTextureMove->Release();
		s_pTextureMove = NULL;
	}

	if (s_pTextureRemix != NULL)
	{// �e�N�X�`���̉��
		s_pTextureRemix->Release();
		s_pTextureRemix = NULL;
	}

	if (s_pTextureMizu != NULL)
	{// �e�N�X�`���̉��
		s_pTextureMizu->Release();
		s_pTextureMizu = NULL;
	}

	if (s_pTextureParty != NULL)
	{// �e�N�X�`���̉��
		s_pTextureParty->Release();
		s_pTextureParty = NULL;
	}

	if (s_pTextureLogo != NULL)
	{// �e�N�X�`���̉��
		s_pTextureLogo->Release();
		s_pTextureLogo = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateBillboard(void)
{
	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	float fSize = 0.0f, fWidth = 0.0f, fHeight = 0.0f;

	switch (GetMode())
	{
	case MODE_TITLE:		// �^�C�g��

		s_nTime++;
		fSize = sinf((s_nTime * 0.01f) * (D3DX_PI * 2.0f)) * LOGO_CHANGE;
		fSize = fSize;

		for (int i = 0; i < MAX_BILLBOARD; i++)
		{
			Billboard *pBillboard = &s_billboard[i];

			if (!pBillboard->bUse)
			{//�g�p����Ă��Ȃ�
				continue;
			}
			
			if (pBillboard->pTexture != s_pTextureMizu &&
				pBillboard->pTexture != s_pTextureParty &&
				pBillboard->pTexture != s_pTextureLogo)
			{// �e�N�X�`�����Ⴄ
				continue;
			}

			/*�� �g�p���Ă��� ��*/

			fWidth = pBillboard->fWidth;
			fHeight = pBillboard->fHeight;

			pBillboard->fWidth += fSize;
			pBillboard->fHeight += fSize;

			// ���_�������b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

			// ���_���W�̐ݒ�
			Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pBillboard->fWidth, pBillboard->fHeight, 0.0f, SETPOS_MIDDLE);

			// ���_�o�b�t�@���A�����b�N����
			s_pVtxBuff->Unlock();
		}

		break;

	case MODE_GAME:			// �Q�[��

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

		break;

	default:
		assert(false);
		break;
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
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight, bool bYRot, bool bResult, bool bCamera, LPDIRECT3DTEXTURE9 *pTexture)
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
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f, SETPOS_MIDDLE);

		// ���_�̖@���̐ݒ�
		Setnor(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Billboard *GetBillboard(void)
{
	return &s_billboard[0];
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
// �����蔻��
//--------------------------------------------------
void CollisionBillboard(void)
{
	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (!pBillboard->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		if (pBillboard->pTexture != s_pTextureWalking &&
			pBillboard->pTexture != s_pTextureRotation &&
			pBillboard->pTexture != s_pTextureStop && 
			pBillboard->pTexture != s_pTextureSlope &&
			pBillboard->pTexture != s_pTextureRemix)
		{// �e�N�X�`�����Ⴄ
			continue;
		}

		if (GetTitleCnt() < REMIX_OK)
		{
			if (pBillboard->pTexture == s_pTextureRemix)
			{// �e�N�X�`�����Ⴄ
				continue;
			}
		}

		D3DXVECTOR3 pos = GetPlayer()->pos;

		if (pBillboard->pos.x + pBillboard->fWidth >= pos.x &&
			pBillboard->pos.x - pBillboard->fWidth <= pos.x &&
			pBillboard->pos.z + pBillboard->fHeight >= pos.z &&
			pBillboard->pos.z - pBillboard->fHeight <= pos.z)
		{
			if (pBillboard->pTexture == s_pTextureWalking)
			{
				// �^�C�g���̐ݒ�
				SetTitle(MENU_WALKING);

				// ���~�b�N�X�ݒ�
				SetRemix(false);
			}
			else if (pBillboard->pTexture == s_pTextureRotation)
			{
				// �^�C�g���̐ݒ�
				SetTitle(MENU_ROTATION);

				// ���~�b�N�X�ݒ�
				SetRemix(false);
			}
			else if (pBillboard->pTexture == s_pTextureStop)
			{
				// �^�C�g���̐ݒ�
				SetTitle(MENU_STOP);

				// ���~�b�N�X�ݒ�
				SetRemix(false);
			}
			else if (pBillboard->pTexture == s_pTextureSlope)
			{
				// �^�C�g���̐ݒ�
				SetTitle(MENU_SLOPE);

				// ���~�b�N�X�ݒ�
				SetRemix(false);
			}
			else if (pBillboard->pTexture == s_pTextureRemix)
			{
				// �^�C�g���̐ݒ�
				SetTitle(MENU_WALKING);

				// ���~�b�N�X�ݒ�
				SetRemix(true);
			}

			// �t�F�[�h�̐ݒ�
			SetFade(MODE_GAME);

			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_���艹);
		}
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
	
	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureTarget);

	fWidth = CHEAT_WIDTH * 0.5f;
	fHeight = CHEAT_HEIGHT * 0.5f;

	fPosX = (GetField()->pos.x + GetField()->vtxMax.x);

	pos = D3DXVECTOR3(fPosX, 0.0f, 0.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, true, &s_pTextureTargetCheat);

	fWidth = PLAYER_WIDTH * 0.5f;
	fHeight = PLAYER_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(GetPlayer()->pos.x * 0.5f, GetPlayer()->pos.y + 400.0f, 30.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, true, &s_pTexturePlayer);
}

//--------------------------------------------------
// �^�C�g��
//--------------------------------------------------
void InitBillboardTitle(void)
{
	// �^�C�g���̐���
	TitleRule();

	// �^�C�g���̃��j���[
	TitleMenu();

	// �^�C�g�����S
	TitleLogo();
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
			SetBillboard(pText[i].pos, pText[i].fWidth, pText[i].fHeight, bYRot, bResult, bCamera, &pText[i].pTexture);
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
// �^�C�g���̃��j���[
//--------------------------------------------------
static void TitleMenu(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/menu000.png",
		&s_pTextureWalking);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/menu003.png",
		&s_pTextureRotation);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/menu001.png",
		&s_pTextureStop);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/menu002.png",
		&s_pTextureSlope);

	float fWidth = TITLE_WIDTH * 0.5f;
	float fHeight = TITLE_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(-210.0f, 0.0f, -90.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureWalking);

	// �g�̐ݒ�
	SetFrame(pos, fWidth, fHeight, &s_pTextureWalking);

	pos.x = -70.0f;

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureRotation);

	// �g�̐ݒ�
	SetFrame(pos, fWidth, fHeight, &s_pTextureRotation);

	pos.x = 70.0f;

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureStop);

	// �g�̐ݒ�
	SetFrame(pos, fWidth, fHeight, &s_pTextureStop);

	pos.x = 210.0f;

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureSlope);

	// �g�̐ݒ�
	SetFrame(pos, fWidth, fHeight, &s_pTextureSlope);
}

//--------------------------------------------------
// �^�C�g���̐���
//--------------------------------------------------
static void TitleRule(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title000.png",
		&s_pTextureRule);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title001.png",
		&s_pTextureMove);

	float fWidth = RULE_WIDTH * 0.5f;
	float fHeight = RULE_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(-210.0f, 0.0f, 100.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureRule);

	if (GetTitleCnt() >= REMIX_OK)
	{// ���~�b�N�X�o����
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			"data/TEXTURE/Remix.png",
			&s_pTextureRemix);

		fWidth = REMIX_WIDTH * 0.5f;
		fHeight = REMIX_HEIGHT * 0.5f;

		pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);

		// �r���{�[�h�̐ݒ�
		SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureRemix);

		// �g�̐ݒ�
		SetFrame(pos, fWidth, fHeight, &s_pTextureRemix);
	}

	fWidth = MOVE_WIDTH * 0.5f;
	fHeight = MOVE_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(200.0f, 0.0f, 50.0f);
	
	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureMove);
}

//--------------------------------------------------
// �^�C�g�����S
//--------------------------------------------------
static void TitleLogo(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title002.png",
		&s_pTextureMizu);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title003.png",
		&s_pTextureParty);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title004.png",
		&s_pTextureLogo);

	float fWidth = MIZU_WIDTH * 0.5f;
	float fHeight = MIZU_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(-30.0f, 0.0f, 130.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureMizu);

	fWidth = PARTY_WIDTH * 0.5f;
	fHeight = PARTY_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(140.0f, 0.0f, 130.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureParty);

	fWidth = LOGO_WIDTH * 0.5f;
	fHeight = LOGO_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(275.0f, 0.0f, 125.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureLogo);
}
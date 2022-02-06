//==================================================
// 
// 3D�Q�[������ ( target.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "game.h"
#include "number.h"
#include "setup.h"
#include "title.h"

#include <assert.h>
#include <time.h>
//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_DIGITS					(2)				// �����̍ő�l
#define MIN_TARGET					(37)			// �ڕW�̍ŏ��l
#define MAX_RANDOM					(30)			// �����_���̍ő�l
#define START_NUMBER_WIDTH			(120.0f)		// �X�^�[�g�̐��̕�
#define START_NUMBER_HEIGHT			(200.0f)		// �X�^�[�g�̐��̍���
#define START_WIDTH_INTERVAL		(5.0f)			// �X�^�[�g�̐��̕��̊Ԋu
#define START_METER_WIDTH			(80.0f)			// �X�^�[�g�̃��[�g���̕�
#define START_METER_HEIGHT			(140.0f)		// �X�^�[�g�̃��[�g���̍���
#define START_WIDTH					(500.0f)		// �X�^�[�g�̕�
#define START_HEIGHT				(200.0f)		// �X�^�[�g�̍���
#define GAME_NUMBER_WIDTH			(80.0f)			// �Q�[���̐��̕�
#define GAME_NUMBER_HEIGHT			(140.0f)		// �Q�[���̐��̍���
#define GAME_WIDTH_INTERVAL			(5.0f)			// �Q�[���̐��̕��̊Ԋu
#define GAME_METER_WIDTH			(80.0f)			// �Q�[���̃��[�g���̕�
#define GAME_METER_HEIGHT			(140.0f)		// �Q�[���̃��[�g���̍���
#define STOP_WIDTH					(1000.0f)		// �X�g�b�v�̕�
#define STOP_HEIGHT					(600.0f)		// �X�g�b�v�̍���
#define JUST_WIDTH					(1000.0f)		// �҂�����̕�
#define JUST_HEIGHT					(400.0f)		// �҂�����̍���
#define SLOPE_WIDTH					(400.0f)		// ��̕�
#define SLOPE_HEIGHT				(200.0f)		// ��̍���

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;					// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;					// ���_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureMeter = NULL;				// ���[�g���̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffStartMeter = NULL;		// �X�^�[�g�̃��[�g���̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffGameMeter = NULL;			// �Q�[���̃��[�g���̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureStop = NULL;				// �X�g�b�v�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffStop = NULL;				// �X�g�b�v�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureJust = NULL;				// �҂�����̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffJust = NULL;				// �҂�����̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureSlope = NULL;				// ��̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffSlope = NULL;				// ��̒��_�o�b�t�@�̃|�C���^
static int							s_nTarget;							// �ڕW�n�_

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void InitStartNumber(void);
static void InitStartMeter(void);
static void InitStartTarget(void);
static void InitGameNumber(void);
static void InitGameMeter(void);
static void DrawStartMeter(void);
static void DrawGameMeter(void);
static void InitStop(void);
static void InitJust(void);
static void InitSlope(void);
static void DrawStartSlope(void);
static void DrawGameSlope(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitTarget(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O

		//���E�̎�q�̏�����
		srand((unsigned)time(NULL));

		s_nTarget = (rand() % MAX_RANDOM) + MIN_TARGET;

		// �X�^�[�g�̐��̏�����
		InitStartNumber();

		// �X�^�[�g�̃��[�g���̏�����
		InitStartMeter();

		// �X�^�[�g�̖ڎw���̏�����
		InitStartTarget();

		// �Q�[���̐��̏�����
		InitGameNumber();

		// �Q�[���̃��[�g���̏�����
		InitGameMeter();

		break;

	case MENU_STOP:			// �X�g�b�v

		// �~�߂�
		InitStop();

		break;

	case MENU_SLOPE:		// ��

		// �҂�����
		InitJust();

		// ��
		InitSlope();

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitTarget(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̔j��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pTextureMeter != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureMeter->Release();
		s_pTextureMeter = NULL;
	}

	if (s_pVtxBuffStartMeter != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffStartMeter->Release();
		s_pVtxBuffStartMeter = NULL;
	}

	if (s_pVtxBuffGameMeter != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffGameMeter->Release();
		s_pVtxBuffGameMeter = NULL;
	}

	if (s_pTextureStop != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureStop->Release();
		s_pTextureStop = NULL;
	}

	if (s_pVtxBuffStop != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffStop->Release();
		s_pVtxBuffStop = NULL;
	}

	if (s_pTextureJust != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureJust->Release();
		s_pTextureJust = NULL;
	}

	if (s_pVtxBuffJust != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffJust->Release();
		s_pVtxBuffJust = NULL;
	}

	if (s_pTextureSlope != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureSlope->Release();
		s_pTextureSlope = NULL;
	}

	if (s_pVtxBuffSlope != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffSlope->Release();
		s_pVtxBuffSlope = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateTarget(void)
{
	
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawTarget(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O

		if (GetGame().gameState == GAMESTATE_START)
		{// �X�^�[�g���
			// �X�^�[�g�̕`��
			DrawStartMeter();
		}
		else if (GetGame().gameState != GAMESTATE_START)
		{// �Q�[�����
			// �Q�[���̕`��
			DrawGameMeter();
		}
		
		break;

	case MENU_STOP:			// �X�g�b�v

		if (GetGame().gameState == GAMESTATE_START)
		{// �X�^�[�g���
			// �f�o�C�X�ւ̃|�C���^�̎擾
			LPDIRECT3DDEVICE9 pDevice = GetDevice();

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, s_pVtxBuffStop, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTextureStop);

			// �|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);							// �v���~�e�B�u(�|���S��)��
		}

		break;

	case MENU_SLOPE:		// ��

		if (GetGame().gameState == GAMESTATE_START)
		{// �X�^�[�g���
			// �X�^�[�g�̕`��
			DrawStartSlope();
		}
		else if (GetGame().gameState != GAMESTATE_START)
		{// �Q�[�����
			// �Q�[���̕`��
			DrawGameSlope();
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �X�^�[�g�̐���
//--------------------------------------------------
static void InitStartNumber(void)
{
	float fWidth = SCREEN_WIDTH * 0.25f;
	float fHeight = SCREEN_HEIGHT * 0.75f;

	// �ʒu��������
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	int aNumber[MAX_DIGITS];
	int nTarget = s_nTarget;

	for (int i = 0; i < MAX_DIGITS; i++)
	{
		aNumber[i] = 0;
	}

	for (int i = 0; i < MAX_DIGITS; i++)
	{// �P�����ɕ�����
		aNumber[i] = nTarget % 10;
		nTarget /= 10;

		float fInterval = (START_NUMBER_WIDTH * i) + (START_WIDTH_INTERVAL * i);
		float fCenter = (START_NUMBER_WIDTH * 0.5) * (MAX_DIGITS - 1) + (START_WIDTH_INTERVAL * 0.5f) * (MAX_DIGITS - 1);

		D3DXVECTOR3 posNumber = D3DXVECTOR3((pos.x) - fInterval + fCenter + (START_NUMBER_WIDTH * 0.5f), pos.y, 0.0f);

		// ���̐ݒ�
		SetRightNumber(posNumber, START_NUMBER_WIDTH, START_NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_START);
	}
}

//--------------------------------------------------
// �X�^�[�g�̃��[�g��
//--------------------------------------------------
static void InitStartMeter(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/METER/m.png",
		&s_pTextureMeter);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffStartMeter,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffStartMeter->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = START_METER_WIDTH * 0.5f;
	float fHeight = START_METER_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.375f, SCREEN_HEIGHT * 0.81f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̐ݒ菈��
	Setcol(pVtx, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffStartMeter->Unlock();
}

//--------------------------------------------------
// �X�^�[�g�̖ڎw���ꏊ
//--------------------------------------------------
static void InitStartTarget(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/game000.png",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, START_WIDTH, START_HEIGHT * 0.5f, SETPOS_LEFT);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̐ݒ菈��
	Setcol(pVtx, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �Q�[���̐���
//--------------------------------------------------
static void InitGameNumber(void)
{
	float fWidth = SCREEN_WIDTH * 0.1f;
	float fHeight = SCREEN_HEIGHT * 0.15f;

	// �ʒu��������
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	int aNumber[MAX_DIGITS];
	int nTarget = s_nTarget;

	for (int i = 0; i < MAX_DIGITS; i++)
	{
		aNumber[i] = 0;
	}

	for (int i = 0; i < MAX_DIGITS; i++)
	{// �P�����ɕ�����
		aNumber[i] = nTarget % 10;
		nTarget /= 10;

		float fInterval = (GAME_NUMBER_WIDTH * i) + (GAME_WIDTH_INTERVAL * i);
		float fCenter = (GAME_NUMBER_WIDTH * 0.5) * (MAX_DIGITS - 1) + (GAME_WIDTH_INTERVAL * 0.5f) * (MAX_DIGITS - 1);

		D3DXVECTOR3 posNumber = D3DXVECTOR3((pos.x) - fInterval + fCenter + (GAME_NUMBER_WIDTH * 0.5f), pos.y, 0.0f);

		// ���̐ݒ�
		SetRightNumber(posNumber, GAME_NUMBER_WIDTH, GAME_NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_GAME);
	}
}

//--------------------------------------------------
// �Q�[���̃��[�g��
//--------------------------------------------------
static void InitGameMeter(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffGameMeter,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffGameMeter->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.225f;
	float fHeight = SCREEN_HEIGHT * 0.175f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// ���_���W�̐ݒ�
	Setpos(pVtx, pos, GAME_METER_WIDTH, GAME_METER_HEIGHT * 0.5f, SETPOS_RIGHT);

	// rhw�̏�����
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏�����
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffGameMeter->Unlock();
}

//--------------------------------------------------
// �X�^�[�g�̕`��
//--------------------------------------------------
static void DrawStartMeter(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffStartMeter, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureMeter);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��
}

//--------------------------------------------------
// �Q�[���̕`��
//--------------------------------------------------
static void DrawGameMeter(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffGameMeter, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureMeter);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��
}

//--------------------------------------------------
// �~�߂�
//--------------------------------------------------
static void InitStop(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/game001.png",
		&s_pTextureStop);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffStop,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffStop->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = STOP_WIDTH * 0.5f;
	float fHeight = STOP_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.35f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffStop->Unlock();
}

//--------------------------------------------------
// �҂�����
//--------------------------------------------------
static void InitJust(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/game002.png",
		&s_pTextureJust);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffJust,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffJust->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = JUST_WIDTH * 0.5f;
	float fHeight = JUST_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffJust->Unlock();

}

//--------------------------------------------------
// ��
//--------------------------------------------------
static void InitSlope(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/game003.png",
		&s_pTextureSlope);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffSlope,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffSlope->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SLOPE_WIDTH * 0.5f;
	float fHeight = SLOPE_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.15f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffSlope->Unlock();
}

//--------------------------------------------------
// ��
//--------------------------------------------------
static void DrawStartSlope(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffJust, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureJust);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��
}

//--------------------------------------------------
// ��
//--------------------------------------------------
static void DrawGameSlope(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffSlope, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureSlope);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��
}

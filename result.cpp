//==================================================
// 
// 3D�Q�[������ ( result.cpp )
// Author  : katsuki mizuki
// 
//==================================================
#include "fade.h"
#include "field.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "number.h"
#include "player.h"
#include "result.h"
#include "setup.h"
#include "sound.h"
#include "time.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define RESULT_WIDTH			(450.0f)		// ��
#define RESULT_HEIGHT			(180.0f)		// ����
#define METER_WIDTH				(100.0f)		// ���[�g���̕�
#define METER_HEIGHT			(160.0f)		// ���[�g���̍���
#define MINUS_WIDTH				(60.0f)			// �}�C�i�X�̕�
#define MINUS_HEIGHT			(100.0f)		// �}�C�i�X�̍���
#define GAMEOVER_WIDTH			(1000.0f)		// �Q�[���I�[�o�[�̕�
#define GAMEOVER_HEIGHT			(300.0f)		// �Q�[���I�[�o�[�̍���
#define NUMBER_WIDTH			(100.0f)		// ���̕�
#define NUMBER_HEIGHT			(200.0f)		// ���̍���
#define DECIMAL_WIDTH			(80.0f)			// �����_�̕�
#define DECIMAL_HEIGHT			(150.0f)		// �����_�̍���
#define DECIMAL_INTERVAL		(40.0f)			// �����̊Ԋu
#define MAX_RESULT				(4)				// ���U���g�̍ő吔
#define MIN_RESULT				(2)				// ���U���g�̍ŏ���
#define STOP_SCORE				(-1)			// �~�߂�̃X�R�A�̔�����

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// ���_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureMeter = NULL;			// ���[�g���̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMeter = NULL;			// ���[�g���̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureMinus = NULL;			// �}�C�i�X�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMinus = NULL;			// �}�C�i�X�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureDecimal = NULL;		// �����_�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffDecimal = NULL;		// �����_�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureGameOver = NULL;		// �Q�[���I�[�o�[�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffGameOver = NULL;		// �Q�[���I�[�o�[�̒��_�o�b�t�@�̃|�C���^
static int							s_nTime;						// �^�C��
static int							s_nDigit;						// ����
static RESULT						s_result;						// ���U���g

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void InitClear(void);
static void InitGameOver(void);
static void InitPosNumber(void);
static void InitMeter(void);
static void InitDecimal(void);
static void InitMinus(void);
static void DrawClear(void);
static void DrawGameOver(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitResult(void)
{
	s_nTime = 0;
	s_nDigit = 0;

	switch (s_result)
	{
	case RESULT_CLEAR:			// �N���A

		// �N���A
		InitClear();

		// �T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_�Q�[���N���A);

		break;

	case RESULT_GAMEOVER:		// �Q�[���I�[�o�[

		// �Q�[���I�[�o�[
		InitGameOver();

		// �T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_�Q�[���I�[�o�[);

		break;

	case RESULT_NONE:			// �܂����܂��ĂȂ�

		/* �����Ȃ� */

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitResult(void)
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

	if (s_pVtxBuffMeter != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffMeter->Release();
		s_pVtxBuffMeter = NULL;
	}

	if (s_pVtxBuffMinus != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffMinus->Release();
		s_pVtxBuffMinus = NULL;
	}

	if (s_pTextureGameOver != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureGameOver->Release();
		s_pTextureGameOver = NULL;
	}

	if (s_pVtxBuffGameOver != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffGameOver->Release();
		s_pVtxBuffGameOver = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateResult(void)
{
	if (GetKeyboardAllTrigger() || GetJoypadAllTrigger())
	{// �{�^���������ꂽ���ǂ���

		if (GetRemix())
		{// ���~�b�N�X��
			int nMenu = (int)GetTitle() + 1;

			if (nMenu >= MENU_MAX)
			{// �I���
				// ���[�h����
				SetFade(MODE_TITLE);
			}
			else
			{// �܂�����
				// �t�F�[�h�̐ݒ�
				SetFade(MODE_GAME);
			}
		}
		else
		{// ���~�b�N�X�ł͂Ȃ�
			// ���[�h����
			SetFade(MODE_TITLE);
		}
	}

	s_nTime++;

	if (GetRemix())
	{// ���~�b�N�X��
		if (s_nTime >= 180)
		{// 180�b�o���܂���
			int nMenu = (int)GetTitle() + 1;

			if (nMenu >= MENU_MAX)
			{// �I���
				// ���[�h����
				SetFade(MODE_TITLE);
			}
			else
			{// �܂�����
				// �t�F�[�h�̐ݒ�
				SetFade(MODE_GAME);
			}
		}
	}
	else
	{// ���~�b�N�X�ł͂Ȃ�
		if (s_nTime >= 900)
		{// 15�b�o���܂���
			// ���[�h����
			SetFade(MODE_TITLE);
		}
	}

	
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawResult(void)
{
	switch (s_result)
	{
	case RESULT_CLEAR:			// �N���A

		// �N���A
		DrawClear();

		break;

	case RESULT_GAMEOVER:		// �Q�[���I�[�o�[

		// �Q�[���I�[�o�[
		DrawGameOver();

		break;

	case RESULT_NONE:			// �܂����܂��ĂȂ�

		/* �����Ȃ� */

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetResult(RESULT result)
{
	if (s_result == RESULT_NONE || result == RESULT_NONE || result == RESULT_GAMEOVER)
	{
		s_result = result;
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
RESULT GetResult(void)
{
	return s_result;
}

//--------------------------------------------------
// �N���A
//--------------------------------------------------
static void InitClear(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\result010.png",
		&s_pTexture);

	s_nTime = 0;

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

	float fWidth = RESULT_WIDTH * 0.5f;
	float fHeight = RESULT_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.225f, SCREEN_HEIGHT * 0.75f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	switch (GetTitle())
	{// �ǂ̃Q�[���H
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_STOP:			// �~�߂�

		// ���_�J���[�̐ݒ菈��
		Setcol(pVtx, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

		break;

	case MENU_SLOPE:		// ��

		// ���_�J���[�̏�����
		Initcol(pVtx);

		break;

	default:
		assert(false);
		break;
	}

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	// �ʒu�̐���
	InitPosNumber();

	// ���[�g��
	InitMeter();

	// �����_
	InitDecimal();

	// �}�C�i�X
	InitMinus();
}

//--------------------------------------------------
// �Q�[���I�[�o�[
//--------------------------------------------------
static void InitGameOver(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/gameover.png",
		&s_pTextureGameOver);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffGameOver,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffGameOver->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = GAMEOVER_WIDTH * 0.5f;
	float fHeight = GAMEOVER_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̐ݒ菈��
	Setcol(pVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffGameOver->Unlock();
}

//--------------------------------------------------
// �ʒu�̐���
//--------------------------------------------------
static void InitPosNumber(void)
{
	// �ʒu������������
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.75f, 0.0f);

	int nPos = 0;
	float fModel = 0.0f, fPlayer = 0.0f;

	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
		
		nPos = (int)(GetPlayer()->pos.x / 3);

		if (GetPlayer()->pos.x <= 0.0f)
		{// �}�C�i�X
			nPos *= -1;
		}

		break;

	case MENU_STOP:			// �~�߂�
		
		fModel = GetModel()->pos.y;
		fPlayer = GetPlayer()->pos.y + GetPlayer()->fHeight;

		nPos = (int)(((fModel - fPlayer) + STOP_SCORE) * 10.0f);

		break;

	case MENU_SLOPE:		// ��

		fModel = GetField()->pos.x + GetField()->vtxMax.x;
		fPlayer = GetPlayer()->pos.x + GetPlayer()->fSize;

		nPos = (int)(fModel - fPlayer);

		break;

	default:
		assert(false);
		break;
	}

	int nNumber = nPos;

	while (1)
	{
		
		if (nNumber >= 10)
		{
			nNumber /= 10;
			s_nDigit++;
		}
		else
		{
			s_nDigit++;

			if (s_nDigit < MIN_RESULT)
			{
				s_nDigit = MIN_RESULT;
			}

			break;
		}
	}

	int aNumber[MAX_RESULT];

	for (int i = 0; i < s_nDigit; i++)
	{// �P�����ɕ�����
		aNumber[i] = nPos % 10;
		nPos /= 10;

		float fInterval = (NUMBER_WIDTH * i);

		float fDecimal = 0.0f;

		if (i >= 1)
		{
			fDecimal = DECIMAL_INTERVAL;
		}

		// ���̐ݒ菈��
		SetRightNumber(D3DXVECTOR3(pos.x - fInterval - fDecimal, pos.y, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_RESULT);
	}
}

//--------------------------------------------------
// ���[�g��
//--------------------------------------------------
static void InitMeter(void)
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
		&s_pVtxBuffMeter,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffMeter->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = METER_WIDTH * 0.5f;
	float fHeight = METER_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.825f, 0.0f);

	// ���_���W�̐ݒ�
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏�����
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏�����
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffMeter->Unlock();
}

//--------------------------------------------------
// �����_
//--------------------------------------------------
static void InitDecimal(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/decimal.png",
		&s_pTextureDecimal);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffDecimal,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffDecimal->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = DECIMAL_WIDTH * 0.5f;
	float fHeight = DECIMAL_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.755f, SCREEN_HEIGHT * 0.77f, 0.0f);

	// ���_���W�̐ݒ�
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏�����
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏�����
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffDecimal->Unlock();
}

//--------------------------------------------------
// �}�C�i�X
//--------------------------------------------------
static void InitMinus(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/minus.png",
		&s_pTextureMinus);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMinus,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffMinus->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = MINUS_WIDTH * 0.5f;
	float fHeight = MINUS_HEIGHT * 0.5f;

	float fPosX = (SCREEN_WIDTH * 0.85f) - DECIMAL_INTERVAL - (NUMBER_WIDTH * s_nDigit);
	
	D3DXVECTOR3 pos = D3DXVECTOR3(fPosX - fWidth, SCREEN_HEIGHT * 0.75f, 0.0f);

	// ���_���W�̐ݒ�
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏�����
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏�����
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffMinus->Unlock();
}

//--------------------------------------------------
// �N���A
//--------------------------------------------------
static void DrawClear(void)
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
	pDevice->SetStreamSource(0, s_pVtxBuffMeter, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureMeter);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffDecimal, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureDecimal);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffMinus, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureMinus);

	if (GetPlayer()->pos.x <= -30.0f)
	{// �}�C�i�X
		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��
	}
}

//--------------------------------------------------
// �Q�[���I�[�o�[
//--------------------------------------------------
static void DrawGameOver(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffGameOver, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureGameOver);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��
}
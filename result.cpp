//==================================================
// 
// 3D�Q�[������ ( result.cpp )
// Author  : katsuki mizuki
// 
//==================================================
#include "fade.h"
#include "input.h"
#include "model.h"
#include "number.h"
#include "player.h"
#include "result.h"
#include "setup.h"
#include "time.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define RESULT_WIDTH		(500.0f)		// ��
#define RESULT_HEIGHT		(200.0f)		// ����
#define METER_WIDTH			(100.0f)		// ���[�g���̕�
#define METER_HEIGHT		(160.0f)		// ���[�g���̍���
#define MINUS_WIDTH			(60.0f)			// �}�C�i�X�̕�
#define MINUS_HEIGHT		(40.0f)			// �}�C�i�X�̍���
#define GAMEOVER_WIDTH		(1000.0f)		// �Q�[���I�[�o�[�̕�
#define GAMEOVER_HEIGHT		(300.0f)		// �Q�[���I�[�o�[�̍���
#define NUMBER_WIDTH		(140.0f)		// ���̕�
#define NUMBER_HEIGHT		(250.0f)		// ���̍���
#define WIDTH_INTERVAL		(0.0f)			// ���̊Ԋu
#define MAX_RESULT			(3)				// ���U���g�̍ő吔
#define MIN_RESULT			(2)				// ���U���g�̍ŏ���

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// ���_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureMeter = NULL;			// ���[�g���̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMeter = NULL;			// ���[�g���̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMinus = NULL;			// �}�C�i�X�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureGameOver = NULL;		// �Q�[���I�[�o�[�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffGameOver = NULL;		// �Q�[���I�[�o�[�̒��_�o�b�t�@�̃|�C���^
static int							s_nTime;						// �^�C��
static RESULT						s_result;						// ���U���g

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void InitClear(void);
static void InitGameOver(void);
static void InitPosNumber(void);
static void InitMeter(void);
static void InitMinus(void);
static void DrawClear(void);
static void DrawGameOver(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitResult(void)
{
	switch (s_result)
	{
	case RESULT_CLEAR:			// �N���A

		// �N���A
		InitClear();

		break;

	case RESULT_GAMEOVER:		// �Q�[���I�[�o�[

		// �Q�[���I�[�o�[
		InitGameOver();

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
	if (GetKeyboardTrigger(DIK_RETURN) ||
		GetJoypadTrigger(JOYKEY_B, 0) || GetJoypadTrigger(JOYKEY_START, 0))
	{// ����L�[(ENTER�L�[)�������ꂽ���ǂ���
		// ���[�h����
		SetFade(MODE_TITLE);
	}

	s_nTime++;

	if (s_nTime >= 900)
	{// 15�b�o���܂���
		// ���[�h����
		SetFade(MODE_TITLE);
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
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.75f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̐ݒ菈��
	Setcol(pVtx, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	// �ʒu�̐���
	InitPosNumber();

	// ���[�g��
	InitMeter();

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
	D3DXVECTOR3 posNumber = D3DXVECTOR3(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.75f, 0.0f);

	int nPos = 0;
	float fModel = 0.0f, fPlayer = 0.0f;

	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
		
		nPos = (int)(GetPlayer()->pos.x / 30);

		if (GetPlayer()->pos.x <= 0.0f)
		{// �}�C�i�X
			nPos *= -1;
		}

		break;

	case MENU_STOP:			// �~�߂�
		
		fModel = GetModel()->pos.y + GetModel()->vtxMin.y;
		fPlayer = GetPlayer()->pos.y + GetPlayer()->fHeight;

		nPos = (int)(fModel - fPlayer);

		break;

	case MENU_RANKING:		// �����L���O

		break;

	default:
		assert(false);
		break;
	}

	int nNumber = nPos, nDigit = 0;

	while (1)
	{
		
		if (nNumber >= 10)
		{
			nNumber /= 10;
			nDigit++;
		}
		else
		{
			nDigit++;

			if (nDigit < MIN_RESULT)
			{
				nDigit = MIN_RESULT;
			}

			break;
		}
	}

	int aNumber[MAX_RESULT];

	for (int i = 0; i < nDigit; i++)
	{// �P�����ɕ�����
		aNumber[i] = nPos % 10;
		nPos /= 10;

		float fInterval = (NUMBER_WIDTH * i) + (WIDTH_INTERVAL * (i / 2));

		// ���̐ݒ菈��
		SetRightNumber(D3DXVECTOR3(posNumber.x - fInterval, posNumber.y, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_RESULT);
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
// �}�C�i�X
//--------------------------------------------------
static void InitMinus(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.75f, 0.0f);

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
	pDevice->SetStreamSource(0, s_pVtxBuffMinus, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

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
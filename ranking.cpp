//==================================================
// 
// 3D�Q�[������ ( ranking.cpp )
// Author  : katsuki mizuki
// 
//==================================================
//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "field.h"
#include "model.h"
#include "number.h"
#include "player.h"
#include "ranking.h"
#include "setup.h"
#include "target.h"
#include "title.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define RANKING_FILE		"data/TEXT/Ranking.txt"		// �e�L�X�g�ւ̃p�X
#define MAX_DIGIT					(5)					// �����̍ő吔
#define MIN_DIGIT					(2)					// �����̍ŏ���
#define NUMBER_WIDTH				(20.0f)				// ���̕�
#define NUMBER_HEIGHT				(30.0f)				// ���̍���
#define METER_WIDTH					(20.0f)				// ���[�g���̕�
#define METER_HEIGHT				(30.0f)				// ���[�g���̍���
#define DECIMAL_WIDTH				(15.0f)				// �����_�̕�
#define DECIMAL_HEIGHT				(25.0f)				// �����_�̍���
#define DECIMAL_INTERVAL			(10.0f)				// �����̊Ԋu
#define OPERATOR_WIDTH				(15.0f)				// ���Z�q�̕�
#define OPERATOR_HEIGHT				(15.0f)				// ���Z�q�̍���

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureMeter = NULL;			// ���[�g���̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		s_pTextureDecimal = NULL;		// �����_�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		s_pTextureOperator = NULL;		// ���Z�q�̃e�N�X�`���ւ̃|�C���^
static int						s_nWalking;						// �E�H�[�L���O
static int						s_nStop;						// �~�߂�
static int						s_nSlope;						// ��
static int						s_nDigitWalking;				// �����̃E�H�[�L���O
static int						s_nDigitStop;					// �����̎~�߂�
static int						s_nDigitSlope;					// �����̍�

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void NumberWalking(void);
static void InitWalking(void);
static void NumberStop(void);
static void InitStop(void);
static void NumberSlope(void);
static void InitSlope(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitRanking(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/METER/m.png",
		&s_pTextureMeter);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/decimal.png",
		&s_pTextureDecimal);

	s_nDigitWalking = 0;
	s_nDigitStop = 0;
	s_nDigitSlope = 0;

	// �E�H�[�L���O�̐���
	NumberWalking();

	// �E�H�[�L���O�̏�����
	InitWalking();

	// �~�߂�̐���
	NumberStop();

	// �~�߂�̏�����
	InitStop();

	// ��̐���
	NumberSlope();

	// ��̏�����
	InitSlope();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitRanking(void)
{
	if (s_pTextureMeter != NULL)
	{// �e�N�X�`���̉��
		s_pTextureMeter->Release();
		s_pTextureMeter = NULL;
	}

	if (s_pTextureDecimal != NULL)
	{// �e�N�X�`���̉��
		s_pTextureDecimal->Release();
		s_pTextureDecimal = NULL;
	}

	if (s_pTextureOperator != NULL)
	{// �e�N�X�`���̉��
		s_pTextureOperator->Release();
		s_pTextureOperator = NULL;
	}
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void LoadRanking(void)
{
	FILE *pFile;		// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(RANKING_FILE, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fscanf(pFile, "%d", &s_nWalking);
		fscanf(pFile, "%d", &s_nStop);
		fscanf(pFile, "%d", &s_nSlope);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//--------------------------------------------------
// �Z�[�u
//--------------------------------------------------
void SaveRanking(void)
{
	FILE *pFile;		// �t�@�C���|�C���^��錾

	// �t�@�C�����J��
	pFile = fopen(RANKING_FILE, "w");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fprintf(pFile, "%d\n\n", s_nWalking);
		fprintf(pFile, "%d\n\n", s_nStop);
		fprintf(pFile, "%d\n\n", s_nSlope);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//--------------------------------------------------
// �E�H�[�L���O�̐ݒ�
//--------------------------------------------------
void SetScoreWalking(int nScore)
{
	int nWalking = s_nWalking;
	int nScoreSave = nScore;

	if (nWalking < 0.0f)
	{// �}�C�i�X
		nWalking *= -1;
	}

	if (nScoreSave < 0.0f)
	{// �}�C�i�X
		nScoreSave *= -1;
	}

	if (nWalking > nScoreSave)
	{// �X�R�A��������
		s_nWalking = nScore;
	}
}

//--------------------------------------------------
// �~�߂�̐ݒ�
//--------------------------------------------------
void SetScoreStop(int nScore)
{
	if (s_nStop > nScore)
	{// �X�R�A��������
		s_nStop = nScore;
	}
}

//--------------------------------------------------
// ��̐ݒ�
//--------------------------------------------------
void SetScoreSlope(int nScore)
{
	if (s_nSlope > nScore)
	{// �X�R�A��������
		s_nSlope = nScore;
	}
}

//--------------------------------------------------
// �E�H�[�L���O�̐���
//--------------------------------------------------
static void NumberWalking(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nWalking = s_nWalking;

	if (nWalking < 0)
	{// �}�C�i�X
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			"data/TEXTURE/plus.png",
			&s_pTextureOperator);
	}
	else
	{// �v���X
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			"data/TEXTURE/minus.png",
			&s_pTextureOperator);
	}

	if (nWalking < 0.0f)
	{// �}�C�i�X
		nWalking *= -1;
	}

	int nNumber = nWalking;

	while (1)
	{// �������[�v
		if (nNumber >= 10)
		{// 2���ȏ�
			nNumber /= 10;
			s_nDigitWalking++;
		}
		else
		{// 1��
			s_nDigitWalking++;

			if (s_nDigitWalking < MIN_DIGIT)
			{// ����������ĂȂ�
				s_nDigitWalking = MIN_DIGIT;
			}

			break;
		}
	}

	int aNumber[MAX_DIGIT];

	// �ʒu������������
	D3DXVECTOR3 pos = D3DXVECTOR3(-170.0f, 0.0f, -5.0f);

	for (int i = 0; i < s_nDigitWalking; i++)
	{// �P�����ɕ�����
		aNumber[i] = nWalking % 10;
		nWalking /= 10;

		float fInterval = (NUMBER_WIDTH * i);

		float fDecimal = 0.0f;

		if (i >= 1)
		{// �����_������ꏊ���󂯂�
			fDecimal = DECIMAL_INTERVAL;
		}

		// ���̐ݒ菈��
		SetNumber3D(D3DXVECTOR3(pos.x - fInterval - fDecimal, 0.0f, pos.z), NUMBER_WIDTH * 0.5f, NUMBER_HEIGHT * 0.5f, aNumber[i], i);
	}

	float fWidth = OPERATOR_WIDTH * 0.5f;
	float fHeight = OPERATOR_HEIGHT * 0.5f;

	float fPosX = -170.0f - DECIMAL_INTERVAL - (NUMBER_WIDTH * s_nDigitWalking) + (NUMBER_WIDTH * 0.5f);

	pos = D3DXVECTOR3(fPosX - fWidth, 0.0f, -5.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureOperator);
}

//--------------------------------------------------
// �E�H�[�L���O�̏�����
//--------------------------------------------------
static void InitWalking(void)
{
	float fWidth = METER_WIDTH * 0.5f;
	float fHeight = METER_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(245.0f, 0.0f, -8.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureMeter);

	fWidth = DECIMAL_WIDTH * 0.5f;
	fHeight = DECIMAL_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(210.0f, 0.0f, -6.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureDecimal);
}

//--------------------------------------------------
// �~�߂�̐���
//--------------------------------------------------
static void NumberStop(void)
{
	// �ʒu������������
	D3DXVECTOR3 pos = D3DXVECTOR3(30.0f, 0.0f, -5.0f);

	int nNumber = s_nStop;

	while (1)
	{// �������[�v
		if (nNumber >= 10)
		{// 2���ȏ�
			nNumber /= 10;
			s_nDigitStop++;
		}
		else
		{// 1��
			s_nDigitStop++;

			if (s_nDigitStop < MIN_DIGIT)
			{// ����������ĂȂ�
				s_nDigitStop = MIN_DIGIT;
			}

			break;
		}
	}

	int aNumber[MAX_DIGIT];
	int nStop = s_nStop;

	for (int i = 0; i < s_nDigitStop; i++)
	{// �P�����ɕ�����
		aNumber[i] = nStop % 10;
		nStop /= 10;

		float fInterval = (NUMBER_WIDTH * i);

		float fDecimal = 0.0f;

		if (i >= 1)
		{// �����_������ꏊ���󂯂�
			fDecimal = DECIMAL_INTERVAL;
		}

		// ���̐ݒ菈��
		SetNumber3D(D3DXVECTOR3(pos.x - fInterval - fDecimal, 0.0f, pos.z), NUMBER_WIDTH * 0.5f, NUMBER_HEIGHT * 0.5f, aNumber[i], i);
	}
}

//--------------------------------------------------
// �~�߂�̏�����
//--------------------------------------------------
static void InitStop(void)
{
	float fWidth = METER_WIDTH * 0.5f;
	float fHeight = METER_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(50.0f, 0.0f, -8.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureMeter);

	fWidth = DECIMAL_WIDTH * 0.5f;
	fHeight = DECIMAL_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(15.0f, 0.0f, -6.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureDecimal);
}

//--------------------------------------------------
// ��̐���
//--------------------------------------------------
static void NumberSlope(void)
{
	// �ʒu������������
	D3DXVECTOR3 pos = D3DXVECTOR3(225.0f, 0.0f, -5.0f);

	int nNumber = s_nSlope;

	while (1)
	{// �������[�v
		if (nNumber >= 10)
		{// 2���ȏ�
			nNumber /= 10;
			s_nDigitSlope++;
		}
		else
		{// 1��
			s_nDigitSlope++;

			if (s_nDigitSlope < MIN_DIGIT)
			{// ����������ĂȂ�
				s_nDigitSlope = MIN_DIGIT;
			}

			break;
		}
	}

	int aNumber[MAX_DIGIT];
	int nSlope = s_nSlope;

	for (int i = 0; i < s_nDigitSlope; i++)
	{// �P�����ɕ�����
		aNumber[i] = nSlope % 10;
		nSlope /= 10;

		float fInterval = (NUMBER_WIDTH * i);

		float fDecimal = 0.0f;

		if (i >= 1)
		{// �����_������ꏊ���󂯂�
			fDecimal = DECIMAL_INTERVAL;
		}

		// ���̐ݒ菈��
		SetNumber3D(D3DXVECTOR3(pos.x - fInterval - fDecimal, 0.0f, pos.z), NUMBER_WIDTH * 0.5f, NUMBER_HEIGHT * 0.5f, aNumber[i], i);
	}
}

//--------------------------------------------------
// ��̏�����
//--------------------------------------------------
static void InitSlope(void)
{
	float fWidth = METER_WIDTH * 0.5f;
	float fHeight = METER_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(-150.0f, 0.0f, -8.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureMeter);

	fWidth = DECIMAL_WIDTH * 0.5f;
	fHeight = DECIMAL_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(-185.0f, 0.0f, -6.0f);

	// �r���{�[�h�̐ݒ�
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureDecimal);
}

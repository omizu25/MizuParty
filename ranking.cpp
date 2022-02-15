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
#define DECIMAL_WIDTH				(80.0f)				// �����_�̕�
#define DECIMAL_HEIGHT				(150.0f)			// �����_�̍���
#define DECIMAL_INTERVAL			(10.0f)				// �����̊Ԋu

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static int		s_nWalking;				// �E�H�[�L���O
static int		s_nStop;				// �~�߂�
static int		s_nSlope;				// ��
static int		s_nDigitWalking;		// �����̃E�H�[�L���O
static int		s_nDigitStop;			// �����̎~�߂�
static int		s_nDigitSlope;			// �����̍�

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void NumberWalking(void);
static void NumberStop(void);
static void NumberSlope(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitRanking(void)
{
	s_nDigitWalking = 0;
	s_nDigitStop = 0;
	s_nDigitSlope = 0;

	// �E�H�[�L���O�̐���
	NumberWalking();

	// �~�߂�̐���
	NumberStop();

	// ��̐���
	NumberSlope();
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
// �E�H�[�L���O�̐���
//--------------------------------------------------
static void NumberWalking(void)
{
	// �ʒu������������
	D3DXVECTOR3 pos = D3DXVECTOR3(-170.0f, 0.0f, -5.0f);

	int nNumber = s_nWalking;

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
	int nWalking = s_nWalking;

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
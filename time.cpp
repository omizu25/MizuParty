//==================================================
// 
// 3D�Q�[������ ( time.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "game.h"
#include "number.h"
#include "time.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define NUMBER_WIDTH		(80.0f)			// ��
#define NUMBER_HEIGHT		(140.0f)		// ����
#define WIDTH_INTERVAL		(5.0f)			// ���̊Ԋu
#define START_TIME			(10)			// �ŏ��̒l
#define ONE_SECONDS			(60)			// �P�b

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static D3DXVECTOR3		s_pos;			// �ʒu
static int				s_nTime;		// �l
static int				s_nSecond;		// �P�b�Ԋu�𐔂���

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitTime(void)
{
	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.15f;

	// �ʒu��������
	s_pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	s_nTime = START_TIME;

	int nTime = s_nTime;
	int aNumber[MAX_TIME];

	for (int i = 0; i < MAX_TIME; i++)
	{
		aNumber[i] = 0;
	}

	for (int i = 0; i < MAX_TIME; i++)
	{// �P�����ɕ�����
		aNumber[i] = nTime % 10;
		nTime /= 10;

		float fInterval = (NUMBER_WIDTH * i) + (WIDTH_INTERVAL * i);
		float fCenter = (NUMBER_WIDTH * 0.5) * (MAX_TIME - 1) + (WIDTH_INTERVAL * 0.5f) * (MAX_TIME - 1);

		// ���̐ݒ�
		SetRightNumber(D3DXVECTOR3((s_pos.x) - fInterval + fCenter + (NUMBER_WIDTH * 0.5f), s_pos.y, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_GAME_ONLY);
	}
}

//--------------------------------------------------
// ���Z
//--------------------------------------------------
void SubTime(int nValue)
{
	s_nSecond += nValue;

	if (s_nSecond % ONE_SECONDS == 0 && s_nTime > 0)
	{// �P�b�o�߁A�܂����Ԃ�����
		s_nTime--;

		int nTime = s_nTime;
		int aNumber[MAX_TIME];

		for (int i = 0; i < MAX_TIME; i++)
		{// �P�����ɕ�����
			aNumber[i] = nTime % 10;
			nTime /= 10;

			// ���̃e�N�X�`��
			TexNumber(aNumber[i], i, USE_GAME_ONLY);
		}
	}
	else if (s_nTime <= 0)
	{// �������Ԃ�����
		// �Q�[���̐ݒ�
		SetGameState(GAMESTATE_END);
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
int GetTime(void)
{
	return s_nTime;
}

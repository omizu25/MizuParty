//==================================================
// 
// 3D�Q�[������ ( particle.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "effect.h"
#include "particle.h"
#include "setup.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_PARTICLE		(4096)		// �p�[�e�B�N���̍ő吔
#define MAX_STAR			(30)		// �X�^�[�̍ő吔
#define MAX_EXPLOSION		(150)		// �����̍ő吔

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		move;			// �ړ���
	D3DXCOLOR		col;			// �F
	float			fWidth;			// ��
	float			fHeight;		// ����
	int				nLife;			// ����(�\������)
	bool			bAdd;			// ���Z���������邩�ǂ���
	bool			bUse;			// �g�p���Ă��邩�ǂ���
}Particle;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Particle		s_aParticle[MAX_PARTICLE];		// �p�[�e�B�N���̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitParticle(void)
{
	// �������̃N���A
	memset(&s_aParticle, 0, sizeof(s_aParticle));
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateParticle(void)
{
	
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetParticle(D3DXVECTOR3 pos, float fDistance, float fSize, bool bAdd, int nRepeat)
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle *pParticle = &s_aParticle[i];

		if (pParticle->bUse)
		{// �g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/
		for (int j = 0; j < nRepeat; j++)
		{
			for (int k = 0; k < MAX_STAR; k++)
			{
				pParticle->pos.x = (float)((rand() % 1601) - 800) / 30.0f + pos.x + fDistance;
				pParticle->pos.y = (float)((rand() % 1601) - 800) / 30.0f + pos.y;
				pParticle->pos.z = (float)((rand() % 1601) - 800) / 30.0f + pos.z;

				pParticle->move.x = (float)((rand() % 81) - 80) / 10.0f * nRepeat;
				pParticle->move.y = (float)((rand() % 81) - 20) / 10.0f * (nRepeat * 0.5f);
				pParticle->move.z = (float)((rand() % 61) - 30) / 10.0f;

				pParticle->col.r = (float)(rand() % 101) / 100.0f;
				pParticle->col.g = (float)(rand() % 101) / 100.0f;
				pParticle->col.b = (float)(rand() % 101) / 100.0f;
				pParticle->col.a = 1.0f;

				pParticle->fWidth = (float)(rand() % 51) / 100.0f + fSize;
				pParticle->fHeight = pParticle->fWidth;

				pParticle->nLife = (rand() % 21) + 15;

				//�G�t�F�N�g�̐ݒ�
				SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fWidth, pParticle->fHeight, pParticle->nLife, bAdd);
			}

			for (int k = 0; k < MAX_STAR; k++)
			{
				pParticle->pos.x = (float)((rand() % 1601) - 800) / 30.0f + pos.x - fDistance;
				pParticle->pos.y = (float)((rand() % 1601) - 800) / 30.0f + pos.y;
				pParticle->pos.z = (float)((rand() % 1601) - 800) / 30.0f + pos.z;

				pParticle->move.x = (float)(rand() % 81) / 10.0f * nRepeat;
				pParticle->move.y = (float)((rand() % 81) - 20) / 10.0f * (nRepeat * 0.5f);
				pParticle->move.z = (float)((rand() % 61) - 30) / 10.0f;

				pParticle->col.r = (float)(rand() % 101) / 100.0f;
				pParticle->col.g = (float)(rand() % 101) / 100.0f;
				pParticle->col.b = (float)(rand() % 101) / 100.0f;
				pParticle->col.a = 1.0f;

				pParticle->fWidth = (float)(rand() % 51) / 100.0f + fSize;
				pParticle->fHeight = pParticle->fWidth;

				pParticle->nLife = (rand() % 21) + 15;

				//�G�t�F�N�g�̐ݒ�
				SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fWidth, pParticle->fHeight, pParticle->nLife, bAdd);
			}
		}

		break;		// ������for���𔲂���
	}
}

//--------------------------------------------------
// �����̐ݒ�
//--------------------------------------------------
void SetExplosion(D3DXVECTOR3 pos, float fSize, bool bAdd)
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle *pParticle = &s_aParticle[i];

		if (pParticle->bUse)
		{// �g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		for (int j = 0; j < MAX_EXPLOSION; j++)
		{
			pParticle->pos.x = (float)((rand() % 1601) - 800) / 30.0f + pos.x;
			pParticle->pos.y = (float)((rand() % 1601) - 800) / 30.0f + pos.y;
			pParticle->pos.z = (float)((rand() % 1601) - 800) / 30.0f + pos.z;

			pParticle->move.x = (float)((rand() % 201) - 100) / 10.0f;
			pParticle->move.y = (float)((rand() % 201) - 100) / 10.0f;
			pParticle->move.z = (float)((rand() % 201) - 100) / 10.0f;

			pParticle->col.r = (float)(rand() % 101) / 100.0f;
			pParticle->col.g = (float)(rand() % 101) / 100.0f;
			pParticle->col.b = (float)(rand() % 101) / 100.0f;
			pParticle->col.a = 1.0f;

			pParticle->fWidth = (float)(rand() % 51) / 100.0f + fSize;
			pParticle->fHeight = pParticle->fWidth;

			pParticle->nLife = (rand() % 31) + 20;

			//�G�t�F�N�g�̐ݒ�
			SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fWidth, pParticle->fHeight, pParticle->nLife, bAdd);
		}

		break;		// ������for���𔲂���
	}
}
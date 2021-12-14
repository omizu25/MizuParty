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
#define MAX_STAR			(50)		//�X�^�[�̍ő吔

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
	memset(&s_aParticle, NULL, sizeof(s_aParticle));
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateParticle(void)
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle *pParticle = &s_aParticle[i];

		if (!pParticle->bUse)
		{// �g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		//for (int j = 0; j < MAX_STAR; j++)
		//{
		//	pParticle->pos.x = (float)((rand() % 1601) - 800) / 30.0f + pParticle->pos.x;
		//	pParticle->pos.y = (float)((rand() % 1601) - 800) / 30.0f + pParticle->pos.y;
		//	pParticle->pos.z = 0.0f;

		//	pParticle->move.x = ((j % 2) * -1.0f) * pParticle->move.x;
		//	pParticle->move.y = (float)(((rand() % 160) - 80) / 30.0f) + pParticle->move.y;
		//	pParticle->move.z = pParticle->move.z;

		//	pParticle->col.r = (float)(rand() % 101) / 100.0f;
		//	pParticle->col.g = (float)(rand() % 101) / 100.0f;
		//	pParticle->col.b = (float)(rand() % 101) / 100.0f;
		//	pParticle->col.a = 1.0f;

		//	pParticle->fWidth = (float)(rand() % 51) / 100.0f + 15.0f;
		//	pParticle->fHeight = pParticle->fWidth;

		//	pParticle->nLife = (rand() % 11) + 10;

		//	//�G�t�F�N�g�̐ݒ�
		//	SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fWidth, pParticle->fHeight, pParticle->nLife, true);
		//}
	}
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nLife, bool bAdd)
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle *pParticle = &s_aParticle[i];

		if (pParticle->bUse)
		{// �g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		for (int j = 0; j < MAX_STAR; j++)
		{
			pParticle->pos.x = (float)((rand() % 1601) - 800) / 30.0f + pos.x;
			pParticle->pos.y = (float)((rand() % 1601) - 800) / 30.0f + pos.y;
			pParticle->pos.z = 0.0f;

			pParticle->move.x = (float)((rand() % 161) - 80) / 10.0f;
			pParticle->move.y = (float)((rand() % 161) - 80) / 10.0f;
			pParticle->move.z = move.z;

			pParticle->col.r = (float)(rand() % 101) / 100.0f;
			pParticle->col.g = (float)(rand() % 101) / 100.0f;
			pParticle->col.b = (float)(rand() % 101) / 100.0f;
			pParticle->col.a = 1.0f;

			pParticle->fWidth = (float)(rand() % 51) / 100.0f + 15.0f;
			pParticle->fHeight = fWidth;

			pParticle->nLife = (rand() % 11) + 10;

			//�G�t�F�N�g�̐ݒ�
			SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fWidth, pParticle->fHeight, pParticle->nLife, true);
		}

		break;		// ������for���𔲂���
	}
}

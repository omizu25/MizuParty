//--------------------------------------------------
//
// 3D�Q�[������ ( particle.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _PARTICLE_H_		// ���̃}�N����`������ĂȂ�������
#define _PARTICLE_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nLife, bool bAdd);

#endif // !_PARTICLE_H_

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
void UpdateParticle(void);
void SetParticle(D3DXVECTOR3 pos, float fSize, bool bAdd);

#endif // !_PARTICLE_H_

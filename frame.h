//==================================================
//
// 3D�Q�[������ ( frame.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _FRAME_H_		// ���̃}�N����`������ĂȂ�������
#define _FRAME_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitFrame(void);
void UninitFrame(void);
void UpdateFrame(void);
void DrawFrame(void);
void SetFrame(D3DXVECTOR3 pos, float fWidth, float fHeight, LPDIRECT3DTEXTURE9 *pTexture);
void CollisionFrame(D3DXVECTOR3 *pPos);

#endif // !_FRAME_H_

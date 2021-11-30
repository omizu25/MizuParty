//==================================================
//
// 3D�Q�[������ ( billboard.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _BILLBOARD_H_		// ���̃}�N����`������ĂȂ�������
#define _BILLBOARD_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight);
void InstallationBillboard(void);

#endif // !_BILLBOARD_H_
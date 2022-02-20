//==================================================
//
// 3D�Q�[������ ( shadow.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _SHADOW_H_		// ���̃}�N����`������ĂȂ�������
#define _SHADOW_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);
void SetPosShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void UseStopShadow(int nIdxShadow);

#endif // !_SHADOW_H_

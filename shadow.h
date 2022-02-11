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
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize);
void SetPosShadow(int nIdxShadow, D3DXVECTOR3 pos);
void UseStopShadow(int nIdxShadow);

#endif // !_SHADOW_H_

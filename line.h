//==================================================
//
// 3D�Q�[������ ( line.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _LINE_H_		// ���̃}�N����`������ĂȂ�������
#define _LINE_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitLine(void);
void UninitLine(void);
void UpdateLine(void);
void DrawLine(void);
void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col);

#endif // !_LINE_H_

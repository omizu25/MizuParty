//==================================================
//
// 3D�Q�[������ ( locus.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _LOCUS_H_		// ���̃}�N����`������ĂȂ�������
#define _LOCUS_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �\����
//--------------------------------------------------

/*�� �O�� ��*/

typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	float			fWidth;			// ��
	float			fHeight;		// ����
	float			fDepth;			// ���s��
}Locus;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitLocus(void);
void UninitLocus(void);
void UpdateLocus(void);
void DrawLocus(void);
void SetLocus(void);
Locus *GetLocus(void);

#endif // !_LOCUS_H_

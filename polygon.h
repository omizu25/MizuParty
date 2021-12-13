//==================================================
//
// 3D�Q�[������ ( polygon.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _POLYGON_H_		// ���̃}�N����`������ĂȂ�������
#define _POLYGON_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	float			fWidth;			// ��
	float			fHeight;		// ����
	float			fDepth;			// ���s��
}polygon;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);
polygon *GetPolygon(void);

#endif // !_POLYGON_H_

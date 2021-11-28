//==================================================
//
// 3D�Q�[������ ( model.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MODEL_H_		// ���̃}�N����`������ĂȂ�������
#define _MODEL_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// ���f���̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXVECTOR3		rotDest;		// �ړI�̌���
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
}Model;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model *GetModel(void);

#endif // !_MODEL_H_
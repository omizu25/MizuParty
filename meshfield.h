//==================================================
//
// 3D�Q�[������ ( meshfield.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MESHFIELD_H_		// ���̃}�N����`������ĂȂ�������
#define _MESHFIELD_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	float			fWidth;			// ��
	float			fHeight;		// ����
	float			fDepth;			// ���s��
}MeshField;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void SetMeshField(void);
MeshField *GetMeshField(void);
int GetHorizontal(void);
int GetVertical(void);
int GetVtxNumber(void);
int GetIdxNumber(void);
int GetPolygonNumber(void);

#endif // !_MESHFIELD_H_

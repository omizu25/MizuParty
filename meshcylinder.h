//==================================================
//
// 3D�Q�[������ ( meshcylinder.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MESHCYLINDER_H_		// ���̃}�N����`������ĂȂ�������
#define _MESHCYLINDER_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �\���̂��`
//--------------------------------------------------

/*�� ���b�V���~�� ��*/

typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
}MeshCylinder;

/*�� ���b�V���~���̐��n ��*/

typedef struct
{
	int		nHorizontal;		// ��
	int		nVertical;			// �c
	int		nVtx;				// ���_��
	int		nIdx;				// �C���f�b�N�X��
	int		nPolygon;			// �|���S����
}MeshCylinderNumber;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);
void SetMeshCylinder(void);
MeshCylinder *GetMeshCylinder(void);
MeshCylinderNumber *GetMeshCylinderNumber(void);

#endif // !_MESHCYLINDER_H_
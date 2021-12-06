//==================================================
//
// 3D�Q�[������ ( meshsphere.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MESHSPHERE_H_		// ���̃}�N����`������ĂȂ�������
#define _MESHSPHERE_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �\���̂��`
//--------------------------------------------------

/*�� ���b�V���� ��*/

typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
}MeshSphere;

/*�� ���b�V�����̐��n ��*/

typedef struct
{
	int		nHorizontal;		// ��
	int		nVertical;			// �c
	int		nVtx;				// ���_��
	int		nIdx;				// �C���f�b�N�X��
	int		nPolygon;			// �|���S����
}MeshSphereNumber;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitMeshSphere(void);
void UninitMeshSphere(void);
void UpdateMeshSphere(void);
void DrawMeshSphere(void);
void SetMeshSphere(void);
MeshSphere *GetMeshSphere(void);
MeshSphereNumber *GetMeshSphereNumber(void);

#endif // !_MESHSPHERE_H_
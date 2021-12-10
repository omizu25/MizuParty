//==================================================
//
// 3D�Q�[������ ( mesh_sky.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MESH_SKY_H_		// ���̃}�N����`������ĂȂ�������
#define _MESH_SKY_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �\����
//--------------------------------------------------

/*�� ���b�V���� ��*/

typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
}MeshSky;

/*�� ���b�V����̐��n ��*/

typedef struct
{
	int		nHorizontal;		// ��
	int		nVertical;			// �c
	int		nVtx;				// ���_��
	int		nIdx;				// �C���f�b�N�X��
	int		nPolygon;			// �|���S����
}MeshSkyNumber;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitMeshSky(void);
void UninitMeshSky(void);
void UpdateMeshSky(void);
void DrawMeshSky(void);
void SetMeshSky(void);
MeshSky *GetMeshSky(void);
MeshSkyNumber *GetMeshSkyNumber(void);

#endif // !_MESH_SKY_H
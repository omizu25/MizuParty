//==================================================
//
// 3D�Q�[������ ( field.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _FIELD_H_		// ���̃}�N����`������ĂȂ�������
#define _FIELD_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	D3DXVECTOR3				vtxMin;			// �ŏ��l
	D3DXVECTOR3				vtxMax;			// �ő�l
	D3DXVECTOR3				rot;			// ����
	D3DXVECTOR3				rotDest;		// �ړI�̌���
	D3DXMATRIX				mtxWorld;		// ���[���h�}�g���b�N�X
	LPD3DXMESH				pMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			pBuffMat;		// �}�e���A�����ւ̃|�C���^
	LPDIRECT3DTEXTURE9		*pTexture;		// �e�N�X�`���ւ̃|�C���^
	DWORD					nNumMat;		// �}�e���A�����̐�
	float					fMove;			// �ړ���
	int						nIdxShadow;		// �g�p���Ă���e�̔ԍ�
}Field;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
Field *GetField(void);
bool CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size);

#endif // !_FIELD_H_

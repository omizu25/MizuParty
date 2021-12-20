//==================================================
//
// 3D�Q�[������ ( model.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MODEL_H_		// ���̃}�N����`������ĂȂ�������
#define _MODEL_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_PARTS		(2)		// �p�[�c�̍ő吔

//--------------------------------------------------
// �\����
//--------------------------------------------------

/*�� ���f���p�[�c ��*/

typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	D3DXVECTOR3				rot;			// ����
	D3DXVECTOR3				rotDest;		// �ړI�̌���
	LPD3DXMESH				pMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			pBuffMat;		// �}�e���A�����ւ̃|�C���^
	LPDIRECT3DTEXTURE9		*pTexture;		// �e�N�X�`���ւ̃|�C���^
	DWORD					nNumMat;		// �}�e���A�����̐�
	D3DXMATRIX				mtxWorld;		// ���[���h�}�g���b�N�X
	int						nIdxParent;		// �e�̔ԍ�
}ModelParts;

/*�� ���f�� ��*/

typedef struct
{
	D3DXVECTOR3				pos;					// �ʒu
	D3DXVECTOR3				vtxMin;					// �ŏ��l
	D3DXVECTOR3				vtxMax;					// �ő�l
	D3DXVECTOR3				rot;					// ����
	D3DXVECTOR3				rotDest;				// �ړI�̌���
	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X
	ModelParts				parts[MAX_PARTS];		// �p�[�c�̏��
	int						nIdxShadow;				// �g�p���Ă���e�̔ԍ�
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

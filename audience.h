//==================================================
//
// 3D�Q�[������ ( audience.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _AUDIENCE_H_		// ���̃}�N����`������ĂȂ�������
#define _AUDIENCE_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	D3DXVECTOR3				vtxMin;			// �ŏ��l
	D3DXVECTOR3				vtxMax;			// �ő�l
	D3DXVECTOR3				rot;			// ����
	D3DXMATRIX				mtxWorld;		// ���[���h�}�g���b�N�X
	LPD3DXMESH				pMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			pBuffMat;		// �}�e���A�����ւ̃|�C���^
	LPDIRECT3DTEXTURE9		*pTexture;		// �e�N�X�`���ւ̃|�C���^
	DWORD					nNumMat;		// �}�e���A�����̐�
	int						nIdxShadow;		// �g�p���Ă���e�̔ԍ�
	bool					bDraw;			// �`�悷�邩�ǂ���
}Audience;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitAudience(void);
void UninitAudience(void);
void UpdateAudience(void);
void DrawAudience(void);
void LoadAudience(void);
Audience *GetAudience(int nAudience);
int GetNumAudience(void);

#endif // !_AUDIENCE_H_

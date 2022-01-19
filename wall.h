//==================================================
//
// 3D�Q�[������ ( wall.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _WALL_H_		// ���̃}�N����`������ĂȂ�������
#define _WALL_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_WALL		(64)		// �ǂ̍ő吔

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	bool			bUse;			// �g�p���Ă��邩�ǂ���
	float			fWidth;			// ��
	float			fHeight;		// ����
}Wall;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, float fURight, bool bCulling);
void InstallationWall(void);
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size);
Wall *GetWall(void);
void ResetWall(void);

#endif // !_WALL_H_

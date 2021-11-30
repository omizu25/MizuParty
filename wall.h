//==================================================
//
// 3D�Q�[������ ( wall.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _WALL_H_		// ���̃}�N����`������ĂȂ�������
#define _WALL_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, bool bCulling);
void InstallationWall(void);

#endif // !_WALL_H_
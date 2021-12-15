//==================================================
//
// 3D�Q�[������ ( player.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _PLAYER_H_		// ���̃}�N����`������ĂȂ�������
#define _PLAYER_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXVECTOR3		rotDest;		// �ړI�̌���
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	int				nIdxShadow;		// �g�p���Ă���e�̔ԍ�
	int				nStopTime;		// �~�܂��Ă��鎞��
}Player;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif // !_PLAYER_H_

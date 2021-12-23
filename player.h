//==================================================
//
// 3D�Q�[������ ( player.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _PLAYER_H_		// ���̃}�N����`������ĂȂ�������
#define _PLAYER_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_MOTION		(1)		// ���[�V������

//--------------------------------------------------
// �\����
//--------------------------------------------------

/*�� �L�[ ��*/

typedef struct
{
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ����
}Key;

/*�� �L�[�ݒ� ��*/

typedef struct
{
	int		nFrame;		// �t���[����
	Key		*key;		// �L�[
}KeySet;

/*�� ���[�V�����ݒ� ��*/

typedef struct
{
	bool		bLoop;			// ���[�v���邩�ǂ���
	int			nNumKey;		// �L�[��
	KeySet		*keySet;		// �L�[�ݒ�
}MotionSet;

/*�� �v���C���[�p�[�c ��*/

typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	D3DXVECTOR3				rot;			// ����
	LPD3DXMESH				pMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			pBuffMat;		// �}�e���A�����ւ̃|�C���^
	LPDIRECT3DTEXTURE9		*pTexture;		// �e�N�X�`���ւ̃|�C���^
	DWORD					nNumMat;		// �}�e���A�����̐�
	D3DXMATRIX				mtxWorld;		// ���[���h�}�g���b�N�X
	int						nIdxModel;		// �g�p���郂�f���̔ԍ�
	int						nIdxParent;		// �e�̔ԍ�
}PlayerParts;

/*�� �v���C���[ ��*/

typedef struct
{
	D3DXVECTOR3		pos;					// �ʒu
	D3DXVECTOR3		posOld;					// �O��̈ʒu
	D3DXVECTOR3		rot;					// ����
	D3DXVECTOR3		rotDest;				// �ړI�̌���
	D3DXMATRIX		mtxWorld;				// ���[���h�}�g���b�N�X
	PlayerParts		*parts;					// �p�[�c�̏��
	MotionSet		*Motion;				// ���[�V�����̏��
	int				nNumParts;				// �p�[�c��
	int				nIdxShadow;				// �g�p���Ă���e�̔ԍ�
	int				nStopTime;				// �~�܂��Ă��鎞��
	float			fSize;					// �T�C�Y
	char			aText[MAX_TEXT];		// �e�L�X�g��
}Player;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void LoadPlayer(HWND hWnd);

#endif // !_PLAYER_H_

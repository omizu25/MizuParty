//==================================================
//
// 3D�Q�[������ ( game.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _GAME_H_		// ���̃}�N����`������ĂȂ�������
#define _GAME_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �񋓌^
//--------------------------------------------------
typedef enum
{
	GAMESTATE_NONE = 0,			// �������Ă��Ȃ����
	GAMESTATE_START,			// �J�n��� (�Q�[���J�n�O)
	GAMESTATE_COUNTDOWN,		// �J�E���g�_�E����� (�Q�[���J�n��)
	GAMESTATE_NORMAL,			// �ʏ��� (�Q�[���i�s��)
	GAMESTATE_END,				// �I����� (�Q�[���I����)
	GAMESTATE_RESULT,			// ���U���g��� (�Q�[���I����)
	GAMESTATE_LOOP,				// �J��Ԃ���� (���U���g�I����)
	GAMESTATE_MAX
}GAMESTATE;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGame(void);
void SetRemix(bool bRemix);
bool GetRemix(void);

#endif // !_GAME_H_

//==================================================
//
// 3D�Q�[������ ( title.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _TITLE_H_		//���̃}�N����`������ĂȂ�������
#define _TITLE_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �񋓌^
//--------------------------------------------------
typedef enum
{
	MENU_WALKING = 0,		// �E�H�[�L���O
	MENU_ROTATION,			// ��]
	MENU_STOP,				// �~�߂�
	MENU_SLOPE,				// ��
	MENU_MAX
}MENU;

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define REMIX_OK		(2)		// ���~�b�N�X���o����悤�ɂȂ鐔

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void SetTitle(MENU menu);
MENU GetTitle(void);
void SetTitleCnt(void);
int GetTitleCnt(void);

#endif // !_TITLE_H_
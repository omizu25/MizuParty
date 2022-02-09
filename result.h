//==================================================
//
// 3D�Q�[������ ( result.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _RESULT_H_		//���̃}�N����`������ĂȂ�������
#define _RESULT_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
// �񋓌^
//--------------------------------------------------
typedef enum
{
	RESULT_NONE = 0,		// �܂����܂��ĂȂ�
	RESULT_CLEAR,			// �N���A
	RESULT_GAMEOVER,		// �Q�[���I�[�o�[
}RESULT;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResult(RESULT result);
RESULT GetResult(void);

#endif // !_RESULT_H_
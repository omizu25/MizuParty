//==================================================
//
// 3D�Q�[������ ( number.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _NUMBER_H_		//���̃}�N����`������ĂȂ�������
#define _NUMBER_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
// �񋓌^
//--------------------------------------------------
typedef enum
{
	USE_NONE = 0,		// �Ȃ�
	USE_GAME,			// �Q�[��
	USE_RESULT,			// ���U���g
	USE_MAX
}USE;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitNumber(void);
void UninitNumber(void);
void UpdateNumber(void);
void DrawNumber(USE use);
void SetRightNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, USE use);
void SetMiddleNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, USE use);
void TexNumber(int nNumber, int nDigit, USE use);

#endif // !_NUMBER_H_

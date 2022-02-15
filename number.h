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
	USE_START,			// �X�^�[�g
	USE_GAME_ONLY,		// �Q�[������
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
void DrawNumber2D(USE use);
void DrawNumber3D(void);
void SetNumber3D(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit);
void SetRightNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, USE use);
void SetMiddleNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, USE use);
void TexNumber(int nNumber, int nDigit, USE use);
void UseNumber(int nDigit, USE use);

#endif // !_NUMBER_H_

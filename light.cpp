//==================================================
// 
// 3D�Q�[������ ( light.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "light.h"
#include "main.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_LIGHT		(3)		//���C�g�̍ő吔

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static D3DLIGHT9		s_light[MAX_LIGHT];		// ���C�g�̏��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void SetLight(D3DXCOLOR col, D3DXVECTOR3 vec, int nNum);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitLight(void)
{
	// ���C�g���N���A����
	ZeroMemory(&s_light[0], sizeof(s_light));

	// ���C�g�̐ݒ�
	SetLight(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.2f, -0.8f, 0.4f), 0);
	SetLight(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f), D3DXVECTOR3(0.2f, -0.8f, -0.4f), 1);
	SetLight(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), D3DXVECTOR3(-0.2f, 0.8f, 0.4f), 2);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitLight(void)
{
	
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateLight(void)
{

}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
static void SetLight(D3DXCOLOR col, D3DXVECTOR3 vec, int nNum)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR3 vecDir;		// ���C�g�̕����x�N�g��

	// ���C�g�̎�ނ�ݒ� ( ���s���� )
	s_light[nNum].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g�̊g�U����ݒ� ( ���C�g�̐F )
	s_light[nNum].Diffuse = col;

	// ���C�g�̕�����ݒ�
	vecDir = vec;

	// ���K������ ( �傫�� 1 �̃x�N�g���ɂ��� )
	D3DXVec3Normalize(&vecDir, &vecDir);
	s_light[nNum].Direction = vecDir;

	// ���C�g��ݒ肷��
	pDevice->SetLight(nNum, &s_light[nNum]);

	// ���C�g��L���ɂ���
	pDevice->LightEnable(nNum, TRUE);
}

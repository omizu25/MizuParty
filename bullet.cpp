//==================================================
// 
// 3D�Q�[������ ( player.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "input.h"
#include "player.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_MOVE		(7.0f)		// �ړ��ʂ̍ő�l
#define MAX_WIDTH		(20.0f)		// ���̍ő�l
#define MAX_HEIGHT		(20.0f)		// �����̍ő�l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitBullet(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko000.jpg",
		&s_pTexture);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitBullet(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		s_pTexture->Release();
		s_pTexture = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateBullet(void)
{
	if (GetKeyboardTrigger(DIK_SPACE))
	{// �X�y�[�X�L�[�������ꂽ
		D3DXVECTOR3 move = D3DXVECTOR3(MAX_MOVE, 0.0f, 0.0f);
		Player *pPlayer = GetPlayer();

		// �r���{�[�h�̐ݒ�
		SetBillboard(pPlayer->pos, move, MAX_WIDTH, MAX_HEIGHT, false, &s_pTexture);
	}
}

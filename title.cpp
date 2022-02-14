//==================================================
// 
// 3D�Q�[������ ( title.cpp )
// Author  : katsuki mizuki
// 
//==================================================
//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "camera.h"
#include "fade.h"
#include "frame.h"
#include "game.h"
#include "input.h"
#include "light.h"
#include "model.h"
#include "player.h"
#include "polygon.h"
#include "shadow.h"
#include "sound.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static MENU		s_Menu;			// ���j���[
static int		s_nCount;		// ����^�C�g���ɗ������̃J�E���g

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitTitle(void)
{
	s_nCount++; 

	// �|���S���̏�����
	InitPolygon();

	// �e�̏�����
	InitShadow();

	// �v���C���[�̓ǂݍ���
	LoadPlayer();

	// �v���C���[�̏�����
	InitPlayer();

	// �g�̏�����
	InitFrame();

	// �r���{�[�h�̏�����
	InitBillboard();

	// �^�C�g���̎��̓ǂݍ���
	InitBillboardTitle();

	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// �T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_�^�C�g��);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitTitle(void)
{
	// �T�E���h�̒�~
	StopSound();

	// �|���S���̏I��
	UninitPolygon();

	// �g�̏I��
	UninitFrame();

	// �r���{�[�h�̏I��
	UninitBillboard();

	// �e�̏I��
	UninitShadow();

	// �v���C���[�̏I��
	UninitPlayer();

	// �J�����̏I��
	UninitCamera();

	// ���C�g�̏I��
	UninitLight();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateTitle(void)
{
	// �v���C���[�̍X�V
	UpdatePlayer();

	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) ||
		GetJoypadTrigger(JOYKEY_B) || GetJoypadTrigger(JOYKEY_A) ||
		GetJoypadTrigger(JOYKEY_START))
	{// �X�y�[�X�L�[�������ꂽ
		// �r���{�[�h�̓����蔻��
		CollisionBillboard();
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawTitle(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DVIEWPORT9 viewport;
	pDevice->GetViewport(&viewport);

	// �r���[�{�[�h�̃N���A
	pDevice->SetViewport(&GetCamera(0)->viewport);

	// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	pDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �J�����̐ݒ�
	SetCamera(0);

	// �|���S���̕`��
	DrawPolygon();

	// �v���C���[�̕`��
	DrawPlayer();

	// �g�̕`��
	DrawFrame();

	// �r���{�[�h�̕`��
	DrawBillboard(false, false);

	// �e�̕`��
	DrawShadow();

	// �r���[�{�[�h�̃N���A
	pDevice->SetViewport(&viewport);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetTitle(MENU menu)
{
	s_Menu = menu;
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
MENU GetTitle(void)
{
	return s_Menu;
}

//--------------------------------------------------
// �J�E���g�̐ݒ�
//--------------------------------------------------
void SetTitleCnt(void)
{
	s_nCount = 0;
}

//--------------------------------------------------
// �J�E���g�̎擾
//--------------------------------------------------
int GetTitleCnt(void)
{
	return s_nCount;
}
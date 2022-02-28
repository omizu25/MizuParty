//==================================================
// 
// 3D�Q�[������ ( loop.cpp )
// Author  : katsuki mizuki
// 
//==================================================
//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "loop.h"
#include "setup.h"
#include "sound.h"
#include "title.h"

#include <assert.h>
#include <math.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define NEXT_TIME			(900)			// ���ɍs���܂Ŏ���
#define LOOP_WIDTH			(1000.0f)		// ���[�v�̕�
#define LOOP_HEIGHT			(300.0f)		// ���[�v�̍���
#define SELECT_WIDTH		(350.0f)		// �I�����̕�
#define SELECT_HEIGHT		(200.0f)		// �I�����̍���
#define DEAD_ZONE			(0.1f)			// �X�e�B�b�N�̗V��

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTextureLoop = NULL;		// ���[�v�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffLoop = NULL;		// ���[�v�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureEnd = NULL;		// ��߂�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffEnd = NULL;		// ��߂�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTexturePlay = NULL;		// �V�Ԃ̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffPlay = NULL;		// �V�Ԃ̒��_�o�b�t�@�̃|�C���^
static int							s_nTime;					// ����
static int							s_nAlpha;					// ���l��sin�J�[�u������p
static bool							s_bLoop;					// ���[�v���邩�ǂ���
static bool							s_bStickLeft;				// ���X�e�B�b�N���͂����邩�ǂ���
static bool							s_bStickRight;				// �E�X�e�B�b�N���͂����邩�ǂ���

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void Input(void);
static void SelectChange(bool bDirection);
static void SelectAlpha(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitLoop(void)
{
	s_nTime = 0;
	s_nAlpha = 0;
	s_bLoop = true;
	s_bStickLeft = false;
	s_bStickRight = false;

	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/loop.png",
		&s_pTextureLoop);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/end.png",
		&s_pTextureEnd);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/play.png",
		&s_pTexturePlay);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffLoop,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffEnd,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPlay,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffLoop->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = LOOP_WIDTH * 0.5f;
	float fHeight = LOOP_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f);

	// ���_���W�̐ݒ�
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏�����
	Initrhw(pVtx);

	// ���_�J���[�̐ݒ�
	Initcol(pVtx);

	// �e�N�X�`�����W�̏�����
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffLoop->Unlock();

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	fWidth = SELECT_WIDTH * 0.5f;
	fHeight = SELECT_HEIGHT * 0.5f;
	pos = D3DXVECTOR3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.7f, 0.0f);

	// ���_���W�̐ݒ�
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏�����
	Initrhw(pVtx);

	// ���_�J���[�̐ݒ�
	Initcol(pVtx);

	// �e�N�X�`�����W�̏�����
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffEnd->Unlock();

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPlay->Lock(0, 0, (void**)&pVtx, 0);

	pos.x = SCREEN_WIDTH * 0.3f;

	// ���_���W�̐ݒ�
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏�����
	Initrhw(pVtx);

	// ���_�J���[�̐ݒ�
	Initcol(pVtx);

	// �e�N�X�`�����W�̏�����
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPlay->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitLoop(void)
{
	if (s_pTextureLoop != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureLoop->Release();
		s_pTextureLoop = NULL;
	}

	if (s_pVtxBuffLoop != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffLoop->Release();
		s_pVtxBuffLoop = NULL;
	}

	if (s_pTextureEnd != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureEnd->Release();
		s_pTextureEnd = NULL;
	}

	if (s_pVtxBuffEnd != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffEnd->Release();
		s_pVtxBuffEnd = NULL;
	}

	if (s_pTexturePlay != NULL)
	{// �e�N�X�`���̔j��
		s_pTexturePlay->Release();
		s_pTexturePlay = NULL;
	}

	if (s_pVtxBuffPlay != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffPlay->Release();
		s_pVtxBuffPlay = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateLoop(void)
{
	// ����
	Input();

	// �I�����̃��l�ύX
	SelectAlpha();

	s_nTime++;

	if (s_nTime >= NEXT_TIME)
	{// 15�b�o���܂���
		if (s_bLoop)
		{// ���[�v����
			// �t�F�[�h�̐ݒ�
			SetFade(MODE_GAME);
		}
		else
		{// ���[�v���Ȃ�
			// ���[�h����
			SetFade(MODE_TITLE);
		}
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawLoop(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffLoop, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureLoop);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffEnd, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureEnd);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffPlay, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexturePlay);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��
}

//--------------------------------------------------
// ����
//--------------------------------------------------
static void Input(void)
{
	if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x <= DEAD_ZONE &&
		GetJoypadStick(JOYKEY_LEFT_STICK, 0).x >= -DEAD_ZONE)
	{// ���X�e�B�b�N���X���ĂȂ�
		s_bStickLeft = false;
	}

	if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x <= DEAD_ZONE &&
		GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x >= -DEAD_ZONE)
	{// �E�X�e�B�b�N���X���ĂȂ�
		s_bStickRight = false;
	}

	if (!s_bStickLeft)
	{// ���X�e�B�b�N���X���ĂȂ�
		if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -DEAD_ZONE)
		{// ���X�e�B�b�N���X����
			s_bStickLeft = true;

			if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < 0.0f)
			{// ��
				// �I�����̕ύX
				SelectChange(true);
			}
			else
			{// �E
				// �I�����̕ύX
				SelectChange(false);
			}
		}
	}

	if (!s_bStickRight)
	{// �E�X�e�B�b�N���X���ĂȂ�
		if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x < -DEAD_ZONE)
		{// �E�X�e�B�b�N���X����
			s_bStickRight = true;

			if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x < 0.0f)
			{// ��
				// �I�����̕ύX
				SelectChange(true);
			}
			else
			{// �E
				// �I�����̕ύX
				SelectChange(false);
			}
		}
	}

	if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFT) ||
		GetJoypadTrigger(JOYKEY_LEFT))
	{// A�L�[�������ꂽ
		// �I�����̕ύX
		SelectChange(true);
	}
	else if(GetKeyboardTrigger(DIK_D) ||GetKeyboardTrigger(DIK_RIGHT) ||
		GetJoypadTrigger(JOYKEY_RIGHT))
	{// D�L�[�������ꂽ
		// �I�����̕ύX
		SelectChange(false);
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) ||
		GetJoypadTrigger(JOYKEY_B) || GetJoypadTrigger(JOYKEY_A) ||
		GetJoypadTrigger(JOYKEY_START))
	{// �X�y�[�X�L�[�������ꂽ
		if (s_bLoop)
		{// ���[�v����
			// �t�F�[�h�̐ݒ�
			SetFade(MODE_GAME);
		}
		else
		{// ���[�v���Ȃ�
			// ���[�h����
			SetFade(MODE_TITLE);
		}

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_���艹);
	}
}

//--------------------------------------------------
// �I�����̕ύX
// false : �E true : ��
//--------------------------------------------------
static void SelectChange(bool bDirection)
{
	if (bDirection)
	{// ��
		s_bLoop = true;

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̏�����
		Initcol(pVtx);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuffEnd->Unlock();
	}
	else
	{// �E
		s_bLoop = false;

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffPlay->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̏�����
		Initcol(pVtx);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuffPlay->Unlock();
	}

	s_nAlpha = 0;
	s_nTime = 0;

	// �T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_SE_�I����);
}

//--------------------------------------------------
// �I�����̃��l�ύX
//--------------------------------------------------
static void SelectAlpha(void)
{
	s_nAlpha++;

	if (s_bLoop)
	{// ���[�v����
		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		float fAlpha = (sinf((s_nAlpha * 0.01f) * (D3DX_PI * 2.0f)) * 0.4f) + 0.6f;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffPlay->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̐ݒ�
		Setcol(pVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuffPlay->Unlock();
	}
	else
	{// ���[�v���Ȃ�
		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		float fAlpha = (sinf((s_nAlpha * 0.01f) * (D3DX_PI * 2.0f)) * 0.4f) + 0.6f;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̐ݒ�
		Setcol(pVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuffEnd->Unlock();
	}
}
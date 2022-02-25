//==================================================
// 
// 3D�Q�[������ ( rule.cpp )
// Author  : katsuki mizuki
// 
//==================================================
#include "fade.h"
#include "input.h"
#include "number.h"
#include "player.h"
#include "rule.h"
#include "setup.h"
#include "time.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define RULE_WIDTH			(160.0f)		// �����̕�
#define RULE_HEIGHT			(100.0f)		// �����̍���
#define A_OR_D_WIDTH		(170.0f)		// AorD�̕�
#define A_OR_D_HEIGHT		(70.0f)			// AorD�̍���
#define STICK_WIDTH			(220.0f)		// �X�e�B�b�N�̕�
#define STICK_HEIGHT		(100.0f)		// �X�e�B�b�N�̍���
#define SPACE_WIDTH			(170.0f)		// �X�y�[�X�̕�
#define SPACE_HEIGHT		(70.0f)			// �X�y�[�X�̍���
#define HINT_WIDTH			(300.0f)		// �q���g�̕�
#define HINT_HEIGHT			(80.0f)			// �q���g�̍���

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureAorD = NULL;		// AorD�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffAorD = NULL;		// AorD�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureStick= NULL;		// �X�e�B�b�N�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffStick = NULL;		// �X�e�B�b�N�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureStop = NULL;		// �~�߂�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffStop = NULL;		// �~�߂�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureAorB = NULL;		// AorB�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffAorB = NULL;		// AorB�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureSpace = NULL;		// �X�y�[�X�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffSpace = NULL;		// �X�y�[�X�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureHint = NULL;		// �q���g�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffHint = NULL;		// �q���g�̒��_�o�b�t�@�̃|�C���^

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void InitMove(void);
static void InitAorD(void);
static void InitStick(void);
static void InitStop(void);
static void InitAorB(void);
static void InitSpace(void);
static void InitHint(void);
static void DrawStop(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitRule(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_SLOPE:		// ��

		// �ړ�
		InitMove();

		// AorD
		InitAorD();

		// �X�e�B�b�N
		InitStick();

		break;

	case MENU_ROTATION:		// ��]
	case MENU_STOP:			// �~�߂�

		// �~�߂�
		InitStop();

		// AorB
		InitAorB();

		// �X�y�[�X
		InitSpace();

		// �q���g
		InitHint();

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitRule(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̔j��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pTextureAorD != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureAorD->Release();
		s_pTextureAorD = NULL;
	}

	if (s_pVtxBuffAorD != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffAorD->Release();
		s_pVtxBuffAorD = NULL;
	}

	if (s_pTextureStick != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureStick->Release();
		s_pTextureStick = NULL;
	}

	if (s_pVtxBuffStick != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffStick->Release();
		s_pVtxBuffStick = NULL;
	}

	if (s_pTextureStop != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureStop->Release();
		s_pTextureStop = NULL;
	}

	if (s_pVtxBuffStop != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffStop->Release();
		s_pVtxBuffStop = NULL;
	}

	if (s_pTextureAorB != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureAorB->Release();
		s_pTextureAorB = NULL;
	}

	if (s_pVtxBuffAorB != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffAorB->Release();
		s_pVtxBuffAorB = NULL;
	}

	if (s_pTextureSpace != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureSpace->Release();
		s_pTextureSpace = NULL;
	}

	if (s_pVtxBuffSpace != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffSpace->Release();
		s_pVtxBuffSpace = NULL;
	}

	if (s_pTextureHint != NULL)
	{// �e�N�X�`���̔j��
		s_pTextureHint->Release();
		s_pTextureHint = NULL;
	}

	if (s_pVtxBuffHint != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuffHint->Release();
		s_pVtxBuffHint = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateRule(void)
{

}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawRule(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_SLOPE:		// ��

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuffAorD, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTextureAorD);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuffStick, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTextureStick);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��

		break;

	case MENU_ROTATION:		// ��]
	case MENU_STOP:			// �~�߂�

		// �~�߂�
		DrawStop();

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �ړ�
//--------------------------------------------------
static void InitMove(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\move.png",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = RULE_WIDTH * 0.5f;
	float fHeight = RULE_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.91f, SCREEN_HEIGHT * 0.15f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// AorD
//--------------------------------------------------
static void InitAorD(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/AorD.png",
		&s_pTextureAorD);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffAorD,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffAorD->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = A_OR_D_WIDTH * 0.5f;
	float fHeight = A_OR_D_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.085f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffAorD->Unlock();
}

//--------------------------------------------------
// �X�e�B�b�N
//--------------------------------------------------
static void InitStick(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/stick.png",
		&s_pTextureStick);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffStick,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffStick->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = STICK_WIDTH * 0.5f;
	float fHeight = STICK_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.225f, 0.0f);

	// ���_���W�̐ݒ�
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏�����
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏�����
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffStick->Unlock();
}

//--------------------------------------------------
// �Ƃ߂�
//--------------------------------------------------
static void InitStop(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\stop.png",
		&s_pTextureStop);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffStop,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffStop->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = RULE_WIDTH * 0.5f;
	float fHeight = RULE_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.15f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffStop->Unlock();
}

//--------------------------------------------------
// AorB
//--------------------------------------------------
static void InitAorB(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/AorB.png",
		&s_pTextureAorB);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffAorB,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffAorB->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = A_OR_D_WIDTH * 0.5f;
	float fHeight = A_OR_D_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.225f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffAorB->Unlock();
}

//--------------------------------------------------
// �X�y�[�X
//--------------------------------------------------
static void InitSpace(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/space.png",
		&s_pTextureSpace);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffSpace,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffSpace->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SPACE_WIDTH * 0.5f;
	float fHeight = SPACE_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.075f, 0.0f);

	// ���_���W�̐ݒ�
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏�����
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏�����
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffSpace->Unlock();
}

//--------------------------------------------------
// �q���g
//--------------------------------------------------
static void InitHint(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/hint.png",
		&s_pTextureHint);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffHint,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffHint->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = HINT_WIDTH * 0.5f;
	float fHeight = HINT_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// ���_���W�̐ݒ�
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏�����
	Initrhw(pVtx);

	// ���_�J���[�̏�����
	Initcol(pVtx);

	// �e�N�X�`�����W�̏�����
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffHint->Unlock();
}

//--------------------------------------------------
// �~�߂�
//--------------------------------------------------
static void DrawStop(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffStop, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureStop);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffAorB, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureAorB);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffSpace, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureSpace);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffHint, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureHint);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��
}
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

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define RESULT_WIDTH		(400.0f)		// ��
#define RESULT_HEIGHT		(150.0f)		// ����
#define A_OR_D_WIDTH		(200.0f)		// AorD�̕�
#define A_OR_D_HEIGHT		(80.0f)		// AorD�̍���
#define STICK_WIDTH			(200.0f)		// �X�e�B�b�N�̕�
#define STICK_HEIGHT		(80.0f)		// �X�e�B�b�N�̍���

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureAorD = NULL;		// AorD�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffAorD = NULL;		// AorD�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureStick= NULL;		// �X�e�B�b�N�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffStick = NULL;		// �X�e�B�b�N�̒��_�o�b�t�@�̃|�C���^

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void InitAorD(void);
static void InitStick(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitRule(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\word_move.png",
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

	float fWidth = RESULT_WIDTH * 0.5f;
	float fHeight = RESULT_HEIGHT * 0.5f;
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
	s_pVtxBuff->Unlock();

	// AorD
	InitAorD();

	// �X�e�B�b�N
	InitStick();
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
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.075f, 0.0f);

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
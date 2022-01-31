//==================================================
// 
// 3D�Q�[������ ( result.cpp )
// Author  : katsuki mizuki
// 
//==================================================
#include "fade.h"
#include "input.h"
#include "number.h"
#include "result.h"
#include "setup.h"
#include "time.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define RESULT_WIDTH		(600.0f)		// �����Ə����̕�
#define RESULT_HEIGHT		(300.0f)		// �����Ə����̍���
#define TIME_WIDTH			(400.0f)		// �^�C���̕�
#define TIME_HEIGHT			(160.0f)		// �^�C���̍���
#define NUMBER_WIDTH		(100.0f)		// ���̕�
#define NUMBER_HEIGHT		(250.0f)		// ���̍���
#define WIDTH_INTERVAL		(40.0f)			// ���̊Ԋu
#define HEIGHT_INTERVAL		(20.0f)			// �����̊Ԋu

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�̃|�C���^
static int							s_nTime;					// �^�C��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitResult(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result004.png",
		&s_pTexture);

	s_nTime = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	/* �������� */

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = RESULT_WIDTH * 0.5f;
	float fHeight = RESULT_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, RESULT_HEIGHT * 0.35f, 0.0f);

	// ���_���W�̐ݒ菈��
	Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

	// rhw�̏���������
	Initrhw(pVtx);

	// ���_�J���[�̐ݒ菈��
	Setcol(pVtx, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// �e�N�X�`�����W�̏���������
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	/* �����֘A�� */

	// �ʒu������������
	D3DXVECTOR3 posNumber = D3DXVECTOR3(1000.0f, SCREEN_HEIGHT * 0.7f, 0.0f);

	int nTime = GetTime();
	int aNumber[MAX_TIME];

	for (int i = 0; i < MAX_TIME; i++)
	{// �P�����ɕ�����
		aNumber[i] = nTime % 10;
		nTime /= 10;

		float fInterval = (NUMBER_WIDTH * i) + (WIDTH_INTERVAL * (i / 2));

		// ���̐ݒ菈��
		SetRightNumber(D3DXVECTOR3(posNumber.x - fInterval, posNumber.y, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, USE_RESULT);
	}
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitResult(void)
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
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) ||
		GetJoypadTrigger(JOYKEY_B, 0) || GetJoypadTrigger(JOYKEY_START, 0))
	{// ����L�[(ENTER�L�[)�������ꂽ���ǂ���
		// ���[�h����
		SetFade(MODE_TITLE);
	}

	s_nTime++;

	if (s_nTime >= 900)
	{// 15�b�o���܂���
		// ���[�h����
		SetFade(MODE_TITLE);
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawResult(void)
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
}

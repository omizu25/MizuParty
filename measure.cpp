//==================================================
// 
// 3D�Q�[������ ( measure.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "measure.h"
#include "player.h"
#include "setup.h"

#include <assert.h>
//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_HEIGHT		(1.0f)			// �����̍ő�l
#define MAX_DEPTH		(50.0f)			// ���s���̍ő�l
#define START_WIDTH		(0.0f)			// ���̍ŏ��̒l
#define START_POS_Z		(-30.0f)		// Z�̍ŏ��̈ʒu

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	float			fWidth;			// ��
}Measure;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static Measure						s_Measure;				// ���W���[�̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitMeasure(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	s_Measure.pos = D3DXVECTOR3(0.0f, 0.0f, START_POS_Z);
	s_Measure.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_Measure.fWidth = START_WIDTH;

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	Initpos(pVtx);

	// �e���_�̖@���̐ݒ�
	Initnor(pVtx);

	// ���_�J���[�̐ݒ�
	Setcol(pVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// �e�N�X�`�����W�̐ݒ�
	Inittex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitMeasure(void)
{
	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMeasure(void)
{
	s_Measure.fWidth = GetCamera(0)->posV.x;

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (GetPlayer()->pos.x >= 0.0f)
	{// �v���X
		// ���_���W�̐ݒ�
		Setpos(pVtx, s_Measure.pos, s_Measure.fWidth, MAX_HEIGHT, MAX_DEPTH * 0.5f, SETPOS_LEFT);
	}
	else
	{// �}�C�i�X
		// ���_���W�̐ݒ�
		Setpos(pVtx, s_Measure.pos, -s_Measure.fWidth, MAX_HEIGHT, MAX_DEPTH * 0.5f, SETPOS_RIGHT);
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawMeasure(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_Measure.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Measure.rot.y, s_Measure.rot.x, s_Measure.rot.z);
	D3DXMatrixMultiply(&s_Measure.mtxWorld, &s_Measure.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_Measure.pos.x, s_Measure.pos.y, s_Measure.pos.z);
	D3DXMatrixMultiply(&s_Measure.mtxWorld, &s_Measure.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_Measure.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
	
	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);
}

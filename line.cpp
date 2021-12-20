//==================================================
// 
// 3D�Q�[������ ( line.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "line.h"
#include "setup.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_LINE		(256)		//���̍ő吔
#define BASIC_WIDTH		(15.0f)		//���̊�l
#define BASIC_HEIGHT	(0.1f)		//�����̊�l
#define BASIC_DEPTH		(15.0f)		//���s���̊�l

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXCOLOR		col;			// �F
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	bool			bUse;			// �g�p���Ă��邩�ǂ���
}Line;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static Line							s_Line[MAX_LINE];		// ���̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitLine(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_LINE) * 2 * MAX_LINE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_LINE,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//�������̃N���A
	memset(s_Line, NULL, sizeof(s_Line));

	VERTEX_LINE *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_LINE; i++)
	{
		// �S�Ă̏�����
		InitAll(pVtx);

		pVtx += 2;
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitLine(void)
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
void UpdateLine(void)
{

}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawLine(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < MAX_LINE; i++)
	{
		Line *pLine = &s_Line[i];

		if (!pLine->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pLine->mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pLine->rot.y, pLine->rot.x, pLine->rot.z);
		D3DXMatrixMultiply(&pLine->mtxWorld, &pLine->mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pLine->pos.x, pLine->pos.y, pLine->pos.z);
		D3DXMatrixMultiply(&pLine->mtxWorld, &pLine->mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pLine->mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_LINE));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_LINE);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_LINELIST,		// �v���~�e�B�u�̎��
			i * 2,				// �`�悷��ŏ��̒��_�C���f�b�N�X
			1);					// �v���~�e�B�u(�|���S��)��
	}

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

	// ���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	VERTEX_LINE *pVtx = NULL;		// ���_���ւ̃|�C���^

	for (int i = 0; i < MAX_LINE; i++)
	{
		Line *pLine = &s_Line[i];

		if (pLine->bUse)
		{//�g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		pLine->pos = pos;
		pLine->rot = rot;
		pLine->col = col;
		pLine->bUse = true;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 2);		//�Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), start, end);

		// ���_�J���[�̐ݒ�
		//SetcolLine(pVtx, col);

		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

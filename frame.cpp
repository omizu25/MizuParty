//==================================================
// 
// 3D�Q�[������ ( frame.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "frame.h"
#include "player.h"
#include "setup.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_FRAME		(256)		// �g�̍ő吔
#define FRAME_SIZE		(5.0f)		// �g�̃T�C�Y

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	D3DXMATRIX				mtxWorld;		// ���[���h�}�g���b�N�X
	float					fWidth;			// ��
	float					fHeight;		// ����
	bool					bUse;			// �g�p���Ă��邩�ǂ���
	bool					bDraw;			// �\�����邩�ǂ���
	LPDIRECT3DTEXTURE9		pTexture;		// �e�N�X�`��
}Frame;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static Frame						s_frame[MAX_FRAME];		// �g�̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitFrame(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/block005.png",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_FRAME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//�������̃N���A
	memset(s_frame, 0, sizeof(s_frame));

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_FRAME; i++)
	{
		//�S�Ă̏���������
		InitAll(pVtx);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitFrame(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateFrame(void)
{

}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawFrame(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_FRAME; i++)
	{
		Frame *pFrame = &s_frame[i];

		if (!pFrame->bUse || !pFrame->bDraw)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pFrame->mtxWorld);
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �J�����̋t�s���ݒ�
		pFrame->mtxWorld._11 = mtxView._11;
		pFrame->mtxWorld._12 = mtxView._21;
		pFrame->mtxWorld._13 = mtxView._31;
		pFrame->mtxWorld._21 = mtxView._12;
		pFrame->mtxWorld._22 = mtxView._22;
		pFrame->mtxWorld._23 = mtxView._32;
		pFrame->mtxWorld._31 = mtxView._13;
		pFrame->mtxWorld._32 = mtxView._23;
		pFrame->mtxWorld._33 = mtxView._33;

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pFrame->pos.x, pFrame->pos.y, pFrame->pos.z);
		D3DXMatrixMultiply(&pFrame->mtxWorld, &pFrame->mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pFrame->mtxWorld);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			i * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��
	}

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

	// ���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�o�b�t�@�̒l�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// �V�K�[�x�l <= Z�o�b�t�@�[�x�l (�����ݒ�)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetFrame(D3DXVECTOR3 pos, float fWidth, float fHeight, LPDIRECT3DTEXTURE9 *pTexture)
{
	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	for (int i = 0; i < MAX_FRAME; i++)
	{
		Frame *pFrame = &s_frame[i];

		if (pFrame->bUse)
		{//�g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		pFrame->pos = pos;
		pFrame->fWidth = fWidth;
		pFrame->fHeight = fHeight;
		pFrame->pTexture = *pTexture;
		pFrame->bUse = true;
		pFrame->bDraw = false;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth + FRAME_SIZE, fHeight + FRAME_SIZE, 0.0f, SETPOS_MIDDLE);

		// ���_�̖@���̐ݒ�
		Setnor(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// �����蔻��
//--------------------------------------------------
void CollisionFrame(D3DXVECTOR3 *pPos)
{
	for (int i = 0; i < MAX_FRAME; i++)
	{
		Frame *pFrame = &s_frame[i];

		if (!pFrame->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		pFrame->bDraw = false;

	}

	Billboard *pBillboard = GetBillboard();

	for (int i = 0; i < MAX_BILLBOARD; i++, pBillboard++)
	{
		if (!pBillboard->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		if (pBillboard->pos.x + pBillboard->fWidth >= pPos->x &&
			pBillboard->pos.x - pBillboard->fWidth <= pPos->x &&
			pBillboard->pos.z + pBillboard->fHeight >= pPos->z &&
			pBillboard->pos.z - pBillboard->fHeight <= pPos->z)
		{// �������Ă�
			for (int j = 0; j < MAX_FRAME; j++)
			{
				Frame *pFrame = &s_frame[j];

				if (!pFrame->bUse || pBillboard->pTexture != pFrame->pTexture)
				{//�g�p����Ă��Ȃ�
					continue;
				}

				/*�� �g�p����Ă��� ��*/

				pFrame->bDraw = true;
			}
		}
	}
}

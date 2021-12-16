//==================================================
// 
// 3D�Q�[������ ( effect.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "effect.h"
#include "setup.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_EFFECT		(4096)		// �G�t�F�N�g�̍ő吔

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		move;			// �ړ���
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXCOLOR		col;			// �F
	float			fWidth;			// ��
	float			fHeight;		// ����
	int				nLife;			// ����(�\������)
	bool			bAdd;			// ���Z���������邩�ǂ���
	bool			bUse;			// �g�p���Ă��邩�ǂ���
}Effect;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�̃|�C���^
static Effect						s_aEffect[MAX_EFFECT];		// �G�t�F�N�g�̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitEffect(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\effect002.jpg",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �������̃N���A
	memset(&s_aEffect, NULL, sizeof(s_aEffect));

	// �������̐ݒ�
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		// �S�Ă̏���������
		InitAll3D(pVtx);

		pVtx += 4;		// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitEffect(void)
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
void UpdateEffect(void)
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		Effect *pEffect = &s_aEffect[i];

		if (!pEffect->bUse)
		{// �g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		pEffect->pos += pEffect->move;

		pEffect->nLife--;

		if (pEffect->nLife <= 0)
		{// ����������
			pEffect->bUse = false;
		}

		VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// �Y���̈ʒu�܂Ői�߂�

		// ���_���W�̍X�V
		Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pEffect->fWidth, pEffect->fHeight, 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawEffect(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�o�b�t�@�̒l��ύX����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// �K����������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// �G�t�F�N�g�̕`��
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		Effect *pEffect = &s_aEffect[i];

		if (!pEffect->bUse)
		{// �g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		if (pEffect->bAdd)
		{// ���Z��������
			// ���u�����f�B���O�����H�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);				// ���̂܂ܑ���
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pEffect->mtxWorld);
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �J�����̋t�s���ݒ�
		pEffect->mtxWorld._11 = mtxView._11;
		pEffect->mtxWorld._13 = mtxView._31;
		pEffect->mtxWorld._31 = mtxView._13;
		pEffect->mtxWorld._33 = mtxView._33;

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pEffect->pos.x, pEffect->pos.y, pEffect->pos.z);
		D3DXMatrixMultiply(&pEffect->mtxWorld, &pEffect->mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorld);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			i * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��
	}

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ��������

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
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nLife, bool bAdd)
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		Effect *pEffect = &s_aEffect[i];

		if (pEffect->bUse)
		{// �g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		pEffect->pos = pos;
		pEffect->move = move;
		pEffect->col = col;
		pEffect->fWidth = fWidth;
		pEffect->fHeight = fHeight;
		pEffect->nLife = nLife;
		pEffect->bAdd = bAdd;
		pEffect->bUse = true;		// �g�p���Ă����Ԃɂ���

		VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// �Y���̈ʒu�܂Ői�߂�

		// ���_���W�̍X�V
		Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		// ���_�J���[�̐ݒ�
		Setcol3D(pVtx, col);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;		// ������for���𔲂���
	}
}

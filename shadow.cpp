//==================================================
// 
// 3D�Q�[������ ( shadow.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "polygon.h"
#include "setup.h"
#include "shadow.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_SHADOW		(256)		//�e�̍ő吔
#define BASIC_WIDTH		(15.0f)		//���̊�l
#define BASIC_HEIGHT	(0.1f)		//�����̊�l
#define BASIC_DEPTH		(15.0f)		//���s���̊�l

//--------------------------------------------------
// �\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	bool			bUse;			// �g�p���Ă��邩�ǂ���
}Shadow;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�̃|�C���^
static Shadow						s_shadow[MAX_SHADOW];		// �e�̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitShadow(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//�������̃N���A
	memset(s_shadow, NULL, sizeof(s_shadow));

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_SHADOW; i++)
	{
		// �S�Ă̏�����
		InitAll3D(pVtx);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitShadow(void)
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
void UpdateShadow(void)
{

}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawShadow(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// ���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_SHADOW; i++)
	{
		Shadow *pShadow = &s_shadow[i];

		if (!pShadow->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pShadow->mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);
		D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);
		D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			i * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��
	}

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

	// ���ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int i;
	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	for (i = 0; i < MAX_SHADOW; i++)
	{
		Shadow *pShadow = &s_shadow[i];

		if (pShadow->bUse)
		{//�g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		pShadow->pos = pos;
		pShadow->rot = rot;
		pShadow->bUse = true;

		polygon *pPolygon = GetPolygon();

		pShadow->pos.y = pPolygon->pos.y + 0.1f;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		float fSize = pos.y * 0.15f;

		// ���_���W�̐ݒ�
		Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BASIC_WIDTH + fSize, BASIC_HEIGHT, BASIC_DEPTH + fSize);

		float Alpha = 1.0f - (pos.y * 0.005f);

		// ���_�J���[�̐ݒ�
		Setcol3D(pVtx, 1.0f, 1.0f, 1.0f, Alpha);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}

	return i;
}

//--------------------------------------------------
// �ʒu�̐ݒ�
//--------------------------------------------------
void SetPosShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	Shadow *pShadow = &s_shadow[nIdxShadow];

	pShadow->pos = pos;

	polygon *pPolygon = GetPolygon();

	pShadow->pos.y = pPolygon->pos.y + 0.1f;

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxShadow * 4);		//�Y���̈ʒu�܂Ői�߂�

	float fSize = pos.y * 0.15f;

	// ���_���W�̐ݒ�
	Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BASIC_WIDTH + fSize, BASIC_HEIGHT, BASIC_DEPTH + fSize);

	float Alpha = 1.0f - (pos.y * 0.005f);

	// ���_�J���[�̐ݒ�
	Setcol3D(pVtx, 1.0f, 1.0f, 1.0f, Alpha);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}
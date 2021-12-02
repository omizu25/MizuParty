//==================================================
// 
// 3D�Q�[������ ( meshfield.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "input.h"
#include "main.h"
#include "meshfield.h"
#include "setup.h"
#include "wall.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_WIDTH		(150.0f)		// ���̍ő�l
#define MAX_HEIGHT		(0.0f)			// �����̍ő�l
#define MAX_DEPTH		(150.0f)		// ���s���̍ő�l
#define MAX_SIZE		(10)			// �T�C�Y�̍ő�l
#define MIN_SIZE		(1)				// �T�C�Y�̍ŏ��l
#define START_SIZE		(3)				// �T�C�Y�̍ŏ��̒l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DINDEXBUFFER9		s_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
static MeshField					s_meshfield;			// ���b�V���t�B�[���h�̏��
static int							s_nMeshFieldSize;		// ���b�V���t�B�[���h�̃T�C�Y
static int							s_nVtxNumber;			// ���_��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void ResetBuff(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitMeshField(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko000.jpg",
		&s_pTexture);

	s_nMeshFieldSize = START_SIZE;

	// �������̃N���A
	memset(&s_meshfield, NULL, sizeof(s_meshfield));
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitMeshField(void)
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

	if (s_pIdxBuff != NULL)
	{// �C���f�b�N�X�o�b�t�@�̉��
		s_pIdxBuff->Release();
		s_pIdxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMeshField(void)
{
	if (GetKeyboardTrigger(DIK_V))
	{// V�L�[�������ꂽ
		s_nMeshFieldSize++;
	}
	else if (GetKeyboardTrigger(DIK_B))
	{// B�L�[�������ꂽ
		s_nMeshFieldSize--;
	}

	if (GetKeyboardTrigger(DIK_V) || GetKeyboardTrigger(DIK_B))
	{// V,B�L�[�������ꂽ

		if (s_nMeshFieldSize >= MAX_SIZE)
		{// �w��̐��ȏ�
			s_nMeshFieldSize = MAX_SIZE;
		}
		else if (s_nMeshFieldSize <= MIN_SIZE)
		{// �w��̐��ȉ�
			s_nMeshFieldSize = MIN_SIZE;
		}

		// �o�b�t�@�̃��Z�b�g
		ResetBuff();

		// �ݒ�
		SetMeshField();

		// �ǂ̃��Z�b�g
		ResetWall();

		// �ǂ̐ݒu
		InstallationWall();
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawMeshField(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_meshfield.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_meshfield.rot.y, s_meshfield.rot.x, s_meshfield.rot.z);
	D3DXMatrixMultiply(&s_meshfield.mtxWorld, &s_meshfield.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_meshfield.pos.x, s_meshfield.pos.y, s_meshfield.pos.z);
	D3DXMatrixMultiply(&s_meshfield.mtxWorld, &s_meshfield.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_meshfield.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(s_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	int nPolygonNumber = (s_nVtxNumber * 2) - 6;

	// �|���S���`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�o�b�t�@
		0,							// �C���f�b�N�X�̍ŏ��l
		s_nVtxNumber,				// ���_��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		nPolygonNumber);			// �v���~�e�B�u(�|���S��)��

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetMeshField(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�����v�Z
	s_nVtxNumber = (int)pow(s_nMeshFieldSize + 1, 2.0f);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * s_nVtxNumber,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �������̃N���A
	memset(&s_meshfield, NULL, sizeof(s_meshfield));

	// ���E�����E���s���̐ݒ�
	s_meshfield.fWidth = MAX_WIDTH * (s_nMeshFieldSize * 0.5f);
	s_meshfield.fHeight = MAX_HEIGHT;
	s_meshfield.fDepth = MAX_DEPTH * (s_nMeshFieldSize * 0.5f);

	int nWrapping = s_nMeshFieldSize + 1;

	for (int i = 0; i < s_nVtxNumber; i++)
	{
		float fXPos = (float)(i % nWrapping) - (s_nMeshFieldSize * 0.5f);
		float fZPos = ((float)(i / nWrapping) - (s_nMeshFieldSize * 0.5f)) * -1.0f;

		// ���_���W�̐ݒ�
		pVtx[i].pos = D3DXVECTOR3(MAX_WIDTH * fXPos, 0.0f, MAX_DEPTH * fZPos);

		// �e���_�̖@���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		float fUTex = (float)(i % nWrapping);
		float fVTex = (float)(i / nWrapping);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = D3DXVECTOR2(fUTex, fVTex);
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	// �C���f�b�N�X�����v�Z
	int nPolygonNumber = (s_nVtxNumber * 2) - 6;
	int nIdxNumber = nPolygonNumber + 2;

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(VERTEX_3D) * nIdxNumber,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		NULL);

	WORD *pIdx = NULL;		// �C���f�b�N�X���ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N
	s_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	for (int x = 0, z = 0; z < s_nMeshFieldSize; x++, z++)
	{
		for (; x < (nWrapping * (z + 1)) + z; x++)
		{
			pIdx[x * 2] = (WORD)x - (WORD)z + (WORD)nWrapping;
			pIdx[(x * 2) + 1] = (WORD)x - (WORD)z;
		}

		if (z < s_nMeshFieldSize - 1)
		{// ����ŏI��肶��Ȃ��Ȃ�
			int nData = (x % nWrapping) * nWrapping;

			pIdx[x * 2] = (WORD)nData + (WORD)s_nMeshFieldSize;
			pIdx[(x * 2) + 1] = (WORD)nData + (WORD)(nWrapping * 2);
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	s_pIdxBuff ->Unlock();
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
MeshField *GetMeshField(void)
{
	return &s_meshfield;
}

//--------------------------------------------------
// �o�b�t�@�̃��Z�b�g
//--------------------------------------------------
static void ResetBuff(void)
{
	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pIdxBuff != NULL)
	{// �C���f�b�N�X�o�b�t�@�̉��
		s_pIdxBuff->Release();
		s_pIdxBuff = NULL;
	}
}
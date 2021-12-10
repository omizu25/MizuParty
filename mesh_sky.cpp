//==================================================
// 
// 3D�Q�[������ ( mesh_sky.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "input.h"
#include "main.h"
#include "mesh_sky.h"
#include "setup.h"
#include "wall.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_SIZE				(1000.0f)		// �T�C�Y�̍ő�l
#define MAX_HORIZONTAL			(30)			// ���̍ő�l
#define MIN_HORIZONTAL			(5)				// ���̍ŏ��l
#define START_HORIZONTAL		(8)				// ���̍ŏ��̒l
#define MAX_VERTICAL			(30)			// �c�̍ő�l
#define MIN_VERTICAL			(5)				// �c�̍ŏ��l
#define START_VERTICAL			(10)			// �c�̍ŏ��̒l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffCone = NULL;		// �~���̒��_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DINDEXBUFFER9		s_pIdxBuff = NULL;			// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
static MeshSky						s_meshSky;					// ���b�V����̏��
static MeshSkyNumber				s_Number;					// ���b�V����̐��n�̏��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void Input(void);
static void ResetBuff(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitMeshSky(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko003.jpg",
		&s_pTexture);

	// �������̃N���A
	memset(&s_meshSky, NULL, sizeof(s_meshSky));
	memset(&s_Number, NULL, sizeof(s_Number));

	// ���E�c�̏�����
	s_Number.nHorizontal = START_HORIZONTAL;
	s_Number.nVertical = START_VERTICAL;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitMeshSky(void)
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

	if (s_pVtxBuffCone != NULL)
	{// �~���̒��_�o�b�t�@�̉��
		s_pVtxBuffCone->Release();
		s_pVtxBuffCone = NULL;
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
void UpdateMeshSky(void)
{
	// ����
	Input();

	if (GetKeyboardTrigger(DIK_V) || GetKeyboardTrigger(DIK_B) ||
		GetKeyboardTrigger(DIK_N) || GetKeyboardTrigger(DIK_M))
	{// V, B, N, M�L�[�������ꂽ

		// �w��̒l�ȏ�E�ȉ�
		Specified(&s_Number.nHorizontal, MAX_HORIZONTAL, MIN_HORIZONTAL);

		// �w��̒l�ȏ�E�ȉ�
		Specified(&s_Number.nVertical, MAX_VERTICAL, MIN_VERTICAL);

		// �o�b�t�@�̃��Z�b�g
		ResetBuff();

		// �ݒ�
		SetMeshSky();

		// �ǂ̃��Z�b�g
		ResetWall();

		// �ǂ̐ݒu
		InstallationWall();
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawMeshSky(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_meshSky.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_meshSky.rot.y, s_meshSky.rot.x, s_meshSky.rot.z);
	D3DXMatrixMultiply(&s_meshSky.mtxWorld, &s_meshSky.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_meshSky.pos.x, s_meshSky.pos.y, s_meshSky.pos.z);
	D3DXMatrixMultiply(&s_meshSky.mtxWorld, &s_meshSky.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_meshSky.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(s_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	// �|���S���`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�o�b�t�@
		0,							// �C���f�b�N�X�̍ŏ��l
		s_Number.nVtx,				// ���_��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		s_Number.nPolygon);			// �v���~�e�B�u(�|���S��)��

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffCone, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLEFAN,			// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		s_Number.nHorizontal);		// �v���~�e�B�u(�|���S��)��
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetMeshSky(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nXLine = s_Number.nHorizontal + 1;
	int nYLine = s_Number.nVertical + 1;

	// ���_�����v�Z
	s_Number.nVtx = nXLine * nYLine;

	// �C���f�b�N�X�����v�Z
	s_Number.nIdx = ((nXLine * 2) * s_Number.nVertical) + ((s_Number.nVertical - 1) * 2);

	// �|���S�������v�Z
	s_Number.nPolygon = (s_Number.nHorizontal * s_Number.nVertical * 2) + ((s_Number.nVertical - 1) * 4);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * s_Number.nVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// �������̃N���A
	memset(&s_meshSky, NULL, sizeof(s_meshSky));

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int y = 0; y < nYLine; y++)
	{
		float fYRot = (((D3DX_PI * 0.25f) / s_Number.nVertical) * y) + (D3DX_PI * 0.25f);

		float fYPos = cosf(fYRot) * MAX_SIZE;

		for (int x = 0; x < nXLine; x++)
		{
			float fRot = ((D3DX_PI * 2.0f) / s_Number.nHorizontal) * x;

			// �p�x�̐��K��
			NormalizeRot(&fRot);

			float fXPos = sinf(fRot) * sinf(fYRot) * MAX_SIZE;
			float fZPos = cosf(fRot) * sinf(fYRot) * MAX_SIZE;
			D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

			// ���_���W�̐ݒ�
			pVtx[x + (y * nXLine)].pos = pos;

			D3DXVECTOR3 vec;

			// ���K������ ( �傫�� 1 �̃x�N�g���ɂ��� )
			D3DXVec3Normalize(&vec, &pos);

			// �e���_�̖@���̐ݒ�
			pVtx[x + (y * nXLine)].nor = vec;

			// ���_�J���[�̐ݒ�
			pVtx[x + (y * nXLine)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			float fVTex = (1.0f / s_Number.nVertical) * y;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[x + (y * nXLine)].tex = D3DXVECTOR2((float)x, fVTex);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(VERTEX_3D) * s_Number.nIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		NULL);

	WORD *pIdx = NULL;		// �C���f�b�N�X���ւ̃|�C���^
			
	// �C���f�b�N�X�o�b�t�@�����b�N
	s_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	for (int x = 0, y = 0; y < s_Number.nVertical; x++, y++)
	{
		for (; x < (nXLine * (y + 1)) + y; x++)
		{
			pIdx[x * 2] = (WORD)(x - y + nXLine);
			pIdx[(x * 2) + 1] = (WORD)(x - y);
			x = x;
		}

		if (y < s_Number.nVertical - 1)
		{// ����ŏI��肶��Ȃ��Ȃ�
			pIdx[x * 2] = (WORD)(x - (y + 1));
			pIdx[(x * 2) + 1] = (WORD)((x * 2) - (y * (s_Number.nHorizontal + 3)));
			x = x;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	s_pIdxBuff->Unlock();

	// �~���̒��_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * (s_Number.nHorizontal + 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffCone,
		NULL);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffCone->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < nXLine; i++)
	{
		float fYRot = D3DX_PI * 0.25f;
		float fRot = ((D3DX_PI * 2.0f) / s_Number.nHorizontal) * i;

		// �p�x�̐��K��
		NormalizeRot(&fRot);

		float fXPos = sinf(-fRot) * sinf(fYRot) * MAX_SIZE;
		float fYPos = cosf(fYRot) * MAX_SIZE;
		float fZPos = cosf(-fRot) * sinf(fYRot) * MAX_SIZE;
		D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

		// ���_���W�̐ݒ�
		pVtx[i + 1].pos = pos;

		D3DXVECTOR3 vec;

		// ���K������ ( �傫�� 1 �̃x�N�g���ɂ��� )
		D3DXVec3Normalize(&vec, &pos);

		// �e���_�̖@���̐ݒ�
		pVtx[i + 1].nor = vec;

		// ���_�J���[�̐ݒ�
		pVtx[i + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i + 1].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	float fYRot = ((D3DX_PI * 0.25f) / s_Number.nVertical);

	float fYPos = cosf(fYRot) * MAX_SIZE;

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, fYPos, 0.0f);

	// ���_���W�̐ݒ�
	pVtx[0].pos = pos;

	D3DXVECTOR3 vec;

	// ���K������ ( �傫�� 1 �̃x�N�g���ɂ��� )
	D3DXVec3Normalize(&vec, &pos);

	// �e���_�̖@���̐ݒ�
	pVtx[0].nor = vec;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffCone->Unlock();
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
MeshSky *GetMeshSky(void)
{
	return &s_meshSky;
}

//--------------------------------------------------
// ���n�̎擾
//--------------------------------------------------
MeshSkyNumber *GetMeshSkyNumber(void)
{
	return &s_Number;
}

//--------------------------------------------------
// ����
//--------------------------------------------------
static void Input(void)
{
	if (GetDebug() == DEBUG_MESH)
	{// �f�o�b�O�\�������b�V���̎�
		if (GetKeyboardTrigger(DIK_V))
		{// V�L�[�������ꂽ
			s_Number.nHorizontal++;
		}
		else if (GetKeyboardTrigger(DIK_B))
		{// B�L�[�������ꂽ
			s_Number.nHorizontal--;
		}

		if (GetKeyboardTrigger(DIK_N))
		{// N�L�[�������ꂽ
			s_Number.nVertical++;
		}
		else if (GetKeyboardTrigger(DIK_M))
		{// M�L�[�������ꂽ
			s_Number.nVertical--;
		}
	}
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

	if (s_pVtxBuffCone != NULL)
	{// �~���̒��_�o�b�t�@�̉��
		s_pVtxBuffCone->Release();
		s_pVtxBuffCone = NULL;
	}
}
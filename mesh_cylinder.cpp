//==================================================
// 
// 3D�Q�[������ ( mesh_cylinder.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "input.h"
#include "mesh_cylinder.h"
#include "player.h"
#include "setup.h"
#include "wall.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_HEIGHT				(50.0f)			// �����̍ő�l
#define START_POS_Z				(-35.0f)		// Z�̈ʒu�̍ŏ��̒l
#define START_HORIZONTAL		(50)			// ���̍ŏ��̒l
#define START_VERTICAL			(1)				// �c�̍ŏ��̒l
#define MOVE_X					(5.0f)			// X�̈ړ���
#define SIZE_RATIO				(0.01f)			// �T�C�Y�̔䗦

//--------------------------------------------------
// �\����
//--------------------------------------------------

/*�� ���b�V�� ��*/

typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	float			fSize;			// �T�C�Y
}Mesh;

/*�� ���n ��*/

typedef struct
{
	int		nHorizontal;		// ��
	int		nVertical;			// �c
	int		nVtx;				// ���_��
	int		nIdx;				// �C���f�b�N�X��
	int		nPolygon;			// �|���S����
}Number;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DINDEXBUFFER9		s_pIdxBuff = NULL;			// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureLid = NULL;		// �ӂ��̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffLid = NULL;		// �ӂ��̒��_�o�b�t�@�ւ̃|�C���^
static Mesh							s_mesh;						// ���b�V���̏��
static Number						s_Number;					// ���n�̏��
static float						s_fStartSize;				// �n�܂�̃T�C�Y

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitMeshCylinder(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\lid.png",
		&s_pTextureLid);

	// �������̃N���A
	memset(&s_mesh, 0, sizeof(s_mesh));
	memset(&s_Number, 0, sizeof(s_Number));

	//���E�c�̏�����
	s_Number.nHorizontal = START_HORIZONTAL;
	s_Number.nVertical = START_VERTICAL;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitMeshCylinder(void)
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

	if (s_pTextureLid != NULL)
	{// �e�N�X�`���̉��
		s_pTextureLid->Release();
		s_pTextureLid = NULL;
	}

	if (s_pVtxBuffLid != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuffLid->Release();
		s_pVtxBuffLid = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMeshCylinder(void)
{
	if (s_mesh.fSize > 0.0f)
	{// �T�C�Y���w��̒l���傫��
		s_mesh.pos.x = GetCamera(0)->posV.x;

		int nXLine = s_Number.nHorizontal + 1;
		int nYLine = s_Number.nVertical + 1;

		float fPosDiff = GetPlayer()->pos.x - s_mesh.pos.x;

		if (fPosDiff < 0.0f)
		{// �}�C�i�X
			fPosDiff *= -1.0f;
		}
		
		s_mesh.fSize = sqrtf(fPosDiff);
		s_mesh.rot.z = s_mesh.fSize;

		if (s_mesh.fSize <= 0.1f)
		{// �w��̒l�ȉ�
			s_mesh.fSize = 0.0f;
		}

		s_mesh.pos.y = s_mesh.fSize;

		VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int y = 0; y < nYLine; y++)
		{
			float fYPos = MAX_HEIGHT * y;

			for (int x = 0; x < nXLine; x++)
			{
				int nVtx = x + (y * nXLine);
				float fRot = (D3DX_PI * 2.0f) / s_Number.nHorizontal * x;

				// �p�x�̐��K��
				NormalizeRot(&fRot);

				float fXPos = cosf(fRot) * s_mesh.fSize;
				float fZPos = sinf(fRot) * s_mesh.fSize;
				D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

				// ���_���W�̐ݒ�
				pVtx[nVtx].pos = pos;

				D3DXVECTOR3 vec;

				// ���K������ ( �傫�� 1 �̃x�N�g���ɂ��� )
				D3DXVec3Normalize(&vec, &pos);

				// �e���_�̖@���̐ݒ�
				pVtx[nVtx].nor = vec;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffLid->Lock(0, 0, (void**)&pVtx, 0);

		float fScaleUV = s_mesh.fSize / s_fStartSize;

		for (int i = 1; i <= nXLine; i++)
		{
			float fRot = ((D3DX_PI * 2.0f) / s_Number.nHorizontal) * i;

			// �p�x�̐��K��
			NormalizeRot(&fRot);

			float fXPos = cosf(fRot) * s_mesh.fSize;
			float fYPos = sinf(fRot) * s_mesh.fSize;

			// ���_���W�̐ݒ�
			pVtx[i].pos = D3DXVECTOR3(fXPos, MAX_HEIGHT, fYPos);

			float fTexU = cosf(fRot)/* * 0.5f */ * fScaleUV + 0.5f;
			float fTexV = sinf(fRot)/* * 0.5f */ * fScaleUV + 0.5f;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[i].tex = D3DXVECTOR2(fTexU, fTexV);
		}

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuffLid->Unlock();
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawMeshCylinder(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// �����_�[�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_mesh.mtxWorld);

	// X�̌����𔽉f
	D3DXMatrixRotationX(&mtxRot, s_mesh.rot.x);
	D3DXMatrixMultiply(&s_mesh.mtxWorld, &s_mesh.mtxWorld, &mtxRot);

	// Z�̌����𔽉f
	D3DXMatrixRotationZ(&mtxRot, s_mesh.rot.z);
	D3DXMatrixMultiply(&s_mesh.mtxWorld, &s_mesh.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_mesh.pos.x, s_mesh.pos.y, s_mesh.pos.z);
	D3DXMatrixMultiply(&s_mesh.mtxWorld, &s_mesh.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_mesh.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(s_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�o�b�t�@
		0,							// �C���f�b�N�X�̍ŏ��l
		s_Number.nVtx,				// ���_��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		s_Number.nPolygon);			// �v���~�e�B�u(�|���S��)��

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffLid, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureLid);

	// �e�N�X�`����U, V�̌J��Ԃ�����ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLEFAN,			// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		s_Number.nHorizontal);		// �v���~�e�B�u(�|���S��)��

	// �e�N�X�`����U, V�̌J��Ԃ��������ɖ߂�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// ���ɖ߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetMeshCylinder(void)
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
	memset(&s_mesh, 0, sizeof(s_mesh));

	float fPosDiff = GetPlayer()->pos.x - s_mesh.pos.x;

	if (fPosDiff < 0.0f)
	{// �}�C�i�X
		fPosDiff *= -1.0f;
	}

	s_mesh.fSize = sqrtf(fPosDiff);

	if (s_mesh.fSize < 0.0f)
	{// �}�C�i�X
		s_mesh.fSize *= -1.0f;
	}

	s_fStartSize = s_mesh.fSize;

	s_mesh.pos = D3DXVECTOR3(0.0f, s_mesh.fSize, START_POS_Z);
	s_mesh.rot = D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f);

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int y = 0; y < nYLine; y++)
	{
		float fYPos = MAX_HEIGHT * y;

		for (int x = 0; x < nXLine; x++)
		{
			int nVtx = x + (y * nXLine);
			float fRot = (D3DX_PI * 2.0f) / s_Number.nHorizontal * x;

			// �p�x�̐��K��
			NormalizeRot(&fRot);

			float fXPos = cosf(fRot) * s_mesh.fSize;
			float fZPos = sinf(fRot) * s_mesh.fSize;
			
			// ���_���W�̐ݒ�
			pVtx[nVtx].pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

			D3DXVECTOR3 vec;

			// ���K������ ( �傫�� 1 �̃x�N�g���ɂ��� )
			D3DXVec3Normalize(&vec, &pVtx[x + (y * nXLine)].pos);

			// �e���_�̖@���̐ݒ�
			pVtx[nVtx].nor = vec;

			// ���_�J���[�̐ݒ�
			pVtx[nVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			float fVTex = (1.0f / s_Number.nVertical) * y;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[nVtx].tex = D3DXVECTOR2((float)x, fVTex);
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
		&s_pVtxBuffLid,
		NULL);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffLid->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, MAX_HEIGHT, 0.0f);

	// �e���_�̖@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.5f, 0.5f);

	for (int i = 1; i <= nXLine; i++)
	{
		float fRot = ((D3DX_PI * 2.0f) / s_Number.nHorizontal) * i;

		// �p�x�̐��K��
		NormalizeRot(&fRot);

		float fXPos = cosf(fRot) * s_mesh.fSize;
		float fYPos = sinf(fRot) * s_mesh.fSize;

		// ���_���W�̐ݒ�
		pVtx[i].pos = D3DXVECTOR3(fXPos, MAX_HEIGHT, fYPos);

		// �e���_�̖@���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		float fTexU = (cosf(fRot) + 1.0f)/* * 0.5f */;
		float fTexV = (sinf(fRot) + 1.0f)/* * 0.5f */;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = D3DXVECTOR2(fTexU, fTexV);
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffLid->Unlock();
}

//==================================================
// 
// 3D�Q�[������ ( mesh_field.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "input.h"
#include "mesh_field.h"
#include "setup.h"
#include "wall.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_WIDTH		(-150.0f)		// ���̍ő�l
#define MAX_HEIGHT		(45.0f)			// �����̍ő�l
#define MAX_DEPTH		(-1000.0f)		// ���s���̍ő�l
#define START_SIZE		(5)				// �T�C�Y�̍ŏ��̒l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DINDEXBUFFER9		s_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
static MeshField					s_mesh;					// ���b�V���t�B�[���h�̏��
static MeshFieldNumber				s_Number;				// ���b�V���t�B�[���h�̐��n�̏��
static int							*s_pIdx = { NULL };		// �C���f�b�N�X�̔z��

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
		"data\\TEXTURE\\block011.png",
		&s_pTexture);

	// �������̃N���A
	memset(&s_mesh, 0, sizeof(s_mesh));
	memset(&s_Number, 0, sizeof(s_Number));

	// ���E�c�̏�����
	s_Number.nHorizontal = START_SIZE;
	s_Number.nVertical = START_SIZE;
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

	if (s_pIdx != NULL)
	{// �C���f�b�N�X�̔z��̊J��
		delete[] s_pIdx;
		s_pIdx = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMeshField(void)
{

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
	D3DXMatrixIdentity(&s_mesh.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_mesh.rot.y, s_mesh.rot.x, s_mesh.rot.z);
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
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetMeshField(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nXLine = s_Number.nHorizontal + 1;
	int nZLine = s_Number.nVertical + 1;

	// ���_�����v�Z
	s_Number.nVtx = nXLine * nZLine;

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

	// ���E�����E���s���̐ݒ�
	s_mesh.fWidth = MAX_WIDTH * s_Number.nHorizontal;
	s_mesh.fHeight = MAX_HEIGHT;
	s_mesh.fDepth = MAX_DEPTH * (s_Number.nVertical * 0.5f);

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < nZLine; i++)
	{
		for (int j = 0; j < nXLine; j++)
		{
			int nVtx = j + (i * nXLine);

			float fXPos = (MAX_WIDTH * s_Number.nHorizontal) - (j * MAX_WIDTH);
			float fZPos = (i - (s_Number.nVertical * 0.5f)) * MAX_DEPTH;
			float fYPos = (MAX_HEIGHT * s_Number.nVertical) - (j * MAX_HEIGHT);

			// ���_���W�̐ݒ�
			pVtx[nVtx].pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

			// �e���_�̖@���̐ݒ�
			pVtx[nVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[nVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[nVtx].tex = D3DXVECTOR2((float)j, (float)i);
		}
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(VERTEX_3D) * s_Number.nIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		NULL);

	// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
	s_pIdx = new int[s_Number.nIdx];

	WORD *pIdx = NULL;		// �C���f�b�N�X���ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N
	s_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	for (int x = 0, z = 0; z < s_Number.nVertical; x++, z++)
	{
		for (; x < (nXLine * (z + 1)) + z; x++)
		{
			pIdx[x * 2] = (WORD)(x - z + nXLine);
			pIdx[(x * 2) + 1] = (WORD)(x - z);
			s_pIdx[x * 2] = pIdx[x * 2];
			s_pIdx[(x * 2) + 1] = pIdx[(x * 2) + 1];
		}

		if (z < s_Number.nVertical - 1)
		{// ����ŏI��肶��Ȃ��Ȃ�
			pIdx[x * 2] = (WORD)(x - (z + 1));
			pIdx[(x * 2) + 1] = (WORD)((x * 2) - (z * (s_Number.nHorizontal + 3)));
			s_pIdx[x * 2] = pIdx[x * 2];
			s_pIdx[(x * 2) + 1] = pIdx[(x * 2) + 1];
		}
	}

	//�O�p�̒��_��
	const int nTri = 3;

	D3DXVECTOR3 posLineVec[nTri];		//�x�N�g��

	for (int nCnt = 0; nCnt < s_Number.nPolygon; nCnt++)
	{// �v���~�e�B�u�̐������܂킷�B
		//�x�N�g�������߂�
		posLineVec[0] = pVtx[pIdx[nCnt + 0]].pos;
		posLineVec[1] = pVtx[pIdx[nCnt + 1]].pos;
		posLineVec[2] = pVtx[pIdx[nCnt + 2]].pos;

		if ((pIdx[nCnt + 0] == pIdx[nCnt + 1]) ||
			(pIdx[nCnt + 0] == pIdx[nCnt + 2]) ||
			(pIdx[nCnt + 2] == pIdx[nCnt + 1]))
		{
			continue;
		}

		D3DXVECTOR3 V1 = posLineVec[1] - posLineVec[0];
		D3DXVECTOR3 V2 = posLineVec[2] - posLineVec[0];

		D3DXVECTOR3 Normal;

		if (nCnt % 2 == 0)
		{
			//A��B�̖@�������߂���
			D3DXVec3Cross(&Normal, &V2, &V1);
		}
		else
		{
			//A��B�̖@�������߂���
			D3DXVec3Cross(&Normal, &V1, &V2);
		}

		//Normal���m�[�}���C�Y���āA���� 1�ɂ���B
		D3DXVec3Normalize(&Normal, &Normal);

		for (int i = 0; i < nTri; i++)
		{//�@���v�Z
			pVtx[pIdx[nCnt + i]].nor += Normal;
		}
	}

	for (int nCnt = 0; nCnt < s_Number.nVtx; nCnt++)
	{
		//nor���m�[�}���C�Y���āA���� 1�ɂ���B
		D3DXVec3Normalize(&pVtx[nCnt].nor, &pVtx[nCnt].nor);
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	s_pIdxBuff->Unlock();

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//--------------------------------------------------
// �����蔻��
//--------------------------------------------------
bool CollisionMeshField(D3DXVECTOR3 * pos)
{
	VERTEX_3D* pVtx = NULL;
	D3DXVECTOR3 vecField[3];	// �|���S���̐���
	D3DXVECTOR3 vecModel[3];	// ���f������|���S���̐���

	bool bCollision = false;

	// ���_���W�����b�N
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̔��f
	for (int i = 0; i <= (s_Number.nIdx - 3); i++)
	{
		if (s_pIdx[i] == s_pIdx[i + 1] || s_pIdx[i + 1] == s_pIdx[i + 2] || s_pIdx[i] == s_pIdx[i + 2])
		{
			continue;
		}

		vecField[0] = pVtx[s_pIdx[i + 1]].pos - pVtx[s_pIdx[i]].pos;
		vecField[1] = pVtx[s_pIdx[i + 2]].pos - pVtx[s_pIdx[i + 1]].pos;
		vecField[2] = pVtx[s_pIdx[i]].pos - pVtx[s_pIdx[i + 2]].pos;

		vecModel[0] = *pos - pVtx[s_pIdx[i]].pos;
		vecModel[1] = *pos - pVtx[s_pIdx[i + 1]].pos;
		vecModel[2] = *pos - pVtx[s_pIdx[i + 2]].pos;

		float crs_v1 = Vec2Cross(&vecModel[0], &vecField[0]);
		float crs_v2 = Vec2Cross(&vecModel[1], &vecField[1]);
		float crs_v3 = Vec2Cross(&vecModel[2], &vecField[2]);

		// ����Ă郁�b�V�����`�F�b�N
		if (i % 2 == 0)
		{
			if (crs_v1 >= 0.0f && crs_v2 >= 0.0f && crs_v3 >= 0.0f)
			{
				D3DXVECTOR3 N;
				D3DXVec3Cross(&N, &vecField[0], &vecField[1]);

				if (N.y < 0.0f)
				{
					N *= -1.0f;
				}

				D3DXVec3Normalize(&N, &N);

				pos->y = pVtx[s_pIdx[i]].pos.y - 1.0f / N.y * (N.x * (pos->x - pVtx[s_pIdx[i]].pos.x) + N.z * (pos->z - pVtx[s_pIdx[i]].pos.z));
			
				bCollision = true;
			}
		}
		else
		{
			if (crs_v1 <= 0.0f && crs_v2 <= 0.0f && crs_v3 <= 0.0f)
			{
				D3DXVECTOR3 N;
				D3DXVec3Cross(&N, &vecField[0], &vecField[1]);

				if (N.y < 0.0f)
				{
					N *= -1.0f;
				}

				D3DXVec3Normalize(&N, &N);

				pos->y = pVtx[s_pIdx[i]].pos.y - 1.0f / N.y * (N.x * (pos->x - pVtx[s_pIdx[i]].pos.x) + N.z * (pos->z - pVtx[s_pIdx[i]].pos.z));
			
				bCollision = true;
			}
		}
	}

	// ���_���W���A�����b�N
	s_pVtxBuff->Unlock();
	
	return bCollision;
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
MeshField *GetMeshField(void)
{
	return &s_mesh;
}

//--------------------------------------------------
// ���n�̎擾
//--------------------------------------------------
MeshFieldNumber *GetMeshFieldNumber(void)
{
	return &s_Number;
}

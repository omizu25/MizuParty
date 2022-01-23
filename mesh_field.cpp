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
static MeshField					s_mesh;					// ���b�V���t�B�[���h�̏��
static MeshFieldNumber				s_Number;				// ���b�V���t�B�[���h�̐��n�̏��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void Input(void);
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
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMeshField(void)
{
	if (GetDebug() == DEBUG_MESH)
	{// �f�o�b�O�\�������b�V���̎�
		// ����
		Input();

		if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_D) ||
			GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_S))
		{// A, D, W, S�L�[�������ꂽ

			// �w��̒l�ȏ�E�ȉ�
			Specified(&s_Number.nHorizontal, MAX_SIZE, MIN_SIZE);

			// �w��̒l�ȏ�E�ȉ�
			Specified(&s_Number.nVertical, MAX_SIZE, MIN_SIZE);

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
	s_mesh.fWidth = MAX_WIDTH * (s_Number.nHorizontal * 0.5f);
	s_mesh.fHeight = MAX_HEIGHT;
	s_mesh.fDepth = MAX_DEPTH * (s_Number.nVertical * 0.5f);

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < s_Number.nVtx; i++)
	{
		float fXPos = (float)(i % nXLine) - (s_Number.nHorizontal * 0.5f);
		float fZPos = ((float)(i / nXLine) - (s_Number.nVertical * 0.5f)) * -1.0f;

		// ���_���W�̐ݒ�
		pVtx[i].pos = D3DXVECTOR3(MAX_WIDTH * fXPos, 0.0f, MAX_DEPTH * fZPos);

		// �e���_�̖@���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		float fUTex = (float)(i % nXLine);
		float fVTex = (float)(i / nXLine);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = D3DXVECTOR2(fUTex, fVTex);
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
	for (int x = 0, z = 0; z < s_Number.nVertical; x++, z++)
	{
		for (; x < (nXLine * (z + 1)) + z; x++)
		{
			pIdx[x * 2] = (WORD)(x - z + nXLine);
			pIdx[(x * 2) + 1] = (WORD)(x - z);
		}

		if (z < s_Number.nVertical - 1)
		{// ����ŏI��肶��Ȃ��Ȃ�
			pIdx[x * 2] = (WORD)(x - (z + 1));
			pIdx[(x * 2) + 1] = (WORD)((x * 2) - (z * (s_Number.nHorizontal + 3)));
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	s_pIdxBuff->Unlock();
}

//--------------------------------------------------
// �����蔻��
//--------------------------------------------------
void CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size)
{
	//// �f�o�C�X�ւ̃|�C���^�̎擾
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	//for (int i = 0; i < MAX_WALL; i++)
	//{
	//	MeshField *pMesh = &s_mesh;

	//	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	//	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

	//	D3DXMATRIX mtxWorld[VTX];
	//	D3DXVECTOR3 vtx[VTX];

	//	for (int j = 0; j < VTX; j++)
	//	{
	//		D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	//		// ���[���h�}�g���b�N�X�̏�����
	//		D3DXMatrixIdentity(&mtxWorld[j]);

	//		// �ʒu�𔽉f
	//		D3DXMatrixTranslation(&mtxTrans, pVtx[j].pos.x, pVtx[j].pos.y, pVtx[j].pos.z);
	//		D3DXMatrixMultiply(&mtxWorld[j], &mtxWorld[j], &mtxTrans);

	//		// �����𔽉f
	//		D3DXMatrixRotationYawPitchRoll(&mtxRot, pWall->rot.y, pWall->rot.x, pWall->rot.z);
	//		D3DXMatrixMultiply(&mtxWorld[j], &mtxWorld[j], &mtxRot);

	//		// �ʒu�𔽉f
	//		D3DXMatrixTranslation(&mtxTrans, pWall->pos.x, pWall->pos.y, pWall->pos.z);
	//		D3DXMatrixMultiply(&mtxWorld[j], &mtxWorld[j], &mtxTrans);

	//		// ���[���h���W�s��̐ݒ�
	//		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld[j]);

	//		vtx[j] = D3DXVECTOR3(mtxWorld[j]._41, mtxWorld[j]._42, mtxWorld[j]._43);
	//	}

	//	D3DXVECTOR3 vecWall[VTX];

	//	// �ǂ̃x�N�g��
	//	for (int j = 0, k = 0; j < VTX; j++, k++)
	//	{
	//		if (k == (VTX / 2))
	//		{// �܂�Ԃ��ɗ���
	//			k = (k + 1) * -1;
	//		}

	//		vecWall[j] = vtx[k + 1] - vtx[j];
	//	}

	//	// �ړ��ʂ̃x�N�g��
	//	D3DXVECTOR3 vecMove = *pPos - *pPosOld;

	//	// �ړ��ʂ𐳋K��
	//	D3DXVec3Normalize(&vecMove, &vecMove);

	//	vecWall[0] -= vecMove * size.x;

	//	// �v���C���[�̈ʒu�܂ł̃x�N�g��
	//	D3DXVECTOR3 vecPos = *pPos - vtx[0];

	//	// �O�όv�Z
	//	float fVecLine = Vec2Cross(&vecPos, &vecWall[0]);

	//	if (fVecLine <= 0.0f)
	//	{// �ǂɓ������Ă�
	//	 // �v���C���[�̑O��̈ʒu����̃x�N�g��
	//		D3DXVECTOR3 vecPosOld = vtx[0] - *pPosOld;

	//		// �@��
	//		D3DXVECTOR3 nor = *D3DXVec3Cross(&nor, &vecWall[3], &vecWall[2]);

	//		// �@���𐳋K��
	//		D3DXVec3Normalize(&nor, &nor);

	//		// �ړ��ʂ̋t�����̃x�N�g��
	//		D3DXVECTOR3 ReverseMove = vecMove * -1.0f;

	//		// �ǂɐ����ȃx�N�g��
	//		D3DXVECTOR3 vecNor = nor * Vec2Dot(&ReverseMove, &nor);

	//		float fVec[2], fMove;		// �v�Z�p

	//									// �O�όv�Z
	//		fVec[0] = Vec2Cross(&vecPosOld, &vecWall[0]);
	//		fVec[1] = Vec2Cross(&vecMove, &vecWall[0]);

	//		// �ړ���
	//		fMove = fVec[0] / fVec[1];

	//		pPos->x = pPosOld->x + ((vecMove.x * fMove) + vecNor.x);
	//		pPos->z = pPosOld->z + ((vecMove.z * fMove) + vecNor.z);
	//	}

	//	// ���_�o�b�t�@���A�����b�N����
	//	s_pVtxBuff->Unlock();
	//}
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

//--------------------------------------------------
// ����
//--------------------------------------------------
static void Input(void)
{
	if (GetKeyboardTrigger(DIK_A))
	{// A�L�[�������ꂽ
		s_Number.nHorizontal++;
	}
	else if (GetKeyboardTrigger(DIK_D))
	{// D�L�[�������ꂽ
		s_Number.nHorizontal--;
	}

	if (GetKeyboardTrigger(DIK_W))
	{// W�L�[�������ꂽ
		s_Number.nVertical++;
	}
	else if (GetKeyboardTrigger(DIK_S))
	{// S�L�[�������ꂽ
		s_Number.nVertical--;
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
}

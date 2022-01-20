//==================================================
// 
// 3D�Q�[������ ( wall.cpp )
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

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static Wall							s_wall[MAX_WALL];		// �ǂ̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitWall(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko002.jpg",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//�������̃N���A
	memset(s_wall, 0, sizeof(s_wall));

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_WALL; i++)
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
void UninitWall(void)
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
void UpdateWall(void)
{
	for (int i = 0; i < MAX_WALL; i++)
	{
		Wall *pWall = &s_wall[i];

		if (!pWall->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		if (GetKeyboardPress(DIK_1))
		{// 1�L�[�������ꂽ
			pWall->rot.y += 0.01f;
		}
		else if (GetKeyboardPress(DIK_2))
		{// 2�L�[�������ꂽ
			pWall->rot.y -= 0.01f;
		}
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawWall(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_WALL; i++)
	{
		Wall *pWall = &s_wall[i];

		if (!pWall->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pWall->mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pWall->rot.y, pWall->rot.x, pWall->rot.z);
		D3DXMatrixMultiply(&pWall->mtxWorld, &pWall->mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pWall->pos.x, pWall->pos.y, pWall->pos.z);
		D3DXMatrixMultiply(&pWall->mtxWorld, &pWall->mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pWall->mtxWorld);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			i * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��
	}

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, float fURight, bool bCulling)
{
	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	for (int i = 0; i < MAX_WALL; i++)
	{
		Wall *pWall = &s_wall[i];

		if (pWall->bUse)
		{//�g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		pWall->pos = pos;
		pWall->rot = rot;
		pWall->fWidth = fWidth;
		pWall->fHeight = fHeight;
		pWall->bUse = true;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		if (bCulling)
		{
			// ���_�J���[�̐ݒ�
			Setcol(pVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// ���_�J���[�̐ݒ�
			Setcol(pVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}

		// �e�N�X�`���̐ݒ�
		Settex(pVtx, 0.0f, fURight, 0.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// �ݒu
//--------------------------------------------------
void InstallationWall(void)
{
	MeshField *pMeshField = GetMeshField();					//���b�V���t�B�[���h�̎擾
	MeshFieldNumber *pNumber = GetMeshFieldNumber();		//���b�V���t�B�[���h�̐��n�̎擾

	float fWidth = pMeshField->fWidth;
	float fHeight = pMeshField->fWidth * 0.4f;
	float fDepth = pMeshField->fDepth;

	// �ǂ̐ݒ�

	/*�� ���� ��*/

	SetWall(D3DXVECTOR3(0.0f, fHeight, -fDepth), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), fWidth, fHeight, (float)pNumber->nHorizontal, true);
	SetWall(D3DXVECTOR3(0.0f, fHeight, fDepth), D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, (float)pNumber->nHorizontal, true);
	SetWall(D3DXVECTOR3(-fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, (float)pNumber->nVertical, true);
	SetWall(D3DXVECTOR3(fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, (float)pNumber->nVertical, true);

	/*�� �O�� ��*/

	/*SetWall(D3DXVECTOR3(0.0f, fHeight, fDepth), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), fWidth, fHeight, (float)pNumber->nHorizontal, false);
	SetWall(D3DXVECTOR3(0.0f, fHeight, -fDepth), D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, (float)pNumber->nHorizontal, false);
	SetWall(D3DXVECTOR3(fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, (float)pNumber->nVertical, false);
	SetWall(D3DXVECTOR3(-fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, (float)pNumber->nVertical, false);*/
}

//--------------------------------------------------
// �����蔻��
//--------------------------------------------------
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	for (int i = 0; i < MAX_WALL; i++)
	{
		Wall *pWall = &s_wall[i];

		if (!pWall->bUse)
		{// �g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		D3DXMATRIX mtxWorld[VTX];
		D3DXVECTOR3 vtx[VTX];

		for (int j = 0; j < VTX; j++)
		{
			D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld[j]);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pVtx[j].pos.x, pVtx[j].pos.y, pVtx[j].pos.z);
			D3DXMatrixMultiply(&mtxWorld[j], &mtxWorld[j], &mtxTrans);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pWall->rot.y, pWall->rot.x, pWall->rot.z);
			D3DXMatrixMultiply(&mtxWorld[j], &mtxWorld[j], &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pWall->pos.x, pWall->pos.y, pWall->pos.z);
			D3DXMatrixMultiply(&mtxWorld[j], &mtxWorld[j], &mtxTrans);

			// ���[���h���W�s��̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld[j]);

			vtx[j] = D3DXVECTOR3(mtxWorld[j]._41, mtxWorld[j]._42, mtxWorld[j]._43);
		}

		D3DXVECTOR3 vecWall[VTX];

		// �ǂ̃x�N�g��
		for (int j = 0, k = 0; j < VTX; j++, k++)
		{
			if (k == (VTX / 2))
			{// �܂�Ԃ��ɗ���
				k = (k + 1) * -1;
			}

			vecWall[j] = vtx[k + 1] - vtx[j];
		}

		// �ړ��ʂ̃x�N�g��
		D3DXVECTOR3 vecMove = *pPos - *pPosOld;

		// �ړ��ʂ𐳋K��
		D3DXVec3Normalize(&vecMove, &vecMove);

		vecWall[0] -= vecMove * size.x;
		
		// �v���C���[�̈ʒu�܂ł̃x�N�g��
		D3DXVECTOR3 vecPos = *pPos - vtx[0];

		// �O�όv�Z
		float fVecLine = Vec2Cross(&vecPos, &vecWall[0]);

		if (fVecLine <= 0.0f)
		{// �ǂɓ������Ă�
			// �v���C���[�̑O��̈ʒu����̃x�N�g��
			D3DXVECTOR3 vecPosOld = vtx[0] - *pPosOld;

			// �@��
			D3DXVECTOR3 nor = *D3DXVec3Cross(&nor, &vecWall[3], &vecWall[2]);

			// �@���𐳋K��
			D3DXVec3Normalize(&nor, &nor);

			// �ړ��ʂ̋t�����̃x�N�g��
			D3DXVECTOR3 ReverseMove = vecMove * -1.0f;

			// �ǂɐ����ȃx�N�g��
			D3DXVECTOR3 vecNor = nor * Vec2Dot(&ReverseMove, &nor);

			float fVec[2], fMove;		// �v�Z�p

			// �O�όv�Z
			fVec[0] = Vec2Cross(&vecPosOld, &vecWall[0]);
			fVec[1] = Vec2Cross(&vecMove, &vecWall[0]);

			// �ړ���
			fMove = fVec[0] / fVec[1];

			pPos->x = pPosOld->x + ((vecMove.x * fMove) + vecNor.x);
			pPos->z = pPosOld->z + ((vecMove.z * fMove) + vecNor.z);
		}

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Wall *GetWall(void)
{
	return &s_wall[0];
}

//--------------------------------------------------
// ���Z�b�g
//--------------------------------------------------
void ResetWall(void)
{
	//�������̃N���A
	memset(s_wall, 0, sizeof(s_wall));
}

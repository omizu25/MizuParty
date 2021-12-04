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
#include "meshfield.h"
#include "setup.h"
#include "wall.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_WALL		(64)			// �ǂ̍ő吔
#define MAX_WIDTH		(150.0f)		// ���̍ő�l
#define MAX_HEIGHT		(0.0f)			// �����̍ő�l
#define MAX_DEPTH		(150.0f)		// ���s���̍ő�l

//--------------------------------------------------
// �\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	bool			bUse;			// �g�p���Ă��邩�ǂ���
}Wall;

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
	memset(s_wall, NULL, sizeof(s_wall));

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_WALL; i++)
	{
		//�S�Ă̏���������
		InitAll3D(pVtx);

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
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, bool bCulling)
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
		pWall->bUse = true;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		if (bCulling)
		{
			// ���_�J���[�̐ݒ�
			Setcol3D(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			// ���_�J���[�̐ݒ�
			Setcol3D(pVtx, 1.0f, 1.0f, 1.0f, 0.5f);
		}

		//float fULeft = fWidth % (MAX_WIDTH * 0.5f);
		//float fURight = fWidth / (MAX_WIDTH * 0.5f);

		//// �e�N�X�`���̐ݒ�
		//Settex3D(pVtx, fULeft, fURight + 1.5f, 0.0f, 1.0f);

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
	MeshField *pMeshField = GetMeshField();

	float fWidth = pMeshField->fWidth;
	float fHeight = pMeshField->fWidth * 0.4f;
	float fDepth = pMeshField->fDepth;

	// �ǂ̐ݒ�

	/*�� ���� ��*/

	SetWall(D3DXVECTOR3(0.0f, fHeight, -fDepth), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), fWidth, fHeight, true);
	SetWall(D3DXVECTOR3(0.0f, fHeight, fDepth), D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, true);
	SetWall(D3DXVECTOR3(-fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, true);
	SetWall(D3DXVECTOR3(fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, true);

	/*�� �O�� ��*/

	SetWall(D3DXVECTOR3(0.0f, fHeight, fDepth), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), fWidth, fHeight, false);
	SetWall(D3DXVECTOR3(0.0f, fHeight, -fDepth), D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, false);
	SetWall(D3DXVECTOR3(fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, false);
	SetWall(D3DXVECTOR3(-fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, false);
}

//--------------------------------------------------
// ���Z�b�g
//--------------------------------------------------
void ResetWall(void)
{
	//�������̃N���A
	memset(s_wall, NULL, sizeof(s_wall));
}
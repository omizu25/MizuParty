//==================================================
// 
// 3D�Q�[������ ( player.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "billboard.h"
#include "camera.h"
#include "input.h"
#include "main.h"
#include "player.h"
#include "setup.h"
#include "shadow.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_MOVE			(1.0f)			//�ړ��ʂ̍ő�l
#define MAX_ROTATION		(0.035f)		//��]�̍ő�l
#define MAX_ATTENUATION		(0.1f)			//�����W���̍ő�l
#define MAX_HEIGHT			(80.0f)			//�����̍ő�l
#define MIN_HEIGHT			(10.0f)			//�����̍ŏ��l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPD3DXMESH				s_pMesh = NULL;				// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER				s_pBuffMat = NULL;			// �}�e���A�����ւ̃|�C���^
static LPDIRECT3DTEXTURE9		*s_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
static DWORD					s_nNumMat = 0;				// �}�e���A�����̐�
static Player					s_player;					// ���f���̏��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void FollowMove(void);
static void Move(void);
static void Rot(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitPlayer(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data\\MODEL\\�Ă�Ă�S����.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

	// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
	s_pTexture = new LPDIRECT3DTEXTURE9[s_nNumMat];

	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();

	// �e���b�V���̃}�e���A�������擾����
	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		s_pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_pTexture[i]);
		}
		else
		{
			s_pTexture[i] = NULL;
		}
	}

	s_player.pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	s_player.rot = D3DXVECTOR3(0.0f, (-D3DX_PI * 0.5f), 0.0f);
	s_player.rotDest = s_player.rot;
	s_player.nStopTime = 0;

	// �e�̐ݒ�
	s_player.nIdxShadow = SetShadow(s_player.pos, s_player.rot);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitPlayer(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		for (int i = 0; i < (int)s_nNumMat; i++)
		{
			if (s_pTexture[i] != NULL)
			{
				s_pTexture[i]->Release();
				s_pTexture[i] = NULL;
			}
		}

		delete[](s_pTexture);
		s_pTexture = NULL;
	}

	if (s_pMesh != NULL)
	{// ���b�V���̉��
		s_pMesh->Release();
		s_pMesh = NULL;
	}

	if (s_pBuffMat != NULL)
	{// �}�e���A���̉��
		s_pBuffMat->Release();
		s_pBuffMat = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdatePlayer(void)
{
	s_player.nStopTime++;

	Camera *pCamera = GetCamera();

	if (pCamera->bFollow)
	{// �Ǐ]����
		// �Ǐ]�̈ړ�
		FollowMove();
	}
	else
	{// �Ǐ]���Ȃ�
		// �ړ�
		Move();

		// ��]
		Rot();
	}

	float fAngle = s_player.rotDest.y - s_player.rot.y;

	// �p�x�̐��K��
	NormalizeRot(&fAngle);

	//�����E�������X�V (����������)
	s_player.rot.y += fAngle * MAX_ATTENUATION;

	// �p�x�̐��K��
	NormalizeRot(&s_player.rot.y);

	// �e�̈ʒu�̐ݒ�
	SetPosShadow(s_player.nIdxShadow, s_player.pos);
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawPlayer(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_player.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_player.rot.y, s_player.rot.x, s_player.rot.z);
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_player.mtxWorld);

	// ���݂̃}�e���A���ێ�
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[i]);

		// ���f���p�[�c�̕`��
		s_pMesh->DrawSubset(i);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player;
}

//--------------------------------------------------
// �Ǐ]�̈ړ�
//--------------------------------------------------
static void FollowMove(void)
{
	if (GetDebug() != DEBUG_MESH)
	{// �f�o�b�O�\�������b�V���ł͂Ȃ���
		float fRot = 0.0f;

		/* �����f���̈ړ��� */

		if (GetKeyboardPress(DIK_LEFT))
		{// ���L�[�������ꂽ
			if (GetKeyboardPress(DIK_UP))
			{// ���L�[�������ꂽ
				fRot = -D3DX_PI * 0.25f;

				s_player.rotDest.y = D3DX_PI * 0.75f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = -D3DX_PI * 0.75f;

				s_player.rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{
				fRot = -D3DX_PI * 0.5f;

				s_player.rotDest.y = D3DX_PI * 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_RIGHT))
		{// ���L�[�������ꂽ
			if (GetKeyboardPress(DIK_UP))
			{// ���L�[�������ꂽ
				fRot = D3DX_PI * 0.25f;

				s_player.rotDest.y = -D3DX_PI * 0.75f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = D3DX_PI * 0.75f;

				s_player.rotDest.y = -D3DX_PI * 0.25f;
			}
			else
			{
				fRot = D3DX_PI * 0.5f;

				s_player.rotDest.y = -D3DX_PI * 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = 0.0f;

			s_player.rotDest.y = D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
			fRot = D3DX_PI;

			s_player.rotDest.y = 0.0f;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// ��, ��, ��, ���L�[�������ꂽ
			s_player.pos.x += sinf(fRot) * MAX_MOVE;
			s_player.pos.z += cosf(fRot) * MAX_MOVE;

			s_player.nStopTime = 0;
		}

		if (GetKeyboardPress(DIK_I))
		{// I�L�[�������ꂽ
			s_player.pos.y += sinf(D3DX_PI * 0.5f) * MAX_MOVE;

			s_player.nStopTime = 0;
		}
		else if (GetKeyboardPress(DIK_K))
		{// K�L�[�������ꂽ
			s_player.pos.y += sinf(-D3DX_PI * 0.5f) * MAX_MOVE;

			s_player.nStopTime = 0;
		}

		// �w��̒l�ȏ�E�ȉ�
		Specified(&s_player.pos.y, MAX_HEIGHT, MIN_HEIGHT);
	}
}

//--------------------------------------------------
// �ړ�
//--------------------------------------------------
static void Move(void)
{
	if (GetDebug() != DEBUG_MESH)
	{// �f�o�b�O�\�������b�V���ł͂Ȃ���

		Camera *pCamera = GetCamera();		//�J�����̎擾
		float fRot = 0.0f;

		/* �����f���̈ړ��� */

		if (GetKeyboardPress(DIK_LEFT))
		{// ���L�[�������ꂽ
			if (GetKeyboardPress(DIK_UP))
			{// ���L�[�������ꂽ
				fRot = pCamera->rot.y + (-D3DX_PI * 0.25f);

				s_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = pCamera->rot.y + (-D3DX_PI * 0.75f);

				s_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.25f);
			}
			else
			{
				fRot = pCamera->rot.y + (-D3DX_PI * 0.5f);

				s_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_RIGHT))
		{// ���L�[�������ꂽ
			if (GetKeyboardPress(DIK_UP))
			{// ���L�[�������ꂽ
				fRot = pCamera->rot.y + (D3DX_PI * 0.25f);

				s_player.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ���L�[�������ꂽ
				fRot = pCamera->rot.y + (D3DX_PI * 0.75f);

				s_player.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.25f);
			}
			else
			{
				fRot = pCamera->rot.y + (D3DX_PI * 0.5f);

				s_player.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y;

			s_player.rotDest.y = pCamera->rot.y + D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + D3DX_PI;

			s_player.rotDest.y = pCamera->rot.y;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// ��, ��, ��, ���L�[�������ꂽ
			s_player.pos.x += sinf(fRot) * MAX_MOVE;
			s_player.pos.z += cosf(fRot) * MAX_MOVE;
		}

		if (GetKeyboardPress(DIK_I))
		{// I�L�[�������ꂽ
			s_player.pos.y += sinf(D3DX_PI * 0.5f) * MAX_MOVE;
		}
		else if (GetKeyboardPress(DIK_K))
		{// K�L�[�������ꂽ
			s_player.pos.y += sinf(-D3DX_PI * 0.5f) * MAX_MOVE;
		}

		// �w��̒l�ȏ�E�ȉ�
		Specified(&s_player.pos.y, MAX_HEIGHT, MIN_HEIGHT);
	}
}

//--------------------------------------------------
// ��]
//--------------------------------------------------
static void Rot(void)
{
	if (GetDebug() != DEBUG_MESH)
	{// �f�o�b�O�\�������b�V���ł͂Ȃ���

		/* �����f���̉�]�� */

		if (GetKeyboardPress(DIK_LSHIFT))
		{// ���V�t�g�L�[�������ꂽ
			s_player.rotDest.y += -MAX_ROTATION;
		}
		else if (GetKeyboardPress(DIK_RSHIFT))
		{// �E�V�t�g�L�[�������ꂽ
			s_player.rotDest.y += MAX_ROTATION;
		}
	}
}

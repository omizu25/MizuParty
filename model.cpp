//==================================================
// 
// 3D�Q�[������ ( model.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "countdown.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "setup.h"
#include "shadow.h"
#include "particle.h"
#include "player.h"
#include "result.h"
#include "sound.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define START_ROTATION_X		(-79.0f)		// ��]�̃X�^�[�g�̕�
#define START_ROTATION_Y		(132.0f)		// ��]�̃X�^�[�g�̍���
#define START_STOP_X			(-79.0f)		// �~�߂�̃X�^�[�g�̕�
#define START_STOP_Y			(354.0f)		// �~�߂�̃X�^�[�g�̍���
#define MAX_ROTATION			(0.03f)			// ��]�ʂ̍ő�l
#define MAX_RANDOM				(2)				// �����_���̍ő�l
#define MAX_MOVE				(6.0f)			// �ړ��ʂ̍ő�l
#define STOP_GOOD				(15.0f)			// �~�߂�̏��
#define STOP_NORMAL				(70.0f)			// �~�߂�̕���
#define STOP_COLLISION			(0.0f)			// �~�߂�̓�����ʒu
#define START_TIME				(45)			// �n�܂鎞��

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static Model		s_model;			// ���f���̏��
static bool			s_bStop;			// �~�߂邩�ǂ���
static bool			s_bCollision;		// �����������ǂ���
static bool			s_bDraw;			// �`�悷�邩�ǂ���

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void UpdateRotation(void);
static void UpdateStop(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitModel(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;		// ���_��
	DWORD SizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVexBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data\\MODEL\\Hammer.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_model.pBuffMat,
		NULL,
		&s_model.nNumMat,
		&s_model.pMesh);

	s_model.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_model.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// ���_�����擾
	nNumVtx = s_model.pMesh->GetNumVertices();

	// �t�H�[�}�b�g�̃T�C�Y���擾
	SizeFVF = D3DXGetFVFVertexSize(s_model.pMesh->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	s_model.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

	for (int i = 0; i < nNumVtx; i++)
	{
		// ���_���̑��
		D3DXVECTOR3 pos = *(D3DXVECTOR3*)pVexBuff;

		// �������E�傫�� [x]
		VtxSmallBig(&s_model.vtxMin.x, &s_model.vtxMax.x, pos.x);

		// �������E�傫�� [y]
		VtxSmallBig(&s_model.vtxMin.y, &s_model.vtxMax.y, pos.y);

		// �������E�傫�� [z]
		VtxSmallBig(&s_model.vtxMin.z, &s_model.vtxMax.z, pos.z);

		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVexBuff += SizeFVF;
	}

	// ���_�o�b�t�@�̃A�����b�N
	s_model.pMesh->UnlockVertexBuffer();

	// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
	s_model.pTexture = new LPDIRECT3DTEXTURE9[s_model.nNumMat];

	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_model.pBuffMat->GetBufferPointer();

	// �e���b�V���̃}�e���A�������擾����
	for (int i = 0; i < (int)s_model.nNumMat; i++)
	{
		s_model.pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_model.pTexture[i]);
		}
		else
		{
			s_model.pTexture[i] = NULL;
		}
	}

	float fRand = 0.0f;

	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_SLOPE:		// ��

		break;

	case MENU_ROTATION:		// ��]

		fRand = (rand() % 100) * 0.01f * D3DX_PI * 0.5f;
		fRand -= D3DX_PI * 0.25f;

		s_model.pos = D3DXVECTOR3(START_ROTATION_X, START_ROTATION_Y, 0.0f);
		s_model.rot = D3DXVECTOR3(0.0f, 0.0f, (D3DX_PI * 0.5f) + fRand);

		break;

	case MENU_STOP:			// �~�߂�

		s_model.pos = D3DXVECTOR3(START_STOP_X, START_STOP_Y, 0.0f);
		s_model.rot = D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f);

		break;

	default:
		assert(false);
		break;
	}
	
	s_model.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	s_model.nRepeat = 0;

	s_bStop = false;
	s_bCollision = false;
	s_bDraw = true;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitModel(void)
{
	if (s_model.pTexture != NULL)
	{// �e�N�X�`���̉��
		for (int i = 0; i < (int)s_model.nNumMat; i++)
		{
			if (s_model.pTexture[i] != NULL)
			{
				s_model.pTexture[i]->Release();
				s_model.pTexture[i] = NULL;
			}
		}

		delete[](s_model.pTexture);
		s_model.pTexture = NULL;
	}

	if (s_model.pMesh != NULL)
	{// ���b�V���̉��
		s_model.pMesh->Release();
		s_model.pMesh = NULL;
	}

	if (s_model.pBuffMat != NULL)
	{// �}�e���A���̉��
		s_model.pBuffMat->Release();
		s_model.pBuffMat = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateModel(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// �E�H�[�L���O
	case MENU_SLOPE:		// ��
		
		/* �����Ȃ� */

		break;

	case MENU_ROTATION:		// ��]
		
		UpdateRotation();
		
		break;

	case MENU_STOP:			// �~�߂�

		UpdateStop();

		break;

	default:
		assert(false);
		break;
	}

}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawModel(void)
{
	if (s_bDraw)
	{// �`�悷��
		// �f�o�C�X�ւ̃|�C���^�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
		D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
		D3DXMATERIAL *pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

			// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&s_model.mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, s_model.rot.y, s_model.rot.x, s_model.rot.z);
		D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, s_model.pos.x, s_model.pos.y, s_model.pos.z);
		D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &s_model.mtxWorld);

		// ���݂̃}�e���A���ێ�
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)s_model.pBuffMat->GetBufferPointer();

		for (int i = 0; i < (int)s_model.nNumMat; i++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[i].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_model.pTexture[i]);

			// ���f���p�[�c�̕`��
			s_model.pMesh->DrawSubset(i);
		}

		// �ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);
	}
}

//--------------------------------------------------
// �擾 (���f�����)
//--------------------------------------------------
Model *GetModel(void)
{
	return &s_model;
}

//--------------------------------------------------
// �擾 (�~�܂��Ă邩�ǂ���)
//--------------------------------------------------
bool GetStop(void)
{
	return s_bStop;
}

//--------------------------------------------------
// �擾 (�����������ǂ���)
//--------------------------------------------------
bool GetCollision(void)
{
	return s_bCollision;
}

//--------------------------------------------------
// ��]
//--------------------------------------------------
static void UpdateRotation(void)
{
	if (GetCountdown())
	{// �J�E���g�_�E���I�����
		switch (GetGame())
		{
		case GAMESTATE_NORMAL:			// �Q�[����
		case GAMESTATE_END:				// �I���
		case GAMESTATE_RESULT:			// ���U���g

			if (!s_bStop)
			{// �~�܂��ĂȂ�
				s_model.rot.z -= D3DX_PI * MAX_ROTATION;

				// �p�x�̐��K��
				NormalizeRot(&s_model.rot.z);
			}

			if (!s_bCollision)
			{// �������ĂȂ�
				if (s_model.rot.z < -D3DX_PI * 0.5f)
				{// �Փ�
					s_bCollision = true;

					D3DXVECTOR3 pos = GetPlayer()->pos;

					pos.y += 50.0f;

					// �v���C���[�̕`�悷�邩�̐ݒ�
					SetDrawPlayer(false);

					// �����̐ݒ�
					SetExplosion(pos, 20.0f, true);

					// ���U���g�̐ݒ�
					SetResult(RESULT_GAMEOVER);

					// �Q�[���̐ݒ�
					SetGameState(GAMESTATE_END);

					// �T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_KO);
				}
			}
			else
			{// �������Ă�
				if (s_model.rot.z < -D3DX_PI * 0.75f)
				{// �~�߂�
					s_bStop = true;
				}
			}

			break;

		case GAMESTATE_NONE:			// �����Ȃ�
		case GAMESTATE_COUNTDOWN:		// �J�E���g�_�E��
		case GAMESTATE_START:			// �n�܂�

			/* �����Ȃ� */

			break;

		default:
			assert(false);
			break;
		}

		if (GetGame() == GAMESTATE_NORMAL)
		{// �Q�[����
			if (!s_bCollision && !s_bStop)
			{// �������ĂȂ��A�~�܂��ĂȂ�
				if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) ||
					GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_B) ||
					GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
				{// F4�L�[�������ꂽ
					s_bStop = true;

					// ���U���g�̐ݒ�
					SetResult(RESULT_CLEAR);

					// �Q�[���̐ݒ�
					SetGameState(GAMESTATE_END);

					float fDiff = 90.0f + (s_model.rot.z * (180 / D3DX_PI));

					if (fDiff <= STOP_GOOD)
					{// �~�߂�̏��
						// �T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_�~�߂�̏��);

						s_model.nRepeat = 10;
					}
					else if (fDiff <= STOP_NORMAL)
					{// �~�߂�̕���
						// �T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_�~�߂�̕���);

						s_model.nRepeat = 3;
					}
					else
					{// �~�߂�̉���
						// �T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_�~�߂�̉���);

						s_model.nRepeat = 1;
					}
				}
			}
		}
	}
}

//--------------------------------------------------
// �~�߂�
//--------------------------------------------------
static void UpdateStop(void)
{
	if (GetCountdown())
	{// �J�E���g�_�E���I�����
		switch (GetGame())
		{
		case GAMESTATE_NORMAL:			// �Q�[����
		case GAMESTATE_END:				// �I���
		case GAMESTATE_RESULT:			// ���U���g

			if (!s_bCollision)
			{
				if (!s_bStop)
				{// �~�܂�Ȃ�
					s_model.pos.y -= MAX_MOVE;
				}

				float fModel = s_model.pos.y - s_model.vtxMax.x;
				float fPlayer = GetPlayer()->pos.y + GetPlayer()->fHeight;

				float fPos = (fModel - fPlayer) + 1.0f;

				if (fPos <= 0.0f)
				{
					s_bCollision = true;
					s_bStop = true;
					s_bDraw = false;

					D3DXVECTOR3 pos = D3DXVECTOR3(GetPlayer()->pos.x, s_model.pos.y, s_model.pos.z);

					// �����̐ݒ�
					SetExplosion(pos, 20.0f, true);

					// ���U���g�̐ݒ�
					SetResult(RESULT_GAMEOVER);

					// �Q�[���̐ݒ�
					SetGameState(GAMESTATE_END);

					// �T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_KO);
				}
			}

			break;

		case GAMESTATE_NONE:			// �����Ȃ�
		case GAMESTATE_COUNTDOWN:		// �J�E���g�_�E��
		case GAMESTATE_START:			// �n�܂�

			/* �����Ȃ� */

			break;

		default:
			assert(false);
			break;
		}
	}

	if (GetGame() == GAMESTATE_NORMAL)
	{// �Q�[����
		if (!s_bCollision && !s_bStop)
		{
			if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) ||
				GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_B) ||
				GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
			{// F4�L�[�������ꂽ
				s_bStop = true;

				// ���U���g�̐ݒ�
				SetResult(RESULT_CLEAR);

				// �Q�[���̐ݒ�
				SetGameState(GAMESTATE_END);

				float fModel = s_model.pos.y - s_model.vtxMax.x;
				float fPlayer = GetPlayer()->pos.y + GetPlayer()->fHeight;

				float fDiff = fModel - fPlayer;

				if (fDiff <= STOP_GOOD)
				{// �~�߂�̏��
					// �T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_�~�߂�̏��);

					s_model.nRepeat = 10;
				}
				else if (fDiff <= STOP_NORMAL)
				{// �~�߂�̕���
					// �T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_�~�߂�̕���);

					s_model.nRepeat = 3;
				}
				else
				{// �~�߂�̉���
					// �T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_�~�߂�̉���);

					s_model.nRepeat = 1;
				}
			}
		}
	}
}

//==================================================
// 
// 3D�Q�[������ ( billboard.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "billboard.h"
#include "effect.h"
#include "main.h"
#include "setup.h"
#include "wall.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define FILE_NAME		"data\\TXT\\billboard.txt"		// �t�@�C���̖��O
#define MAX_BILLBOARD			(256)					// �r���{�[�h�̍ő吔
#define MAX_TEXTURE				(256)					// �e�N�X�`���̍ő吔
#define DO_NOT_ROT_Y			(0)						// Y����]�����Ȃ����l

//--------------------------------------------------
// �\����
//--------------------------------------------------

/*�� �r���{�[�h ��*/

typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	D3DXVECTOR3				move;			// �ړ���
	D3DXMATRIX				mtxWorld;		// ���[���h�}�g���b�N�X
	float					fWidth;			// ��
	float					fHeight;		// ����
	bool					bUse;			// �g�p���Ă��邩�ǂ���
	bool					bYRot;			// Y����]�����邩�ǂ���
	bool					bZBuffer;		// Z�o�b�t�@���K���������邩�ǂ���
	LPDIRECT3DTEXTURE9		pTexture;		// �e�N�X�`��
}Billboard;

/*�� �ǂݍ��ޓ��e ��*/

typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	float					fWidth;			// ��
	float					fHeight;		// ����
	int						nIdxTex;		// �e�N�X�`���ԍ�
	int						nYRot;			// Y����]�����邩�ǂ���
	LPDIRECT3DTEXTURE9		pTexture;		// �e�N�X�`��
}Text;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			*s_pTexture;					// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
static Billboard					s_billboard[MAX_BILLBOARD];		// �r���{�[�h�̏��
static int							s_nUseTex;						// �e�N�X�`���̎g�p��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitBillboard(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//�������̃N���A
	memset(s_billboard, NULL, sizeof(s_billboard));

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BILLBOARD; i++)
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
void UninitBillboard(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		for (int i = 0; i < s_nUseTex; i++)
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

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateBillboard(void)
{
	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (!pBillboard->bUse)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		pBillboard->pos += pBillboard->move;

		Wall *pWall = GetWall();

		for (int j = 0; j < MAX_WALL; j++, pWall++)
		{
			if (!pWall->bUse)
			{//�g�p����Ă��Ȃ�
				continue;
			}

			/*�� �g�p����Ă��� ��*/

			/*�� �r���{�[�h ��*/
			float fRightBillboard = pBillboard->pos.x + pBillboard->fWidth;
			float fLeftBillboard = pBillboard->pos.x - pBillboard->fWidth;
			float fTopBillboard = pBillboard->pos.y + pBillboard->fHeight;
			float fBottomBillboard = pBillboard->pos.y - pBillboard->fHeight;

			/*�� �� ��*/
			float fRightWall = pWall->pos.x + cosf(pWall->rot.y) * pWall->fWidth;
			float fLeftWall = pWall->pos.x - cosf(pWall->rot.y) * pWall->fWidth;
			float fTopWall = pWall->pos.y + pWall->fHeight;
			float fBottomWall = pWall->pos.y - pWall->fHeight;

			if (fRightBillboard >= fLeftWall && fLeftBillboard <= fRightWall &&
				fTopBillboard >= fBottomWall && fBottomBillboard <= fTopWall &&
				fRightBillboard >= pWall->pos.z && fLeftBillboard <= pWall->pos.z)
			{//	�������Ă�
				pBillboard->bZBuffer = true;
			}
		}

		// �G�t�F�N�g�̐ݒ�
		SetEffect(pBillboard->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.615f, 0.215f, 0.341f, 1.0f), pBillboard->fWidth, pBillboard->fHeight, 25, true);
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawBillboard(bool bZBuffer)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (!pBillboard->bUse || pBillboard->bZBuffer != bZBuffer)
		{//�g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		if (pBillboard->bZBuffer)
		{// Z�o�b�t�@�̒l��ύX����
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// �K����������
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pBillboard->mtxWorld);
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �J�����̋t�s���ݒ�
		if (pBillboard->bYRot)
		{// Y����]������
			pBillboard->mtxWorld._11 = mtxView._11;
			pBillboard->mtxWorld._12 = mtxView._21;
			pBillboard->mtxWorld._13 = mtxView._31;
			pBillboard->mtxWorld._21 = mtxView._12;
			pBillboard->mtxWorld._22 = mtxView._22;
			pBillboard->mtxWorld._23 = mtxView._32;
			pBillboard->mtxWorld._31 = mtxView._13;
			pBillboard->mtxWorld._32 = mtxView._23;
			pBillboard->mtxWorld._33 = mtxView._33;
		}
		else
		{// Y����]�����Ȃ�
			pBillboard->mtxWorld._11 = mtxView._11;
			pBillboard->mtxWorld._13 = mtxView._31;
			pBillboard->mtxWorld._31 = mtxView._13;
			pBillboard->mtxWorld._33 = mtxView._33;
		}

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);
		D3DXMatrixMultiply(&pBillboard->mtxWorld, &pBillboard->mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pBillboard->mtxWorld);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pBillboard->pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			i * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��
	}

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

	// ���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�o�b�t�@�̒l�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// �V�K�[�x�l <= Z�o�b�t�@�[�x�l (�����ݒ�)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, bool bYRot, LPDIRECT3DTEXTURE9 *pTexture)
{
	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (pBillboard->bUse)
		{//�g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		pBillboard->pos = pos;
		pBillboard->move = move;
		pBillboard->fWidth = fWidth;
		pBillboard->fHeight = fHeight;
		pBillboard->pTexture = *pTexture;
		pBillboard->bYRot = bYRot;
		pBillboard->bZBuffer = false;
		pBillboard->bUse = true;

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		// ���_���W�̐ݒ�
		Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		// ���_�̖@���̐ݒ�
		Setnor3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void LoadBillboard(HWND hWnd)
{
	FILE *pFile;			// �t�@�C���|�C���^��錾
	int nUseText = 0;		// �e�L�X�g�œǂݍ��񂾃r���{�[�h�̎g�p��

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		// �g�p���̓ǂݍ���
		fscanf(pFile, "%d", &s_nUseTex);
		fscanf(pFile, "%d", &nUseText);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(hWnd, "�e�L�X�g�t�@�C���̓ǂݍ��݂Ɏ��s�I\n�G���[�ꏊ  : [ �r���{�[�h ]", "�x���I", MB_ICONWARNING);
		assert(false);
	}

	// txt�ɏ����Ă�ő吔���̓ǂݍ��ݗp�̔z���p�ӂ���
	Text *pText = new Text[nUseText];

	char aTexture[MAX_TEXTURE][1024];

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		// �g�p���̓ǂݍ���
		fscanf(pFile, "%d", &s_nUseTex);
		fscanf(pFile, "%d", &nUseText);

		for (int i = 0; i < s_nUseTex; i++)
		{//�e�N�X�`�����̓ǂݍ���
			fscanf(pFile, "%s", aTexture[i]);
		}

		for (int i = 0; i < nUseText; i++)
		{//�e�N�X�`���̔ԍ��E���E�����E�ʒu�̓ǂݍ���
			fscanf(pFile, "%d", &pText[i].nIdxTex);
			fscanf(pFile, "%d", &pText[i].nYRot);
			fscanf(pFile, "%f", &pText[i].fWidth);
			fscanf(pFile, "%f", &pText[i].fHeight);
			fscanf(pFile, "%f", &pText[i].pos.x);
			fscanf(pFile, "%f", &pText[i].pos.y);
			fscanf(pFile, "%f", &pText[i].pos.z);
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(hWnd, "�e�L�X�g�t�@�C���̓ǂݍ��݂Ɏ��s�I\n�G���[�ꏊ  : [ �r���{�[�h ]", "�x���I", MB_ICONWARNING);
		assert(false);
	}

	for (int i = 0; i < nUseText; i++)
	{
		if (pText[i].nIdxTex >= s_nUseTex)
		{// �Y�����Ȃ��e�N�X�`���ԍ�
			MessageBox(hWnd, "�Y�����Ȃ��e�N�X�`���ԍ��ł��I\n�G���[�ꏊ  : [ �r���{�[�h ]", "�x���I", MB_ICONWARNING);
			assert(false);
		}
	}

	// txt�ɏ����Ă�ő吔���̃e�N�X�`���̔z���p�ӂ���
	s_pTexture = new LPDIRECT3DTEXTURE9[s_nUseTex];

	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < s_nUseTex; i++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			&aTexture[i][0],
			&s_pTexture[i]);
	}

	for (int i = 0; i < nUseText; i++)
	{
		pText[i].pTexture = s_pTexture[pText[i].nIdxTex];

		bool bYRot = true;

		if (pText[i].nYRot == DO_NOT_ROT_Y)
		{// Y����]�����Ȃ����l�̎�
			bYRot = false;
		}

		// �ݒ�
		SetBillboard(pText[i].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pText[i].fWidth, pText[i].fHeight, bYRot,&pText[i].pTexture);
	}

	//�������̃N���A
	memset(pText, NULL, sizeof(pText));

	delete[](pText);

	pText = NULL;
}

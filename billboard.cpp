//==================================================
// 
// 3D�Q�[������ ( billboard.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "effect.h"
#include "particle.h"
#include "setup.h"
#include "wall.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_BILLBOARD		(256)		// �r���{�[�h�̍ő吔
#define MAX_TEXTURE			(256)		// �e�N�X�`���̍ő吔
#define DO_NOT_ROT_Y		(0)			// Y����]�����Ȃ����l

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
	int						nTexIdx;		// �e�N�X�`���ԍ�
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
		InitAll(pVtx);

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
				pBillboard->bUse = false;

				// �p�[�e�B�N���̐ݒ�
				SetParticle(pBillboard->pos, pBillboard->fWidth, true);
			}
		}

		if (pBillboard->move.x != 0.0f || pBillboard->move.z != 0.0f)
		{// �����Ă�
			// �G�t�F�N�g�̐ݒ�
			SetEffect(pBillboard->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.615f, 0.215f, 0.341f, 1.0f), pBillboard->fWidth, pBillboard->fHeight, 25, true);
		}
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
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		// ���_�̖@���̐ݒ�
		Setnor(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

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

	Text *pText;
	char aFile[MAX_TEXT];
	char aTexture[MAX_TEXT];

	//�������̃N���A
	memset(&pText, NULL, sizeof(pText));

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[MAX_TEXT] = {};

		while (strcmp(&aRead[0], "SCRIPT") != 0)
		{// �n�܂肪����܂ŌJ��Ԃ�
			fscanf(pFile, "%s", &aRead);
		}

		while (strcmp(&aRead[0], "END_SCRIPT") != 0)
		{// �I��肪����܂ŌJ��Ԃ�
			fscanf(pFile, "%s", &aRead);

			if (strncmp(&aRead[0], "#-", 2) == 0)
			{// �R�����g
				continue;
			}
			else if (strncmp(&aRead[0], "#", 1) == 0)
			{// �R�����g
				fgets(aRead, MAX_TEXT, pFile);
				continue;
			}

			if (strcmp(&aRead[0], "BILLBOARD_FILENAME") == 0)
			{// ���f���̏��
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", aFile);
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(hWnd, "�V�X�e���t�@�C���̓ǂݍ��݂Ɏ��s�I\n�G���[�ꏊ  : [ ���f�� ]", "�x���I", MB_ICONWARNING);
		assert(false);
	}

	// �t�@�C�����J��
	pFile = fopen(aFile, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		char aRead[MAX_TEXT] = {};
		int nTex = 0, nText = 0;

		while (strcmp(&aRead[0], "SCRIPT") != 0)
		{// �n�܂肪����܂ŌJ��Ԃ�
			fscanf(pFile, "%s", &aRead);
		}

		while (strcmp(&aRead[0], "END_SCRIPT") != 0)
		{// �I��肪����܂ŌJ��Ԃ�
 			fscanf(pFile, "%s", &aRead);

			if (strncmp(&aRead[0], "#-", 2) == 0)
			{// �R�����g
				continue;
			}
			else if (strncmp(&aRead[0], "#", 1) == 0)
			{// �R�����g
				fgets(aRead, MAX_TEXT, pFile);
				continue;
			}

			if (strcmp(&aRead[0], "NUM_TEXTURE") == 0)
			{// �e�N�X�`���̎g�p��
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%d", &s_nUseTex);

				// txt�ɏ����Ă�ő吔���̃e�N�X�`���̔z���p�ӂ���
				s_pTexture = new LPDIRECT3DTEXTURE9[s_nUseTex];
			}
			else if (strcmp(&aRead[0], "TEXTURE_FILENAME") == 0)
			{// �e�N�X�`���̏��
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", aTexture);

				// �f�o�C�X�ւ̃|�C���^�̎擾
				LPDIRECT3DDEVICE9 pDevice = GetDevice();

				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(
					pDevice,
					&aTexture[0],
					&s_pTexture[nTex]);

				nTex++;
			}
			else if (strcmp(&aRead[0], "NUM_MODEL") == 0)
			{// �r���{�[�h�̎g�p��
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%d", &nUseText);

				// txt�ɏ����Ă�ő吔���̓ǂݍ��ݗp�̔z���p�ӂ���
				pText = new Text[nUseText];
			}
			else if (strcmp(&aRead[0], "BILLBOARD_SET") == 0)
			{// �r���{�[�h�̏��
				while (strcmp(&aRead[0], "END_BILLBOARD_SET") != 0)
				{// �I��肪����܂ŌJ��Ԃ�
					fscanf(pFile, "%s", &aRead);

					if (strncmp(&aRead[0], "#", 1) == 0)
					{// �R�����g
						fgets(aRead, MAX_TEXT, pFile);
						continue;
					}

					if (strcmp(&aRead[0], "TEXIDX") == 0)
					{// �e�N�X�`���ԍ�
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pText[nText].nTexIdx);
					}
					else if (strcmp(&aRead[0], "YROT") == 0)
					{// Y��]�����邩�ǂ���
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pText[nText].nYRot);
					}
					else if (strcmp(&aRead[0], "WIDTH") == 0)
					{// ��
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pText[nText].fWidth);
					}
					else if (strcmp(&aRead[0], "HEIGHT") == 0)
					{// ����
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pText[nText].fHeight);
					}
					else if (strcmp(&aRead[0], "POS") == 0)
					{// �ʒu
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pText[nText].pos.x);
						fscanf(pFile, "%f", &pText[nText].pos.y);
						fscanf(pFile, "%f", &pText[nText].pos.z);
					}
				}
				nText++;
			}
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
		if (pText[i].nTexIdx >= s_nUseTex)
		{// �Y�����Ȃ��e�N�X�`���ԍ�
			MessageBox(hWnd, "�Y�����Ȃ��e�N�X�`���ԍ��ł��I\n�G���[�ꏊ  : [ �r���{�[�h ]", "�x���I", MB_ICONWARNING);
			assert(false);
		}
	}

	for (int i = 0; i < nUseText; i++)
	{
		pText[i].pTexture = s_pTexture[pText[i].nTexIdx];

		bool bYRot = true;

		if (pText[i].nYRot == DO_NOT_ROT_Y)
		{// Y����]�����Ȃ����l�̎�
			bYRot = false;
		}

		// �ݒ�
		SetBillboard(pText[i].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pText[i].fWidth, pText[i].fHeight, bYRot,&pText[i].pTexture);
	}

	delete[](pText);

    pText = NULL;
}

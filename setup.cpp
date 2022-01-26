//==================================================
// 
// 3D�Q�[������ ( setup.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "setup.h"

#include <assert.h>

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void SetMiddlepos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetLeftpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetRightpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetToppos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetBottompos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);

//--------------------------------------------------
// ���_���W�̐ݒ� [2D]
//--------------------------------------------------
void Setpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight, SETPOS setpos)
{
	switch (setpos)
	{
	case SETPOS_MIDDLE:		// �^��

		//���_���W�̐ݒ� [2D] (�^�񒆂ɒ��S)
		SetMiddlepos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_LEFT:		// ��

		//���_���W�̐ݒ� [2D] (���ɒ��S)
		SetLeftpos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_RIGHT:		// �E

		//���_���W�̐ݒ� [2D] (�E�ɒ��S)
		SetRightpos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_TOP:		// ��

		//���_���W�̐ݒ� [2D] (��ɒ��S)
		SetToppos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_BOTTOM:		// ��

		//���_���W�̐ݒ� [2D] (���ɒ��S)
		SetBottompos(pVtx, pos, fWidth, fHeight);

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//���_���W�̐ݒ� [2D] (�^�񒆂ɒ��S)
//--------------------------------------------------
static void SetMiddlepos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);
}

//--------------------------------------------------
//���_���W�̐ݒ� [2D] (���ɒ��S)
//--------------------------------------------------
static void SetLeftpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(0.0f, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(0.0f, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);
}

//--------------------------------------------------
//���_���W�̐ݒ� [2D] (�E�ɒ��S)
//--------------------------------------------------
static void SetRightpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(0.0f, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(0.0f, fHeight, 0.0f);
}

//--------------------------------------------------
//���_���W�̐ݒ� [2D] (��ɒ��S)
//--------------------------------------------------
static void SetToppos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, 0.0f, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, 0.0f, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);
}

//--------------------------------------------------
//���_���W�̐ݒ� [2D] (���ɒ��S)
//--------------------------------------------------
static void SetBottompos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, 0.0f, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, 0.0f, 0.0f);
}

//--------------------------------------------------
// ���_�J���[�̐ݒ� [2D]
//--------------------------------------------------
void Setcol(VERTEX_2D *pVtx, D3DXCOLOR col)
{
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
}

//--------------------------------------------------
// �e�N�X�`���̐ݒ� [2D]
//--------------------------------------------------
void Settex(VERTEX_2D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom)
{
	pVtx[0].tex = D3DXVECTOR2(fULeft,  fVTop);
	pVtx[1].tex = D3DXVECTOR2(fURight, fVTop);
	pVtx[2].tex = D3DXVECTOR2(fULeft,  fVBottom);
	pVtx[3].tex = D3DXVECTOR2(fURight, fVBottom);
}

//--------------------------------------------------
// ���_���W�̏����� [2D]
//--------------------------------------------------
void Initpos(VERTEX_2D *pVtx)
{
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// rhw�̏����� [2D]
//--------------------------------------------------
void Initrhw(VERTEX_2D *pVtx)
{
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//--------------------------------------------------
// ���_�J���[�̏����� [2D]
//--------------------------------------------------
void Initcol(VERTEX_2D *pVtx)
{
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//--------------------------------------------------
// �e�N�X�`���̏����� [2D]
//--------------------------------------------------
void Inittex(VERTEX_2D *pVtx)
{
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//--------------------------------------------------
// �S�Ă̏����� [2D]
//--------------------------------------------------
void InitAll(VERTEX_2D *pVtx)
{
	// ���_���W�̏����� [2D]
	Initpos(pVtx);

	// rhw�̏����� [2D]
	Initrhw(pVtx);

	// ���_�J���[�̏����� [2D]
	Initcol(pVtx);

	// �e�N�X�`���̏����� [2D]
	Inittex(pVtx);
}

//--------------------------------------------------
// ���_���W�̐ݒ� [3D]
//--------------------------------------------------
void Setpos(VERTEX_3D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth,  fHeight,  fDepth);
	pVtx[1].pos = pos + D3DXVECTOR3( fWidth,  fHeight,  fDepth);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, -fDepth);
	pVtx[3].pos = pos + D3DXVECTOR3( fWidth, -fHeight, -fDepth);
}

//--------------------------------------------------
// ���_�̖@���̐ݒ� [3D]
//--------------------------------------------------
void Setnor(VERTEX_3D * pVtx, D3DXVECTOR3 nor)
{
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;
}

//--------------------------------------------------
// ���_�J���[�̐ݒ� [3D]
//--------------------------------------------------
void Setcol(VERTEX_3D *pVtx, D3DXCOLOR col)
{
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
}

//--------------------------------------------------
// �e�N�X�`���̐ݒ� [3D]
//--------------------------------------------------
void Settex(VERTEX_3D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom)
{
	pVtx[0].tex = D3DXVECTOR2(fULeft,  fVTop);
	pVtx[1].tex = D3DXVECTOR2(fURight, fVTop);
	pVtx[2].tex = D3DXVECTOR2(fULeft,  fVBottom);
	pVtx[3].tex = D3DXVECTOR2(fURight, fVBottom);
}

//--------------------------------------------------
// ���_���W�̏����� [3D]
//--------------------------------------------------
void Initpos(VERTEX_3D *pVtx)
{
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// ���_�̖@���̏����� [3D]
//--------------------------------------------------
void Initnor(VERTEX_3D *pVtx)
{
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

//--------------------------------------------------
// ���_�J���[�̏����� [3D]
//--------------------------------------------------
void Initcol(VERTEX_3D *pVtx)
{
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//--------------------------------------------------
// �e�N�X�`���̏����� [3D]
//--------------------------------------------------
void Inittex(VERTEX_3D *pVtx)
{
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//--------------------------------------------------
// �S�Ă̏����� [3D]
//--------------------------------------------------
void InitAll(VERTEX_3D *pVtx)
{
	// ���_���W�̏����� [3D]
	Initpos(pVtx);

	// ���_�̖@���̏�����[3D]
	Initnor(pVtx);

	// ���_�J���[�̏����� [3D]
	Initcol(pVtx);

	// �e�N�X�`���̏����� [3D]
	Inittex(pVtx);
}

//--------------------------------------------------
// ���_���W�̐ݒ� [��]
//--------------------------------------------------
void Setpos(VERTEX_LINE *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	pVtx[0].pos = pos + start;
	pVtx[1].pos = pos + end;
}

//--------------------------------------------------
// ���_�J���[�̐ݒ� [��]
//--------------------------------------------------
void Setcol(VERTEX_LINE *pVtx, D3DXCOLOR col)
{
	pVtx[0].col = col;
	pVtx[1].col = col;
}

//--------------------------------------------------
// ���_���W�̏����� [��]
//--------------------------------------------------
void Initpos(VERTEX_LINE *pVtx)
{
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// ���_�J���[�̏����� [��]
//--------------------------------------------------
void Initcol(VERTEX_LINE *pVtx)
{
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//--------------------------------------------------
// �S�Ă̏����� [��]
//--------------------------------------------------
void InitAll(VERTEX_LINE *pVtx)
{
	// ���_���W�̏����� [��]
	Initpos(pVtx);

	// ���_�J���[�̏����� [��]
	Initcol(pVtx);
}

//--------------------------------------------------
// �p�x�̐��K��
//--------------------------------------------------
void NormalizeRot(float *pRot)
{
	if (*pRot >= D3DX_PI)
	{// 3.14���傫��
		*pRot -= D3DX_PI * 2.0f;
	}
	else if (*pRot <= -D3DX_PI)
	{// -3.14��菬����
		*pRot += D3DX_PI * 2.0f;
	}
}

//--------------------------------------------------
// �w��̒l�ȏ�E�ȉ� [int]
//--------------------------------------------------
void Specified(int *pNumber, int nMax, int nMin)
{
	if (*pNumber >= nMax)
	{// �w��̒l�ȏ�
		*pNumber = nMax;
	}
	else if (*pNumber <= nMin)
	{// �w��̒l�ȉ�
		*pNumber = nMin;
	}
}

//--------------------------------------------------
// �w��̒l�ȏ�E�ȉ� [float]
//--------------------------------------------------
void Specified(float *pNumber, float fMax, float fMin)
{
	if (*pNumber >= fMax)
	{// �w��̒l�ȏ�
		*pNumber = fMax;
	}
	else if (*pNumber <= fMin)
	{// �w��̒l�ȉ�
		*pNumber = fMin;
	}
}

//--------------------------------------------------
// �o�[�e�b�N�X�̏������E�傫��
//--------------------------------------------------
void VtxSmallBig(float *pfMin, float *pfMax, float fPos)
{
	if (fPos < *pfMin)
	{// ������
		*pfMin = fPos;
	}

	if (fPos > *pfMax)
	{// �傫��
		*pfMax = fPos;
	}
}

//--------------------------------------------------
// 2D�x�N�g���̊O��
//--------------------------------------------------
float Vec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
	return (v1->x * v2->z) - (v1->z * v2->x);
}

//--------------------------------------------------
// 2D�x�N�g���̓���
//--------------------------------------------------
float Vec2Dot(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
	return (v1->x * v2->x) + (v1->z * v2->z);
}
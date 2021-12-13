//==================================================
// 
// 3D�Q�[������ ( setup.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "setup.h"

//--------------------------------------------------
// ���_���W�̐ݒ� [2D]
//--------------------------------------------------
void Setpos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3( fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth,  fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3( fWidth,  fHeight, 0.0f);
}

//--------------------------------------------------
// ���_�J���[�̐ݒ� [2D]
//--------------------------------------------------
void Setcol2D(VERTEX_2D *pVtx, float fRed, float fGreen, float fBlue, float fAlpha)
{
	pVtx[0].col = D3DXCOLOR(fRed, fGreen, fBlue, fAlpha);
	pVtx[1].col = D3DXCOLOR(fRed, fGreen, fBlue, fAlpha);
	pVtx[2].col = D3DXCOLOR(fRed, fGreen, fBlue, fAlpha);
	pVtx[3].col = D3DXCOLOR(fRed, fGreen, fBlue, fAlpha);
}

//--------------------------------------------------
// �e�N�X�`���̐ݒ� [2D]
//--------------------------------------------------
void Settex2D(VERTEX_2D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom)
{
	pVtx[0].tex = D3DXVECTOR2(fULeft,  fVTop);
	pVtx[1].tex = D3DXVECTOR2(fURight, fVTop);
	pVtx[2].tex = D3DXVECTOR2(fULeft,  fVBottom);
	pVtx[3].tex = D3DXVECTOR2(fURight, fVBottom);
}

//--------------------------------------------------
// ���_���W�̏����� [2D]
//--------------------------------------------------
void Initpos2D(VERTEX_2D *pVtx)
{
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// rhw�̏����� [2D]
//--------------------------------------------------
void Initrhw2D(VERTEX_2D *pVtx)
{
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//--------------------------------------------------
// ���_�J���[�̏����� [2D]
//--------------------------------------------------
void Initcol2D(VERTEX_2D *pVtx)
{
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//--------------------------------------------------
// �e�N�X�`���̏����� [2D]
//--------------------------------------------------
void Inittex2D(VERTEX_2D *pVtx)
{
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//--------------------------------------------------
// �S�Ă̏����� [2D]
//--------------------------------------------------
void InitAll2D(VERTEX_2D *pVtx)
{
	// ���_���W�̏����� [2D]
	Initpos2D(pVtx);

	// rhw�̏����� [2D]
	Initrhw2D(pVtx);

	// ���_�J���[�̏����� [2D]
	Initcol2D(pVtx);

	// �e�N�X�`���̏����� [2D]
	Inittex2D(pVtx);
}

//--------------------------------------------------
// ���_���W�̐ݒ� [3D]
//--------------------------------------------------
void Setpos3D(VERTEX_3D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth,  fHeight,  fDepth);
	pVtx[1].pos = pos + D3DXVECTOR3( fWidth,  fHeight,  fDepth);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, -fDepth);
	pVtx[3].pos = pos + D3DXVECTOR3( fWidth, -fHeight, -fDepth);
}

//--------------------------------------------------
// ���_�̖@���̐ݒ� [3D]
//--------------------------------------------------
void Setnor3D(VERTEX_3D * pVtx, D3DXVECTOR3 nor)
{
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;
}

//--------------------------------------------------
// ���_�J���[�̐ݒ� [3D]
//--------------------------------------------------
void Setcol3D(VERTEX_3D *pVtx, float fRed, float fGreen, float fBlue, float fAlpha)
{
	pVtx[0].col = D3DXCOLOR(fRed, fGreen, fBlue, fAlpha);
	pVtx[1].col = D3DXCOLOR(fRed, fGreen, fBlue, fAlpha);
	pVtx[2].col = D3DXCOLOR(fRed, fGreen, fBlue, fAlpha);
	pVtx[3].col = D3DXCOLOR(fRed, fGreen, fBlue, fAlpha);
}

//--------------------------------------------------
// �e�N�X�`���̐ݒ� [3D]
//--------------------------------------------------
void Settex3D(VERTEX_3D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom)
{
	pVtx[0].tex = D3DXVECTOR2(fULeft,  fVTop);
	pVtx[1].tex = D3DXVECTOR2(fURight, fVTop);
	pVtx[2].tex = D3DXVECTOR2(fULeft,  fVBottom);
	pVtx[3].tex = D3DXVECTOR2(fURight, fVBottom);
}

//--------------------------------------------------
// ���_���W�̏����� [3D]
//--------------------------------------------------
void Initpos3D(VERTEX_3D *pVtx)
{
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// ���_�̖@���̏����� [3D]
//--------------------------------------------------
void Initnor3D(VERTEX_3D *pVtx)
{
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

//--------------------------------------------------
// ���_�J���[�̏����� [3D]
//--------------------------------------------------
void Initcol3D(VERTEX_3D *pVtx)
{
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//--------------------------------------------------
// �e�N�X�`���̏����� [3D]
//--------------------------------------------------
void Inittex3D(VERTEX_3D *pVtx)
{
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//--------------------------------------------------
// �S�Ă̏����� [3D]
//--------------------------------------------------
void InitAll3D(VERTEX_3D *pVtx)
{
	// ���_���W�̏����� [3D]
	Initpos3D(pVtx);

	// ���_�̖@���̏�����[3D]
	Initnor3D(pVtx);

	// ���_�J���[�̏����� [3D]
	Initcol3D(pVtx);

	// �e�N�X�`���̏����� [3D]
	Inittex3D(pVtx);
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

//--------------------------------------------------
//
// ACG���� ( setup.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _SETUP_H_		//���̃}�N����`������ĂȂ�������
#define _SETUP_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------

/*�� ���_��� [2D] ��*/

void Setpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
void Setcol(VERTEX_2D *pVtx, D3DXCOLOR col);
void Settex(VERTEX_2D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom);

void Initpos(VERTEX_2D *pVtx);
void Initrhw(VERTEX_2D *pVtx);
void Initcol(VERTEX_2D *pVtx);
void Inittex(VERTEX_2D *pVtx);
void InitAll(VERTEX_2D *pVtx);

/*�� ���_��� [3D] ��*/

void Setpos(VERTEX_3D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth);
void Setnor(VERTEX_3D *pVtx, D3DXVECTOR3 nor);
void Setcol(VERTEX_3D *pVtx, D3DXCOLOR col);
void Settex(VERTEX_3D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom);

void Initpos(VERTEX_3D *pVtx);
void Initnor(VERTEX_3D *pVtx);
void Initcol(VERTEX_3D *pVtx);
void Inittex(VERTEX_3D *pVtx);
void InitAll(VERTEX_3D *pVtx);

/*�� ���_��� [��] ��*/

void Setpos(VERTEX_LINE *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end);
void Setcol(VERTEX_LINE *pVtx, D3DXCOLOR col);

void Initpos(VERTEX_LINE *pVtx);
void Initcol(VERTEX_LINE *pVtx);
void InitAll(VERTEX_LINE *pVtx);

/*�� �l�̕ύX ��*/

void NormalizeRot(float *pRot);
void Specified(int *pNumber, int nMax, int nMin);
void Specified(float *pNumber, float fMax, float fMin);
void VtxSmallBig(float *pfMin, float *pfMax, float fPos);

#endif // !_SETUP_H_

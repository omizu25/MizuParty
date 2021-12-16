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

void Setpos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
void Setcol2D(VERTEX_2D *pVtx, float fRed, float fGreen, float fBlue, float fAlpha);
void Settex2D(VERTEX_2D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom);
void Initpos2D(VERTEX_2D *pVtx);
void Initrhw2D(VERTEX_2D *pVtx);
void Initcol2D(VERTEX_2D *pVtx);
void Inittex2D(VERTEX_2D *pVtx);
void InitAll2D(VERTEX_2D *pVtx);

/*�� ���_��� [3D] ��*/

void Setpos3D(VERTEX_3D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth);
void Setnor3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor);
void Setcol3D(VERTEX_3D *pVtx, D3DXCOLOR col);
void Settex3D(VERTEX_3D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom);
void Initpos3D(VERTEX_3D *pVtx);
void Initnor3D(VERTEX_3D *pVtx);
void Initcol3D(VERTEX_3D *pVtx);
void Inittex3D(VERTEX_3D *pVtx);
void InitAll3D(VERTEX_3D *pVtx);

/*�� ���_��� [��] ��*/

void SetposLine(VERTEX_LINE *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end);
void SetcolLine(VERTEX_LINE *pVtx, D3DXCOLOR col);

void InitposLine(VERTEX_LINE *pVtx);
void InitcolLine(VERTEX_LINE *pVtx);
void InitAllLine(VERTEX_LINE *pVtx);

/*�� �l�̕ύX ��*/

void NormalizeRot(float *pRot);
void Specified(int *pNumber, int nMax, int nMin);
void Specified(float *pNumber, float fMax, float fMin);

#endif // !_SETUP_H_

//--------------------------------------------------
//
// ACG���� ( setup.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _SETUP_H_		//���̃}�N����`������ĂȂ�������
#define _SETUP_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void Setpos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
void Setcol2D(VERTEX_2D *pVtx, float fRed, float fGreen, float fBlue, float fAlpha);
void Settex2D(VERTEX_2D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom);
void Initpos2D(VERTEX_2D *pVtx);
void Initrhw2D(VERTEX_2D *pVtx);
void Initcol2D(VERTEX_2D *pVtx);
void Inittex2D(VERTEX_2D *pVtx);
void InitAll2D(VERTEX_2D *pVtx);
void Setpos3D(VERTEX_3D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth);
void Setnor3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor);
void Setcol3D(VERTEX_3D *pVtx, float fRed, float fGreen, float fBlue, float fAlpha);
void Settex3D(VERTEX_3D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom);
void Initpos3D(VERTEX_3D *pVtx);
void Initnor3D(VERTEX_3D *pVtx);
void Initcol3D(VERTEX_3D *pVtx);
void Inittex3D(VERTEX_3D *pVtx);
void InitAll3D(VERTEX_3D *pVtx);
void NormalizeRot(float *pRot);

#endif // !_SETUP_H_
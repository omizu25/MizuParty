//==================================================
//
// 3D�Q�[������ ( billboard.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _BILLBOARD_H_		// ���̃}�N����`������ĂȂ�������
#define _BILLBOARD_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_BILLBOARD		(256)		// �r���{�[�h�̍ő吔

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	D3DXMATRIX				mtxWorld;		// ���[���h�}�g���b�N�X
	float					fWidth;			// ��
	float					fHeight;		// ����
	bool					bUse;			// �g�p���Ă��邩�ǂ���
	bool					bYRot;			// Y����]�����邩�ǂ���
	bool					bResult;		// ���U���g�����ŕ\������
	bool					bCamera;		// �J�������Ԗڂ�
	LPDIRECT3DTEXTURE9		pTexture;		// �e�N�X�`��
}Billboard;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(bool bResult, bool bCamera);
void SetBillboard(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, bool bYRot, bool bResult, bool bCamera, LPDIRECT3DTEXTURE9 *pTexture);
Billboard *GetBillboard(void);
void LoadBillboard(void);
void CollisionBillboard(void);
void InitBillboardSlope(void);
void InitBillboardTitle(void);

#endif // !_BILLBOARD_H_

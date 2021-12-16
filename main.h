//==================================================
//
// 3D�Q�[������ ( main.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MAIN_H_		// ���̃}�N����`������ĂȂ�������
#define _MAIN_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include <windows.h>
#include "d3dx9.h"									// �`�揈���ɕK�v
#define  DIRECTINPUT_VERSION		(0x0800)		// �r���h���̌x���Ώ��p�}�N��
#include "dinput.h"									// ���͏����ɕK�v
#include "xinput.h"									// �W���C�p�b�h�����ɕK�v
#include "xaudio2.h"								// �T�E���h�����ɕK�v

//--------------------------------------------------
// ���C�u�����̃����N
//--------------------------------------------------
#pragma comment(lib, "d3d9.lib")		// �`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")		// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib, "dxguid.lib")		// DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib, "winmm.lib")		// �V�X�e�������擾�ɕK�v

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define SCREEN_WIDTH		(1280)																// �E�C���h�E�̕�
#define SCREEN_HEIGHT		(720)																// �E�C���h�E�̍���
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)						// ���_�t�H�[�}�b�g[2D]
#define FVF_VERTEX_3D		(D3DFVF_XYZ |  D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// ���_�t�H�[�}�b�g[3D] �ʒu�E�@���E�J���[�E�e�N�X�`��
#define FVF_VERTEX_LINE		(D3DFVF_XYZ | D3DFVF_DIFFUSE)										// ���_�t�H�[�}�b�g[��] �ʒu�E�J���[

//--------------------------------------------------
// �񋓌^
//--------------------------------------------------
typedef enum
{
	DEBUG_CAMERA = 0,		// �J����
	DEBUG_PLAYER,			// �v���C���[
	DEBUG_MESH,				// ���b�V��
	DEBUG_MAX
}DEBUG;

//--------------------------------------------------
// �\����
//--------------------------------------------------

/*�� ���_���[2D] ��*/

typedef struct
{
	D3DXVECTOR3		pos;		// ���_���W
	float			rhw;		// ���W�ϊ��p�W�� (1.0f�ŌŒ�)
	D3DCOLOR		col;		// ���_�J���[
	D3DXVECTOR2		tex;		// �e�N�X�`�����W
}VERTEX_2D;

/*�� ���_���[3D] ��*/

typedef struct
{
	D3DXVECTOR3		pos;		// ���_���W
	D3DXVECTOR3		nor;		// �@���x�N�g��
	D3DCOLOR		col;		// ���_�J���[
	D3DXVECTOR2		tex;		// �e�N�X�`�����W
}VERTEX_3D;

/*�� ���_���[��] ��*/

typedef struct
{
	D3DXVECTOR3		pos;		// ���_���W
	D3DCOLOR		col;		// ���_�J���[
}VERTEX_LINE;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void);
DEBUG GetDebug(void);

#endif // !_MAIN_H_

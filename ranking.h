//==================================================
//
// 3D�Q�[������ ( ranking.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _RANKING_H_		//���̃}�N����`������ĂȂ�������
#define _RANKING_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitRanking(void);
void UninitRanking(void);
void LoadRanking(void);
void SaveRanking(void);
void SetScoreWalking(int nScore);
void SetScoreStop(int nScore);
void SetScoreSlope(int nScore);

#endif // !_RANKING_H_
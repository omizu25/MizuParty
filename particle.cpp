//==================================================
// 
// 3Dゲーム制作 ( particle.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "effect.h"
#include "particle.h"
#include "setup.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_PARTICLE		(4096)		// パーティクルの最大数
#define MAX_STAR			(50)		// スターの最大数

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		move;			// 移動量
	D3DXCOLOR		col;			// 色
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	int				nLife;			// 寿命(表示時間)
	bool			bAdd;			// 加算合成をするかどうか
	bool			bUse;			// 使用しているかどうか
}Particle;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Particle		s_aParticle[MAX_PARTICLE];		// パーティクルの情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitParticle(void)
{
	// メモリのクリア
	memset(&s_aParticle, NULL, sizeof(s_aParticle));
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateParticle(void)
{
	
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetParticle(D3DXVECTOR3 pos, float fSize, bool bAdd)
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle *pParticle = &s_aParticle[i];

		if (pParticle->bUse)
		{// 使用されている
			continue;
		}

		/*↓ 使用されていない ↓*/

		for (int j = 0; j < MAX_STAR; j++)
		{
			pParticle->pos.x = (float)((rand() % 1601) - 800) / 30.0f + pos.x;
			pParticle->pos.y = (float)((rand() % 1601) - 800) / 30.0f + pos.y;
			pParticle->pos.z = (float)((rand() % 1601) - 800) / 30.0f + pos.z;

			pParticle->move.x = (float)((rand() % 161) - 80) / 10.0f;
			pParticle->move.y = (float)((rand() % 161) - 80) / 10.0f;
			pParticle->move.z = (float)((rand() % 161) - 80) / 10.0f;

			pParticle->col.r = (float)(rand() % 101) / 100.0f;
			pParticle->col.g = (float)(rand() % 101) / 100.0f;
			pParticle->col.b = (float)(rand() % 101) / 100.0f;
			pParticle->col.a = 1.0f;

			pParticle->fWidth = (float)(rand() % 51) / 100.0f + fSize;
			pParticle->fHeight = pParticle->fWidth;

			pParticle->nLife = (rand() % 11) + 10;

			//エフェクトの設定
			SetEffect(pParticle->pos, pParticle->move, pParticle->col, pParticle->fWidth, pParticle->fHeight, pParticle->nLife, bAdd);
		}

		break;		// ここでfor文を抜ける
	}
}

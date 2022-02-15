//==================================================
// 
// 3Dゲーム制作 ( ranking.cpp )
// Author  : katsuki mizuki
// 
//==================================================
//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "field.h"
#include "model.h"
#include "number.h"
#include "player.h"
#include "ranking.h"
#include "setup.h"
#include "target.h"
#include "title.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define RANKING_FILE		"data/TEXT/Ranking.txt"		// テキストへのパス
#define MAX_DIGIT					(5)					// 桁数の最大数
#define MIN_DIGIT					(2)					// 桁数の最小数
#define NUMBER_WIDTH				(20.0f)				// 数の幅
#define NUMBER_HEIGHT				(30.0f)				// 数の高さ
#define DECIMAL_WIDTH				(80.0f)				// 小数点の幅
#define DECIMAL_HEIGHT				(150.0f)			// 小数点の高さ
#define DECIMAL_INTERVAL			(10.0f)				// 小数の間隔

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static int		s_nWalking;				// ウォーキング
static int		s_nStop;				// 止める
static int		s_nSlope;				// 坂
static int		s_nDigitWalking;		// 桁数のウォーキング
static int		s_nDigitStop;			// 桁数の止める
static int		s_nDigitSlope;			// 桁数の坂

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void NumberWalking(void);
static void NumberStop(void);
static void NumberSlope(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRanking(void)
{
	s_nDigitWalking = 0;
	s_nDigitStop = 0;
	s_nDigitSlope = 0;

	// ウォーキングの数字
	NumberWalking();

	// 止めるの数字
	NumberStop();

	// 坂の数字
	NumberSlope();
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadRanking(void)
{
	FILE *pFile;		// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(RANKING_FILE, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		fscanf(pFile, "%d", &s_nWalking);
		fscanf(pFile, "%d", &s_nStop);
		fscanf(pFile, "%d", &s_nSlope);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//--------------------------------------------------
// セーブ
//--------------------------------------------------
void SaveRanking(void)
{
	FILE *pFile;		// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(RANKING_FILE, "w");

	if (pFile != NULL)
	{// ファイルが開いた場合
		fprintf(pFile, "%d\n\n", s_nWalking);
		fprintf(pFile, "%d\n\n", s_nStop);
		fprintf(pFile, "%d\n\n", s_nSlope);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//--------------------------------------------------
// ウォーキングの数字
//--------------------------------------------------
static void NumberWalking(void)
{
	// 位置を初期化する
	D3DXVECTOR3 pos = D3DXVECTOR3(-170.0f, 0.0f, -5.0f);

	int nNumber = s_nWalking;

	while (1)
	{// 無限ループ
		if (nNumber >= 10)
		{// 2桁以上
			nNumber /= 10;
			s_nDigitWalking++;
		}
		else
		{// 1桁
			s_nDigitWalking++;

			if (s_nDigitWalking < MIN_DIGIT)
			{// 桁数が足りてない
				s_nDigitWalking = MIN_DIGIT;
			}

			break;
		}
	}

	int aNumber[MAX_DIGIT];
	int nWalking = s_nWalking;

	for (int i = 0; i < s_nDigitWalking; i++)
	{// １桁ずつに分ける
		aNumber[i] = nWalking % 10;
		nWalking /= 10;

		float fInterval = (NUMBER_WIDTH * i);

		float fDecimal = 0.0f;

		if (i >= 1)
		{// 小数点が入る場所を空ける
			fDecimal = DECIMAL_INTERVAL;
		}

		// 数の設定処理
		SetNumber3D(D3DXVECTOR3(pos.x - fInterval - fDecimal, 0.0f, pos.z), NUMBER_WIDTH * 0.5f, NUMBER_HEIGHT * 0.5f, aNumber[i], i);
	}
}

//--------------------------------------------------
// 止めるの数字
//--------------------------------------------------
static void NumberStop(void)
{
	// 位置を初期化する
	D3DXVECTOR3 pos = D3DXVECTOR3(30.0f, 0.0f, -5.0f);

	int nNumber = s_nStop;

	while (1)
	{// 無限ループ
		if (nNumber >= 10)
		{// 2桁以上
			nNumber /= 10;
			s_nDigitStop++;
		}
		else
		{// 1桁
			s_nDigitStop++;

			if (s_nDigitStop < MIN_DIGIT)
			{// 桁数が足りてない
				s_nDigitStop = MIN_DIGIT;
			}

			break;
		}
	}

	int aNumber[MAX_DIGIT];
	int nStop = s_nStop;

	for (int i = 0; i < s_nDigitStop; i++)
	{// １桁ずつに分ける
		aNumber[i] = nStop % 10;
		nStop /= 10;

		float fInterval = (NUMBER_WIDTH * i);

		float fDecimal = 0.0f;

		if (i >= 1)
		{// 小数点が入る場所を空ける
			fDecimal = DECIMAL_INTERVAL;
		}

		// 数の設定処理
		SetNumber3D(D3DXVECTOR3(pos.x - fInterval - fDecimal, 0.0f, pos.z), NUMBER_WIDTH * 0.5f, NUMBER_HEIGHT * 0.5f, aNumber[i], i);
	}
}

//--------------------------------------------------
// 坂の数字
//--------------------------------------------------
static void NumberSlope(void)
{
	// 位置を初期化する
	D3DXVECTOR3 pos = D3DXVECTOR3(225.0f, 0.0f, -5.0f);

	int nNumber = s_nSlope;

	while (1)
	{// 無限ループ
		if (nNumber >= 10)
		{// 2桁以上
			nNumber /= 10;
			s_nDigitSlope++;
		}
		else
		{// 1桁
			s_nDigitSlope++;

			if (s_nDigitSlope < MIN_DIGIT)
			{// 桁数が足りてない
				s_nDigitSlope = MIN_DIGIT;
			}

			break;
		}
	}

	int aNumber[MAX_DIGIT];
	int nSlope = s_nSlope;

	for (int i = 0; i < s_nDigitSlope; i++)
	{// １桁ずつに分ける
		aNumber[i] = nSlope % 10;
		nSlope /= 10;

		float fInterval = (NUMBER_WIDTH * i);

		float fDecimal = 0.0f;

		if (i >= 1)
		{// 小数点が入る場所を空ける
			fDecimal = DECIMAL_INTERVAL;
		}

		// 数の設定処理
		SetNumber3D(D3DXVECTOR3(pos.x - fInterval - fDecimal, 0.0f, pos.z), NUMBER_WIDTH * 0.5f, NUMBER_HEIGHT * 0.5f, aNumber[i], i);
	}
}
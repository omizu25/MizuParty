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
#include "billboard.h"
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
#define METER_WIDTH					(20.0f)				// メートルの幅
#define METER_HEIGHT				(30.0f)				// メートルの高さ
#define DECIMAL_WIDTH				(15.0f)				// 小数点の幅
#define DECIMAL_HEIGHT				(25.0f)				// 小数点の高さ
#define DECIMAL_INTERVAL			(10.0f)				// 小数の間隔
#define OPERATOR_WIDTH				(15.0f)				// 演算子の幅
#define OPERATOR_HEIGHT				(15.0f)				// 演算子の高さ

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureMeter = NULL;			// メートルのテクスチャへのポインタ
static LPDIRECT3DTEXTURE9		s_pTextureDecimal = NULL;		// 小数点のテクスチャへのポインタ
static LPDIRECT3DTEXTURE9		s_pTextureOperator = NULL;		// 演算子のテクスチャへのポインタ
static int						s_nWalking;						// ウォーキング
static int						s_nStop;						// 止める
static int						s_nSlope;						// 坂
static int						s_nDigitWalking;				// 桁数のウォーキング
static int						s_nDigitStop;					// 桁数の止める
static int						s_nDigitSlope;					// 桁数の坂

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void NumberWalking(void);
static void InitWalking(void);
static void NumberStop(void);
static void InitStop(void);
static void NumberSlope(void);
static void InitSlope(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRanking(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/METER/m.png",
		&s_pTextureMeter);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/decimal.png",
		&s_pTextureDecimal);

	s_nDigitWalking = 0;
	s_nDigitStop = 0;
	s_nDigitSlope = 0;

	// ウォーキングの数字
	NumberWalking();

	// ウォーキングの初期化
	InitWalking();

	// 止めるの数字
	NumberStop();

	// 止めるの初期化
	InitStop();

	// 坂の数字
	NumberSlope();

	// 坂の初期化
	InitSlope();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitRanking(void)
{
	if (s_pTextureMeter != NULL)
	{// テクスチャの解放
		s_pTextureMeter->Release();
		s_pTextureMeter = NULL;
	}

	if (s_pTextureDecimal != NULL)
	{// テクスチャの解放
		s_pTextureDecimal->Release();
		s_pTextureDecimal = NULL;
	}

	if (s_pTextureOperator != NULL)
	{// テクスチャの解放
		s_pTextureOperator->Release();
		s_pTextureOperator = NULL;
	}
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
// ウォーキングの設定
//--------------------------------------------------
void SetScoreWalking(int nScore)
{
	int nWalking = s_nWalking;
	int nScoreSave = nScore;

	if (nWalking < 0.0f)
	{// マイナス
		nWalking *= -1;
	}

	if (nScoreSave < 0.0f)
	{// マイナス
		nScoreSave *= -1;
	}

	if (nWalking > nScoreSave)
	{// スコアが小さい
		s_nWalking = nScore;
	}
}

//--------------------------------------------------
// 止めるの設定
//--------------------------------------------------
void SetScoreStop(int nScore)
{
	if (s_nStop > nScore)
	{// スコアが小さい
		s_nStop = nScore;
	}
}

//--------------------------------------------------
// 坂の設定
//--------------------------------------------------
void SetScoreSlope(int nScore)
{
	if (s_nSlope > nScore)
	{// スコアが小さい
		s_nSlope = nScore;
	}
}

//--------------------------------------------------
// ウォーキングの数字
//--------------------------------------------------
static void NumberWalking(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nWalking = s_nWalking;

	if (nWalking < 0)
	{// マイナス
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			"data/TEXTURE/plus.png",
			&s_pTextureOperator);
	}
	else
	{// プラス
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			"data/TEXTURE/minus.png",
			&s_pTextureOperator);
	}

	if (nWalking < 0.0f)
	{// マイナス
		nWalking *= -1;
	}

	int nNumber = nWalking;

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

	// 位置を初期化する
	D3DXVECTOR3 pos = D3DXVECTOR3(-170.0f, 0.0f, -5.0f);

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

	float fWidth = OPERATOR_WIDTH * 0.5f;
	float fHeight = OPERATOR_HEIGHT * 0.5f;

	float fPosX = -170.0f - DECIMAL_INTERVAL - (NUMBER_WIDTH * s_nDigitWalking) + (NUMBER_WIDTH * 0.5f);

	pos = D3DXVECTOR3(fPosX - fWidth, 0.0f, -5.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureOperator);
}

//--------------------------------------------------
// ウォーキングの初期化
//--------------------------------------------------
static void InitWalking(void)
{
	float fWidth = METER_WIDTH * 0.5f;
	float fHeight = METER_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(245.0f, 0.0f, -8.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureMeter);

	fWidth = DECIMAL_WIDTH * 0.5f;
	fHeight = DECIMAL_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(210.0f, 0.0f, -6.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureDecimal);
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
// 止めるの初期化
//--------------------------------------------------
static void InitStop(void)
{
	float fWidth = METER_WIDTH * 0.5f;
	float fHeight = METER_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(50.0f, 0.0f, -8.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureMeter);

	fWidth = DECIMAL_WIDTH * 0.5f;
	fHeight = DECIMAL_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(15.0f, 0.0f, -6.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureDecimal);
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

//--------------------------------------------------
// 坂の初期化
//--------------------------------------------------
static void InitSlope(void)
{
	float fWidth = METER_WIDTH * 0.5f;
	float fHeight = METER_HEIGHT * 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(-150.0f, 0.0f, -8.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureMeter);

	fWidth = DECIMAL_WIDTH * 0.5f;
	fHeight = DECIMAL_HEIGHT * 0.5f;

	pos = D3DXVECTOR3(-185.0f, 0.0f, -6.0f);

	// ビルボードの設定
	SetBillboard(pos, fWidth, fHeight, true, false, false, &s_pTextureDecimal);
}

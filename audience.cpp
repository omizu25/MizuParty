//==================================================
// 
// 3Dゲーム制作 ( audience.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "game.h"
#include "input.h"
#include "audience.h"
#include "setup.h"
#include "shadow.h"
#include "title.h"

#include <stdio.h>
#include <assert.h>
#include <time.h>
//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
#define MAX_RANDOM		(3)		// 観客の最大数
#define MIN_RANDOM		(1)		// 観客の最小数

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	LPD3DXMESH			pMesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER		pBuffMat;				// マテリアル情報へのポインタ
	DWORD				nNumMat;				// マテリアル情報の数
	char				aText[MAX_TEXT];		// モデルファイル名
	bool				bLoad;					// 読み込みをしたかどうか
}Text;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Audience		*s_pAudience;		// 観客の情報
static Text			*s_pText;			// テキストの情報
static int			s_nNumAudience;		// 観客の数
static int			s_nNumFileName;		// ファイル名の数
static int			s_nLeft;			// 左側の観客の数
static int			s_nRight;			// 右側の観客の数

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void System(char *pFileName);
static void LoadNew(char *pFileName);
static void LoadAudience(char *pFileName);
static void InitRandom(void);
static void RandomNum(void);
static void RandomUse(void);
static void RandomPos(void);
static void Collision(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitAudience(void)
{
	// ランダム
	InitRandom();

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < s_nNumAudience; i++)
	{
		Audience *pAudience = &s_pAudience[i];

		int nNumVtx;		// 頂点数
		DWORD SizeFVF;		// 頂点フォーマットのサイズ
		BYTE *pVexBuff;		// 頂点バッファへのポインタ

		pAudience->vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		pAudience->vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		// 頂点数を取得
		nNumVtx = pAudience->pMesh->GetNumVertices();

		// フォーマットのサイズを取得
		SizeFVF = D3DXGetFVFVertexSize(pAudience->pMesh->GetFVF());

		// 頂点バッファのロック
		pAudience->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

		for (int j = 0; j < nNumVtx; j++)
		{
			// 頂点情報の代入
			D3DXVECTOR3 pos = *(D3DXVECTOR3*)pVexBuff;

			// 小さい・大きい [x]
			VtxSmallBig(&pAudience->vtxMin.x, &pAudience->vtxMax.x, pos.x);

			// 小さい・大きい [y]
			VtxSmallBig(&pAudience->vtxMin.y, &pAudience->vtxMax.y, pos.y);

			// 小さい・大きい [z]
			VtxSmallBig(&pAudience->vtxMin.z, &pAudience->vtxMax.z, pos.z);

			// 頂点フォーマットのサイズ分ポインタを進める
			pVexBuff += SizeFVF;
		}

		// 頂点バッファのアンロック
		pAudience->pMesh->UnlockVertexBuffer();

		// メッシュに使用されているテクスチャ用の配列を用意する
		pAudience->pTexture = new LPDIRECT3DTEXTURE9[pAudience->nNumMat];

		// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)pAudience->pBuffMat->GetBufferPointer();

		// 各メッシュのマテリアル情報を取得する
		for (int j = 0; j < (int)pAudience->nNumMat; j++)
		{
			pAudience->pTexture[j] = NULL;

			if (pMat[j].pTextureFilename != NULL)
			{// マテリアルで設定されているテクスチャ読み込み
				D3DXCreateTextureFromFileA(pDevice,
					pMat[j].pTextureFilename,
					&pAudience->pTexture[j]);
			}
			else
			{// 使用されてない
				pAudience->pTexture[j] = NULL;
			}
		}
		pAudience->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 影の設定
		pAudience->nIdxShadow = SetShadow(pAudience->pos, pAudience->rot, pAudience->vtxMax.x);
	}

	// 当たり判定
	Collision();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitAudience(void)
{
	for (int i = 0; i < s_nNumAudience; i++)
	{
		Audience *pAudience = &s_pAudience[i];

		if (pAudience->pTexture != NULL)
		{// テクスチャの解放
			for (int j = 0; j < (int)pAudience->nNumMat; j++)
			{
				if (pAudience->pTexture[j] != NULL)
				{
					pAudience->pTexture[j]->Release();
					pAudience->pTexture[j] = NULL;
				}
			}

			delete[] pAudience->pTexture;
			pAudience->pTexture = NULL;
		}

		if (pAudience->pMesh != NULL)
		{// メッシュの解放
			pAudience->pMesh = NULL;
		}

		if (pAudience->pBuffMat != NULL)
		{// マテリアルの解放
			pAudience->pBuffMat = NULL;
		}
	}

	for (int i = 0; i < s_nNumFileName; i++)
	{
		Text *pText = &s_pText[i];

		if (pText->pMesh != NULL)
		{// メッシュの解放
			pText->pMesh->Release();
			pText->pMesh = NULL;
		}

		if (pText->pBuffMat != NULL)
		{// マテリアルの解放
			pText->pBuffMat->Release();
			pText->pBuffMat = NULL;
		}
	}

	// 観客の開放
	delete[] s_pAudience;
	s_pAudience = NULL;

	// テキストの開放
	delete[] s_pText;
	s_pText = NULL;
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateAudience(void)
{
	
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawAudience(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

	for (int i = 0; i < s_nNumAudience; i++)
	{
		Audience *pAudience = &s_pAudience[i];

		if (!pAudience->bDraw)
		{// 描画しない
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pAudience->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pAudience->rot.y, pAudience->rot.x, pAudience->rot.z);
		D3DXMatrixMultiply(&pAudience->mtxWorld, &pAudience->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pAudience->pos.x, pAudience->pos.y, pAudience->pos.z);
		D3DXMatrixMultiply(&pAudience->mtxWorld, &pAudience->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pAudience->mtxWorld);

		// 現在のマテリアル保持
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pAudience->pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)pAudience->nNumMat; j++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[j].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pAudience->pTexture[j]);

			// モデルパーツの描画
			pAudience->pMesh->DrawSubset(j);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadAudience(void)
{
	// システム
	char aFileName[MAX_TEXT];

	System(&aFileName[0]);

	// new用の読み込み
	LoadNew(&aFileName[0]);

	// 観客の読み込み
	LoadAudience(&aFileName[0]);
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Audience *GetAudience(int nAudience)
{
	return &s_pAudience[nAudience];
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
int GetNumAudience(void)
{
	return s_nNumAudience;
}

//--------------------------------------------------
// システム
//--------------------------------------------------
static void System(char *pFileName)
{
	FILE *pFile;		// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};
		
		while (strcmp(&aRead[0], "SCRIPT") != 0)
		{// 始まりが来るまで繰り返す
			fscanf(pFile, "%s", &aRead);
		}

		while (strcmp(&aRead[0], "END_SCRIPT") != 0)
		{// 終わりが来るまで繰り返す
			fscanf(pFile, "%s", &aRead);

			if (strncmp(&aRead[0], "#-", 2) == 0)
			{// コメント
				continue;
			}
			else if (strncmp(&aRead[0], "#", 1) == 0)
			{// コメント
				fgets(aRead, MAX_TEXT, pFile);
				continue;
			}

			if (strcmp(&aRead[0], "AUDIENCE_FILENAME") == 0)
			{// プレイヤーのファイル名
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", pFileName);
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//--------------------------------------------------
// new用の読み込み
//--------------------------------------------------
static void LoadNew(char *pFileName)
{
	FILE *pFile;		// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};
		s_nNumFileName = 0;

		while (strncmp(&aRead[0], "END_SCRIPT", 10) != 0)
		{// 終わりが来るまで繰り返す
			fgets(aRead, MAX_TEXT, pFile);

			if (strncmp(&aRead[0], "MODEL_FILENAME", 14) == 0)
			{// モデルの情報
				s_nNumFileName++;
			}
		}

		// ファイルを閉じる
		fclose(pFile);

		// 必要数分の配列を用意する
		s_pText = new Text[s_nNumFileName];

		for (int i = 0; i < s_nNumFileName; i++)
		{// テキストの情報の初期化
			Text *pText = &s_pText[i];

			pText->pBuffMat = NULL;
			pText->pMesh = NULL;
			pText->nNumMat = 0;
			pText->bLoad = false;
		}
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//--------------------------------------------------
// 観客の読み込み
//--------------------------------------------------
static void LoadAudience(char *pFileName)
{
	FILE *pFile;		// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};
		int nFileName = 0;

		while (strcmp(&aRead[0], "SCRIPT") != 0)
		{// 始まりが来るまで繰り返す
			fscanf(pFile, "%s", &aRead);
		}

		while (strcmp(&aRead[0], "END_SCRIPT") != 0)
		{// 終わりが来るまで繰り返す
			fscanf(pFile, "%s", &aRead);

			if (strncmp(&aRead[0], "#-", 2) == 0)
			{// コメント
				continue;
			}
			else if (strncmp(&aRead[0], "#", 1) == 0)
			{// コメント
				fgets(aRead, MAX_TEXT, pFile);
				continue;
			}
			
			if (strcmp(&aRead[0], "MODEL_FILENAME") == 0)
			{// モデルファイル名
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", &s_pText[nFileName].aText);

				nFileName++;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//--------------------------------------------------
// ランダム
//--------------------------------------------------
static void InitRandom(void)
{
	//世界の種子の初期化
	srand((unsigned)time(NULL));

	// 使う数
	RandomNum();

	// 使う観客
	RandomUse();

	// 位置
	RandomPos();
}

//--------------------------------------------------
// 使う数
//--------------------------------------------------
static void RandomNum(void)
{
	s_nLeft = 0;
	s_nRight = 0;
	s_nNumAudience = 0;

	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
	case MENU_STOP:			// 止める
		s_nLeft = (rand() % MAX_RANDOM) + MIN_RANDOM;
		s_nRight = (rand() % MAX_RANDOM) + MIN_RANDOM;

		s_nNumAudience = s_nLeft + s_nRight;

		break;

	case MENU_SLOPE:		// 坂

		s_nNumAudience = (rand() % MAX_RANDOM) + MIN_RANDOM;

		break;

	default:
		assert(false);
		break;
	}

	// 必要数分の配列を用意する
	s_pAudience = new Audience[s_nNumAudience];
}

//--------------------------------------------------
// 使う観客
//--------------------------------------------------
static void RandomUse(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < s_nNumAudience; i++)
	{
		Audience *pAudience = &s_pAudience[i];

		int nUse = (rand() % s_nNumFileName);

		Text *pText = &s_pText[nUse];

		if (!pText->bLoad)
		{// 読み込みをまだしてない
			// Xファイルの読み込み
			D3DXLoadMeshFromX(
				pText->aText,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&pText->pBuffMat,
				NULL,
				&pText->nNumMat,
				&pText->pMesh);

			pText->bLoad = true;
		}

		// 使用する観客の情報を取得
		pAudience->pBuffMat = pText->pBuffMat;
		pAudience->nNumMat = pText->nNumMat;
		pAudience->pMesh = pText->pMesh;

		pAudience->bDraw = true;
	}
}

//--------------------------------------------------
// 位置
//--------------------------------------------------
static void RandomPos(void)
{
	int nPosX = 0;
	float fRand = 0.0f;

	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
		
		for (int i = 0; i < s_nLeft; i++)
		{
			Audience *pAudience = &s_pAudience[i];

			nPosX = (30 * 75) / s_nLeft;
			fRand = (float)((rand() % nPosX) + (nPosX * i));
			fRand += (30.0f * 10.0f);

			pAudience->pos = D3DXVECTOR3(-fRand, 0.0f, 0.0f);
		}

		for (int i = 0; i < s_nRight; i++)
		{
			Audience *pAudience = &s_pAudience[i + s_nLeft];

			nPosX = (30 * 75) / s_nRight;
			fRand = (float)((rand() % nPosX) + (nPosX * i));
			fRand += (30.0f * 10.0f);

			pAudience->pos = D3DXVECTOR3(fRand, 0.0f, 0.0f);
		}

		break;

	case MENU_STOP:			// 止める

		break;

	case MENU_SLOPE:		// 坂

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 当たり判定
//--------------------------------------------------
static void Collision(void)
{
	for (int i = 0; i < (s_nNumAudience - 1); i++)
	{
		Audience *pAudience = &s_pAudience[i];

		float fAudienceLeft = pAudience->pos.x + pAudience->vtxMin.x;
		float fAudienceRight = pAudience->pos.x + pAudience->vtxMax.x;

		Audience *pNext = &s_pAudience[i + 1];

		float fNextLeft = pNext->pos.x + pNext->vtxMin.x;
		float fNextRight = pNext->pos.x + pNext->vtxMax.x;

		if (fAudienceLeft <= fNextRight &&
			fAudienceRight >= fNextLeft)
		{// 重なってる
			float fPosX = fAudienceRight + pNext->vtxMin.x;
			pNext->pos.x = fPosX + pAudience->vtxMax.x;
		}
	}
}

//==================================================
// 
// 3Dゲーム制作 ( player.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "billboard.h"
#include "camera.h"
#include "fade.h"
#include "field.h"
#include "frame.h"
#include "game.h"
#include "input.h"
#include "mesh_field.h"
#include "model.h"
#include "particle.h"
#include "player.h"
#include "result.h"
#include "setup.h"
#include "shadow.h"
#include "wall.h"
#include "sound.h"
#include "title.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_ROTATION		(0.035f)		// 回転の最大値
#define MAX_ATTENUATION		(0.1f)			// 減衰係数の最大値
#define MAX_INERTIA			(0.5f)			// 慣性の最大値
#define MAX_HEIGHT			(80.0f)			// 高さの最大値
#define MIN_HEIGHT			(-80.0f)		// 高さの最小値
#define IDX_PARENT			(-1)			// 親の番号
#define MAX_BLEND			(20)			// ブレンドの最大値
#define SLOPE_LIMIT			(90.0f)			// 坂の移動制限
#define SLOPE_RESULT		(1500.0f)		// 坂のリザルトへの判定
#define TITLE_WIDTH			(255.0f)		// タイトルの移動制限
#define TITLE_DEPTH			(135.0f)		// タイトルの移動制限
#define DEAD_ZONE			(0.1f)			// スティックの遊び
#define UP_SPEED			(0.75f)			// 上がる速度
#define DOWN_SPEED			(1.15f)			// 下がる速度

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	LPD3DXMESH			pMesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER		pBuffMat;				// マテリアル情報へのポインタ
	DWORD				nNumMat;				// マテリアル情報の数
	char				aParts[MAX_TEXT];		// モデルファイル名
}File;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Player		*s_player;				// モデルの情報
static File			**s_file;				// ファイルの情報
static MOTION		s_IdxMotion;			// モーション番号
static int			s_nNumPlayer;			// プレイヤーの数
static int			s_nSelectPlayer;		// 選ばれているプレイヤー
static int			s_nSelectParts;			// 選ばれているパーツ
static int			s_nSelectMotion;		// 選ばれているモーション
static int			s_nFrame;				// フレーム数
static int			s_nIdxKey;				// キー番号
static bool			s_bMotionBlend;			// モーションブレンド
static bool			s_bMotionLoop;			// モーションループ
static float		s_fSlopeMove;			// 坂の移動量
static int			s_nEndTime;				// エンドの時間
static bool			s_bSoundRun;			// 走るサウンドを流したか
static bool			s_bSoundFall;			// 落ちるサウンドを流したか
static bool			s_bKeyBoardWASD;		// WASDのキーボード入力があるかどうか
static bool			s_bKeyBoardArrow;		// やじるしのキーボード入力があるかどうか
static bool			s_bJoyPad;				// ジョイパッド入力があるかどうか
static bool			s_bStickLeft;			// 左スティック入力があるかどうか
static bool			s_bStickRight;			// 右スティック入力があるかどうか
static bool			s_bRot;					// 向き false : 右 true : 左
static bool			s_bDraw;				// 描画するかどうか

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void System(void);
static void LoadNew(int nCnt);
static void LoadParts(int nCnt);
static void LoadMotion(int nCnt);
static void UpdateTitle(Player *pPlayer);
static void UpdateGame(Player *pPlayer);
static void TitleMove(Player *pPlayer);
static void Move(Player *pPlayer);
static void InputMove(void);
static void Rot(Player *pPlayer);
static void Motion(Player *pPlayer);
static void SetMotion(Player *pPlayer);
static void MotionBlend(Player *pPlayer);
static void MotionSlope(Player *pPlayer);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitPlayer(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	switch (GetMode())
	{
	case MODE_TITLE:		// タイトル

		s_nSelectPlayer = 0;

		break;

	case MODE_GAME:			// ゲーム

		switch (GetTitle())
		{// どのゲーム？
		case MENU_WALKING:		// ウォーキング
		case MENU_ROTATION:		// 回転
		case MENU_STOP:			// 止める

			s_nSelectPlayer = 0;

			break;

		case MENU_SLOPE:		// 坂

			s_nSelectPlayer = 1;

			break;

		default:
			assert(false);
			break;
		}

		break;

	default:
		assert(false);
		break;
	}

	s_nSelectParts = 0;
	s_nSelectMotion = 0;
	s_nFrame = 0;
	s_IdxMotion = MOTION_NEUTRAL;
	s_nIdxKey = 0;
	s_bMotionBlend = true;
	s_bMotionLoop = false;
	s_fSlopeMove = 0.0f;
	s_nEndTime = 0;
	s_bSoundRun = false;
	s_bSoundFall = false;
	s_bKeyBoardWASD = false;
	s_bKeyBoardArrow = false;
	s_bJoyPad = false;
	s_bStickLeft = false;
	s_bStickRight = false;
	s_bRot = true;
	s_bDraw = true;

	for (int i = 0; i < s_nNumPlayer; i++)
	{
		Player *pPlayer = &s_player[i];

		for (int j = 0; j < pPlayer->nNumParts; j++)
		{
			PlayerParts *pParts = &pPlayer->parts[j];

			// メッシュに使用されているテクスチャ用の配列を用意する
			pParts->pTexture = new LPDIRECT3DTEXTURE9[pParts->nNumMat];

			// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
			D3DXMATERIAL *pMat = (D3DXMATERIAL*)pParts->pBuffMat->GetBufferPointer();

			// 各メッシュのマテリアル情報を取得する
			for (int k = 0; k < (int)pParts->nNumMat; k++)
			{
				pParts->pTexture[k] = NULL;

				if (pMat[k].pTextureFilename != NULL)
				{// マテリアルで設定されているテクスチャ読み込み
					D3DXCreateTextureFromFileA(pDevice,
						pMat[k].pTextureFilename,
						&pParts->pTexture[k]);
				}
				else
				{// 使用されてない
					pParts->pTexture[k] = NULL;
				}
			}

			pPlayer->parts[j].posSet = pPlayer->parts[j].pos;
			pPlayer->parts[j].rotSet = pPlayer->parts[j].rot;
			pPlayer->parts[j].posOld = pPlayer->parts[j].pos;
			pPlayer->parts[j].rotOld = pPlayer->parts[j].rot;
		}

		pPlayer->posOld = pPlayer->pos;
		pPlayer->rotDest = pPlayer->rot;
		pPlayer->nStopTime = 0;

		pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	Player *pPlayer = &s_player[s_nSelectPlayer];

	D3DXVECTOR3 pos = D3DXVECTOR3(pPlayer->pos.x, 0.1f, pPlayer->pos.z);
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (s_nSelectPlayer)
	{
	case 0:		// てるてる
		pos.y = 0.1f;
		size = D3DXVECTOR3(pPlayer->fSize, 0.0f, pPlayer->fSize);
		break;

	case 1:		// 自転車
		pos.y = pPlayer->pos.y + 0.1f;
		size = D3DXVECTOR3(15.0f, 0.0f, 140.0f);
		break;

	default:
		assert(false);
		break;
	}

	// 影の設定
	pPlayer->nIdxShadow = SetShadow(pos, pPlayer->rot, size);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitPlayer(void)
{
	for (int i = 0; i < s_nNumPlayer; i++)
	{
		for (int j = 0; j < s_player[i].nNumParts; j++)
		{
			PlayerParts *pParts = &s_player[i].parts[j];

			if (pParts->pTexture != NULL)
			{// テクスチャの解放
				for (int k = 0; k < (int)pParts->nNumMat; k++)
				{
					if (pParts->pTexture[k] != NULL)
					{
						pParts->pTexture[k]->Release();
						pParts->pTexture[k] = NULL;
					}
				}

				delete[] pParts->pTexture;
				pParts->pTexture = NULL;
			}
		}
	}

	for (int i = 0; i < s_nNumPlayer; i++)
	{
		for (int j = 0; j < s_player[i].nNumParts; j++)
		{
			PlayerParts *pParts = &s_player[i].parts[j];

			if (pParts->pMesh != NULL)
			{// メッシュの解放
				pParts->pMesh = NULL;
			}

			if (pParts->pBuffMat != NULL)
			{// マテリアルの解放
				pParts->pBuffMat = NULL;
			}
		}

		for (int j = 0; j <  s_player[i].nNumName; j++)
		{
			if (s_file[i][j].pMesh != NULL)
			{// メッシュの解放
				s_file[i][j].pMesh->Release();
				s_file[i][j].pMesh = NULL;
			}

			if (s_file[i][j].pBuffMat != NULL)
			{// マテリアルの解放
				s_file[i][j].pBuffMat->Release();
				s_file[i][j].pBuffMat = NULL;
			}
		}

		// モデルの開放
		delete[] s_file[i];
		s_file[i] = NULL;
	}
	
	// モデルの開放
	delete[] s_file;
	s_file = NULL;

	for (int i = 0; i < s_nNumPlayer; i++)
	{// プレイヤー数
		for (int j = 0; j < s_player[i].nNumMotion; j++)
		{// モーション数
			for (int k = 0; k < s_player[i].Motion[j].nNumKey; k++)
			{// キーセット数
				delete[] s_player[i].Motion[j].keySet[k].key;
				s_player[i].Motion[j].keySet[k].key = NULL;
			}

			delete[] s_player[i].Motion[j].keySet;
			s_player[i].Motion[j].keySet = NULL;
		}

		delete[] s_player[i].Motion;
		s_player[i].Motion = NULL;
	}

	for (int i = 0; i < s_nNumPlayer; i++)
	{// パーツの開放
		delete[] s_player[i].parts;
		s_player[i].parts = NULL;
	}

	// プレイヤーの開放
	delete[] s_player;
	s_player = NULL; 
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdatePlayer(void)
{
	Player *pPlayer = &s_player[s_nSelectPlayer];

	switch (GetMode())
	{
	case MODE_TITLE:		// タイトル

		// タイトルの時の更新
		UpdateTitle(pPlayer);

		break;

	case MODE_GAME:			// ゲーム

		// ゲームの時の更新
		UpdateGame(pPlayer);

		break;

	default:
		assert(false);
		break;
	}

	D3DXVECTOR3 pos = D3DXVECTOR3(pPlayer->pos.x, 0.1f, pPlayer->pos.z);
	
	switch (s_nSelectPlayer)
	{
	case 0:		// てるてる
		pos.y = 0.1f;
		
		break;

	case 1:		// 自転車
		pos.y = pPlayer->pos.y + 0.1f;
		break;

	default:
		assert(false);
		break;
	}

	// 影の位置の設定
	SetPosShadow(pPlayer->nIdxShadow, pos, pPlayer->rot);
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawPlayer(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

	Player *pPlayer = &s_player[s_nSelectPlayer];

	if (s_bDraw)
	{// 描画する
		for (int j = 0; j < pPlayer->nNumParts; j++)
		{
			PlayerParts *pParts = &pPlayer->parts[j];

			// パーツのワールドマトリックスの初期化
			D3DXMatrixIdentity(&pParts->mtxWorld);

			// パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pParts->rot.y, pParts->rot.x, pParts->rot.z);
			D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxRot);

			// パーツの位置を反映
			D3DXMatrixTranslation(&mtxTrans, pParts->pos.x, pParts->pos.y, pParts->pos.z);
			D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxTrans);

			D3DXMATRIX mtxParent;

			if (pParts->nIdxParent == IDX_PARENT)
			{// 親
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&pPlayer->mtxWorld);

				// 向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, pPlayer->rot.y, pPlayer->rot.x, pPlayer->rot.z);
				D3DXMatrixMultiply(&pPlayer->mtxWorld, &pPlayer->mtxWorld, &mtxRot);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
				D3DXMatrixMultiply(&pPlayer->mtxWorld, &pPlayer->mtxWorld, &mtxTrans);

				mtxParent = pPlayer->mtxWorld;
			}
			else
			{// 子
				mtxParent = pPlayer->parts[pParts->nIdxParent].mtxWorld;
			}

			// 親モデルとのマトリックスの掛け算
			D3DXMatrixMultiply(&pParts->mtxWorld, &pParts->mtxWorld, &mtxParent);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pParts->mtxWorld);

			// 現在のマテリアル保持
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)pParts->pBuffMat->GetBufferPointer();

			for (int k = 0; k < (int)pParts->nNumMat; k++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[k].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, pParts->pTexture[k]);

				// パーツの描画
				pParts->pMesh->DrawSubset(k);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player[s_nSelectPlayer];
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadPlayer(void)
{
	// システム
	System();

	for (int i = 0; i < s_nNumPlayer; i++)
	{
		// new用の読み込み
		LoadNew(i);

		// パーツの読み込み
		LoadParts(i);

		// モーションの読み込み
		LoadMotion(i);
	}
}

//--------------------------------------------------
// 次のモーション
//--------------------------------------------------
void NextMotion(MOTION motion)
{
	s_nFrame = 0;
	s_nIdxKey = 0;

	// モーションセット
	SetMotion(&s_player[s_nSelectPlayer]);

	s_IdxMotion = motion;
}

//--------------------------------------------------
// 坂の設定
//--------------------------------------------------
void SetSlopePlayer(void)
{
	float fPosX = s_player[s_nSelectPlayer].pos.x * -0.02f;
	s_fSlopeMove = fPosX;
}

//--------------------------------------------------
// 描画するかの設定
//--------------------------------------------------
void SetDrawPlayer(bool bDraw)
{
	s_bDraw = bDraw;

	if (!bDraw)
	{
		// 影を使うのを止める
		UseStopShadow(s_player[s_nSelectPlayer].nIdxShadow);
	}
}

//--------------------------------------------------
// システム
//--------------------------------------------------
static void System(void)
{
	FILE *pFile;		// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};
		s_nNumPlayer = 0;

		while (strncmp(&aRead[0], "END_SCRIPT", 10) != 0)
		{// 終わりが来るまで繰り返す
			fgets(aRead, MAX_TEXT, pFile);

			if (strncmp(&aRead[0], "PLAYER_SET", 10) == 0)
			{// モデルの情報
				s_nNumPlayer++;
			}
		}

		// ファイルを閉じる
		fclose(pFile);

		// txtに書いてる最大数分のプレイヤーの配列を用意する
		s_player = new Player[s_nNumPlayer];
		s_file = new File*[s_nNumPlayer];
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}

	// ファイルを開く
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};
		int nFileName = 0, nPlayer = 0;

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

			if (strcmp(&aRead[0], "PLAYER_SET") == 0)
			{// モデルの情報
				while (strcmp(&aRead[0], "END_PLAYER_SET") != 0)
				{// 終わりが来るまで繰り返す
					fscanf(pFile, "%s", &aRead);

					if (strncmp(&aRead[0], "#", 1) == 0)
					{// コメント
						fgets(aRead, MAX_TEXT, pFile);
						continue;
					}

					if (strcmp(&aRead[0], "PLAYER_FILENAME") == 0)
					{// プレイヤーのファイル名
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%s", s_player[nFileName].aText);
						nFileName++;
					}
					else if (strcmp(&aRead[0], "POS") == 0)
					{// 位置
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &s_player[nPlayer].pos.x);
						fscanf(pFile, "%f", &s_player[nPlayer].pos.y);
						fscanf(pFile, "%f", &s_player[nPlayer].pos.z);
					}
					else if (strcmp(&aRead[0], "ROT") == 0)
					{// 向き
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &s_player[nPlayer].rot.x);
						fscanf(pFile, "%f", &s_player[nPlayer].rot.y);
						fscanf(pFile, "%f", &s_player[nPlayer].rot.z);
					}
				}
				nPlayer++;
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
static void LoadNew(int nCnt)
{
	FILE *pFile;		// ファイルポインタを宣言

	Player *pPlayer = &s_player[nCnt];

	// ファイルを開く
	pFile = fopen(pPlayer->aText, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};
		pPlayer->nNumMotion = 0;
		pPlayer->nNumName = 0;

		while (strncmp(&aRead[0], "END_SCRIPT", 10) != 0)
		{// 終わりが来るまで繰り返す
			fgets(aRead, MAX_TEXT, pFile);

			if (strncmp(&aRead[0], "MOTIONSET", 9) == 0)
			{// モーションの情報
				pPlayer->nNumMotion++;
			}
			else if (strncmp(&aRead[0], "MODEL_FILENAME", 14) == 0)
			{// モーションの情報
				pPlayer->nNumName++;
			}
		}

		// ファイルを閉じる
		fclose(pFile);

		// txtに書いてる最大数分のモーションの配列を用意する
		pPlayer->Motion = new MotionSet[pPlayer->nNumMotion];

		s_file[nCnt] = new File[pPlayer->nNumName];
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//--------------------------------------------------
// パーツの読み込み
//--------------------------------------------------
static void LoadParts(int nCnt)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE *pFile;		// ファイルポインタを宣言

	Player *pPlayer = &s_player[nCnt];

	// ファイルを開く
	pFile = fopen(pPlayer->aText, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};
		int nParts = 0, nFileName = 0, nNumModel = 0;

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

			if (strcmp(&aRead[0], "NUM_MODEL") == 0)
			{// モデルの使用数
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%d", &nNumModel);
			}
			else if (strcmp(&aRead[0], "MODEL_FILENAME") == 0)
			{// モデルファイル名
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", &s_file[nCnt][nFileName].aParts);

				nFileName++;
			}
			else if (strcmp(&aRead[0], "CHARACTERSET") == 0)
			{// キャラクターの情報
				nParts = 0;

				while (strcmp(&aRead[0], "END_CHARACTERSET") != 0)
				{// 終わりが来るまで繰り返す
					fscanf(pFile, "%s", &aRead);

					if (strncmp(&aRead[0], "#", 1) == 0)
					{// コメント
						fgets(aRead, MAX_TEXT, pFile);
						continue;
					}

					PlayerParts *pParts = &pPlayer->parts[nParts];

					if (strcmp(&aRead[0], "NUM_PARTS") == 0)
					{// パーツの使用数
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pPlayer->nNumParts);

						// txtに書いてる最大数分の配列を用意する
						pPlayer->parts = new PlayerParts[pPlayer->nNumParts];
					}
					else if (strcmp(&aRead[0], "MOVE") == 0)
					{// 移動量
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pPlayer->fMove);
					}
					else if (strcmp(&aRead[0], "JUMP") == 0)
					{// ジャンプ量
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pPlayer->fJump);
					}
					else if (strcmp(&aRead[0], "RADIUS") == 0)
					{// サイズ
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pPlayer->fSize);
					}
					else if (strcmp(&aRead[0], "HEIGHT") == 0)
					{// 高さ
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%f", &pPlayer->fHeight);
					}
					else if (strcmp(&aRead[0], "PARTSSET") == 0)
					{// キャラクターの情報
						while (strcmp(&aRead[0], "END_PARTSSET") != 0)
						{// 終わりが来るまで繰り返す
							fscanf(pFile, "%s", &aRead);

							if (strncmp(&aRead[0], "#", 1) == 0)
							{// コメント
								fgets(aRead, MAX_TEXT, pFile);
								continue;
							}

							if (strcmp(&aRead[0], "INDEX") == 0)
							{// 使用するモデルの番号
								fscanf(pFile, "%s", &aRead);
								fscanf(pFile, "%d", &pParts->nIdxModel);
							}
							else if (strcmp(&aRead[0], "PARENT") == 0)
							{// 親の番号
								fscanf(pFile, "%s", &aRead);
								fscanf(pFile, "%d", &pParts->nIdxParent);
							}
							else if (strcmp(&aRead[0], "POS") == 0)
							{// 位置
								fscanf(pFile, "%s", &aRead);
								fscanf(pFile, "%f", &pParts->pos.x);
								fscanf(pFile, "%f", &pParts->pos.y);
								fscanf(pFile, "%f", &pParts->pos.z);
							}
							else if (strcmp(&aRead[0], "ROT") == 0)
							{// 向き
								fscanf(pFile, "%s", &aRead);
								fscanf(pFile, "%f", &pParts->rot.x);
								fscanf(pFile, "%f", &pParts->rot.y);
								fscanf(pFile, "%f", &pParts->rot.z);

								pParts->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}
						}
						nParts++;
					}
				}
			}
		}

		// ファイルを閉じる
		fclose(pFile);

		for (int j = 0; j < pPlayer->nNumName; j++)
		{// Xファイルの読み込み
			D3DXLoadMeshFromX(
				s_file[nCnt][j].aParts,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&s_file[nCnt][j].pBuffMat,
				NULL,
				&s_file[nCnt][j].nNumMat,
				&s_file[nCnt][j].pMesh);
		}

		for (int j = 0; j < pPlayer->nNumParts; j++)
		{// 使用するモデルの情報を取得
			PlayerParts *Parts = &pPlayer->parts[j];

			Parts->pBuffMat = s_file[nCnt][Parts->nIdxModel].pBuffMat;
			Parts->nNumMat = s_file[nCnt][Parts->nIdxModel].nNumMat;
			Parts->pMesh = s_file[nCnt][Parts->nIdxModel].pMesh;
		}
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//--------------------------------------------------
// モーションの読み込み
//--------------------------------------------------
static void LoadMotion(int nCnt)
{
	FILE *pFile;		// ファイルポインタを宣言

	Player *pPlayer = &s_player[nCnt];

	// ファイルを開く
	pFile = fopen(pPlayer->aText, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};
		int nNumMotion = 0;

		while (strcmp(&aRead[0], "END_CHARACTERSET") != 0)
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

			if (strcmp(&aRead[0], "MOTIONSET") == 0)
			{// モーションの情報
				int nNumKeySet = 0;

				while (strcmp(&aRead[0], "END_MOTIONSET") != 0)
				{// 終わりが来るまで繰り返す
					fscanf(pFile, "%s", &aRead);

					if (strncmp(&aRead[0], "#", 1) == 0)
					{// コメント
						fgets(aRead, MAX_TEXT, pFile);
						continue;
					}

					MotionSet *pMotion = &pPlayer->Motion[nNumMotion];

					if (strcmp(&aRead[0], "LOOP") == 0)
					{// ループ
						int nLoop = 0;
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &nLoop);

						if (nLoop == 0)
						{// ループしない
							pMotion->bLoop = false;
						}
						else
						{// ループする
							pMotion->bLoop = true;
						}
					}
					else if (strcmp(&aRead[0], "NUM_KEY") == 0)
					{// キー数
						fscanf(pFile, "%s", &aRead);
						fscanf(pFile, "%d", &pMotion->nNumKey);

						// txtに書いてる最大数分のキー設定の配列を用意する
						pMotion->keySet = new KeySet[pMotion->nNumKey];

					}
					else if (strcmp(&aRead[0], "KEYSET") == 0)
					{// キー設定
						int nNumKey = 0;

						while (strcmp(&aRead[0], "END_KEYSET") != 0)
						{// 終わりが来るまで繰り返す
							fscanf(pFile, "%s", &aRead);

							if (strncmp(&aRead[0], "#", 1) == 0)
							{// コメント
								fgets(aRead, MAX_TEXT, pFile);
								continue;
							}

							KeySet *pKeySet = &pMotion->keySet[nNumKeySet];

							if (strcmp(&aRead[0], "FRAME") == 0)
							{// フレーム数
								fscanf(pFile, "%s", &aRead);
								fscanf(pFile, "%d", &pKeySet->nFrame);

								// txtに書いてる最大数分のキーの配列を用意する
								pKeySet->key = new Key[pPlayer->nNumParts];
							}
							else if (strcmp(&aRead[0], "KEY") == 0)
							{// キー
								while (strcmp(&aRead[0], "END_KEY") != 0)
								{// 終わりが来るまで繰り返す
									fscanf(pFile, "%s", &aRead);

									if (strncmp(&aRead[0], "#", 1) == 0)
									{// コメント
										fgets(aRead, MAX_TEXT, pFile);
										continue;
									}

									Key *pKey = &pKeySet->key[nNumKey];

									if (strcmp(&aRead[0], "POS") == 0)
									{// 位置
										fscanf(pFile, "%s", &aRead);
										fscanf(pFile, "%f", &pKey->pos.x);
										fscanf(pFile, "%f", &pKey->pos.y);
										fscanf(pFile, "%f", &pKey->pos.z);
									}
									else if (strcmp(&aRead[0], "ROT") == 0)
									{// 向き
										fscanf(pFile, "%s", &aRead);
										fscanf(pFile, "%f", &pKey->rot.x);
										fscanf(pFile, "%f", &pKey->rot.y);
										fscanf(pFile, "%f", &pKey->rot.z);
									}
								}
								nNumKey++;
							}
						}
						nNumKeySet++;
					}
				}
				nNumMotion++;
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
// タイトルの時の更新
//--------------------------------------------------
static void UpdateTitle(Player *pPlayer)
{
	if (GetFade() == FADE_NONE)
	{
		// 移動
		TitleMove(pPlayer);

		// 回転
		Rot(pPlayer);
	}

	// モーション
	Motion(pPlayer);

	// 枠の当たり判定
	CollisionFrame(&pPlayer->pos);
}

//--------------------------------------------------
// ゲームの時の更新
//--------------------------------------------------
static void UpdateGame(Player *pPlayer)
{
	pPlayer->nStopTime++;

	// 前回の位置を保存
	pPlayer->posOld = pPlayer->pos;

	switch (GetTitle())
	{// 移動系
	case MENU_WALKING:		// ウォーキング

		switch (GetGame())
		{
		case GAMESTATE_NONE:			// 何もしていない状態
		case GAMESTATE_START:			// 開始状態 (ゲーム開始前)
		case GAMESTATE_COUNTDOWN:		// カウントダウン状態 (ゲーム開始中)
		case GAMESTATE_RESULT:			// リザルト状態 (ゲーム終了後)
		case GAMESTATE_LOOP:			// 繰り返し状態 (リザルト終了後)

			/* 処理なし */

			break;

		case GAMESTATE_NORMAL:		// 通常状態 (ゲーム進行中)

			// 移動
			Move(pPlayer);

			break;

		case GAMESTATE_END:			// 終了状態 (ゲーム終了時)

			// 向きの初期化
			pPlayer->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;

		default:
			assert(false);
			break;
		}

		break;

	case MENU_SLOPE:		// 坂

		// 重力を加算
		pPlayer->pos.y += -10.8f;

		switch (GetGame())
		{
		case GAMESTATE_NONE:			// 何もしていない状態
		case GAMESTATE_START:			// 開始状態 (ゲーム開始前)
		case GAMESTATE_COUNTDOWN:		// カウントダウン状態 (ゲーム開始中)
		case GAMESTATE_RESULT:			// リザルト状態 (ゲーム終了後)
		case GAMESTATE_LOOP:			// 繰り返し状態 (リザルト終了後)

			/* 処理なし */

			break;

		case GAMESTATE_NORMAL:		// 通常状態 (ゲーム進行中)

			// 移動
			Move(pPlayer);

			break;

		case GAMESTATE_END:			// 終了状態 (ゲーム終了時)

			s_nEndTime++;

			if (s_fSlopeMove != 0.0f)
			{
				pPlayer->move.x = 0.0f;

				if (s_nEndTime >= 120)
				{
					if (!s_bSoundRun)
					{
						s_bSoundRun = true;

						// サウンドの再生
						PlaySound(SOUND_LABEL_SE_RUN);
					}

					pPlayer->pos.x += s_fSlopeMove;

					pPlayer->move.x = s_fSlopeMove;
				}
			}
			else
			{
				if (s_nEndTime >= 120)
				{
					if (CollisionField(&pPlayer->pos, &pPlayer->posOld))
					{
						// リザルトの設定
						SetResult(RESULT_CLEAR);
					}
					else
					{
						// リザルトの設定
						SetResult(RESULT_GAMEOVER);
					}

					pPlayer->pos.y += -10.8f;

					// リザルトの初期化
					InitResult();

					// ゲームの設定
					SetGameState(GAMESTATE_RESULT);
				}
			}
			break;

		default:
			assert(false);
			break;
		}

		break;

	case MENU_ROTATION:		// 回転
	case MENU_STOP:			// 止める

		/* 移動しない */

		break;

	default:
		assert(false);
		break;
	}

	// 回転
	Rot(pPlayer);

	D3DXVECTOR3 size = D3DXVECTOR3(pPlayer->fSize, pPlayer->fHeight, pPlayer->fSize);

	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
	case MENU_ROTATION:		// 回転
	case MENU_STOP:			// 止める

		/* 処理なし */

		break;

	case MENU_SLOPE:		// 坂

		if (!CollisionField(&pPlayer->pos, &pPlayer->posOld))
		{// フィールドとの当たり判定
			if (!CollisionMeshField(&pPlayer->pos))
			{
				if (GetGame() == GAMESTATE_END ||
					GetGame() == GAMESTATE_RESULT)
				{// 落下の回転
					pPlayer->rotDest.x += -D3DX_PI * 0.25f;
					pPlayer->rot.x += -D3DX_PI * 0.25f;
					pPlayer->rotDest.y += -D3DX_PI * 0.25f;
					pPlayer->rot.y += -D3DX_PI * 0.25f;

					// 影を使うのを止める
					UseStopShadow(pPlayer->nIdxShadow);

					if (!s_bSoundFall)
					{
						s_bSoundFall = true;

						// サウンドの再生
						PlaySound(SOUND_LABEL_SE_FALL);
					}
				}
			}
		}

		break;

	default:
		assert(false);
		break;
	}

	if (GetTitle() == MENU_SLOPE)
	{
		// メッシュフィールドとの当たり判定
		if (CollisionMeshField(&pPlayer->pos))
		{// 当たってる
			if (GetGame() != GAMESTATE_END)
			{
				if (pPlayer->rot.y >= 0.0f)
				{// 左向き
					pPlayer->rotDest.x = D3DX_PI * 0.1f;
					pPlayer->rot.x = D3DX_PI * 0.1f;
				}
				else if (pPlayer->rot.y <= 0.0f)
				{// 右向き
					pPlayer->rotDest.x = -D3DX_PI * 0.1f;
					pPlayer->rot.x = -D3DX_PI * 0.1f;
				}
			}
			else if (GetGame() == GAMESTATE_END)
			{
				pPlayer->rotDest.y = -D3DX_PI * 0.5f;
				pPlayer->rotDest.x = -D3DX_PI * 0.1f;
				pPlayer->rot.x = -D3DX_PI * 0.1f;

				if (s_nEndTime >= 120)
				{
					s_fSlopeMove += 0.1f;
				}
			}
		}
		else
		{// 当たってない
			pPlayer->rotDest.x = 0.0f;

			if (s_nEndTime >= 120)
			{
				float fField = GetField()->pos.x + GetField()->vtxMax.x;

				if (pPlayer->pos.x + SLOPE_LIMIT <= fField)
				{
					s_fSlopeMove -= 0.1f;

					if (s_fSlopeMove <= 0.0f)
					{
						s_fSlopeMove = 0.0f;

						s_nEndTime = 0;
					}
				}
				else
				{
					if (pPlayer->pos.y < -SLOPE_RESULT && pPlayer->posOld.y >= -SLOPE_RESULT)
					{
						// リザルトの設定
						SetResult(RESULT_GAMEOVER);

						// リザルトの初期化
						InitResult();

						// ゲームの設定
						SetGameState(GAMESTATE_RESULT);
					}
				}
			}
		}
	}

	switch (GetTitle())
	{// どのゲーム？
	case MENU_WALKING:		// ウォーキング

		// モーション
		Motion(pPlayer);

		break;

	case MENU_ROTATION:		// 回転
	case MENU_STOP:			// 止める

		if (s_bMotionBlend)
		{// モーションブレンド中
			s_nFrame++;

			// モーションブレンド
			MotionBlend(pPlayer);
		}

		break;

	case MENU_SLOPE:		// 坂

		// 坂のモーション
		MotionSlope(pPlayer);

		if (s_bMotionBlend)
		{// モーションブレンド中
			s_nFrame++;

			// モーションブレンド
			MotionBlend(pPlayer);
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// タイトルの時の移動
//--------------------------------------------------
static void TitleMove(Player *pPlayer)
{
	// 移動の入力
	InputMove();

	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	/* ↓モデルの移動↓ */

	if (s_bKeyBoardWASD)
	{// キーボード
		if (GetKeyboardPress(DIK_A))
		{// キーが押された
			vec.x -= 1.0f;
		}
		if (GetKeyboardPress(DIK_D))
		{// キーが押された
			vec.x += 1.0f;
		}
		if (GetKeyboardPress(DIK_W))
		{// キーが押された
			vec.z += 1.0f;
		}
		if (GetKeyboardPress(DIK_S))
		{// キーが押された
			vec.z -= 1.0f;
		}

		// ベクトルの正規化
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bKeyBoardArrow)
	{// キーボード
		if (GetKeyboardPress(DIK_LEFT))
		{// キーが押された
			vec.x -= 1.0f;
		}
		if (GetKeyboardPress(DIK_RIGHT))
		{// キーが押された
			vec.x += 1.0f;
		}
		if (GetKeyboardPress(DIK_UP))
		{// キーが押された
			vec.z += 1.0f;
		}
		if (GetKeyboardPress(DIK_DOWN))
		{// キーが押された
			vec.z -= 1.0f;
		}

		// ベクトルの正規化
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bJoyPad)
	{// ジョイパッド
		if (GetJoypadPress(JOYKEY_LEFT))
		{// ボタンが押された
			vec.x -= 1.0f;
		}
		if (GetJoypadPress(JOYKEY_RIGHT))
		{// ボタンが押された
			vec.x += 1.0f;
		}
		else if (GetJoypadPress(JOYKEY_UP))
		{// ボタンが押された
			vec.z += 1.0f;
		}
		else if (GetJoypadPress(JOYKEY_DOWN))
		{// ボタンが押された
			vec.z -= 1.0f;
		}

		// ベクトルの正規化
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bStickLeft)
	{// 左スティック
		D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		stick.x = GetJoypadStick(JOYKEY_LEFT_STICK, 0).x;
		stick.z = -GetJoypadStick(JOYKEY_LEFT_STICK, 0).y;

		// ベクトルの正規化
		D3DXVec3Normalize(&stick, &stick);

		pPlayer->rotDest.y = atan2f(stick.x, stick.z) + D3DX_PI;
		pPlayer->move += stick * pPlayer->fMove;
	}
	else if (s_bStickRight)
	{// 右スティック
		D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		stick.x = GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x;
		stick.z = -GetJoypadStick(JOYKEY_RIGHT_STICK, 0).y;

		// ベクトルの正規化
		D3DXVec3Normalize(&stick, &stick);

		pPlayer->rotDest.y = atan2f(stick.x, stick.z) + D3DX_PI;
		pPlayer->move += stick * pPlayer->fMove;
	}

	// 移動
	pPlayer->pos.x += pPlayer->move.x;
	pPlayer->pos.z += pPlayer->move.z;

	// 慣性・移動量を更新 (減衰させる)
	pPlayer->move.x += (0.0f - pPlayer->move.x) * MAX_INERTIA;
	pPlayer->move.z += (0.0f - pPlayer->move.z) * MAX_INERTIA;

	if (pPlayer->pos.x <= -TITLE_WIDTH)
	{// 移動制限
		pPlayer->pos.x = -TITLE_WIDTH;
	}
	else if (pPlayer->pos.x >= TITLE_WIDTH)
	{// 移動制限
		pPlayer->pos.x = TITLE_WIDTH;
	}

	if (pPlayer->pos.z <= -TITLE_DEPTH)
	{// 移動制限
		pPlayer->pos.z = -TITLE_DEPTH;
	}
	else if (pPlayer->pos.z >= TITLE_DEPTH)
	{// 移動制限
		pPlayer->pos.z = TITLE_DEPTH;
	}
}

//--------------------------------------------------
// 移動
//--------------------------------------------------
static void Move(Player *pPlayer)
{
	// 移動の入力
	InputMove();

	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	/* ↓モデルの移動↓ */

	if (s_bKeyBoardWASD)
	{// キーボード
		if (GetKeyboardPress(DIK_A))
		{// キーが押された
			vec.x -= 1.0f;
			s_bRot = true;
		}
		if (GetKeyboardPress(DIK_D))
		{// キーが押された
			vec.x += 1.0f;
			s_bRot = false;
		}

		// ベクトルの正規化
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bKeyBoardArrow)
	{// キーボード
		if (GetKeyboardPress(DIK_LEFT))
		{// キーが押された
			vec.x -= 1.0f;
			s_bRot = true;
		}
		if (GetKeyboardPress(DIK_RIGHT))
		{// キーが押された
			vec.x += 1.0f;
			s_bRot = false;
		}

		// ベクトルの正規化
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bJoyPad)
	{// ジョイパッド
		if (GetJoypadPress(JOYKEY_LEFT))
		{// ボタンが押された
			vec.x -= 1.0f;

			s_bRot = true;
		}
		if (GetJoypadPress(JOYKEY_RIGHT))
		{// ボタンが押された
			vec.x += 1.0f;

			s_bRot = false;
		}

		// ベクトルの正規化
		D3DXVec3Normalize(&vec, &vec);

		pPlayer->rotDest.y = atan2f(vec.x, vec.z) + D3DX_PI;
		pPlayer->move += vec * pPlayer->fMove;
	}
	else if (s_bStickLeft)
	{// スティック
		D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		stick.x = GetJoypadStick(JOYKEY_LEFT_STICK, 0).x;

		if (stick.x < 0.0f)
		{// 左
			s_bRot = true;
		}
		else
		{// 右
			s_bRot = false;
		}

		// ベクトルの正規化
		D3DXVec3Normalize(&stick, &stick);

		pPlayer->rotDest.y = atan2f(stick.x, stick.z) + D3DX_PI;
		pPlayer->move += stick * pPlayer->fMove;
	}
	else if (s_bStickRight)
	{// スティック
		D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		stick.x = GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x;

		if (stick.x < 0.0f)
		{// 左
			s_bRot = true;
		}
		else
		{// 右
			s_bRot = false;
		}

		// ベクトルの正規化
		D3DXVec3Normalize(&stick, &stick);

		pPlayer->rotDest.y = atan2f(stick.x, stick.z) + D3DX_PI;
		pPlayer->move += stick * pPlayer->fMove;
	}

	if (GetTitle() == MENU_SLOPE)
	{// 坂の時
		if (s_bRot)
		{// 左
			pPlayer->move.x *= UP_SPEED;
		}
		else
		{// 右
			pPlayer->move.x *= DOWN_SPEED;
		}
	}

	// 移動
	pPlayer->pos.x += pPlayer->move.x;

	// 慣性・移動量を更新 (減衰させる)
	pPlayer->move.x += (0.0f - pPlayer->move.x) * MAX_INERTIA;

	if (GetTitle() == MENU_SLOPE)
	{// 坂
		float fWidth = GetMeshField()->fWidth;

		if (pPlayer->pos.x >= -SLOPE_LIMIT)
		{// 移動制限
			pPlayer->pos.x = -SLOPE_LIMIT;
		}
		else if (pPlayer->pos.x <= fWidth + SLOPE_LIMIT)
		{// 移動制限
			pPlayer->pos.x = fWidth + SLOPE_LIMIT;
		}
	}
}

//--------------------------------------------------
// 移動の入力
//--------------------------------------------------
static void InputMove(void)
{
	s_bKeyBoardWASD = false;
	s_bKeyBoardArrow = false;
	s_bJoyPad = false;
	s_bStickLeft = false;
	s_bStickRight = false;

	switch (GetMode())
	{
	case MODE_TITLE:		// タイトル

		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D) ||
			GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S))
		{// キーが押された
			s_bKeyBoardWASD = true;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// キーが押された
			s_bKeyBoardArrow = true;
		}

		if (GetJoypadPress(JOYKEY_LEFT) || GetJoypadPress(JOYKEY_RIGHT) ||
			GetJoypadPress(JOYKEY_UP) || GetJoypadPress(JOYKEY_DOWN))
		{// ボタンが押された
			s_bJoyPad = true;
		}

		if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, 0).y > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_LEFT_STICK, 0).y < -DEAD_ZONE)
		{// 左スティックが傾いた
			s_bStickLeft = true;
		}

		if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x < -DEAD_ZONE ||
			GetJoypadStick(JOYKEY_RIGHT_STICK, 0).y > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_RIGHT_STICK, 0).y < -DEAD_ZONE)
		{// 右スティックが傾いた
			s_bStickRight = true;
		}
		
		break;

	case MODE_GAME:			// ゲーム

		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
		{// キーが押された
			s_bKeyBoardWASD = true;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT))
		{// キーが押された
			s_bKeyBoardArrow = true;
		}

		if (GetJoypadPress(JOYKEY_LEFT) || GetJoypadPress(JOYKEY_RIGHT))
		{// ボタンが押された
			s_bJoyPad = true;
		}

		if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x > DEAD_ZONE || 
			GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -DEAD_ZONE)
		{// 左スティックが傾いた
			s_bStickLeft = true;
		}

		if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x > DEAD_ZONE ||
			GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x < -DEAD_ZONE)
		{// 右スティックが傾いた
			s_bStickRight = true;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 回転
//--------------------------------------------------
static void Rot(Player *pPlayer)
{
	D3DXVECTOR3 angle;

	// 角度の正規化
	NormalizeRot(&pPlayer->rotDest.x);
	NormalizeRot(&pPlayer->rotDest.y);

	angle.x = pPlayer->rotDest.x - pPlayer->rot.x;
	angle.y = pPlayer->rotDest.y - pPlayer->rot.y;

	// 角度の正規化
	NormalizeRot(&angle.x);
	NormalizeRot(&angle.y);

	//慣性・向きを更新 (減衰させる)
	pPlayer->rot.x += angle.x * MAX_ATTENUATION;
	pPlayer->rot.y += angle.y * MAX_ATTENUATION;

	// 角度の正規化
	NormalizeRot(&pPlayer->rot.x);
	NormalizeRot(&pPlayer->rot.y);
}

//--------------------------------------------------
// モーション
//--------------------------------------------------
static void Motion(Player * pPlayer)
{
	s_nFrame++;

	switch (GetMode())
	{
	case MODE_TITLE:		// タイトル

		if (s_IdxMotion != MOTION_MOVE)
		{
			if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f ||
				pPlayer->move.z >= 0.1f || pPlayer->move.z <= -0.1f)
			{// ←, →, ↑, ↓キーが押された
				// 次のモーション
				NextMotion(MOTION_MOVE);
			}
		}

		break;

	case MODE_GAME:			// ゲーム

		if (GetGame() == GAMESTATE_NORMAL)
		{// 通常状態 (ゲーム進行中)
			switch (GetTitle())
			{// どのゲーム？
			case MENU_WALKING:		// ウォーキング
			case MENU_SLOPE:		// 坂

				if (s_IdxMotion != MOTION_MOVE)
				{
					if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f)
					{// ←, →, ↑, ↓キーが押された
					 // 次のモーション
						NextMotion(MOTION_MOVE);
					}
				}

				break;

			case MENU_ROTATION:		// 回転
			case MENU_STOP:			// 止める

				/* 処理なし */

				break;

			default:
				assert(false);
				break;
			}
		}

		break;

	default:
		assert(false);
		break;
	}

	if (s_bMotionBlend)
	{// モーションブレンド中
		MotionBlend(pPlayer);
	}
	else
	{
		if (s_IdxMotion == MOTION_MOVE)
		{
			s_bMotionLoop = true;
		}
		else if (s_IdxMotion == MOTION_ATTACK)
		{
			s_bMotionLoop = false;
		}

		switch (GetMode())
		{
		case MODE_TITLE:		// タイトル

			if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f ||
				pPlayer->move.z >= 0.1f || pPlayer->move.z <= -0.1f)
			{// ←, →, ↑, ↓キーが押された
				s_bMotionLoop = false;
			}

			break;

		case MODE_GAME:			// ゲーム

			if (GetGame() == GAMESTATE_NORMAL)
			{// 通常状態 (ゲーム進行中)
				if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f)
				{// ←, →, ↑, ↓キーが押された
					s_bMotionLoop = false;
				}
			}

			break;

		default:
			assert(false);
			break;
		}

		if (s_bMotionLoop)
		{// モーションループする
			// 次のモーション
			NextMotion(MOTION_NEUTRAL);

			s_bMotionLoop = false;
		}

		MotionSet *pMotion = &pPlayer->Motion[s_IdxMotion];

		if (s_nFrame >= pMotion->keySet[s_nIdxKey].nFrame)
		{// フレーム数が超えた
			s_nIdxKey++;

			if (pMotion->bLoop)
			{// ループする
				s_nIdxKey %= pMotion->nNumKey;
			}
			else
			{// ループしない
				if (s_nIdxKey >= pMotion->nNumKey)
				{// キー数が超えた
					// 次のモーション
					NextMotion(MOTION_NEUTRAL);
				}
			}

			s_nFrame = 0;
		}

		for (int i = 0; i < pPlayer->nNumParts; i++)
		{
			int nNext = (s_nIdxKey + 1) % pMotion->nNumKey;

			D3DXVECTOR3 pos = pMotion->keySet[nNext].key[i].pos - pMotion->keySet[s_nIdxKey].key[i].pos;
			D3DXVECTOR3 rot = pMotion->keySet[nNext].key[i].rot - pMotion->keySet[s_nIdxKey].key[i].rot;

			pos /= (float)pMotion->keySet[s_nIdxKey].nFrame;
			rot /= (float)pMotion->keySet[s_nIdxKey].nFrame;

			pPlayer->parts[i].pos += pos;
			pPlayer->parts[i].rot += rot;
		}
	}
}

//--------------------------------------------------
// モーションセット
//--------------------------------------------------
static void SetMotion(Player * pPlayer)
{
	for (int i = 0; i < pPlayer->nNumParts; i++)
	{
		pPlayer->parts[i].posOld = pPlayer->parts[i].pos;
		pPlayer->parts[i].rotOld = pPlayer->parts[i].rot;
	}
	
	s_bMotionBlend = true;
}

//--------------------------------------------------
// モーションブレンド
//--------------------------------------------------
static void MotionBlend(Player *pPlayer)
{
	MotionSet *pMotion = &pPlayer->Motion[s_IdxMotion];

	for (int i = 0; i < pPlayer->nNumParts; i++)
	{
		D3DXVECTOR3 posNew = pPlayer->parts[i].posSet + pMotion->keySet[s_nIdxKey].key[i].pos;
		D3DXVECTOR3 rotNew = pPlayer->parts[i].rotSet + pMotion->keySet[s_nIdxKey].key[i].rot;

		D3DXVECTOR3 pos = posNew - pPlayer->parts[i].posOld;
		D3DXVECTOR3 rot = rotNew - pPlayer->parts[i].rotOld;

		pos /= MAX_BLEND;
		rot /= MAX_BLEND;

		pPlayer->parts[i].pos += pos;
		pPlayer->parts[i].rot += rot;
	}

	if (s_nFrame >= MAX_BLEND)
	{// フレーム数が超えた
		s_bMotionBlend = false;
		s_nFrame = 0;
	}
}

//--------------------------------------------------
// 坂のモーション
//--------------------------------------------------
static void MotionSlope(Player *pPlayer)
{
	float fRot = 0.025f;

	switch (GetGame())
	{
	case GAMESTATE_NONE:			// 何もしていない状態
	case GAMESTATE_START:			// 開始状態 (ゲーム開始前)
	case GAMESTATE_COUNTDOWN:		// カウントダウン状態 (ゲーム開始中)
	case GAMESTATE_RESULT:			// リザルト状態 (ゲーム終了後)
	case GAMESTATE_LOOP:			// 繰り返し状態 (リザルト終了後)

		/* 処理なし */

		break;

	case GAMESTATE_NORMAL:		// 通常状態 (ゲーム進行中)

		if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f)
		{// 移動している

			if (GetTitle() == MENU_SLOPE)
			{// 坂の時
				if (s_bRot)
				{// 左
					fRot *= UP_SPEED;
				}
				else
				{// 右
					fRot *= DOWN_SPEED;
				}
			}

			pPlayer->parts[1].rot.x += -D3DX_PI * fRot;
			pPlayer->parts[2].rot.x += -D3DX_PI * fRot;
		}

		break;

	case GAMESTATE_END:			// 終了状態 (ゲーム終了時)

		if (pPlayer->move.x >= 0.1f || pPlayer->move.x <= -0.1f)
		{// 移動している
			pPlayer->parts[1].rot.x += -D3DX_PI * (s_fSlopeMove * 0.005f);
			pPlayer->parts[2].rot.x += -D3DX_PI * (s_fSlopeMove * 0.005f);
		}

		break;

	default:
		assert(false);
		break;
	}
}

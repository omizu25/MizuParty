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
#include "model.h"
#include "billboard.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "setup.h"
#include "shadow.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_ROTATION		(0.035f)		// 回転の最大値
#define MAX_ATTENUATION		(0.1f)			// 減衰係数の最大値
#define MAX_HEIGHT			(80.0f)			// 高さの最大値
#define MIN_HEIGHT			(-80.0f)		// 高さの最小値
#define IDX_PARENT			(-1)			// 親の番号
#define MAX_BLEND			(30)			// ブレンドの最大値

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	LPD3DXMESH			pMesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER		pBuffMat;				// マテリアル情報へのポインタ
	DWORD				nNumMat;				// マテリアル情報の数
	char				aParts[MAX_TEXT];		// モデルファイル名
}ModelFile;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Player		*s_player;				// モデルの情報
static int			s_nNumPlayer;			// プレイヤーの数
static int			s_nSelectPlayer;		// 選ばれているプレイヤー
static int			s_nSelectParts;			// 選ばれているパーツ
static int			s_nSelectMotion;		// 選ばれているモーション
static int			s_nFrame;				// フレーム数
static int			s_nIdxMotion;			// モーション番号
static int			s_nIdxKey;				// キー番号
static bool			s_bMotionBlend;			// モーションブレンド
static bool			s_bMotionLoop;			// モーションループ

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void System(HWND hWnd);
static void LoadParts(HWND hWnd, Player *pPlayer);
static void LoadMotion(HWND hWnd, Player *pPlayer);
static void FollowMove(Player *pPlayer);
static void Move(Player *pPlayer);
static void Rot(Player *pPlayer);
static void Motion(Player *pPlayer);
static void SetMotion(Player *pPlayer);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitPlayer(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nSelectPlayer = 0;
	s_nSelectParts = 0;
	s_nSelectMotion = 0;
	s_nFrame = 0;
	s_nIdxMotion = 0;
	s_nIdxKey = 0;
	s_bMotionBlend = true;
	s_bMotionLoop = false;

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

			//pParts->pos += pPlayer->Motion[s_nIdxMotion].keySet[s_nIdxKey].key[j].pos;
			//pParts->rot += pPlayer->Motion[s_nIdxMotion].keySet[s_nIdxKey].key[j].rot;
		}

		pPlayer->posOld = pPlayer->pos;
		pPlayer->rotDest = pPlayer->rot;
		pPlayer->nStopTime = 0;

		// 影の設定
		pPlayer->nIdxShadow = SetShadow(pPlayer->pos, pPlayer->rot, pPlayer->fSize);
	}
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
				pParts->pMesh->Release();
				pParts->pMesh = NULL;
			}

			if (pParts->pBuffMat != NULL)
			{// マテリアルの解放
				pParts->pBuffMat->Release();
				pParts->pBuffMat = NULL;
			}
		}
	}
	
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

	pPlayer->nStopTime++;

	Camera *pCamera = GetCamera();

	// 前回の位置を保存
	pPlayer->posOld = pPlayer->pos;

	if (pCamera->bFollow)
	{// 追従する
		// 追従の移動
		FollowMove(pPlayer);
	}
	else
	{// 追従しない
		// 移動
		Move(pPlayer);

		// 回転
		Rot(pPlayer);
	}

	float fAngle = pPlayer->rotDest.y - pPlayer->rot.y;

	// 角度の正規化
	NormalizeRot(&fAngle);

	//慣性・向きを更新 (減衰させる)
	pPlayer->rot.y += fAngle * MAX_ATTENUATION;

	// 角度の正規化
	NormalizeRot(&pPlayer->rot.y);

	D3DXVECTOR3 size = D3DXVECTOR3(pPlayer->fSize, pPlayer->fHeight, pPlayer->fSize);

	// モデルとの当たり判定
	CollisionModel(&pPlayer->pos, &pPlayer->posOld, size);

	// モーション
	Motion(pPlayer);

	// 影の位置の設定
	SetPosShadow(pPlayer->nIdxShadow, pPlayer->pos);
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

	for (int i = 0; i < s_nNumPlayer; i++)
	{
		Player *pPlayer = &s_player[i];

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
	return s_player;
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadPlayer(HWND hWnd)
{
	// システム
	System(hWnd);

	for (int i = 0; i < s_nNumPlayer; i++)
	{
		Player *pPlayer = &s_player[i];

		// パーツの読み込み
		LoadParts(hWnd, pPlayer);

		// モーションの読み込み
		LoadMotion(hWnd, pPlayer);
	}
}

//--------------------------------------------------
// システム
//--------------------------------------------------
static void System(HWND hWnd)
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
	}
	else
	{// ファイルが開かない場合
		MessageBox(hWnd, "システムファイルの読み込みに失敗！\nエラー場所  : [ モデル ]", "警告！", MB_ICONWARNING);
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

			else if (strcmp(&aRead[0], "PLAYER_SET") == 0)
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
		MessageBox(hWnd, "システムファイルの読み込みに失敗！\nエラー場所  : [ モデル ]", "警告！", MB_ICONWARNING);
		assert(false);
	}
}

//--------------------------------------------------
// パーツの読み込み
//--------------------------------------------------
static void LoadParts(HWND hWnd, Player *pPlayer)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE *pFile;		// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(pPlayer->aText, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};
		int nParts = 0, nFileName = 0, nNumModel = 0;
		ModelFile *pModelFile = NULL;

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

				// txtに書いてる最大数分の配列を用意する
				pModelFile = new ModelFile[nNumModel];
			}
			else if (strcmp(&aRead[0], "MODEL_FILENAME") == 0)
			{// モデルファイル名
				fscanf(pFile, "%s", &aRead);
				fscanf(pFile, "%s", pModelFile[nFileName].aParts);

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

		for (int j = 0; j < nNumModel; j++)
		{// Xファイルの読み込み
			D3DXLoadMeshFromX(
				pModelFile[j].aParts,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&pModelFile[j].pBuffMat,
				NULL,
				&pModelFile[j].nNumMat,
				&pModelFile[j].pMesh);
		}

		for (int j = 0; j < pPlayer->nNumParts; j++)
		{// 使用するモデルの情報を取得
			PlayerParts *Parts = &pPlayer->parts[j];

			Parts->pBuffMat = pModelFile[Parts->nIdxModel].pBuffMat;
			Parts->nNumMat = pModelFile[Parts->nIdxModel].nNumMat;
			Parts->pMesh = pModelFile[Parts->nIdxModel].pMesh;
		}

		// モデルの開放
		delete[] pModelFile;
		pModelFile = NULL;

		if (nFileName != nNumModel)
		{// モデル数とモデルのファイル名の数が違う
			MessageBox(hWnd, "[ モデル数 ] と [ モデルのファイル名 ] の数が合ってないよ！！\nエラー場所  : [ モデル ]", "警告！", MB_ICONWARNING);
			assert(false);
		}

		if (nParts != pPlayer->nNumParts)
		{// モデル数とモデルの情報の数が違う
			MessageBox(hWnd, "[ モデル数 ] と [ モデルの情報 ] の数が合ってないよ！！\nエラー場所  : [ モデル ]", "警告！", MB_ICONWARNING);
			assert(false);
		}

		bool bParent = false;		// 親がいるかどうか

		for (int j = 0; j < pPlayer->nNumParts; j++)
		{
			if (pPlayer->parts[j].nIdxParent == IDX_PARENT)
			{// 親いる
				bParent = true;
			}
		}

		if (!bParent)
		{// 親がいない
			MessageBox(hWnd, "親がいない、[ -1 ] を書いてよ！！\nエラー場所  : [ モデル ]", "警告！", MB_ICONWARNING);
			assert(false);
		}

		bParent = true;

		for (int j = 0; j < pPlayer->nNumParts; j++)
		{
			if (pPlayer->parts[j].nIdxParent >= pPlayer->nNumParts - 1)
			{// そんな親の番号は存在しない
				bParent = false;
			}
		}

		if (!bParent)
		{// そんな親の番号は存在しない
			MessageBox(hWnd, "そんな親の番号は存在しない！！\nエラー場所  : [ モデル ]", "警告！", MB_ICONWARNING);
			assert(false);
		}
	}
	else
	{// ファイルが開かない場合
		MessageBox(hWnd, "テキストファイルの読み込みに失敗！\nエラー場所  : [ モデル ]", "警告！", MB_ICONWARNING);
		assert(false);
	}
}

//--------------------------------------------------
// モーションの読み込み
//--------------------------------------------------
static void LoadMotion(HWND hWnd, Player *pPlayer)
{
	FILE *pFile;		// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(pPlayer->aText, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		char aRead[MAX_TEXT] = {};
		pPlayer->nNumMotion = 0;

		while (strncmp(&aRead[0], "END_SCRIPT", 10) != 0)
		{// 終わりが来るまで繰り返す
			fgets(aRead, MAX_TEXT, pFile);

			if (strncmp(&aRead[0], "MOTIONSET", 9) == 0)
			{// モーションの情報
				pPlayer->nNumMotion++;
			}
		}

		// ファイルを閉じる
		fclose(pFile);

		// txtに書いてる最大数分のモーションの配列を用意する
		pPlayer->Motion = new MotionSet[pPlayer->nNumMotion];
	}
	else
	{// ファイルが開かない場合
		MessageBox(hWnd, "システムファイルの読み込みに失敗！\nエラー場所  : [ モデル ]", "警告！", MB_ICONWARNING);
		assert(false);
	}

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
		MessageBox(hWnd, "テキストファイルの読み込みに失敗！\nエラー場所  : [ モデル ]", "警告！", MB_ICONWARNING);
		assert(false);
	}
}

//--------------------------------------------------
// 追従の移動
//--------------------------------------------------
static void FollowMove(Player *pPlayer)
{
	if (GetDebug() != DEBUG_MESH)
	{// デバッグ表示がメッシュではない時
		float fRot = 0.0f;

		/* ↓モデルの移動↓ */

		if (GetKeyboardPress(DIK_LEFT))
		{// ←キーが押された
			if (GetKeyboardPress(DIK_UP))
			{// ↑キーが押された
				fRot = -D3DX_PI * 0.25f;

				pPlayer->rotDest.y = D3DX_PI * 0.75f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ↓キーが押された
				fRot = -D3DX_PI * 0.75f;

				pPlayer->rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{
				fRot = -D3DX_PI * 0.5f;

				pPlayer->rotDest.y = D3DX_PI * 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_RIGHT))
		{// →キーが押された
			if (GetKeyboardPress(DIK_UP))
			{// ↑キーが押された
				fRot = D3DX_PI * 0.25f;

				pPlayer->rotDest.y = -D3DX_PI * 0.75f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ↓キーが押された
				fRot = D3DX_PI * 0.75f;

				pPlayer->rotDest.y = -D3DX_PI * 0.25f;
			}
			else
			{
				fRot = D3DX_PI * 0.5f;

				pPlayer->rotDest.y = -D3DX_PI * 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{// ↑キーが押された
			fRot = 0.0f;

			pPlayer->rotDest.y = D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ↓キーが押された
			fRot = D3DX_PI;

			pPlayer->rotDest.y = 0.0f;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// ←, →, ↑, ↓キーが押された
			pPlayer->pos.x += sinf(fRot) * pPlayer->fMove;
			pPlayer->pos.z += cosf(fRot) * pPlayer->fMove;

			pPlayer->nStopTime = 0;
		}

		if (GetKeyboardPress(DIK_I))
		{// Iキーが押された
			pPlayer->pos.y += sinf(D3DX_PI * 0.5f) * pPlayer->fMove;

			pPlayer->nStopTime = 0;
		}
		else if (GetKeyboardPress(DIK_K))
		{// Kキーが押された
			pPlayer->pos.y += sinf(-D3DX_PI * 0.5f) * pPlayer->fMove;

			pPlayer->nStopTime = 0;
		}

		// 指定の値以上・以下
		Specified(&pPlayer->pos.y, MAX_HEIGHT, MIN_HEIGHT);
	}
}

//--------------------------------------------------
// 移動
//--------------------------------------------------
static void Move(Player *pPlayer)
{
	if (GetDebug() != DEBUG_MESH)
	{// デバッグ表示がメッシュではない時

		Camera *pCamera = GetCamera();		//カメラの取得
		float fRot = 0.0f;

		/* ↓モデルの移動↓ */

		if (GetKeyboardPress(DIK_LEFT))
		{// ←キーが押された
			if (GetKeyboardPress(DIK_UP))
			{// ↑キーが押された
				fRot = pCamera->rot.y + (-D3DX_PI * 0.25f);

				pPlayer->rotDest.y = pCamera->rot.y + (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ↓キーが押された
				fRot = pCamera->rot.y + (-D3DX_PI * 0.75f);

				pPlayer->rotDest.y = pCamera->rot.y + (D3DX_PI * 0.25f);
			}
			else
			{
				fRot = pCamera->rot.y + (-D3DX_PI * 0.5f);

				pPlayer->rotDest.y = pCamera->rot.y + (D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_RIGHT))
		{// →キーが押された
			if (GetKeyboardPress(DIK_UP))
			{// ↑キーが押された
				fRot = pCamera->rot.y + (D3DX_PI * 0.25f);

				pPlayer->rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ↓キーが押された
				fRot = pCamera->rot.y + (D3DX_PI * 0.75f);

				pPlayer->rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.25f);
			}
			else
			{
				fRot = pCamera->rot.y + (D3DX_PI * 0.5f);

				pPlayer->rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{// ↑キーが押された
			fRot = pCamera->rot.y;

			pPlayer->rotDest.y = pCamera->rot.y + D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ↓キーが押された
			fRot = pCamera->rot.y + D3DX_PI;

			pPlayer->rotDest.y = pCamera->rot.y;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// ←, →, ↑, ↓キーが押された
			pPlayer->pos.x += sinf(fRot) * pPlayer->fMove;
			pPlayer->pos.z += cosf(fRot) * pPlayer->fMove;
		}

		if (GetKeyboardPress(DIK_I))
		{// Iキーが押された
			pPlayer->pos.y += sinf(D3DX_PI * 0.5f) * pPlayer->fMove;
		}
		else if (GetKeyboardPress(DIK_K))
		{// Kキーが押された
			pPlayer->pos.y += sinf(-D3DX_PI * 0.5f) * pPlayer->fMove;
		}

		// 指定の値以上・以下
		Specified(&pPlayer->pos.y, MAX_HEIGHT, MIN_HEIGHT);
	}
}

//--------------------------------------------------
// 回転
//--------------------------------------------------
static void Rot(Player *pPlayer)
{
	if (GetDebug() != DEBUG_MESH)
	{// デバッグ表示がメッシュではない時

		/* ↓モデルの回転↓ */

		if (GetKeyboardPress(DIK_LSHIFT))
		{// 左シフトキーが押された
			pPlayer->rotDest.y += -MAX_ROTATION;
		}
		else if (GetKeyboardPress(DIK_RSHIFT))
		{// 右シフトキーが押された
			pPlayer->rotDest.y += MAX_ROTATION;
		}
	}
}

//--------------------------------------------------
// モーション
//--------------------------------------------------
static void Motion(Player * pPlayer)
{
	s_nFrame++;

	if (GetKeyboardTrigger(DIK_RETURN))
	{// ENTERが押された
		s_nFrame = 0;
		s_nIdxKey = 0;

		// モーションセット
		SetMotion(pPlayer);
		
		s_nIdxMotion = 2;
	}

	if (GetKeyboardTrigger(DIK_LEFT) || GetKeyboardTrigger(DIK_RIGHT) ||
		GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_DOWN))
	{// ←, →, ↑, ↓キーが押された
		s_nFrame = 0;
		s_nIdxKey = 0;

		// モーションセット
		SetMotion(pPlayer);

		s_nIdxMotion = 1;
	}

	if (s_bMotionBlend)
	{// モーションブレンド中
		MotionSet *pMotion = &pPlayer->Motion[s_nIdxMotion];

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
	else
	{
		if (s_nIdxMotion == 1)
		{
			s_bMotionLoop = true;
		}
		else if (s_nIdxMotion == 2)
		{
			s_bMotionLoop = false;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// ←, →, ↑, ↓キーが押された
			s_bMotionLoop = false;
		}

		if (s_bMotionLoop)
		{// モーションループする
			s_nFrame = 0;
			s_nIdxKey = 0;

			// モーションセット
			SetMotion(pPlayer);

			s_nIdxMotion = 0;
		}

		MotionSet *pMotion = &pPlayer->Motion[s_nIdxMotion];

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
					s_nIdxKey = 0;

					// モーションセット
					SetMotion(pPlayer);
					
					s_nIdxMotion = 0;
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

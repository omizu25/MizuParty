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
#define FILE_NAME		"data\\TXT\\model.txt"		// ファイルの名前
#define MAX_MOVE				(1.0f)				// 移動量の最大値
#define MAX_ROTATION			(0.035f)			// 回転の最大値
#define MAX_ATTENUATION			(0.1f)				// 減衰係数の最大値
#define MAX_HEIGHT				(80.0f)				// 高さの最大値
#define MIN_HEIGHT				(0.0f)				// 高さの最小値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Player		s_player;			// モデルの情報
static int			s_nUsePlayer;		// 使用するプレイヤーの数

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void FollowMove(void);
static void Move(void);
static void Rot(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitPlayer(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;		// 頂点数
	DWORD SizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVexBuff;		// 頂点バッファへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data/MODEL/てるてる体.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_player.parts[0].pBuffMat,
		NULL,
		&s_player.parts[0].nNumMat,
		&s_player.parts[0].pMesh);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data/MODEL/てるてる頭.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_player.parts[1].pBuffMat,
		NULL,
		&s_player.parts[1].nNumMat,
		&s_player.parts[1].pMesh);

	s_player.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_player.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// 頂点数を取得
	nNumVtx = s_player.parts[0].pMesh->GetNumVertices();

	// フォーマットのサイズを取得
	SizeFVF = D3DXGetFVFVertexSize(s_player.parts[0].pMesh->GetFVF());

	// 頂点バッファのロック
	s_player.parts[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

	for (int i = 0; i < nNumVtx; i++)
	{
		// 頂点情報の代入
		D3DXVECTOR3 pos = *(D3DXVECTOR3*)pVexBuff;

		// 小さい・大きい [x]
		VtxSmallBig(&s_player.vtxMin.x, &s_player.vtxMax.x, pos.x);

		// 小さい・大きい [y]
		VtxSmallBig(&s_player.vtxMin.y, &s_player.vtxMax.y, pos.y);

		// 小さい・大きい [z]
		VtxSmallBig(&s_player.vtxMin.z, &s_player.vtxMax.z, pos.z);

		// 頂点フォーマットのサイズ分ポインタを進める
		pVexBuff += SizeFVF;
	}

	// 頂点バッファのアンロック
	s_player.parts[0].pMesh->UnlockVertexBuffer();

	for (int i = 0; i < MAX_PARTS; i++)
	{
		// メッシュに使用されているテクスチャ用の配列を用意する
		s_player.parts[i].pTexture = new LPDIRECT3DTEXTURE9[s_player.parts[i].nNumMat];

		// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_player.parts[i].pBuffMat->GetBufferPointer();

		// 各メッシュのマテリアル情報を取得する
		for (int j = 0; j < (int)s_player.parts[i].nNumMat; j++)
		{
			s_player.parts[i].pTexture[j] = NULL;

			if (pMat[j].pTextureFilename != NULL)
			{// マテリアルで設定されているテクスチャ読み込み
				D3DXCreateTextureFromFileA(pDevice,
					pMat[j].pTextureFilename,
					&s_player.parts[i].pTexture[j]);
			}
			else
			{
				s_player.parts[i].pTexture[j] = NULL;
			}
		}

		s_player.pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		s_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_player.rotDest = s_player.rot;
		s_player.nStopTime = 0;

		// 影の設定
		s_player.nIdxShadow = SetShadow(s_player.pos, s_player.rot);
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitPlayer(void)
{
	for (int i = 0; i < MAX_PARTS; i++)
	{
		if (s_player.parts[i].pTexture != NULL)
		{// テクスチャの解放
			for (int j = 0; j < (int)s_player.parts[i].nNumMat; j++)
			{
				if (s_player.parts[i].pTexture[j] != NULL)
				{
					s_player.parts[i].pTexture[j]->Release();
					s_player.parts[i].pTexture[j] = NULL;
				}
			}

			delete[](s_player.parts[i].pTexture);
			s_player.parts[i].pTexture = NULL;
		}
	}

	for (int i = 0; i < MAX_PARTS; i++)
	{
		if (s_player.parts[i].pMesh != NULL)
		{// メッシュの解放
			s_player.parts[i].pMesh->Release();
			s_player.parts[i].pMesh = NULL;
		}
	}

	for (int i = 0; i < MAX_PARTS; i++)
	{
		if (s_player.parts[i].pBuffMat != NULL)
		{// マテリアルの解放
			s_player.parts[i].pBuffMat->Release();
			s_player.parts[i].pBuffMat = NULL;
		}
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdatePlayer(void)
{
	s_player.nStopTime++;

	Camera *pCamera = GetCamera();

	// 前回の位置を保存
	s_player.posOld = s_player.pos;

	if (pCamera->bFollow)
	{// 追従する
		// 追従の移動
		FollowMove();
	}
	else
	{// 追従しない
		// 移動
		Move();

		// 回転
		Rot();
	}

	float fAngle = s_player.rotDest.y - s_player.rot.y;

	// 角度の正規化
	NormalizeRot(&fAngle);

	//慣性・向きを更新 (減衰させる)
	s_player.rot.y += fAngle * MAX_ATTENUATION;

	// 角度の正規化
	NormalizeRot(&s_player.rot.y);

	// モデルとの当たり判定
	CollisionModel(&s_player.pos, &s_player.posOld, s_player.vtxMin.z, s_player.vtxMin.z);

	// 影の位置の設定
	SetPosShadow(s_player.nIdxShadow, s_player.pos);
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

	for (int i = 0; i < MAX_PARTS; i++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&s_player.mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, s_player.rot.y, s_player.rot.x, s_player.rot.z);
		D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);
		D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &s_player.mtxWorld);

		// 現在のマテリアル保持
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)s_player.parts[i].pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)s_player.parts[i].nNumMat; j++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[j].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, s_player.parts[i].pTexture[j]);

			// モデルパーツの描画
			s_player.parts[i].pMesh->DrawSubset(j);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player;
}

//--------------------------------------------------
// 追従の移動
//--------------------------------------------------
static void FollowMove(void)
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

				s_player.rotDest.y = D3DX_PI * 0.75f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ↓キーが押された
				fRot = -D3DX_PI * 0.75f;

				s_player.rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{
				fRot = -D3DX_PI * 0.5f;

				s_player.rotDest.y = D3DX_PI * 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_RIGHT))
		{// →キーが押された
			if (GetKeyboardPress(DIK_UP))
			{// ↑キーが押された
				fRot = D3DX_PI * 0.25f;

				s_player.rotDest.y = -D3DX_PI * 0.75f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ↓キーが押された
				fRot = D3DX_PI * 0.75f;

				s_player.rotDest.y = -D3DX_PI * 0.25f;
			}
			else
			{
				fRot = D3DX_PI * 0.5f;

				s_player.rotDest.y = -D3DX_PI * 0.5f;
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{// ↑キーが押された
			fRot = 0.0f;

			s_player.rotDest.y = D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ↓キーが押された
			fRot = D3DX_PI;

			s_player.rotDest.y = 0.0f;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// ←, →, ↑, ↓キーが押された
			s_player.pos.x += sinf(fRot) * MAX_MOVE;
			s_player.pos.z += cosf(fRot) * MAX_MOVE;

			s_player.nStopTime = 0;
		}

		if (GetKeyboardPress(DIK_I))
		{// Iキーが押された
			s_player.pos.y += sinf(D3DX_PI * 0.5f) * MAX_MOVE;

			s_player.nStopTime = 0;
		}
		else if (GetKeyboardPress(DIK_K))
		{// Kキーが押された
			s_player.pos.y += sinf(-D3DX_PI * 0.5f) * MAX_MOVE;

			s_player.nStopTime = 0;
		}

		// 指定の値以上・以下
		Specified(&s_player.pos.y, MAX_HEIGHT, MIN_HEIGHT);
	}
}

//--------------------------------------------------
// 移動
//--------------------------------------------------
static void Move(void)
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

				s_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ↓キーが押された
				fRot = pCamera->rot.y + (-D3DX_PI * 0.75f);

				s_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.25f);
			}
			else
			{
				fRot = pCamera->rot.y + (-D3DX_PI * 0.5f);

				s_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_RIGHT))
		{// →キーが押された
			if (GetKeyboardPress(DIK_UP))
			{// ↑キーが押された
				fRot = pCamera->rot.y + (D3DX_PI * 0.25f);

				s_player.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ↓キーが押された
				fRot = pCamera->rot.y + (D3DX_PI * 0.75f);

				s_player.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.25f);
			}
			else
			{
				fRot = pCamera->rot.y + (D3DX_PI * 0.5f);

				s_player.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{// ↑キーが押された
			fRot = pCamera->rot.y;

			s_player.rotDest.y = pCamera->rot.y + D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ↓キーが押された
			fRot = pCamera->rot.y + D3DX_PI;

			s_player.rotDest.y = pCamera->rot.y;
		}

		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
			GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
		{// ←, →, ↑, ↓キーが押された
			s_player.pos.x += sinf(fRot) * MAX_MOVE;
			s_player.pos.z += cosf(fRot) * MAX_MOVE;
		}

		if (GetKeyboardPress(DIK_I))
		{// Iキーが押された
			s_player.pos.y += sinf(D3DX_PI * 0.5f) * MAX_MOVE;
		}
		else if (GetKeyboardPress(DIK_K))
		{// Kキーが押された
			s_player.pos.y += sinf(-D3DX_PI * 0.5f) * MAX_MOVE;
		}

		// 指定の値以上・以下
		Specified(&s_player.pos.y, MAX_HEIGHT, MIN_HEIGHT);
	}
}

//--------------------------------------------------
// 回転
//--------------------------------------------------
static void Rot(void)
{
	if (GetDebug() != DEBUG_MESH)
	{// デバッグ表示がメッシュではない時

		/* ↓モデルの回転↓ */

		if (GetKeyboardPress(DIK_LSHIFT))
		{// 左シフトキーが押された
			s_player.rotDest.y += -MAX_ROTATION;
		}
		else if (GetKeyboardPress(DIK_RSHIFT))
		{// 右シフトキーが押された
			s_player.rotDest.y += MAX_ROTATION;
		}
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadPlayer(HWND hWnd)
{
	//FILE *pFile;			// ファイルポインタを宣言
	//int nUseText = 0;		// テキストで読み込んだビルボードの使用数

	//char aTexture[MAX_MODEL][1024];

	//// ファイルを開く
	//pFile = fopen(FILE_NAME, "r");

	//if (pFile != NULL)
	//{// ファイルが開いた場合
	//	char aRead[256] = {};
	//	int nTex = 0, nText = 0;

	//	while (strcmp(&aRead, "SCRIPT") != 0)
	//	{// 始まりが来るまで繰り返す
	//		fscanf(pFile, "%s", &aRead);
	//	}

	//	while (strcmp(&aRead, "END_SCRIPT") != 0)
	//	{// 終わりが来るまで繰り返す
	//		fscanf(pFile, "%s", &aRead);

	//		if (strncmp(&aRead, "#-", 2) == 0)
	//		{// コメント
	//			continue;
	//		}
	//		else if (strncmp(&aRead, "#", 1) == 0)
	//		{// コメント
	//			fscanf(pFile, "%s", &aRead);
	//			continue;
	//		}

	//		if (strcmp(&aRead, "NUM_MODEL") == 0)
	//		{// テクスチャの使用数
	//			fscanf(pFile, "%d", &s_nUsePlayer);
	//		}
	//		else if (strcmp(&aRead, "MODEL_FILENAME") == 0)
	//		{// テクスチャの情報
	//			fscanf(pFile, "%s", aTexture[nTex]);
	//			nTex++;
	//		}
	//		else if (strcmp(&aRead, "NUM_MODEL") == 0)
	//		{// ビルボードの使用数
	//			fscanf(pFile, "%d", &nUseText);

	//			// txtに書いてる最大数分の読み込み用の配列を用意する
	//			//s_player = new s_player[nUseText];
	//		}
	//		else if (strcmp(&aRead, "BILLBOARD_SET") == 0)
	//		{// ビルボードの情報
	//			while (strcmp(&aRead, "END_BILLBOARD_SET") != 0)
	//			{// 終わりが来るまで繰り返す
	//				fscanf(pFile, "%s", &aRead);

	//				if (strncmp(&aRead, "#", 1) == 0)
	//				{// コメント
	//					fscanf(pFile, "%s", &aRead);
	//					continue;
	//				}

	//				if (strcmp(&aRead, "INDEX") == 0)
	//				{// モデル番号
	//					fscanf(pFile, "%d", &s_player[nText].nIdxModel);
	//				}
	//				else if (strcmp(&aRead, "POS") == 0)
	//				{// 位置
	//					fscanf(pFile, "%f", &s_player[nText].pos.x);
	//					fscanf(pFile, "%f", &s_player[nText].pos.y);
	//					fscanf(pFile, "%f", &s_player[nText].pos.z);
	//				}
	//				else if (strcmp(&aRead, "ROT") == 0)
	//				{// 向き
	//					fscanf(pFile, "%f", &s_player[nText].rot.x);
	//					fscanf(pFile, "%f", &s_player[nText].rot.y);
	//					fscanf(pFile, "%f", &s_player[nText].rot.z);
	//				}
	//			}
	//			nText++;
	//		}
	//	}

	//	// ファイルを閉じる
	//	fclose(pFile);
	//}
	//else
	//{// ファイルが開かない場合
	//	MessageBox(hWnd, "テキストファイルの読み込みに失敗！\nエラー場所  : [ ビルボード ]", "警告！", MB_ICONWARNING);
	//	assert(false);
	//}

	//for (int i = 0; i < nUseText; i++)
	//{
	//	if (s_player[i].nIdxModel >= s_nUsePlayer)
	//	{// 該当しないテクスチャ番号
	//		MessageBox(hWnd, "該当しないテクスチャ番号です！\nエラー場所  : [ ビルボード ]", "警告！", MB_ICONWARNING);
	//		assert(false);
	//	}
	//}

	//// デバイスへのポインタの取得
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//for (int i = 0; i < s_nUsePlayer; i++)
	//{
	//	// テクスチャの読み込み
	//	D3DXCreateTextureFromFile(
	//		pDevice,
	//		&aTexture[i],
	//		&s_pTexture[i]);
	//}

	//for (int i = 0; i < nUseText; i++)
	//{
	//	pText[i].pTexture = s_pTexture[pText[i].nTexIdx];

	//	bool bYRot = true;

	//	if (pText[i].nYRot == DO_NOT_ROT_Y)
	//	{// Y軸回転をしない数値の時
	//		bYRot = false;
	//	}

	//	// 設定
	//	SetBillboard(pText[i].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pText[i].fWidth, pText[i].fHeight, bYRot, &pText[i].pTexture);
	//}
}

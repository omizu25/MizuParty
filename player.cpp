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
#include "input.h"
#include "line.h"
#include "player.h"
#include "setup.h"
#include "shadow.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define FILE_NAME		"data\\TXT\\model.txt"		// ファイルの名前
#define MAX_MOVE				(1.0f)				//移動量の最大値
#define MAX_ROTATION			(0.035f)			//回転の最大値
#define MAX_ATTENUATION			(0.1f)				//減衰係数の最大値
#define MAX_HEIGHT				(80.0f)				//高さの最大値
#define MIN_HEIGHT				(0.0f)				//高さの最小値

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
		"data/MODEL/てるてる４期生.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_player.pBuffMat,
		NULL,
		&s_player.nNumMat,
		&s_player.pMesh);

	s_player.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_player.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// 頂点数を取得
	nNumVtx = s_player.pMesh->GetNumVertices();

	// フォーマットのサイズを取得
	SizeFVF = D3DXGetFVFVertexSize(s_player.pMesh->GetFVF());

	// 頂点バッファのロック
	s_player.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

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
	s_player.pMesh->UnlockVertexBuffer();

	// メッシュに使用されているテクスチャ用の配列を用意する
	s_player.pTexture = new LPDIRECT3DTEXTURE9[s_player.nNumMat];

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_player.pBuffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)s_player.nNumMat; i++)
	{
		s_player.pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_player.pTexture[i]);
		}
		else
		{
			s_player.pTexture[i] = NULL;
		}
	}

	s_player.pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	s_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_player.rotDest = s_player.rot;
	s_player.nStopTime = 0;

	// 影の設定
	s_player.nIdxShadow = SetShadow(s_player.pos, s_player.rot);

	D3DXCOLOR col = D3DXCOLOR(0.615f, 0.215f, 0.341f, 1.0f);

	/*↓ Y棒 ↓*/

	D3DXVECTOR3 start = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMin.z);
	D3DXVECTOR3 end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMin.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMin.z);
	end = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMin.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMax.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMax.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);

	/*↓ X棒 ↓*/

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMax.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMin.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMin.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMax.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMin.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMin.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);

	/*↓ Z棒 ↓*/

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMax.y, s_player.vtxMin.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMax.y, s_player.vtxMin.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMax.x, s_player.vtxMin.y, s_player.vtxMin.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);

	start = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMax.z);
	end = D3DXVECTOR3(s_player.vtxMin.x, s_player.vtxMin.y, s_player.vtxMin.z);

	// 線の設定
	SetLine(s_player.pos, s_player.rot, start, end, col);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitPlayer(void)
{
	if (s_player.pTexture != NULL)
	{// テクスチャの解放
		for (int i = 0; i < (int)s_player.nNumMat; i++)
		{
			if (s_player.pTexture[i] != NULL)
			{
				s_player.pTexture[i]->Release();
				s_player.pTexture[i] = NULL;
			}
		}

		delete[](s_player.pTexture);
		s_player.pTexture = NULL;
	}

	if (s_player.pMesh != NULL)
	{// メッシュの解放
		s_player.pMesh->Release();
		s_player.pMesh = NULL;
	}

	if (s_player.pBuffMat != NULL)
	{// マテリアルの解放
		s_player.pBuffMat->Release();
		s_player.pBuffMat = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdatePlayer(void)
{
	s_player.nStopTime++;

	Camera *pCamera = GetCamera();

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
	pMat = (D3DXMATERIAL*)s_player.pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)s_player.nNumMat; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_player.pTexture[i]);

		// モデルパーツの描画
		s_player.pMesh->DrawSubset(i);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player;
}

////--------------------------------------------------
//// ブロックの当たり判定処理
////--------------------------------------------------
//void CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
//{
//		float fLeft = pBlock->pos.x - pBlock->fWidth;
//		float fRight = pBlock->pos.x + pBlock->fWidth;
//		float fTop = pBlock->pos.y - pBlock->fHeight;
//		float fBottom = pBlock->pos.y + pBlock->fHeight;
//
//		if ((pPos->x + fWidth > fLeft) && (pPos->x - fWidth < fRight))
//		{// Xがブロックの範囲内
//			if ((pPosOld->y <= fTop) && (pPos->y >= fTop))
//			{// ブロックの下から上
//				pPos->y = fTop;
//			}
//			else if ((pPosOld->y - fHeight >= fBottom) && (pPos->y - fHeight < fBottom))
//			{// ブロックの上から下
//				pPos->y = fBottom + fHeight;
//				pMove->y = 0.0f;
//			}
//		}
//
//		if ((pPosOld->y > fTop) && (pPosOld->y - fHeight < fBottom))
//		{//前回のYがブロックの範囲内
//			if ((pPosOld->x + fWidth <= fLeft) && (pPos->x + fWidth > fLeft))
//			{//ブロックの左端
//				pPos->x = fLeft - fWidth;
//				pMove->x *= 0.5f;
//			}
//			else if ((pPosOld->x - fWidth >= fRight) && (pPos->x - fWidth < fRight))
//			{//ブロックの右端
//				pPos->x = fRight + fWidth;
//				pMove->x *= 0.5f;
//			}
//		}
//	}
//}

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

	//	while (strcmp(&aRead[0], "SCRIPT") != 0)
	//	{// 始まりが来るまで繰り返す
	//		fscanf(pFile, "%s", &aRead);
	//	}

	//	while (strcmp(&aRead[0], "END_SCRIPT") != 0)
	//	{// 終わりが来るまで繰り返す
	//		fscanf(pFile, "%s", &aRead);

	//		if (strncmp(&aRead[0], "#-", 2) == 0)
	//		{// コメント
	//			continue;
	//		}
	//		else if (strncmp(&aRead[0], "#", 1) == 0)
	//		{// コメント
	//			fscanf(pFile, "%s", &aRead);
	//			continue;
	//		}

	//		if (strcmp(&aRead[0], "NUM_MODEL") == 0)
	//		{// テクスチャの使用数
	//			fscanf(pFile, "%d", &s_nUsePlayer);
	//		}
	//		else if (strcmp(&aRead[0], "MODEL_FILENAME") == 0)
	//		{// テクスチャの情報
	//			fscanf(pFile, "%s", aTexture[nTex]);
	//			nTex++;
	//		}
	//		else if (strcmp(&aRead[0], "NUM_MODEL") == 0)
	//		{// ビルボードの使用数
	//			fscanf(pFile, "%d", &nUseText);

	//			// txtに書いてる最大数分の読み込み用の配列を用意する
	//			//s_player = new s_player[nUseText];
	//		}
	//		else if (strcmp(&aRead[0], "BILLBOARD_SET") == 0)
	//		{// ビルボードの情報
	//			while (strcmp(&aRead[0], "END_BILLBOARD_SET") != 0)
	//			{// 終わりが来るまで繰り返す
	//				fscanf(pFile, "%s", &aRead);

	//				if (strncmp(&aRead[0], "#", 1) == 0)
	//				{// コメント
	//					fscanf(pFile, "%s", &aRead);
	//					continue;
	//				}

	//				if (strcmp(&aRead[0], "INDEX") == 0)
	//				{// モデル番号
	//					fscanf(pFile, "%d", &s_player[nText].nIdxModel);
	//				}
	//				else if (strcmp(&aRead[0], "POS") == 0)
	//				{// 位置
	//					fscanf(pFile, "%f", &s_player[nText].pos.x);
	//					fscanf(pFile, "%f", &s_player[nText].pos.y);
	//					fscanf(pFile, "%f", &s_player[nText].pos.z);
	//				}
	//				else if (strcmp(&aRead[0], "ROT") == 0)
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
	//		&aTexture[i][0],
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

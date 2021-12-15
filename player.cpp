//==================================================
// 
// 3Dゲーム制作 ( player.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "billboard.h"
#include "camera.h"
#include "input.h"
#include "main.h"
#include "player.h"
#include "setup.h"
#include "shadow.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_MOVE			(1.0f)			//移動量の最大値
#define MAX_ROTATION		(0.035f)		//回転の最大値
#define MAX_ATTENUATION		(0.1f)			//減衰係数の最大値
#define MAX_HEIGHT			(80.0f)			//高さの最大値
#define MIN_HEIGHT			(10.0f)			//高さの最小値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPD3DXMESH				s_pMesh = NULL;				// メッシュ情報へのポインタ
static LPD3DXBUFFER				s_pBuffMat = NULL;			// マテリアル情報へのポインタ
static LPDIRECT3DTEXTURE9		*s_pTexture = NULL;			// テクスチャへのポインタ
static DWORD					s_nNumMat = 0;				// マテリアル情報の数
static Player					s_player;					// モデルの情報

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

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data\\MODEL\\てるてる４期生.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

	// メッシュに使用されているテクスチャ用の配列を用意する
	s_pTexture = new LPDIRECT3DTEXTURE9[s_nNumMat];

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		s_pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_pTexture[i]);
		}
		else
		{
			s_pTexture[i] = NULL;
		}
	}

	s_player.pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	s_player.rot = D3DXVECTOR3(0.0f, (-D3DX_PI * 0.5f), 0.0f);
	s_player.rotDest = s_player.rot;
	s_player.nStopTime = 0;

	// 影の設定
	s_player.nIdxShadow = SetShadow(s_player.pos, s_player.rot);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitPlayer(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの解放
		for (int i = 0; i < (int)s_nNumMat; i++)
		{
			if (s_pTexture[i] != NULL)
			{
				s_pTexture[i]->Release();
				s_pTexture[i] = NULL;
			}
		}

		delete[](s_pTexture);
		s_pTexture = NULL;
	}

	if (s_pMesh != NULL)
	{// メッシュの解放
		s_pMesh->Release();
		s_pMesh = NULL;
	}

	if (s_pBuffMat != NULL)
	{// マテリアルの解放
		s_pBuffMat->Release();
		s_pBuffMat = NULL;
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
	pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[i]);

		// モデルパーツの描画
		s_pMesh->DrawSubset(i);
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

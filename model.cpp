//==================================================
// 
// 3Dゲーム制作 ( model.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "countdown.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "setup.h"
#include "shadow.h"
#include "particle.h"
#include "player.h"
#include "result.h"
#include "sound.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define START_ROTATION_X		(-79.0f)		// 回転のスタートの幅
#define START_ROTATION_Y		(132.0f)		// 回転のスタートの高さ
#define START_STOP_X			(-79.0f)		// 止めるのスタートの幅
#define START_STOP_Y			(354.0f)		// 止めるのスタートの高さ
#define MAX_ROTATION			(0.03f)			// 回転量の最大値
#define MAX_RANDOM				(2)				// ランダムの最大値
#define MAX_MOVE				(6.0f)			// 移動量の最大値
#define STOP_GOOD				(15.0f)			// 止めるの上手
#define STOP_NORMAL				(70.0f)			// 止めるの普通
#define STOP_COLLISION			(0.0f)			// 止めるの当たる位置
#define START_TIME				(45)			// 始まる時間

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static Model		s_model;			// モデルの情報
static bool			s_bStop;			// 止めるかどうか
static bool			s_bCollision;		// 当たったかどうか
static bool			s_bDraw;			// 描画するかどうか

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void UpdateRotation(void);
static void UpdateStop(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitModel(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;		// 頂点数
	DWORD SizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVexBuff;		// 頂点バッファへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data\\MODEL\\Hammer.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_model.pBuffMat,
		NULL,
		&s_model.nNumMat,
		&s_model.pMesh);

	s_model.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_model.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// 頂点数を取得
	nNumVtx = s_model.pMesh->GetNumVertices();

	// フォーマットのサイズを取得
	SizeFVF = D3DXGetFVFVertexSize(s_model.pMesh->GetFVF());

	// 頂点バッファのロック
	s_model.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVexBuff);

	for (int i = 0; i < nNumVtx; i++)
	{
		// 頂点情報の代入
		D3DXVECTOR3 pos = *(D3DXVECTOR3*)pVexBuff;

		// 小さい・大きい [x]
		VtxSmallBig(&s_model.vtxMin.x, &s_model.vtxMax.x, pos.x);

		// 小さい・大きい [y]
		VtxSmallBig(&s_model.vtxMin.y, &s_model.vtxMax.y, pos.y);

		// 小さい・大きい [z]
		VtxSmallBig(&s_model.vtxMin.z, &s_model.vtxMax.z, pos.z);

		// 頂点フォーマットのサイズ分ポインタを進める
		pVexBuff += SizeFVF;
	}

	// 頂点バッファのアンロック
	s_model.pMesh->UnlockVertexBuffer();

	// メッシュに使用されているテクスチャ用の配列を用意する
	s_model.pTexture = new LPDIRECT3DTEXTURE9[s_model.nNumMat];

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_model.pBuffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)s_model.nNumMat; i++)
	{
		s_model.pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_model.pTexture[i]);
		}
		else
		{
			s_model.pTexture[i] = NULL;
		}
	}

	float fRand = 0.0f;

	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
	case MENU_SLOPE:		// 坂

		break;

	case MENU_ROTATION:		// 回転

		fRand = (rand() % 100) * 0.01f * D3DX_PI * 0.5f;
		fRand -= D3DX_PI * 0.25f;

		s_model.pos = D3DXVECTOR3(START_ROTATION_X, START_ROTATION_Y, 0.0f);
		s_model.rot = D3DXVECTOR3(0.0f, 0.0f, (D3DX_PI * 0.5f) + fRand);

		break;

	case MENU_STOP:			// 止める

		s_model.pos = D3DXVECTOR3(START_STOP_X, START_STOP_Y, 0.0f);
		s_model.rot = D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f);

		break;

	default:
		assert(false);
		break;
	}
	
	s_model.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	s_model.nRepeat = 0;

	s_bStop = false;
	s_bCollision = false;
	s_bDraw = true;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitModel(void)
{
	if (s_model.pTexture != NULL)
	{// テクスチャの解放
		for (int i = 0; i < (int)s_model.nNumMat; i++)
		{
			if (s_model.pTexture[i] != NULL)
			{
				s_model.pTexture[i]->Release();
				s_model.pTexture[i] = NULL;
			}
		}

		delete[](s_model.pTexture);
		s_model.pTexture = NULL;
	}

	if (s_model.pMesh != NULL)
	{// メッシュの解放
		s_model.pMesh->Release();
		s_model.pMesh = NULL;
	}

	if (s_model.pBuffMat != NULL)
	{// マテリアルの解放
		s_model.pBuffMat->Release();
		s_model.pBuffMat = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateModel(void)
{
	switch (GetTitle())
	{
	case MENU_WALKING:		// ウォーキング
	case MENU_SLOPE:		// 坂
		
		/* 処理なし */

		break;

	case MENU_ROTATION:		// 回転
		
		UpdateRotation();
		
		break;

	case MENU_STOP:			// 止める

		UpdateStop();

		break;

	default:
		assert(false);
		break;
	}

}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawModel(void)
{
	if (s_bDraw)
	{// 描画する
		// デバイスへのポインタの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
		D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
		D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

			// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&s_model.mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, s_model.rot.y, s_model.rot.x, s_model.rot.z);
		D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, s_model.pos.x, s_model.pos.y, s_model.pos.z);
		D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &s_model.mtxWorld);

		// 現在のマテリアル保持
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)s_model.pBuffMat->GetBufferPointer();

		for (int i = 0; i < (int)s_model.nNumMat; i++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[i].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, s_model.pTexture[i]);

			// モデルパーツの描画
			s_model.pMesh->DrawSubset(i);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);
	}
}

//--------------------------------------------------
// 取得 (モデル情報)
//--------------------------------------------------
Model *GetModel(void)
{
	return &s_model;
}

//--------------------------------------------------
// 取得 (止まってるかどうか)
//--------------------------------------------------
bool GetStop(void)
{
	return s_bStop;
}

//--------------------------------------------------
// 取得 (当たったかどうか)
//--------------------------------------------------
bool GetCollision(void)
{
	return s_bCollision;
}

//--------------------------------------------------
// 回転
//--------------------------------------------------
static void UpdateRotation(void)
{
	if (GetCountdown())
	{// カウントダウン終わった
		switch (GetGame())
		{
		case GAMESTATE_NORMAL:			// ゲーム中
		case GAMESTATE_END:				// 終わり
		case GAMESTATE_RESULT:			// リザルト

			if (!s_bStop)
			{// 止まってない
				s_model.rot.z -= D3DX_PI * MAX_ROTATION;

				// 角度の正規化
				NormalizeRot(&s_model.rot.z);
			}

			if (!s_bCollision)
			{// 当たってない
				if (s_model.rot.z < -D3DX_PI * 0.5f)
				{// 衝突
					s_bCollision = true;

					D3DXVECTOR3 pos = GetPlayer()->pos;

					pos.y += 50.0f;

					// プレイヤーの描画するかの設定
					SetDrawPlayer(false);

					// 爆発の設定
					SetExplosion(pos, 20.0f, true);

					// リザルトの設定
					SetResult(RESULT_GAMEOVER);

					// ゲームの設定
					SetGameState(GAMESTATE_END);

					// サウンドの再生
					PlaySound(SOUND_LABEL_SE_KO);
				}
			}
			else
			{// 当たってる
				if (s_model.rot.z < -D3DX_PI * 0.75f)
				{// 止める
					s_bStop = true;
				}
			}

			break;

		case GAMESTATE_NONE:			// 何もなし
		case GAMESTATE_COUNTDOWN:		// カウントダウン
		case GAMESTATE_START:			// 始まり

			/* 処理なし */

			break;

		default:
			assert(false);
			break;
		}

		if (GetGame() == GAMESTATE_NORMAL)
		{// ゲーム中
			if (!s_bCollision && !s_bStop)
			{// 当たってない、止まってない
				if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) ||
					GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_B) ||
					GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
				{// F4キーが押された
					s_bStop = true;

					// リザルトの設定
					SetResult(RESULT_CLEAR);

					// ゲームの設定
					SetGameState(GAMESTATE_END);

					float fDiff = 90.0f + (s_model.rot.z * (180 / D3DX_PI));

					if (fDiff <= STOP_GOOD)
					{// 止めるの上手
						// サウンドの再生
						PlaySound(SOUND_LABEL_SE_止めるの上手);

						s_model.nRepeat = 10;
					}
					else if (fDiff <= STOP_NORMAL)
					{// 止めるの普通
						// サウンドの再生
						PlaySound(SOUND_LABEL_SE_止めるの普通);

						s_model.nRepeat = 3;
					}
					else
					{// 止めるの下手
						// サウンドの再生
						PlaySound(SOUND_LABEL_SE_止めるの下手);

						s_model.nRepeat = 1;
					}
				}
			}
		}
	}
}

//--------------------------------------------------
// 止める
//--------------------------------------------------
static void UpdateStop(void)
{
	if (GetCountdown())
	{// カウントダウン終わった
		switch (GetGame())
		{
		case GAMESTATE_NORMAL:			// ゲーム中
		case GAMESTATE_END:				// 終わり
		case GAMESTATE_RESULT:			// リザルト

			if (!s_bCollision)
			{
				if (!s_bStop)
				{// 止まらない
					s_model.pos.y -= MAX_MOVE;
				}

				float fModel = s_model.pos.y - s_model.vtxMax.x;
				float fPlayer = GetPlayer()->pos.y + GetPlayer()->fHeight;

				float fPos = (fModel - fPlayer) + 1.0f;

				if (fPos <= 0.0f)
				{
					s_bCollision = true;
					s_bStop = true;
					s_bDraw = false;

					D3DXVECTOR3 pos = D3DXVECTOR3(GetPlayer()->pos.x, s_model.pos.y, s_model.pos.z);

					// 爆発の設定
					SetExplosion(pos, 20.0f, true);

					// リザルトの設定
					SetResult(RESULT_GAMEOVER);

					// ゲームの設定
					SetGameState(GAMESTATE_END);

					// サウンドの再生
					PlaySound(SOUND_LABEL_SE_KO);
				}
			}

			break;

		case GAMESTATE_NONE:			// 何もなし
		case GAMESTATE_COUNTDOWN:		// カウントダウン
		case GAMESTATE_START:			// 始まり

			/* 処理なし */

			break;

		default:
			assert(false);
			break;
		}
	}

	if (GetGame() == GAMESTATE_NORMAL)
	{// ゲーム中
		if (!s_bCollision && !s_bStop)
		{
			if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) ||
				GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_B) ||
				GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
			{// F4キーが押された
				s_bStop = true;

				// リザルトの設定
				SetResult(RESULT_CLEAR);

				// ゲームの設定
				SetGameState(GAMESTATE_END);

				float fModel = s_model.pos.y - s_model.vtxMax.x;
				float fPlayer = GetPlayer()->pos.y + GetPlayer()->fHeight;

				float fDiff = fModel - fPlayer;

				if (fDiff <= STOP_GOOD)
				{// 止めるの上手
					// サウンドの再生
					PlaySound(SOUND_LABEL_SE_止めるの上手);

					s_model.nRepeat = 10;
				}
				else if (fDiff <= STOP_NORMAL)
				{// 止めるの普通
					// サウンドの再生
					PlaySound(SOUND_LABEL_SE_止めるの普通);

					s_model.nRepeat = 3;
				}
				else
				{// 止めるの下手
					// サウンドの再生
					PlaySound(SOUND_LABEL_SE_止めるの下手);

					s_model.nRepeat = 1;
				}
			}
		}
	}
}

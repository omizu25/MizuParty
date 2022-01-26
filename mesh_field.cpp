//==================================================
// 
// 3Dゲーム制作 ( mesh_field.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "input.h"
#include "mesh_field.h"
#include "setup.h"
#include "wall.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_WIDTH		(150.0f)		// 幅の最大値
#define MAX_HEIGHT		(150.0f)		// 高さの最大値
#define MAX_DEPTH		(150.0f)		// 奥行きの最大値
#define MAX_SIZE		(50)			// サイズの最大値
#define MIN_SIZE		(1)				// サイズの最小値
#define START_SIZE		(30)			// サイズの最初の値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static LPDIRECT3DINDEXBUFFER9		s_pIdxBuff = NULL;		// インデックスバッファへのポインタ
static MeshField					s_mesh;					// メッシュフィールドの情報
static MeshFieldNumber				s_Number;				// メッシュフィールドの数系の情報
static int							*s_pIdx = { NULL };		// インデックスの配列

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void Input(void);
static void ResetBuff(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMeshField(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko000.jpg",
		&s_pTexture);

	// メモリのクリア
	memset(&s_mesh, 0, sizeof(s_mesh));
	memset(&s_Number, 0, sizeof(s_Number));

	// 横・縦の初期化
	s_Number.nHorizontal = START_SIZE;
	s_Number.nVertical = START_SIZE;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMeshField(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの解放
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pIdxBuff != NULL)
	{// インデックスバッファの解放
		s_pIdxBuff->Release();
		s_pIdxBuff = NULL;
	}

	if (s_pIdx != NULL)
	{// インデックスの配列の開放
		delete[] s_pIdx;
		s_pIdx = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateMeshField(void)
{
	if (GetDebug() == DEBUG_MESH)
	{// デバッグ表示がメッシュの時
		// 入力
		Input();

		if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_D) ||
			GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_S))
		{// A, D, W, Sキーが押された

			// 指定の値以上・以下
			Specified(&s_Number.nHorizontal, MAX_SIZE, MIN_SIZE);

			// 指定の値以上・以下
			Specified(&s_Number.nVertical, MAX_SIZE, MIN_SIZE);

			// バッファのリセット
			ResetBuff();

			// 設定
			SetMeshField();

			// 壁のリセット
			ResetWall();

			// 壁の設置
			InstallationWall();
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMeshField(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_mesh.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_mesh.rot.y, s_mesh.rot.x, s_mesh.rot.z);
	D3DXMatrixMultiply(&s_mesh.mtxWorld, &s_mesh.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_mesh.pos.x, s_mesh.pos.y, s_mesh.pos.z);
	D3DXMatrixMultiply(&s_mesh.mtxWorld, &s_mesh.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_mesh.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(s_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	// ポリゴン描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点バッファ
		0,							// インデックスの最小値
		s_Number.nVtx,				// 頂点数
		0,							// 描画する最初の頂点インデックス
		s_Number.nPolygon);			// プリミティブ(ポリゴン)数

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetMeshField(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nXLine = s_Number.nHorizontal + 1;
	int nZLine = s_Number.nVertical + 1;

	// 頂点数を計算
	s_Number.nVtx = nXLine * nZLine;

	// インデックス数を計算
	s_Number.nIdx = ((nXLine * 2) * s_Number.nVertical) + ((s_Number.nVertical - 1) * 2);

	// ポリゴン数を計算
	s_Number.nPolygon = (s_Number.nHorizontal * s_Number.nVertical * 2) + ((s_Number.nVertical - 1) * 4);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * s_Number.nVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// メモリのクリア
	memset(&s_mesh, 0, sizeof(s_mesh));

	// 幅・高さ・奥行きの設定
	s_mesh.fWidth = MAX_WIDTH * (s_Number.nHorizontal * 0.5f);
	s_mesh.fHeight = MAX_HEIGHT;
	s_mesh.fDepth = MAX_DEPTH * (s_Number.nVertical * 0.5f);

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < nZLine; i++)
	{
		for (int j = 0; j < nXLine; j++)
		{
			int nVtx = j + (i * nXLine);

			float fXPos = (float)(j - (s_Number.nHorizontal * 0.5f));
			float fZPos = (float)((i - (s_Number.nVertical * 0.5f)) * -1.0f);
			float fYPos = sinf(((j * 0.1f) * (D3DX_PI * 2.0f)));

			// 頂点座標の設定
			pVtx[nVtx].pos = D3DXVECTOR3(MAX_WIDTH * fXPos, MAX_HEIGHT * fYPos, MAX_DEPTH * fZPos);

			// 各頂点の法線の設定
			pVtx[nVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[nVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[nVtx].tex = D3DXVECTOR2((float)j, (float)i);
		}
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(VERTEX_3D) * s_Number.nIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		NULL);

	// メッシュに使用されているテクスチャ用の配列を用意する
	s_pIdx = new int[s_Number.nIdx];

	WORD *pIdx = NULL;		// インデックス情報へのポインタ

	// インデックスバッファをロック
	s_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	for (int x = 0, z = 0; z < s_Number.nVertical; x++, z++)
	{
		for (; x < (nXLine * (z + 1)) + z; x++)
		{
			pIdx[x * 2] = (WORD)(x - z + nXLine);
			pIdx[(x * 2) + 1] = (WORD)(x - z);
			s_pIdx[x * 2] = pIdx[x * 2];
			s_pIdx[(x * 2) + 1] = pIdx[(x * 2) + 1];
		}

		if (z < s_Number.nVertical - 1)
		{// これで終わりじゃないなら
			pIdx[x * 2] = (WORD)(x - (z + 1));
			pIdx[(x * 2) + 1] = (WORD)((x * 2) - (z * (s_Number.nHorizontal + 3)));
			s_pIdx[x * 2] = pIdx[x * 2];
			s_pIdx[(x * 2) + 1] = pIdx[(x * 2) + 1];
		}
	}

	// インデックスバッファをアンロックする
	s_pIdxBuff->Unlock();
}

//--------------------------------------------------
// 当たり判定
//--------------------------------------------------
void CollisionMeshField(D3DXVECTOR3 * pos)
{
	VERTEX_3D* pVtx = NULL;
	D3DXVECTOR3 vecField[3];	// ポリゴンの線分
	D3DXVECTOR3 vecModel[3];	// モデルからポリゴンの線分

	// 頂点座標をロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の反映
	for (int i = 0; i <= (s_Number.nIdx - 3); i++)
	{
		if (s_pIdx[i] == s_pIdx[i + 1] || s_pIdx[i + 1] == s_pIdx[i + 2] || s_pIdx[i] == s_pIdx[i + 2])
		{
			continue;
		}

		vecField[0] = pVtx[s_pIdx[i + 1]].pos - pVtx[s_pIdx[i]].pos;
		vecField[1] = pVtx[s_pIdx[i + 2]].pos - pVtx[s_pIdx[i + 1]].pos;
		vecField[2] = pVtx[s_pIdx[i]].pos - pVtx[s_pIdx[i + 2]].pos;

		vecModel[0] = *pos - pVtx[s_pIdx[i]].pos;
		vecModel[1] = *pos - pVtx[s_pIdx[i + 1]].pos;
		vecModel[2] = *pos - pVtx[s_pIdx[i + 2]].pos;

		float crs_v1 = Vec2Cross(&vecModel[0], &vecField[0]);
		float crs_v2 = Vec2Cross(&vecModel[1], &vecField[1]);
		float crs_v3 = Vec2Cross(&vecModel[2], &vecField[2]);

		// 乗ってるメッシュかチェック
		if (i % 2 == 0)
		{
			if (crs_v1 >= 0.0f && crs_v2 >= 0.0f && crs_v3 >= 0.0f)
			{
				D3DXVECTOR3 N;
				D3DXVec3Cross(&N, &vecField[0], &vecField[1]);

				if (N.y < 0.0f)
				{
					N *= -1.0f;
				}

				D3DXVec3Normalize(&N, &N);

				pos->y = pVtx[s_pIdx[i]].pos.y - 1.0f / N.y * (N.x * (pos->x - pVtx[s_pIdx[i]].pos.x) + N.z * (pos->z - pVtx[s_pIdx[i]].pos.z));
			}
		}
		else
		{
			if (crs_v1 <= 0.0f && crs_v2 <= 0.0f && crs_v3 <= 0.0f)
			{
				D3DXVECTOR3 N;
				D3DXVec3Cross(&N, &vecField[0], &vecField[1]);

				if (N.y < 0.0f)
				{
					N *= -1.0f;
				}

				D3DXVec3Normalize(&N, &N);

				pos->y = pVtx[s_pIdx[i]].pos.y - 1.0f / N.y * (N.x * (pos->x - pVtx[s_pIdx[i]].pos.x) + N.z * (pos->z - pVtx[s_pIdx[i]].pos.z));
			}
		}
	}

	// 頂点座標をアンロック
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
MeshField *GetMeshField(void)
{
	return &s_mesh;
}

//--------------------------------------------------
// 数系の取得
//--------------------------------------------------
MeshFieldNumber *GetMeshFieldNumber(void)
{
	return &s_Number;
}

//--------------------------------------------------
// 入力
//--------------------------------------------------
static void Input(void)
{
	if (GetKeyboardTrigger(DIK_A))
	{// Aキーが押された
		s_Number.nHorizontal++;
	}
	else if (GetKeyboardTrigger(DIK_D))
	{// Dキーが押された
		s_Number.nHorizontal--;
	}

	if (GetKeyboardTrigger(DIK_W))
	{// Wキーが押された
		s_Number.nVertical++;
	}
	else if (GetKeyboardTrigger(DIK_S))
	{// Sキーが押された
		s_Number.nVertical--;
	}
}

//--------------------------------------------------
// バッファのリセット
//--------------------------------------------------
static void ResetBuff(void)
{
	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pIdxBuff != NULL)
	{// インデックスバッファの解放
		s_pIdxBuff->Release();
		s_pIdxBuff = NULL;
	}

	if (s_pIdx != NULL)
	{// インデックスの配列の開放
		delete[] s_pIdx;
		s_pIdx = NULL;
	}
}

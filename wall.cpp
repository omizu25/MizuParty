//==================================================
// 
// 3Dゲーム制作 ( wall.cpp )
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
#define MAX_HEIGHT		(0.0f)			// 高さの最大値
#define MAX_DEPTH		(150.0f)		// 奥行きの最大値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static Wall							s_wall[MAX_WALL];		// 壁の情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitWall(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\InuiToko002.jpg",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//メモリのクリア
	memset(s_wall, 0, sizeof(s_wall));

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_WALL; i++)
	{
		//全ての初期化処理
		InitAll(pVtx);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitWall(void)
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
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateWall(void)
{
	for (int i = 0; i < MAX_WALL; i++)
	{
		Wall *pWall = &s_wall[i];

		if (!pWall->bUse)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		if (GetKeyboardPress(DIK_1))
		{// 1キーが押された
			pWall->rot.y += 0.01f;
		}
		else if (GetKeyboardPress(DIK_2))
		{// 2キーが押された
			pWall->rot.y -= 0.01f;
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawWall(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_WALL; i++)
	{
		Wall *pWall = &s_wall[i];

		if (!pWall->bUse)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pWall->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pWall->rot.y, pWall->rot.x, pWall->rot.z);
		D3DXMatrixMultiply(&pWall->mtxWorld, &pWall->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pWall->pos.x, pWall->pos.y, pWall->pos.z);
		D3DXMatrixMultiply(&pWall->mtxWorld, &pWall->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pWall->mtxWorld);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			i * 4,						// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, float fURight, bool bCulling)
{
	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	for (int i = 0; i < MAX_WALL; i++)
	{
		Wall *pWall = &s_wall[i];

		if (pWall->bUse)
		{//使用されている
			continue;
		}

		/*↓ 使用されていない ↓*/

		pWall->pos = pos;
		pWall->rot = rot;
		pWall->fWidth = fWidth;
		pWall->fHeight = fHeight;
		pWall->bUse = true;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		// 頂点座標の設定
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		if (bCulling)
		{
			// 頂点カラーの設定
			Setcol(pVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// 頂点カラーの設定
			Setcol(pVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}

		// テクスチャの設定
		Settex(pVtx, 0.0f, fURight, 0.0f, 1.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// 設置
//--------------------------------------------------
void InstallationWall(void)
{
	MeshField *pMeshField = GetMeshField();					//メッシュフィールドの取得
	MeshFieldNumber *pNumber = GetMeshFieldNumber();		//メッシュフィールドの数系の取得

	float fWidth = pMeshField->fWidth;
	float fHeight = pMeshField->fWidth * 0.4f;
	float fDepth = pMeshField->fDepth;

	// 壁の設定

	/*↓ 内側 ↓*/

	SetWall(D3DXVECTOR3(0.0f, fHeight, -fDepth), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), fWidth, fHeight, (float)pNumber->nHorizontal, true);
	SetWall(D3DXVECTOR3(0.0f, fHeight, fDepth), D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, (float)pNumber->nHorizontal, true);
	SetWall(D3DXVECTOR3(-fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, (float)pNumber->nVertical, true);
	SetWall(D3DXVECTOR3(fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, (float)pNumber->nVertical, true);

	/*↓ 外側 ↓*/

	/*SetWall(D3DXVECTOR3(0.0f, fHeight, fDepth), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), fWidth, fHeight, (float)pNumber->nHorizontal, false);
	SetWall(D3DXVECTOR3(0.0f, fHeight, -fDepth), D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, (float)pNumber->nHorizontal, false);
	SetWall(D3DXVECTOR3(fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, (float)pNumber->nVertical, false);
	SetWall(D3DXVECTOR3(-fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), fDepth, fHeight, (float)pNumber->nVertical, false);*/
}

//--------------------------------------------------
// 当たり判定
//--------------------------------------------------
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	for (int i = 0; i < MAX_WALL; i++)
	{
		Wall *pWall = &s_wall[i];

		if (!pWall->bUse)
		{// 使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		D3DXMATRIX mtxWorld[VTX];
		D3DXVECTOR3 vtx[VTX];

		for (int j = 0; j < VTX; j++)
		{
			D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld[j]);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pVtx[j].pos.x, pVtx[j].pos.y, pVtx[j].pos.z);
			D3DXMatrixMultiply(&mtxWorld[j], &mtxWorld[j], &mtxTrans);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pWall->rot.y, pWall->rot.x, pWall->rot.z);
			D3DXMatrixMultiply(&mtxWorld[j], &mtxWorld[j], &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pWall->pos.x, pWall->pos.y, pWall->pos.z);
			D3DXMatrixMultiply(&mtxWorld[j], &mtxWorld[j], &mtxTrans);

			// ワールド座標行列の設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld[j]);

			vtx[j] = D3DXVECTOR3(mtxWorld[j]._41, mtxWorld[j]._42, mtxWorld[j]._43);
		}

		D3DXVECTOR3 vecWall[VTX];

		// 壁のベクトル
		for (int j = 0, k = 0; j < VTX; j++, k++)
		{
			if (k == (VTX / 2))
			{// 折り返しに来た
				k = (k + 1) * -1;
			}

			vecWall[j] = vtx[k + 1] - vtx[j];
		}

		// 移動量のベクトル
		D3DXVECTOR3 vecMove = *pPos - *pPosOld;

		// 移動量を正規化
		D3DXVec3Normalize(&vecMove, &vecMove);

		vecWall[0] -= vecMove * size.x;
		
		// プレイヤーの位置までのベクトル
		D3DXVECTOR3 vecPos = *pPos - vtx[0];

		// 外積計算
		float fVecLine = Vec2Cross(&vecPos, &vecWall[0]);

		if (fVecLine <= 0.0f)
		{// 壁に当たってる
			// プレイヤーの前回の位置からのベクトル
			D3DXVECTOR3 vecPosOld = vtx[0] - *pPosOld;

			// 法線
			D3DXVECTOR3 nor = *D3DXVec3Cross(&nor, &vecWall[3], &vecWall[2]);

			// 法線を正規化
			D3DXVec3Normalize(&nor, &nor);

			// 移動量の逆向きのベクトル
			D3DXVECTOR3 ReverseMove = vecMove * -1.0f;

			// 壁に垂直なベクトル
			D3DXVECTOR3 vecNor = nor * Vec2Dot(&ReverseMove, &nor);

			float fVec[2], fMove;		// 計算用

			// 外積計算
			fVec[0] = Vec2Cross(&vecPosOld, &vecWall[0]);
			fVec[1] = Vec2Cross(&vecMove, &vecWall[0]);

			// 移動量
			fMove = fVec[0] / fVec[1];

			pPos->x = pPosOld->x + ((vecMove.x * fMove) + vecNor.x);
			pPos->z = pPosOld->z + ((vecMove.z * fMove) + vecNor.z);
		}

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Wall *GetWall(void)
{
	return &s_wall[0];
}

//--------------------------------------------------
// リセット
//--------------------------------------------------
void ResetWall(void)
{
	//メモリのクリア
	memset(s_wall, 0, sizeof(s_wall));
}

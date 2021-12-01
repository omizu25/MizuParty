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
#include "polygon.h"
#include "setup.h"
#include "wall.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_WALL		(256)		//壁の最大数

//--------------------------------------------------
// 壁の構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	bool			bUse;			// 使用しているかどうか
}Wall;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファのポインタ
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
	memset(s_wall, NULL, sizeof(s_wall));

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_WALL; i++)
	{
		//全ての初期化処理
		InitAll3D(pVtx);

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

		// ポリゴンの描画 四角
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
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, bool bCulling)
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
		pWall->bUse = true;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		// 頂点座標の設定
		Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		if (bCulling)
		{
			// 頂点カラーの設定
			Setcol3D(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			// 頂点カラーの設定
			Setcol3D(pVtx, 1.0f, 1.0f, 1.0f, 0.5f);
		}

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
	polygon *pPolygon = GetPolygon();

	float fWidth = pPolygon->fWidth;
	float fHeight = pPolygon->fWidth * 0.4f;

	// 壁の設定

	SetWall(D3DXVECTOR3(0.0f, fHeight, -fWidth), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), fWidth, fHeight, true);
	SetWall(D3DXVECTOR3(0.0f, fHeight, fWidth), D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, true);
	SetWall(D3DXVECTOR3(-fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), fWidth, fHeight, true);
	SetWall(D3DXVECTOR3(fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), fWidth, fHeight, true);

	SetWall(D3DXVECTOR3(0.0f, fHeight, fWidth), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), fWidth, fHeight, false);
	SetWall(D3DXVECTOR3(0.0f, fHeight, -fWidth), D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, false);
	SetWall(D3DXVECTOR3(fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), fWidth, fHeight, false);
	SetWall(D3DXVECTOR3(-fWidth, fHeight, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), fWidth, fHeight, false);
}
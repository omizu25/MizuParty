//==================================================
// 
// 3Dゲーム制作 ( billboard.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "setup.h"
#include "billboard.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_WALL		(256)		//ビルボードの最大数

//--------------------------------------------------
// ビルボードの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	bool			bUse;			// 使用しているかどうか
}Billboard;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファのポインタ
static Billboard					s_billboard[MAX_WALL];		// ビルボードの情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitBillboard(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Inui Toko 003.jpg",
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
	memset(&s_billboard[0], NULL, sizeof(s_billboard));

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
void UninitBillboard(void)
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
void UpdateBillboard(void)
{

}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawBillboard(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 計算用マトリックス

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_WALL; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (!pBillboard->bUse)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pBillboard->mtxWorld);
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラの逆行列を設定
		pBillboard->mtxWorld._11 = mtxView._11;
		pBillboard->mtxWorld._12 = mtxView._21;
		pBillboard->mtxWorld._13 = mtxView._31;
		pBillboard->mtxWorld._21 = mtxView._12;
		pBillboard->mtxWorld._22 = mtxView._22;
		pBillboard->mtxWorld._23 = mtxView._32;
		pBillboard->mtxWorld._31 = mtxView._13;
		pBillboard->mtxWorld._32 = mtxView._23;
		pBillboard->mtxWorld._33 = mtxView._33;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);
		D3DXMatrixMultiply(&pBillboard->mtxWorld, &pBillboard->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pBillboard->mtxWorld);

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
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	for (int i = 0; i < MAX_WALL; i++)
	{
		Billboard *pBillboard = &s_billboard[i];

		if (pBillboard->bUse)
		{//使用されている
			continue;
		}

		/*↓ 使用されていない ↓*/

		pBillboard->pos = pos;
		pBillboard->bUse = true;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		// 頂点座標の設定
		Setpos3D(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
// 設置
//--------------------------------------------------
void InstallationBillboard(void)
{
	float fWidth = 10.0f;
	float fHeight = 10.0f;

	// ビルボードの設定
	SetBillboard(D3DXVECTOR3(25.0f, fHeight, 0.0f), fWidth, fHeight);
	SetBillboard(D3DXVECTOR3(-25.0f, fHeight, 0.0f), fWidth, fHeight);
	SetBillboard(D3DXVECTOR3(75.0f, fHeight, 0.0f), fWidth, fHeight);
	SetBillboard(D3DXVECTOR3(-75.0f, fHeight, 0.0f), fWidth, fHeight);
}
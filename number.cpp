//==================================================
// 
// 3Dゲーム制作 ( number.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "setup.h"
#include "number.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_NUMBER		(256)		// 数の最大数
#define MAX_TEX			(10)		// texの最大数

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	int				nNumber;		// 数
	int				nDigit;			// 桁数
	USE				use;			// 使用者
	bool			bUse;			// 使用しているかどうか
}Number2D;

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	int				nNumber;		// 数
	int				nDigit;			// 桁数
	bool			bUse;			// 使用しているかどうか
}Number3D;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff2D = NULL;		// 頂点バッファのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff3D = NULL;		// 頂点バッファのポインタ
static Number2D						s_Number2D[MAX_NUMBER];		// 数の情報 [2D]
static Number3D						s_Number3D[MAX_NUMBER];		// 数の情報 [3D]

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void Init2D(void);
static void Init3D(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitNumber(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\number000.png",
		&s_pTexture);

	// メモリのクリア
	memset(&s_Number2D, NULL, sizeof(s_Number2D));

	// メモリのクリア
	memset(&s_Number3D, NULL, sizeof(s_Number3D));

	// 初期化 [2D]
	Init2D();

	// 初期化 [3D]
	Init3D();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitNumber(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの破棄
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff2D != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuff2D->Release();
		s_pVtxBuff2D = NULL;
	}

	if (s_pVtxBuff3D != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuff3D->Release();
		s_pVtxBuff3D = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateNumber(void)
{
	
}

//--------------------------------------------------
// 描画 [2D]
//--------------------------------------------------
void DrawNumber2D(USE use)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff2D, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		if (s_Number2D[i].bUse && s_Number2D[i].use == use)
		{// 数が使用されている
			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				i * 4,						// 描画する最初の頂点インデックス
				2);							// プリミティブ(ポリゴン)数
		}
	}
}

//--------------------------------------------------
// 描画 [3D]
//--------------------------------------------------
void DrawNumber3D(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 計算用マトリックス

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff3D, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number3D *pNumber3D = &s_Number3D[i];

		if (!pNumber3D->bUse)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pNumber3D->mtxWorld);
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラの逆行列を設定
		pNumber3D->mtxWorld._11 = mtxView._11;
		pNumber3D->mtxWorld._12 = mtxView._21;
		pNumber3D->mtxWorld._13 = mtxView._31;
		pNumber3D->mtxWorld._21 = mtxView._12;
		pNumber3D->mtxWorld._22 = mtxView._22;
		pNumber3D->mtxWorld._23 = mtxView._32;
		pNumber3D->mtxWorld._31 = mtxView._13;
		pNumber3D->mtxWorld._32 = mtxView._23;
		pNumber3D->mtxWorld._33 = mtxView._33;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pNumber3D->pos.x, pNumber3D->pos.y, pNumber3D->pos.z);
		D3DXMatrixMultiply(&pNumber3D->mtxWorld, &pNumber3D->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pNumber3D->mtxWorld);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			i * 4,						// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zバッファの値を元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// 新規深度値 <= Zバッファ深度値 (初期設定)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------------
// 設定 (右に中心)
// 1.位置 2.幅 3.高さ 4.数 5.桁数
//--------------------------------------------------
void SetNumber3D(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit)
{
	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number3D *pNumber3D = &s_Number3D[i];

		if (pNumber3D->bUse)
		{// 数が使用されている
			continue;
		}

		/*↓ 数が使用されていない ↓*/

		pNumber3D->pos = pos;
		pNumber3D->fWidth = fWidth;
		pNumber3D->fHeight = fHeight;
		pNumber3D->nNumber = nNumber;
		pNumber3D->nDigit = nDigit;
		pNumber3D->bUse = true;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff3D->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// 該当の位置まで進める

		// 頂点座標の設定
		Setpos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 0.0f, SETPOS_MIDDLE);

		// 頂点の法線の設定
		Setnor(pVtx, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		// テクスチャ座標の設定
		Settex(pVtx, fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff3D->Unlock();

		break;
	}
}

//--------------------------------------------------
// 設定 (右に中心)
// 1.位置 2.色 3.幅 4.高さ 5.数 6.桁数 7.使用者
//--------------------------------------------------
void SetRightNumber(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber, int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number2D *pNumber2D = &s_Number2D[i];

		if (pNumber2D->bUse)
		{// 数が使用されている
			continue;
		}

		/*↓ 数が使用されていない ↓*/

		pNumber2D->pos = pos;
		pNumber2D->fWidth = fWidth;
		pNumber2D->fHeight = fHeight;
		pNumber2D->nNumber = nNumber;
		pNumber2D->nDigit = nDigit;
		pNumber2D->use = use;
		pNumber2D->bUse = true;

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff2D->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// 該当の位置まで進める

		// 頂点座標の設定
		Setpos(pVtx, pos, fWidth, fHeight, SETPOS_RIGHT);

		// 頂点カラーの設定
		Setcol(pVtx, col);

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;
		
		// テクスチャ座標の設定
		Settex(pVtx, 0.0f + fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff2D->Unlock();

		break;
	}
}

//--------------------------------------------------
// 設定 (真ん中に中心)
// 1.位置 2.色 3.幅 4.高さ 5.数 6.桁数 7.順位
//--------------------------------------------------
void SetMiddleNumber(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber, int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number2D *pNumber2D = &s_Number2D[i];

		if (pNumber2D->bUse)
		{// 数が使用されている
			continue;
		}

		/*↓ 数が使用されていない ↓*/

		pNumber2D->pos = pos;
		pNumber2D->fWidth = fWidth;
		pNumber2D->fHeight = fHeight;
		pNumber2D->nNumber = nNumber;
		pNumber2D->nDigit = nDigit;
		pNumber2D->use = use;
		pNumber2D->bUse = true;

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff2D->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// 該当の位置まで進める

		// 頂点座標の設定
		Setpos(pVtx, pos, fWidth, fHeight, SETPOS_MIDDLE);

		// 頂点カラーの設定
		Setcol(pVtx, col);

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		// テクスチャ座標の設定
		Settex(pVtx, fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff2D->Unlock();

		break;
	}
}

//--------------------------------------------------
// テクスチャ
//--------------------------------------------------
void TexNumber(int nNumber, int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number2D *pNumber2D = &s_Number2D[i];

		if (!pNumber2D->bUse || pNumber2D->nDigit != nDigit || pNumber2D->use != use)
		{// 数が使用されていない、桁数が違う
			continue;
		}

		/*↓ 数が使用されている、桁数が同じ ↓*/

		pNumber2D->nNumber = nNumber;

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff2D->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// 該当の位置まで進める

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		// テクスチャ座標の設定
		Settex(pVtx, fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff2D->Unlock();

		break;
	}
}

//--------------------------------------------------
//数の使用処理
//--------------------------------------------------
void UseNumber(int nDigit, USE use)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number2D *pNumber2D = &s_Number2D[i];

		if (!pNumber2D->bUse || pNumber2D->nDigit != nDigit || pNumber2D->use != use)
		{//数が使用されていない、桁数が違う
			continue;
		}

		//数が使用されている、桁数が同じ
		pNumber2D->bUse = false;

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff2D->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		// 該当の位置まで進める

		// 頂点カラーの初期化
		Initcol(pVtx);

		// 頂点バッファをアンロックする
		s_pVtxBuff2D->Unlock();
	}
}

//--------------------------------------------------
// 初期化 [2D]
//--------------------------------------------------
static void Init2D(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff2D,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		// 全ての初期化
		InitAll(pVtx);

		pVtx += 4;		// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff2D->Unlock();
}

//--------------------------------------------------
// 初期化 [3D]
//--------------------------------------------------
static void Init3D(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff3D,
		NULL);

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff3D->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		//全ての初期化処理
		InitAll(pVtx);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff3D->Unlock();
}

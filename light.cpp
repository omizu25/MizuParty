//==================================================
// 
// 3Dゲーム制作 ( light.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "light.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_LIGHT		(3)		//ライトの最大数

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static D3DLIGHT9		s_light[MAX_LIGHT];		// ライトの情報

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void SetLight(D3DXCOLOR col, D3DXVECTOR3 vec, int nNum);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitLight(void)
{
	// ライトをクリアする
	ZeroMemory(&s_light[0], sizeof(s_light));

	// ライトの設定
	SetLight(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.2f, -0.8f, 0.4f), 0);
	SetLight(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f), D3DXVECTOR3(0.2f, -0.8f, -0.4f), 1);
	SetLight(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), D3DXVECTOR3(-0.2f, 0.8f, 0.4f), 2);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitLight(void)
{
	
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateLight(void)
{

}

//--------------------------------------------------
// 設定
//--------------------------------------------------
static void SetLight(D3DXCOLOR col, D3DXVECTOR3 vec, int nNum)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR3 vecDir;		// ライトの方向ベクトル

	// ライトの種類を設定 ( 平行光源 )
	s_light[nNum].Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定 ( ライトの色 )
	s_light[nNum].Diffuse = col;

	// ライトの方向を設定
	vecDir = vec;

	// 正規化する ( 大きさ 1 のベクトルにする )
	D3DXVec3Normalize(&vecDir, &vecDir);
	s_light[nNum].Direction = vecDir;

	// ライトを設定する
	pDevice->SetLight(nNum, &s_light[nNum]);

	// ライトを有効にする
	pDevice->LightEnable(nNum, TRUE);
}

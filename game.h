//==================================================
//
// 3Dゲーム制作 ( game.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _GAME_H_		// このマクロ定義がされてなかったら
#define _GAME_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 列挙型
//--------------------------------------------------
typedef enum
{
	GAMESTATE_NONE = 0,		// 何もしていない状態
	GAMESTATE_START,		// 開始状態 (ゲーム開始中)
	GAMESTATE_NORMAL,		// 通常状態 (ゲーム進行中)
	GAMESTATE_END,			// 終了状態 (ゲーム終了時)
	GAMESTATE_RESULT,		// リザルト状態 (ゲーム終了後)
	GAMESTATE_MAX
}GAMESTATE;

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	GAMESTATE		gameState;		// ゲームの状態
	bool			bWireframe;		// ワイヤーフレーム表示 [表示  : true 非表示  : false]
	bool			bPause;			// ポーズ [する  : true しない  : false]
}Game;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
Game GetGame(void);

#endif // !_GAME_H_

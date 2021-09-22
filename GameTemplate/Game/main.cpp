#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "GameTime.h"
#include "StopWatch.h"
#include "SoundEngine.h"
#include "LightManager.h"

using namespace nsMyGame;

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//ゲームの生成
	NewGO<nsGame::CGame>(nsCommonData::enPriorityFirst);

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		// エンジンのゲームループ
		nsMyEngine::CMyEngine::GetInstance()->ExecuteGameLoop();

	}


	return 0;
}


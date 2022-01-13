#include "stdafx.h"
#include "system/system.h"
#include "Game.h"

using namespace nsNinjaAttract;

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//ゲームの生成
	nsGame::CGame* game = NewGO<nsGame::CGame>(nsCommonData::enPriorityFirst);

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		// エンジンのゲームループ
		nsMyEngine::CMyEngine::GetInstance()->ExecuteGameLoop();
	}

	// ゲームの破棄
	DeleteGO(game);

	// 事前破棄処理
	nsNinjaAttract::nsMyEngine::CMyEngine::GetInstance()->PreDelete();

	// ゲームを破棄するために、オブジェクトマネージャーによるアップデートの実行
	GameObjectManager::GetInstance()->ExecuteUpdate();

	// エンジンのインスタンスを破棄する
	nsNinjaAttract::nsMyEngine::CMyEngine::DeleteInstance();


	return 0;
}


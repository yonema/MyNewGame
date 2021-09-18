#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "GameTime.h"
#include "StopWatch.h"
#include "SoundEngine.h"

using namespace nsMyGame;

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	EffectEngine::CreateInstance();
	nsSound::CSoundEngine::CreateInstance();

	//ストップウォッチを生成する
	nsTimer::CStopWatch stopWatch;
	
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//ゲームの生成
	NewGO<nsGame::CGame>(nsCommonData::enPriorityFirst);

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//ストップウォッチ計測開始
		stopWatch.Start();

		//レンダリング開始。
		g_engine->BeginFrame();
		

		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
		EffectEngine::GetInstance()->Update(nsTimer::GameTime().GetFrameDeltaTime());
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		EffectEngine::GetInstance()->Draw();

#ifdef MY_DEBUG
		//FPSを描画する
		nsTimer::GameTime().DrawFPS(renderContext, (float)stopWatch.GetElapsed());
#endif
		
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();

		//FPS固定の処理
		//スピンロックを行う。
		int restTime = 0;
		do {
			stopWatch.Stop();
			restTime = nsTimer::STOP_WATCH_CONST_DATA::MILLISECOND_FOR_LOCK_60FPS -
				static_cast<int>(stopWatch.GetElapsedMillisecond());
		} while (restTime > 0);

		//ストップウォッチ計測終了
		stopWatch.Stop();

		//デルタタイムをストップウォッチの計測時間から、計算する
		nsTimer::GameTime().PushFrameDeltaTime((float)stopWatch.GetElapsed());
	}

	nsSound::CSoundEngine::DeleteInstance();
	EffectEngine::DeleteInstance();
	PhysicsWorld::DeleteInstance();
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}


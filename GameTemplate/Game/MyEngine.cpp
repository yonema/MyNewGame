#include "stdafx.h"
#include "MyEngine.h"
#include "GameTime.h"
#include "RenderingEngine.h"
#include "SoundEngine.h"
#include "LightManager.h"
#include "StringActionTargetManager.h"
#include "system/system.h"
#include "DebugManager.h"

namespace nsNinjaAttract
{
	/**
	 * @brief このゲーム用のゲームエンジン。TkEngineをラップする
	 * @note シングルトンパターンを使用
	*/
	namespace nsMyEngine
	{
		CMyEngine* CMyEngine::m_instance = nullptr;	// 唯一のインスタンス

		/**
		 * @brief コンストラクタ
		*/
		CMyEngine::CMyEngine()
		{
			// tkEngineの生成と初期化
			m_tkEngine.reset(new TkEngine);
			m_tkEngine->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);

			// 各種インスタンスを生成
			CreateInstances();

			// ストップウォッチの生成
			m_stopWatch.reset(new nsTimer::CStopWatch);

			return;
		}

		/**
		 * @brief デストラクタ
		*/
		CMyEngine::~CMyEngine()
		{
			// 各種インスタンスを破棄
			DeleteInstances();

			return;
		}


		/**
		 * @brief 各種インスタンスを生成
		*/
		void CMyEngine::CreateInstances()
		{
			GameObjectManager::CreateInstance();		// ゲームオブジェクトマネージャーを生成
			PhysicsWorld::CreateInstance();				// 物理ワールドを生成
			nsLight::CLightManager::CreateInstance();	// ライトマネージャーを生成
			CRenderingEngine::CreateInstance();			// レンダリングエンジンの生成
			EffectEngine::CreateInstance();				// エフェクトエンジンを生成
			nsSound::CSoundEngine::CreateInstance();	// サウンドエンジンを生成
			// スイングのターゲットのマネージャーを生成
			nsStringActionTarget::CStringActionTargetManager::CreateInstance();
			nsDebug::CDebugManager::CreateInstance();	// デバックマネージャーを生成

			return;
		}

		/**
		 * @brief 各種インスタンスを破棄
		*/
		void CMyEngine::DeleteInstances()
		{
			nsDebug::CDebugManager::DeleteInstance();	// デバックマネージャーを破棄
			// スイングのターゲットのマネージャーを破棄
			nsStringActionTarget::CStringActionTargetManager::DeleteInstance();
			nsSound::CSoundEngine::DeleteInstance();	// サウンドエンジンを破棄
			EffectEngine::DeleteInstance();				// エフェクトエンジンを破棄
			CRenderingEngine::DeleteInstance();			// レンダリングエンジンの破棄
			nsLight::CLightManager::DeleteInstance();	// ライトマネージャーを破棄
			PhysicsWorld::DeleteInstance();				// 物理ワールドを破棄
			GameObjectManager::DeleteInstance();		// ゲームオブジェクトマネージャーを破棄

			return;
		}

		/**
		 * @brief ゲームループを実行する
		*/
		void CMyEngine::ExecuteGameLoop()
		{
			// tkEngineのフレームの最初の処理
			m_tkEngine->BeginFrame();

			// 各種アップデートを実行させる
			ExecuteUpdate();

			// レンダリングエンジンを実行
			CRenderingEngine::GetInstance()->Execute(*m_stopWatch);

			// スピンロックによるFPS固定処理
			Spinlock();

			//ストップウォッチ計測終了
			m_stopWatch->Stop();
			//デルタタイムをストップウォッチの計測時間から、計算する
			nsTimer::GameTime().PushFrameDeltaTime(static_cast<float>(m_stopWatch->GetElapsed()));

			//ストップウォッチ計測開始
			m_stopWatch->Start();

			// tkEngineのフレームの最後の処理
			m_tkEngine->EndFrame();

			return;
		}

		/**
		 * @brief 各種アップデートを実行させる
		*/
		void CMyEngine::ExecuteUpdate()
		{
			// オブジェクトマネージャーによるアップデートの実行
			GameObjectManager::GetInstance()->ExecuteUpdate();
			// エフェクトのアップデート
			EffectEngine::GetInstance()->Update(nsTimer::GameTime().GetFrameDeltaTime());
			// ライトマネージャーのアップデート
			nsLight::CLightManager::GetInstance()->Update();

			return;
		}

		/**
		 * @brief スピンロックによるFPS固定処理
		*/
		void CMyEngine::Spinlock()
		{
			// スピンロックを行う
			int restTime = 0;
			do {
				m_stopWatch->Stop();
				restTime = nsTimer::STOP_WATCH_CONST_DATA::MILLISECOND_FOR_LOCK_60FPS -
					static_cast<int>(m_stopWatch->GetElapsedMillisecond());
			} while (restTime > 0);

			return;
		}


		/**
		 * @brief 事前の破棄処理
		*/
		void CMyEngine::PreDelete()
		{
			nsDebug::CDebugManager::GetInstance()->PreDelete();
			CRenderingEngine::GetInstance()->PreDelete();

			return;
		}
	}
}

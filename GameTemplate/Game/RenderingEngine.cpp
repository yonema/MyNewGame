#include "stdafx.h"
#include "RenderingEngine.h"
#include "system/system.h"

namespace nsMyGame
{

	/**
	 * @brief このゲーム用のゲームエンジンネームスペース
	*/
	namespace nsMyEngine
	{
		CRenderingEngine* CRenderingEngine::m_instance = nullptr;	// 唯一のインスタンス

		/**
		 * @brief コンストラクタ
		*/
		CRenderingEngine::CRenderingEngine()
		{

			return;
		}
		/**
		 * @brief デストラクタ
		*/
		CRenderingEngine::~CRenderingEngine()
		{
			return;
		}

		/**
		 * @brief レンダリングエンジンを実行
		*/
		void CRenderingEngine::Execute()
		{
			// レンダリングコンテキスト
			RenderContext& rc = g_graphicsEngine->GetRenderContext();

			// ゲームオブジェクトの描画
			GameObjectManager::GetInstance()->ExecuteRender(rc);
			// エフェクトの描画
			EffectEngine::GetInstance()->Draw();

			return;
		}
	}
}
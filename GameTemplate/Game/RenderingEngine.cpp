#include "stdafx.h"
#include "RenderingEngine.h"
#include "system/system.h"
#include "Render.h"

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
			m_renderObjects.reserve(m_kRenderObjectsCapacityNum);

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

			// 描画オブジェクトの登録
			GameObjectManager::GetInstance()->ExecuteAddRender();

			// GBufferに描画する
			RenderToGBuffer(rc);

			// エフェクトの描画
			EffectEngine::GetInstance()->Draw();

			// 2Dを描画する
			Render2D(rc);

			m_renderObjects.clear();

			return;
		}

		/**
		 * @brief GBufferに描画する
		 * @param rc レンダリングコンテキスト
		*/
		void CRenderingEngine::RenderToGBuffer(RenderContext& rc)
		{
			for (nsGraphic::CRender* renderObject : m_renderObjects)
			{
				renderObject->OnRenderToGBuffer(rc);
			}
			return;
		}

		/**
		 * @brief 2Dを描画する
		 * @param rc レンダリングコンテキスト
		*/
		void CRenderingEngine::Render2D(RenderContext& rc)
		{
			for (nsGraphic::CRender* renderObject : m_renderObjects)
			{
				renderObject->OnRender2D(rc);
			}
			return;
		}
	}
}
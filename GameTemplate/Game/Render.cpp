#include "stdafx.h"
#include "Render.h"

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief GBufferに描画する関数を実行
		 * @param[in] rc レンダリングコンテキスト
		*/
		void CRender::OnRenderToGBuffer(RenderContext& rc)
		{
			m_onRenderToGBufferFunc(rc);

			return;
		}

		/**
		 * @brief フォワードレンダリングする関数を実行
		 * @param[in] rc レンダリングコンテキスト
		*/
		void CRender::OnForwardRender(RenderContext& rc)
		{
			m_onForwardRenderFunc(rc);

			return;
		}

		/**
		 * @brief 2Dを描画をする関数を実行
		 * @param[in] rc レンダリングコンテキスト
		*/
		void CRender::OnRender2D(RenderContext& rc)
		{
			m_onRender2DFunc(rc);

			return;
		}
	}
}
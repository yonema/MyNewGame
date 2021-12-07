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
		 * @brief シャドウマップを描画する関数を実行
		 * @param[in] rc レンダリングコンテキスト
		 * @param[in] ligNo ライトの番号
		 * @param[in] shadowMapNo シャドウマップの番号
		 * @param[in] lvpMatrix ライトビュープロジェクション行列
		*/
		void CRender::OnRenderShadowMap(
			RenderContext& rc,
			const int ligNo,
			const int shadowMapNo,
			const Matrix& lvpMatrix
		)
		{
			m_onRenderShadowMapFunc(rc, ligNo, shadowMapNo, lvpMatrix);

			return;
		}

		/**
		 * @brief プレイヤー専用のシャドウマップを描画する関数を実行
		 * @param[in] rc レンダリングコンテキスト
		 * @param[in] ligNo ライトの番号
		 * @param[in] lvpMatrix ライトビュープロジェクション行列
		*/
		void CRender::OnRenderPlayerShadowMap(RenderContext& rc, const int ligNo, const Matrix& lvpMatrix)
		{
			m_onRenderPlayerShadowMapFunc(rc, ligNo, lvpMatrix);
		}

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
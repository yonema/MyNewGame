#include "stdafx.h"
#include "PostEffect.h"

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief ポストエフェクト関連のネームスペース
		*/
		namespace nsPostEffect
		{
			/**
			 * @brief 初期化関数
			 * @param mainRenderTarget
			*/
			void CPostEffect::Init(RenderTarget& mainRenderTarget)
			{
				// ブルームを初期化
				m_bloom.OnInit(mainRenderTarget);

				return;
			}


			/**
			 * @brief 描画関数
			 * @param[in] rc レンダリンコンテキスト
			 * @param[in] mainRenderTarget メインレンダリングターゲット
			*/
			void CPostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
			{
				// ブルームを描画
				m_bloom.OnRender(rc, mainRenderTarget);


				return;
			}
		}
	}
}
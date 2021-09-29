#pragma once

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief レンダラークラス
		*/
		class CRender
		{
		private:	// エイリアス宣言
			using RenderFunc = std::function<void(RenderContext&)>;		// レンダリング用関数の型
			// シャドウマップの描画用の関数の型
			using RenderShadowMapFunc = std::function<void(
				RenderContext&,
				const int ligNo,
				const int shadowMapNo,
				const Matrix& lvpMatrix
				)>;

		public:		// コンストラクタとデストラクタ
			CRender() = default;	// コンストラクタ
			~CRender() = default;	// デストラクタ

		public:		// メンバ関数

			/**
			 * @brief シャドウマップに描画する関数を設定する
			 * @param fundtion シャドウマップに描画する関数
			*/
			void SetOnShadwMapRender(const RenderShadowMapFunc& fundtion)
			{
				m_onRenderShadowMapFunc = fundtion;
			}

			/**
			 * @brief GBufferに描画する関数を設定する
			 * @param[in] function GBufferに描画する関数
			*/
			void SetOnRenderToBGuuferFunc(const RenderFunc& function)
			{
				m_onRenderToGBufferFunc = function;
			}

			/**
			 * @brief フォワードレンダリングをする関数を設定する
			 * @param[in] function フォワードレンダリングをする関数
			*/
			void SetOnForwardRenderFunc(const RenderFunc& function)
			{
				m_onForwardRenderFunc = function;
			}

			/**
			 * @brief 2Dを描画をする関数を設定
			 * @param[in] function 2Dを描画をする関数
			*/
			void SetOnRender2DFunc(const RenderFunc& function)
			{
				m_onRender2DFunc = function;
			}

			/**
			 * @brief シャドウマップを描画する関数を実行
			 * @param[in] rc レンダリングコンテキスト
			 * @param[in] ligNo ライトの番号
			 * @param[in] shadowMapNo シャドウマップの番号
			 * @param[in] lvpMatrix ライトビュープロジェクション行列
			*/
			void OnRenderShadowMap(RenderContext& rc,const int ligNo, const int shadowMapNo, const Matrix& lvpMatrix);

			/**
			 * @brief GBufferに描画する関数を実行
			 * @param[in] rc レンダリングコンテキスト
			*/
			void OnRenderToGBuffer(RenderContext& rc);

			/**
			 * @brief フォワードレンダリングする関数を実行
			 * @param[in] rc レンダリングコンテキスト
			*/
			void OnForwardRender(RenderContext& rc);

			/**
			 * @brief 2Dを描画をする関数を実行
			 * @param[in] rc レンダリングコンテキスト
			*/
			void OnRender2D(RenderContext& rc);

		private:
			//!< シャドウマップを描画する関数
			RenderShadowMapFunc m_onRenderShadowMapFunc = [](RenderContext&,const int, const int, const Matrix& ) {};
			RenderFunc m_onRenderToGBufferFunc = [](RenderContext&) {};	//!< GBufferに描画する関数
			RenderFunc m_onForwardRenderFunc = [](RenderContext&) {};	//!< フォワードレンダリングをする関数
			RenderFunc m_onRender2DFunc = [](RenderContext&) {};		//!< 2Dを描画をする関数
		};
	}
}



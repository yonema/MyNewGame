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

		public:		// コンストラクタとデストラクタ
			CRender() = default;	// コンストラクタ
			~CRender() = default;	// デストラクタ

		public:		// メンバ関数

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
			RenderFunc m_onRenderToGBufferFunc = [](RenderContext&) {};	// GBufferに描画する関数
			RenderFunc m_onForwardRenderFunc = [](RenderContext&) {};	// フォワードレンダリングをする関数
			RenderFunc m_onRender2DFunc = [](RenderContext&) {};		// 2Dを描画をする関数
		};
	}
}



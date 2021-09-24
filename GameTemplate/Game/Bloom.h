#pragma once
#include "PostEffectConstData.h"
#include "GaussianBlur.h"

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
			 * @brief ブルームクラス
			*/
			class CBloom : private nsUtil::Noncopyable
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CBloom() = default;
				/**
				 * @brief デストラクタ
				*/
				~CBloom() = default;

			public:		// メンバ関数

				/**
				 * @brief 初期化関数
				 * @param[in] mainRenderTarget メインレンダリングターゲット
				*/
				void OnInit(RenderTarget& mainRenderTarget);

				/**
				 * @brief 描画関数
				 * @param[in] rc レンダリングコンテキスト
				 * @param[in] mainRenderTarget メインレンダリングターゲット
				*/
				void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget);

			private:	// privateなメンバ関数

				/**
				 * @brief レンダリングターゲットを初期化
				 * @param[in] mainRenderTarget メインレンダリングターゲット
				*/
				void InitRenderTarget(const RenderTarget& mainRenderTarget);

				/**
				 * @brief スプライトとガウシアンブラーの初期化
				 * @param[in] mainRenderTarget メインレンダリングターゲット
				*/
				void InitSpriteAndGaussianBlur(RenderTarget& mainRenderTarget);

			private:	// データメンバ
				RenderTarget m_luminanceRenderTarget;	//!< 輝度抽出用レンダリングターゲット
				Sprite m_luminanceSprite;				//!< 輝度抽出用スプライト
				Sprite m_finalSprite;					//!< 最終合成用スプライト
				//!< ガウシアンブラークラス
				CGaussianBlur m_gaussianBlur[nsBloomConstData::kGaussianBlurNum];
			};
		}
	}
}

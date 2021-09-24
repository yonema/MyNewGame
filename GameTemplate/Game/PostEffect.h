#pragma once
#include "Noncopyable.h"
#include "PostEffectConstData.h"
#include "Bloom.h"

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
			 * @brief ポストエフェクトクラス
			*/
			class CPostEffect : private nsUtil::Noncopyable
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CPostEffect() = default;
				/**
				 * @brief デストラクタ
				*/
				~CPostEffect() = default;

			public:		// メンバ関数

				/**
				 * @brief 初期化関数
				 * @param mainRenderTarget 
				*/
				void Init(RenderTarget& mainRenderTarget);

				/**
				 * @brief 描画関数
				 * @param[in] rc レンダリンコンテキスト
				 * @param[in] mainRenderTarget メインレンダリングターゲット
				*/
				void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

			private:	// データメンバ
				CBloom m_bloom;		//!< ブルームクラス
			};
		}
	}
}


#pragma once
#include "GaussianBlurConstData.h"


namespace nsNinjaAttract
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{

		/**
		 * @brief ガウシアンブラークラス
		*/
		class CGaussianBlur
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CGaussianBlur() = default;
			/**
			 * @brief デストラクタ
			*/
			~CGaussianBlur() = default;

		public:		// メンバ関数

			/**
			 * @brief 初期化関数
			 * @param[in,out] originalTexture ガウシアンブラーをかけるオリジナルテクスチャ
			 * @param[in] isBlurAlpha アルファ値チャンネルにもブラーをかけるか？
			*/
			void Init(Texture* originalTexture, const bool isBlurAlpha = false);

			/**
			 * @brief ガウシアンブラーをGPU上で実行
			 * @warning 本関数の呼び出しは、DirectX12を利用した描画コマンド生成中に呼び出す必要があります。
			 * @param[in] rc レンダリングコンテキスト
			 * @param[in] blurPower ブラーの強さ。値が大きいほどボケが強くなる。
			*/
			void ExecuteOnGPU(RenderContext& rc, const float blurPower);

			/**
			 * @brief ボケテクスチャを取得
			 * @return ボケテクスチャ
			*/
			Texture& GetBokeTexture()
			{
				return m_yBlurRenderTarget.GetRenderTargetTexture();
			}

		private:	// privateなメンバ関数

			/**
			 * @brief レンダリングターゲットを初期化
			*/
			void InitRenderTargets();

			/**
			 * @brief スプライトを初期化
			 * @param isBlurAlpha アルファチャンネルにもブラーをかけるか？
			*/
			void InitSprites(const bool isBlurAlpha);

			/**
			 * @brief 重みテーブルを更新する
			 * @param blurPower ブラーの強さ。値が大きいほどボケが強くなる。
			*/
			void UpdateWeightsTable(const float blurPower);

		private:	// データメンバ
			float m_weights[nsGaussianBlurConstData::kWeightsNum];	//!< 重みテーブル
			Texture* m_originalTexture = nullptr;	//!< オリジナルテクスチャ
			RenderTarget m_xBlurRenderTarget;		//!< 横ボケ画像を描画するレンダリングターゲット
			RenderTarget m_yBlurRenderTarget;		//!< 縦ボケ画像を描画するレンダリングターゲット
			Sprite m_xBlurSprite;					//!< 横ボケ画像を描画するためのスプライト
			Sprite m_yBlurSprite;					//!< 縦ボケ画像を描画するためのスプライト
		};

	}
}

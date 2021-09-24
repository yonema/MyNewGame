#include "stdafx.h"
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
			// ガウシアンブラーの定数データを使用可能にする
			using namespace nsGaussianBlurConstData;

			/**
			 * @brief 初期化関数
			 * @param[in,out] originalTexture ガウシアンブラーをかけるオリジナルテクスチャ
			 * @param[in] isBlurAlpha アルファ値チャンネルにもブラーをかけるか？
			*/
			void CGaussianBlur::Init(Texture* originalTexture, const bool isBlurAlpha)
			{
				m_originalTexture = originalTexture;

				//レンダリングターゲットを初期化。
				InitRenderTargets();
				//スプライトを初期化。
				InitSprites(isBlurAlpha);

				return;
			}

			/**
			 * @brief ガウシアンブラーをGPU上で実行
			 * @warning 本関数の呼び出しは、DirectX12を利用した描画コマンド生成中に呼び出す必要があります。
			 * @param[in] rc レンダリングコンテキスト
			 * @param[in] blurPower ブラーの強さ。値が大きいほどボケが強くなる。
			*/
			void CGaussianBlur::ExecuteOnGPU(RenderContext& rc, const float blurPower)
			{
				//////// 1.重みテーブルを更新する。 ////////
				UpdateWeightsTable(blurPower);

				////// 2.横ブラーを実行。 ////////
				// レンダリングターゲットとして利用できるようになるまでwaitを入れる。
				rc.WaitUntilToPossibleSetRenderTarget(m_xBlurRenderTarget);
				// レンダリングターゲットを設定。
				rc.SetRenderTargetAndViewport(m_xBlurRenderTarget);

				// ドロー。
				m_xBlurSprite.Draw(rc);
				//レンダリングターゲットへの書き込み終了待ち。
				rc.WaitUntilFinishDrawingToRenderTarget(m_xBlurRenderTarget);

				//////// 3.縦ブラーを実行。 ////////
				// レンダリングターゲットとして利用できるようになるまでwaitを入れる。
				rc.WaitUntilToPossibleSetRenderTarget(m_yBlurRenderTarget);
				// レンダリングターゲットを設定。
				rc.SetRenderTargetAndViewport(m_yBlurRenderTarget);

				// ドロー。
				m_yBlurSprite.Draw(rc);
				// レンダリングターゲットへの書き込み終了待ち。
				rc.WaitUntilFinishDrawingToRenderTarget(m_yBlurRenderTarget);

				return;
			}

			/**
			 * @brief レンダリングターゲットを初期化
			*/
			void CGaussianBlur::InitRenderTargets()
			{
				int w = m_originalTexture->GetWidth();
				int h = m_originalTexture->GetHeight();
				//Xブラー用のレンダリングターゲットを作成する。
				m_xBlurRenderTarget.Create(
					w / 2,	// x軸方向にダウンサンプリング
					h,
					1,
					1,
					m_originalTexture->GetFormat(),
					DXGI_FORMAT_UNKNOWN
				);

				//Yブラー用のレンダリングターゲットを作成する。
				m_yBlurRenderTarget.Create(
					w / 2,	// x軸方向と
					h / 2,	// y軸方向両方にダウンサンプリング
					1,
					1,
					m_originalTexture->GetFormat(),
					DXGI_FORMAT_UNKNOWN
				);

				return;
			}

			/**
			 * @brief スプライトを初期化
			 * @param isBlurAlpha アルファチャンネルにもブラーをかけるか？
			*/
			void CGaussianBlur::InitSprites(const bool isBlurAlpha)
			{
				// ピクセルシェーダーのエントリーポイント
				const char* psEntryFunc = nullptr;
				// アルファチャンネルにもブラーをかけるか？
				if (isBlurAlpha)
				{
					psEntryFunc = kPsEntryFunc_alpha;	// かける
				}
				else 
				{
					psEntryFunc = kPsEntryFunc;			// かけない
				}

				//横ブラー用のスプライトを初期化する。
				{
					// スプライトの初期化データ
					SpriteInitData xBlurSpriteInitData;
					xBlurSpriteInitData.m_fxFilePath = kfxFilePath;
					xBlurSpriteInitData.m_vsEntryPointFunc = kVsEntryFunc_xBlur;
					xBlurSpriteInitData.m_psEntryPoinFunc = psEntryFunc;
					//スプライトの解像度はm_xBlurRenderTargetと同じ。
					xBlurSpriteInitData.m_width = m_xBlurRenderTarget.GetWidth();
					xBlurSpriteInitData.m_height = m_xBlurRenderTarget.GetHeight();
					//テクスチャは輝度テクスチャ。
					xBlurSpriteInitData.m_textures[0] = m_originalTexture;
					//描き込むレンダリングターゲットのフォーマットを指定する。
					xBlurSpriteInitData.m_colorBufferFormat[0] = m_xBlurRenderTarget.GetColorBufferFormat();
					//ユーザー拡張の定数バッファにブラー用のパラメーターを設定する。
					xBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
					xBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

					//初期化情報をもとに横ブラー用のスプライトを初期化する。
					m_xBlurSprite.Init(xBlurSpriteInitData);
				}
				//縦ブラー用のスプライトを初期化する。
				{
					// スプライトの初期化データ
					SpriteInitData yBlurSpriteInitData;
					yBlurSpriteInitData.m_fxFilePath = kfxFilePath;
					yBlurSpriteInitData.m_vsEntryPointFunc = kVsEntryFunc_yBlur;
					yBlurSpriteInitData.m_psEntryPoinFunc = psEntryFunc;
					//スプライトの解像度はm_yBlurRenderTargetと同じ。
					yBlurSpriteInitData.m_width = m_yBlurRenderTarget.GetWidth();
					yBlurSpriteInitData.m_height = m_yBlurRenderTarget.GetHeight();
					//テクスチャは横ブラーをかけたもの。
					yBlurSpriteInitData.m_textures[0] = &m_xBlurRenderTarget.GetRenderTargetTexture();
					//描き込むレンダリングターゲットのフォーマットを指定する。
					yBlurSpriteInitData.m_colorBufferFormat[0] = m_yBlurRenderTarget.GetColorBufferFormat();
					//ユーザー拡張の定数バッファにブラー用のパラメーターを設定する。
					yBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
					yBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

					//初期化情報をもとに縦ブラー用のスプライトを初期化する。
					m_yBlurSprite.Init(yBlurSpriteInitData);
				}

				return;
			}

			/**
			 * @brief 重みテーブルを更新する
			 * @param blurPower ブラーの強さ。値が大きいほどボケが強くなる。
			*/
			void CGaussianBlur::UpdateWeightsTable(const float blurPower)
			{
				float total = 0;
				for (int i = 0; i < kWeightsNum; i++) {
					m_weights[i] = expf(-0.5f * (float)(i * i) / blurPower);
					total += 2.0f * m_weights[i];

				}
				// 規格化
				for (int i = 0; i < kWeightsNum; i++) {
					m_weights[i] /= total;
				}

				return;
			}
		}
	}
}
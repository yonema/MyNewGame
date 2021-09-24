#include "stdafx.h"
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
			// ブルームの定数データを使用可能にする
			using namespace nsBloomConstData;

			/**
			 * @brief 初期化関数
			 * @param[in] mainRenderTarget メインレンダリングターゲット
			*/
			void CBloom::OnInit(RenderTarget& mainRenderTarget)
			{
				// レンダリングターゲットを初期化
				InitRenderTarget(mainRenderTarget);

				// スプライトとガウシアンブラーを初期化
				InitSpriteAndGaussianBlur(mainRenderTarget);

				return;
			}

			/**
			 * @brief 描画関数
			 * @param[in] rc レンダリングコンテキスト
			 * @param[in] mainRenderTarget メインレンダリングターゲット
			*/
			void CBloom::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
			{
				//////// 1.輝度抽出 ////////
				// 輝度抽出用のレンダリングターゲットに変更
				rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
				// レンダリングターゲットを設定
				rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
				// レンダリングターゲットをクリア
				rc.ClearRenderTargetView(m_luminanceRenderTarget);
				// 輝度抽出を行う
				m_luminanceSprite.Draw(rc);
				// レンダリングターゲットへの書き込み終了待ち
				rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

				//////// 2.ガウシアンブラーを4回実行する ////////
				for (int i = 0; i < kGaussianBlurNum; i++)
				{
					m_gaussianBlur[i].ExecuteOnGPU(rc, kGaussianBulrPower);
				}

				//////// 3.4枚のボケ画像を合成してメインレンダリングターゲットに加算合成////////
				// レンダリングターゲットとして利用できるまで待つ
				rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
				// レンダリングターゲットを設定
				rc.SetRenderTargetAndViewport(mainRenderTarget);
				// 最終合成
				m_finalSprite.Draw(rc);
				// レンダリングターゲットへの書き込み終了待ち
				rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

				return;
			}

			/**
			 * @brief レンダリングターゲットを初期化
			 * @param[in] mainRenderTarget メインレンダリングターゲット
			*/
			void CBloom::InitRenderTarget(const RenderTarget& mainRenderTarget)
			{
				m_luminanceRenderTarget.Create(
					mainRenderTarget.GetWidth(),   // 解像度はメインレンダリングターゲットと同じ
					mainRenderTarget.GetHeight(),  // 解像度はメインレンダリングターゲットと同じ
					1,
					1,
					mainRenderTarget.GetColorBufferFormat(),
					DXGI_FORMAT_UNKNOWN
				);

				return;
			}

			/**
			 * @brief スプライトとガウシアンブラーの初期化
			 * @param[in] mainRenderTarget メインレンダリングターゲット
			*/
			void CBloom::InitSpriteAndGaussianBlur(RenderTarget& mainRenderTarget)
			{
				//////// 1.輝度抽出用のスプライトを初期化 ////////
				{
					// スプライトの初期化データ
					SpriteInitData spriteInitData;
					// ブルーム用のシェーダーのファイルパスを指定する
					spriteInitData.m_fxFilePath = kfxFilePath;
					// 輝度抽出用のピクセルシェーダーのエントリーポイントを指定する
					spriteInitData.m_psEntryPoinFunc = kPsEntryFunc_samplingLuminance;
					// スプライトの幅と高さはluminnceRenderTargetと同じ
					spriteInitData.m_width = mainRenderTarget.GetWidth();
					spriteInitData.m_height = mainRenderTarget.GetHeight();
					// テクスチャはメインレンダリングターゲットのカラーバッファー
					spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
					// 描き込むレンダリングターゲットのフォーマットを指定する
					spriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

					// 輝度抽出用のスプライトを初期化
					m_luminanceSprite.Init(spriteInitData);
				}

				//////// 2.ガウシアンブラーを初期化 ////////
				// gaussianBlur[0]は輝度テクスチャにガウシアンブラーをかける
				m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
				// gaussianBlur[1]はgaussianBlur[0]のテクスチャにガウシアンブラーをかける
				m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
				// gaussianBlur[2]はgaussianBlur[1]のテクスチャにガウシアンブラーをかける
				m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
				// gaussianBlur[3]はgaussianBlur[2]のテクスチャにガウシアンブラーをかける
				m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

				//////// 3.最終合成用のスプライトを初期化する ////////
				{
					// スプライトの初期化データ
					SpriteInitData spriteInitData;
					// ブルーム用のシェーダーのファイルパスを指定する
					spriteInitData.m_fxFilePath = kfxFilePath;
					// 最終合成用のピクセルシェーダーのエントリーポイントを指定する
					spriteInitData.m_psEntryPoinFunc = kPsEntryFunc_bloomFinal;
					// ボケテクスチャを4枚指定する
					for (int i = 0; i < kGaussianBlurNum; i++)
					{
						spriteInitData.m_textures[i] = &m_gaussianBlur[i].GetBokeTexture();
					}
					// 解像度はmainRenderTargetの幅と高さ
					spriteInitData.m_width = mainRenderTarget.GetWidth();
					spriteInitData.m_height = mainRenderTarget.GetHeight();
					// 描き込むレンダリングターゲットのフォーマットを指定する
					spriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();
					// ただし、加算合成で描画するので、アルファブレンディングモードを加算にする
					spriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;

					// 最終合成用スプライトの初期化
					m_finalSprite.Init(spriteInitData);
				}

				return;
			}
		}
	}
}
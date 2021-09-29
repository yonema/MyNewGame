#include "stdafx.h"
#include "ShadowMapRender.h"
#include "Render.h"
#include "LightManager.h"

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		// 前方宣言
		class CRender;

		/**
		 * @brief シャドウ関連のネームスペース
		*/
		namespace nsShadow
		{
			using namespace nsShadowConstData;
			/**
			 * @brief 初期化関数
			*/
			void CShadowMapRender::Init()
			{
				// カスケードシャドウのエリア率の初期化
				for (int i = 0; i < enShadowMapArea_num; i++)
				{
					m_cascadeAreaRateArray[i] = kDefaultCascadeAreaRateArray[i];
				}

				// カラーフォーマット
				DXGI_FORMAT colorFormat = DXGI_FORMAT_R32G32_FLOAT;
				// デプスステンシルフォーマット
				DXGI_FORMAT depthFormat = DXGI_FORMAT_D32_FLOAT;
				// クリアカラー
				float clearColor[4];
				for (int i = 0; i < 4; i++)
				{
					clearColor[i] = kDefaultClearColor[i];
				}

				// 近景用のシャドウマップの生成
				m_shadowMaps[enShadowMapArea_near].Create(
					2048,
					2048,
					1,
					1,
					colorFormat,
					depthFormat,
					clearColor
				);
				// 中景用のシャドウマップの生成
				m_shadowMaps[enShadowMapArea_middle].Create(
					1024,
					1024,
					1,
					1,
					colorFormat,
					depthFormat,
					clearColor
				);
				// 遠景用のシャドウマップの生成
				m_shadowMaps[enShadowMapArea_far].Create(
					512,
					512,
					1,
					1,
					colorFormat,
					depthFormat,
					clearColor
				);

				// ガウシアンブラーの初期化
				m_blur[0].Init(&m_shadowMaps[enShadowMapArea_near].GetRenderTargetTexture());
				m_blur[1].Init(&m_shadowMaps[enShadowMapArea_middle].GetRenderTargetTexture());
				m_blur[2].Init(&m_shadowMaps[enShadowMapArea_far].GetRenderTargetTexture());

				return;
			}


			/**
			 * @brief 描画関数
			 * @param[in] rc レンダリングコンテキスト
			 * @param[in] ligNo ライトの番号
			 * @param[in] lightDirection ライトの方向
			 * @param[in] renderObjects 描画するオブジェクト
			*/
			void CShadowMapRender::Render(
				RenderContext& rc,
				const int ligNo,
				const Vector3& lightDirection,
				std::vector<CRender*>& renderObjects
			)
			{

				// ライトの最大の高さをレンダラーのAABBから計算する。
				m_cascadeShadowMapMatrix.CalcLightViewProjectionCropMatrix(
					lightDirection,
					m_cascadeAreaRateArray
				);

				// シャドウマップの番号
				int shadowMapNo = 0;
				// それぞれのシャドウマップにアクセス
				for (auto& shadowMap : m_shadowMaps)
				{
					// シャドウマップを使用可能にする
					rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
					rc.SetRenderTargetAndViewport(shadowMap);
					rc.ClearRenderTargetView(shadowMap);

					// 描画するオブジェクトを取り出して描画する
					for (auto& renderer : renderObjects)
					{
						renderer->OnRenderShadowMap(
							rc,
							ligNo,
							shadowMapNo,
							m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(shadowMapNo)
						);
					}

					// 書き込み完了待ち
					rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
					shadowMapNo++;
				}

				// ブラーを実行する。
				for (auto& blur : m_blur) {
					blur.ExecuteOnGPU(rc, kBlurPower);
				}


				return;
			}


		}
	}
}
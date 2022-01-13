#include "stdafx.h"
#include "PlayerShadowMap.h"
#include "ShadowConstDatah.h"

namespace nsNinjaAttract
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief シャドウ関連のネームスペース
		*/
		namespace nsShadow
		{
			using namespace nsShadowConstData;

			/**
			 * @brief コンストラクタ
			*/
			CPlayerShadowMap::CPlayerShadowMap()
			{

				// 初期化処理
				Init();

				return;
			}

			/**
			 * @brief 初期化処理
			*/
			void CPlayerShadowMap::Init()
			{
				// シャドウマップのクリアカラー
				float clearColor[4];
				for (int i = 0; i < 4; i++)
				{
					clearColor[i] = kDefaultClearColor[i];
				}

				// シャドウマップの作製
				m_shadowMap.Create
				(
					kShadowMapSizes[0],
					kShadowMapSizes[0],
					1,
					1,
					DXGI_FORMAT_R32G32_FLOAT,
					DXGI_FORMAT_D32_FLOAT,
					clearColor
				);

				// ガウシアンブラーの初期化
				m_blur.Init(&m_shadowMap.GetRenderTargetTexture());

				return;
			}

			/**
			 * @brief 描画関数
			 * @param[in] rc レンダーコンテキスト
			 * @param[in] ligNo ライトの番号
			 * @param[in] lightDirection ライトの方向
			*/
			void CPlayerShadowMap::Render(RenderContext& rc,const int ligNo, const Vector3& lightDirection)
			{
				if (m_playerRender == nullptr)
				{
					// プレイヤーレンダラーがセットされていなかったら、早期リターン。
					return;
				}

				// ライトビュープロジェクション行列を計算
				CalcLightViewProjectionMatrix(&m_lvpMatrix, lightDirection);

				// シャドウマップを使用可能にする
				rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
				rc.SetRenderTargetAndViewport(m_shadowMap);
				rc.ClearRenderTargetView(m_shadowMap);

				// プレイヤーを描画
				m_playerRender->OnRenderPlayerShadowMap(
					rc, ligNo, m_lvpMatrix
				);

				// 書き込み完了待ち
				rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

				// ブラーを実行する
				m_blur.ExecuteOnGPU(rc, kBlurPower);


				return;
			}

			/**
			 * @brief ライトビュープロジェクション行列を計算する
			 * @param[out] lvpMatrixOut ライトビュープロジェクションの出力
			 * @param[in] lightDirection ライトの方向
			*/
			void CPlayerShadowMap::CalcLightViewProjectionMatrix(Matrix* lvpMatrixOut, const Vector3& lightDirection)
			{
				// ビュー行列を計算する
				Matrix viewMatrix;
				// ライトのターゲット
				Vector3 lightTarget = g_camera3D->GetTarget();
				// ライトのポジション
				Vector3 lightPos = lightTarget;
				// ライトターゲットからライトの方向に応じた距離をとる
				Vector3 targetToLightPos = lightDirection;
				targetToLightPos.Scale(-100.0f);
				lightPos += targetToLightPos;
				//if (lightDirection.y != 0.0f)
				//	lightPos += (lightDirection) * (kLightMaxHeight / lightDirection.y);
				//else
				//	lightPos += lightDirection * kLightMaxHeight;
				// 上方向を設定
				if (fabsf(lightDirection.y) > 0.9999f) {
					// ほぼ真上、真下を向いている
					// Xアップ
					viewMatrix.MakeLookAt(lightPos, lightTarget, Vector3::AxisX);
				}
				else
				{
					// Yアップ
					viewMatrix.MakeLookAt(lightPos, lightTarget, Vector3::AxisY);
				}

				// プロジェクション行列を計算する
				Matrix projMatrix;
				// 並行投影行列で計算する
				projMatrix.MakeOrthoProjectionMatrix(
					500.0f,
					//5000.0f,
					500.0f,
					//5000.0f,
					kLightMinFar,
					1000.0f
				);

				// ライトビュープロジェクション行列を計算する
				*lvpMatrixOut = viewMatrix * projMatrix;


				return;
			}


		}
	}
}
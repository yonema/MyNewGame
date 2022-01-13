#pragma once
#include "Noncopyable.h"
#include "ShadowConstDatah.h"
#include "CascadeShadowMapMatrix.h"
#include "GaussianBlur.h"
#include "PlayerShadowMap.h"


namespace nsNinjaAttract
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		// 前方宣言
		class CRender;
		namespace nsShadow { class CPlayerShadowMap; }

		/**
		 * @brief シャドウ関連のネームスペース
		*/
		namespace nsShadow
		{


			/**
			 * @brief シャドウマップレンダラークラス
			*/
			class CShadowMapRender : private nsUtil::Noncopyable
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CShadowMapRender() = default;
				/**
				 * @brief コンストラクタ
				*/
				~CShadowMapRender() = default;

			public:		// メンバ関数

				/**
				 * @brief 初期化関数
				*/
				void Init();

				/**
				 * @brief 描画関数
				 * @param[in] rc レンダリングコンテキスト
				 * @param[in] ligNo ライトの番号
				 * @param[in] lightDirection ライトの方向
				 * @param[in] renderObjects 描画するオブジェクト
				 * @param[in] sceneMaxPosition ゲームシーンの最大座標
				 * @param[in] sceneMinPosition ゲームシーンの最小座標
				*/
				void Render(
					RenderContext& rc,
					const int ligNo, 
					const Vector3& lightDirection,
					std::vector<CRender*>& renderObjects,
					const Vector3& sceneMaxPosition,
					const Vector3& sceneMinPosition
				);

				/**
				 * @brief シャドウマップを取得
				 * @param[in] areaNo エリア番号
				 * @return シャドウマップのテクスチャの参照
				*/
				Texture& GetShadowMap(const int areaNo)
				{
					return m_blur[areaNo].GetBokeTexture();
				}

				/**
				 * @brief ライトビュープロジェクション行列を取得
				 * @param[in] areaNo エリア番号
				 * @return ライトビュープロジェクション行列
				*/
				const Matrix& GetLVPMatrix(const int areaNo) const
				{
					return m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(areaNo);
				}

				/**
				 * @brief プレイヤー専用のライトビュープロジェクション行列を取得
				 * @return プレイヤー専用のライトビュープロジェクション行列
				*/
				const Matrix& GetPlayerLVPMatrix() const
				{
					return m_playerShadowMapPtr->GetLVPMatrix();
				}

				/**
				 * @brief カスケードシャドウのエリア率を設定。
				 * @details ゲームカメラの近平面から遠平面までのエリアの、何%の範囲を近距離用の高解像度
				 * のシャドウマップに、何%を中距離用のシャドウマップに、何%を遠距離用のシャドウマップに
				 * 描画するかを指定したもの。例えば、nearArea  = 0.1(10%)、middleArea = 0.3(30%), 
				 * farArea = 0.8(80%)と指定されていたら、カメラから10%のエリアは近距離用のシャドウマップ
				 * に描画、カメラから10%～30%のエリアは中距離用のシャドウマップに描画、カメラから30%～80%
				 * のエリアは遠距離用のシャドウマップに描画となる。
				 * @param[in] nearArea 近影のエリア率
				 * @param[in] middleArea 中影のエリア率
				 * @param[in] farArea 遠影のエリア率
				*/
				void SetCascadeNearAreaRates(const float nearArea, const float middleArea, const float farArea)
				{
					// 中距離エリアの範囲が近距離エリアの範囲より小さくなっていると
					// 計算上不都合が起きるので、補正。
					const float newMiddleArea = max(nearArea + 0.01f, middleArea);
					// 遠い距離エリアの範囲が中距離エリアの範囲より小さくなっていると
					// 計算上不都合が起きるので、補正。
					const float newFarArea = max(newMiddleArea + 0.01f, farArea);
					m_cascadeAreaRateArray[nsShadowConstData::enShadowMapArea_near] = nearArea;
					m_cascadeAreaRateArray[nsShadowConstData::enShadowMapArea_middle] = newMiddleArea;
					m_cascadeAreaRateArray[nsShadowConstData::enShadowMapArea_far] = newFarArea;
				}

				/**
				 * @brief プレイヤーのレンダラーを設定
				 * @param[in] playerRender プレイヤーのレンダラー
				*/
				void SetPlayerRender(CRender* playerRender)
				{
					m_playerShadowMapPtr->SetPlayerRender(playerRender);
				}

				/**
				 * @brief プレイヤー専用のシャドウマップを取得
				 * @return シャドウマップのテクスチャの参照
				*/
				Texture& GetPlayerShadowMap()
				{
					return m_playerShadowMapPtr->GetShadowMap();
				}

			private:	// データメンバ

				//!< カスケードシャドウマップで使用するマトリクスクラス
				CCascadeShadowMapMatrix m_cascadeShadowMapMatrix;
				//!< それぞれのエリアのシャドウマップ
				RenderTarget m_shadowMaps[nsShadowConstData::enShadowMapArea_num];

				//!< カスケードシャドウのエリア率
				float m_cascadeAreaRateArray[nsShadowConstData::enShadowMapArea_num] = {};
				//!< シャドウマップにかけるガウシアンブラークラス
				CGaussianBlur m_blur[nsShadowConstData::enShadowMapArea_num];

				//!< プレイヤー専用のシャドウマップ
				std::unique_ptr<CPlayerShadowMap> m_playerShadowMapPtr;

			};
		}
	}
}



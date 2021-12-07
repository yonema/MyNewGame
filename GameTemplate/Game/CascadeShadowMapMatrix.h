#pragma once
#include "ShadowConstDatah.h"
#include "Noncopyable.h"

namespace nsMyGame
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
			/**
			 * @brief カスケードシャドウマップで使用するマトリクスクラス
			*/
			class CCascadeShadowMapMatrix : private nsUtil::Noncopyable
			{
			public:		// コンストラクタとデストラクタ

			public:		// メンバ関数

				/**
				 * @brief ライトビュープロジェクションクロップ行列を計算する
				 * @param[in] lightDirection ライトの方向
				 * @param[in] cascadeAreaRateTbl カスケードシャドウのエリア率テーブル
				*/
				void CalcLightViewProjectionCropMatrix(
					const Vector3& lightDirection,
					const float cascadeAreaRateTbl[nsShadowConstData::enShadowMapArea_num]
				);

				/**
				 * @brief 計算されたライトビュープロジェクションクロップ行列を取得
				 * @param[in] areaNum シャドウマップのエリア番号
				 * @return クロップ行列
				*/
				const Matrix& GetLightViewProjectionCropMatrix(const int areaNum) const 
				{
					return m_lvpcMatrix[areaNum];
				};

			private:	// privateなメンバ関数

				/**
				 * @brief ライトビュープロジェクション行列を計算してから得る
				 * @param[out] lvpMatrixOut ライトビュープロジェクション行列
				 * @param[in] lightDirection ライトの方向
				*/
				void CalcAndGetLightViewProjectionMatrix(
					Matrix* lvpMatrixOut,
					const Vector3& lightDirection
				);

				/**
				 * @brief エリアの8頂点の座標を計算してから得る
				 * @param[out] vertexOut 頂点の座標を入れる配列
				 * @param[in] nearDepth エリアの最小深度値
				 * @param[in] farDepth エリアの最大深度値
				 * @param[in] cameraForward カメラの前方向
				 * @param[in] cameraRight カメラの右方向
				 * @param[in] cameraUp カメラの上方向
				*/
				void CalcAndGetAreaVertex(
					Vector3 vertexOut[8],
					const float nearDepth,
					const float farDepth,
					const Vector3& cameraForward,
					const Vector3& cameraRight,
					const Vector3& cameraUp
					);

				/**
				 * @brief クロップ行列を計算してから得る
				 * @param[out] clopMatrixOut クロップ行列
				 * @param[in] vMax 最大値
				 * @param[in] vMin 最小値
				*/
				void CalcAndGetClopMatrix(Matrix* clopMatrixOut, const Vector3& vMax, const Vector3& vMin);

			private:	// データメンバ
				//!< ライトビュープロジェクションクロップ行列
				Matrix m_lvpcMatrix[nsShadowConstData::enShadowMapArea_num];
			};
		}
	}
}


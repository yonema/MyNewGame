#include "stdafx.h"
#include "CascadeShadowMapMatrix.h"


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
			// シャドウの定数データを使用可能にする
			using namespace nsShadowConstData;

			/**
			 * @brief ライトビュープロジェクションクロップ行列を計算する
			 * @param[in] lightDirection ライトの方向
			 * @param[in] cascadeAreaRateTbl カスケードシャドウのエリア率テーブル
			 * @param[in] sceneMaxPosition ゲームシーンの最大座標
			 * @param[in] sceneMinPosition ゲームシーンの最小座標
			*/
			void CCascadeShadowMapMatrix::CalcLightViewProjectionCropMatrix(
				const Vector3& lightDirection,
				const float cascadeAreaRateTbl[enShadowMapArea_num],
				const Vector3& sceneMaxPosition,
				const Vector3& sceneMinPosition
			)
			{
				// ライトビュープロジェクション
				Matrix lvpMatrix;
				// ライトビュープロジェクションを計算する
				CalcAndGetLightViewProjectionMatrix(&lvpMatrix,
					lightDirection,
					sceneMaxPosition,
					sceneMinPosition
				);

				// カメラの前方向
				const Vector3& cameraForward = g_camera3D->GetForward();
				// カメラの右方向
				const Vector3& cameraRight = g_camera3D->GetRight();
				// カメラの上方向
				const Vector3 cameraUp = Cross(cameraForward, cameraRight);

				// nearDepthはエリアの最小深度値を表す
				// 一番近いエリアの最小深度値はカメラのニアクリップ
				float nearDepth = g_camera3D->GetNear();

				// カスケードシャドウの最大深度
				float maxDepth = kCascadeMaxDepth;

				// 分割エリアの最大深度値を定義する
				float cascadeAreaTbl[enShadowMapArea_num] = {
					maxDepth* cascadeAreaRateTbl[enShadowMapArea_near],     // 近影を映す最大深度値
					maxDepth* cascadeAreaRateTbl[enShadowMapArea_middle],   // 中影を映す最大深度値
					maxDepth* cascadeAreaRateTbl[enShadowMapArea_far] ,     // 遠影を映す最大深度値。
				};

				// それぞれのシャドウマップのエリアを計算する
				for (int areaNo = 0; areaNo < enShadowMapArea_num; areaNo++)
				{
					//////// 1.エリアを内包する視錐台の８頂点を求める ////////

					// 8頂点の座標
					Vector3 vertex[8];

					// エリアの8頂点の座標を計算してから得る
					CalcAndGetAreaVertex(
						vertex,
						nearDepth,
						cascadeAreaTbl[areaNo],
						cameraForward,
						cameraRight,
						cameraUp
					);

					//////// 2.求めた8頂点をLVP空間に変換して、最大値、最小値を求める ////////

					// 最大値と最小値
					Vector3 vMax, vMin;
					vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };// 最大値の初期値は一番小さい数値を入れる
					vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };	// 最小値の初期値は一番大きい数値を入れる
					// 全ての頂点にアクセス
					for (auto& v : vertex)
					{
						lvpMatrix.Apply(v);	// LVP空間に変換
						vMax.Max(v);		// 最大値を設定
						vMin.Min(v);		// 最小値を設定
					}

					//////// 3.クロップ行列を求める ////////

					// クロップ行列
					Matrix clopMatrix;
					// クロップ行列を計算してから得る
					CalcAndGetClopMatrix(&clopMatrix, vMax, vMin);

					//////// 4.LVP行列にクロップ行列を乗算して最終的な行列を求める ////////

					// ライトビュープロジェクション行列にクロップ行列を乗算する
					m_lvpcMatrix[areaNo] = lvpMatrix * clopMatrix;

					// 次のエリアの近平面までの距離を代入する
					nearDepth = cascadeAreaTbl[areaNo];
				}


				return;
			}

			/**
			 * @brief ライトビュープロジェクション行列を計算してから得る
			 * @param[out] lvpMatrixOut ライトビュープロジェクション行列
			 * @param[in] lightDirection ライトの方向
			 * @param[in] sceneMaxPosition ゲームシーンの最大座標
			 * @param[in] sceneMinPosition ゲームシーンの最小座標
			*/
			void CCascadeShadowMapMatrix::CalcAndGetLightViewProjectionMatrix(
				Matrix* lvpMatrixOut,
				const Vector3& lightDirection,
				const Vector3& sceneMaxPosition,
				const Vector3& sceneMinPosition
			)
			{
				// ビュー行列を計算する
				Matrix viewMatrix;
				// ライトのターゲット
				//const Vector3 lightTarget = kLightTargetPos;
				//Vector3 lightTarget = g_camera3D->GetTarget();
				Vector3 lightTarget = (sceneMaxPosition + sceneMinPosition) * 0.5f;
				//// 高さは固定
				lightTarget.y = 0.0f;
				// ライトのポジション
				Vector3 lightPos = lightTarget;
				// ライトターゲットからライトの方向に応じた距離をとる
				Vector3 targetToLightPos = lightDirection;
				targetToLightPos.Scale(-kLightMaxHeight);
				lightPos += targetToLightPos;

				//lightPos += (lightDirection) * (kLightMaxHeight / lightDirection.y);
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
					//1280,
					5000.0f,
					//720,
					5000.0f,
					kLightMinFar,
					kLightMaxFar
				);

				// ライトビュープロジェクション行列を計算する
				*lvpMatrixOut = viewMatrix * projMatrix;

				return;
			}


			/**
			 * @brief エリアの8頂点の座標を計算してから得る
			 * @param[out] vertexOut 頂点の座標を入れる配列
			 * @param[in] nearDepth エリアの最小深度値
			 * @param[in] farDepth エリアの最大深度値
			 * @param[in] cameraForward カメラの前方向
			 * @param[in] cameraRight カメラの右方向
			 * @param[in] cameraUp カメラの上方向
			*/
			void CCascadeShadowMapMatrix::CalcAndGetAreaVertex(
				Vector3 vertexOut[8],
				const float nearDepth,
				const float farDepth,
				const Vector3& cameraForward,
				const Vector3& cameraRight,
				const Vector3& cameraUp
			)
			{
				// エリアの近平面、遠平面の上下左右の面への距離をそれぞれ求める

				// エリアの近平面の中心からの上面、下面までの距離を求める
				float nearY = tanf(g_camera3D->GetViewAngle() * 0.5f) * nearDepth;
				// エリアの近平面の中心からの右面、左面までの距離を求める
				float nearX = nearY * g_camera3D->GetAspect();
				// エリアの遠平面の中心からの上面、下面までの距離を求める
				float farY = tanf(g_camera3D->GetViewAngle() * 0.5f) * farDepth;
				// エリアの遠平面の中心からの右面、左面までの距離を求める
				float farX = farY * g_camera3D->GetAspect();
				// エリアの近平面の中心座標を求める
				Vector3 nearPos = g_camera3D->GetPosition() + cameraForward * nearDepth;
				// エリアの遠平面の中心座標を求める
				Vector3 farPos = g_camera3D->GetPosition() + cameraForward * farDepth;

				// 8頂点の座標を求める

				// 近平面の右上の頂点
				vertexOut[0] += nearPos + cameraUp * nearY + cameraRight * nearX;
				// 近平面の左上の頂点
				vertexOut[1] += nearPos + cameraUp * nearY + cameraRight * -nearX;
				// 近平面の右下の頂点
				vertexOut[2] += nearPos + cameraUp * -nearY + cameraRight * nearX;
				// 近平面の左下の頂点
				vertexOut[3] += nearPos + cameraUp * -nearY + cameraRight * -nearX;
				// 遠平面の右上の頂点
				vertexOut[4] += farPos + cameraUp * farY + cameraRight * farX;
				// 遠平面の左上の頂点
				vertexOut[5] += farPos + cameraUp * farY + cameraRight * -farX;
				// 遠平面の右下の頂点
				vertexOut[6] += farPos + cameraUp * -farY + cameraRight * farX;
				// 遠平面の左下の頂点
				vertexOut[7] += farPos + cameraUp * -farY + cameraRight * -farX;

				return;
			}

			/**
			 * @brief クロップ行列を計算してから得る
			 * @param[out] clopMatrixOut クロップ行列
			 * @param[in] vMax 最大値
			 * @param[in] vMin 最小値
			*/
			void CCascadeShadowMapMatrix::CalcAndGetClopMatrix(
				Matrix* clopMatrixOut,
				const Vector3& vMax,
				const Vector3& vMin
			)
			{
				// 正規化スクリーン座標系にピッタリ収まる拡大率と平行移動量を求める

				// 拡大率を求める
				float xScale = kNormalScreenWidth / (vMax.x - vMin.x);
				float yScale = kNormalScreenHeight / (vMax.y - vMin.y);

				// 平行移動量を求める
				// 中心座標の分だけ反対向きに平行移動させる
				float xOffset = (vMax.x + vMin.x) * -0.5f * xScale;
				float yOffset = (vMax.y + vMin.y) * -0.5f * yScale;

				// クロップ行列を求める
				clopMatrixOut->m[0][0] = xScale;
				clopMatrixOut->m[1][1] = yScale;
				clopMatrixOut->m[3][0] = xOffset;
				clopMatrixOut->m[3][1] = yOffset;

				return;
			}

		}
	}
}
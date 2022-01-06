#pragma once

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
			 * @brief シャドウの定数データ
			*/
			namespace nsShadowConstData
			{
				/**
				 * @brief シャドウマップのエリアの列挙型
				*/
				enum EnShadowMapArea
				{
					enShadowMapArea_near,	//!< 近影範囲のシャドウマップ
					enShadowMapArea_middle,	//!< 中影範囲のシャドウマップ
					enShadowMapArea_far,	//!< 遠影範囲のシャドウマップ
					enShadowMapArea_num		//!< シャドウマップのエリアの数
				};

				//!< シャドウマップのサイズ
				constexpr int kShadowMapSizes[enShadowMapArea_num]
				{
					4096,
					2048,
					1024
				};
				//!< カスケードシャドウの最大深度
				constexpr float kCascadeMaxDepth = 50000.0f;
				//!< デフォルトのカスケードシャドウのエリア率
				constexpr float kDefaultCascadeAreaRateArray[enShadowMapArea_num] = { 0.3f,0.5f, 1.0f };
				//constexpr float kDefaultCascadeAreaRateArray[enShadowMapArea_num] = { 0.01f,0.3f, 1.0f };

				//!< デフォルトのクリアカラー
				constexpr float kDefaultClearColor[4] = { -1.0f,-1.0f,-1.0f,1.0f };

				constexpr float kBlurPower = 1.0f;	//!< ブラーの強さ

				const Vector3 kLightTargetPos = Vector3::Zero;	//!< ライトのターゲットの座標
				constexpr float kLightMaxHeight = 15000.0f;		//!< ライトの最大の高さ
				constexpr float kLightMinFar = 1.0f;	//!< ライトの最小ファー
				constexpr float kLightMaxFar = 30000.0f;	//!< ライトの最大ファー
				//constexpr float kLightMaxFar[enShadowMapArea_num] = //!< ライトの最大ファー
				//{
				//	7500.0f,
				//	15000.0f,
				//	30000.0f 
				//};

				constexpr float kNormalScreenWidth = 2.0f;	//!< 正規化スクリーン座標の横幅
				constexpr float kNormalScreenHeight = 2.0f;	//!< 正規化スクリーン座標の縦幅
			}
		}
	}
}
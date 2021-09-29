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

				//!< デフォルトのカスケードシャドウのエリア率
				constexpr float kDefaultCascadeAreaRateArray[enShadowMapArea_num] = { 0.05f,0.3f, 1.0f };

				//!< デフォルトのクリアカラー
				constexpr float kDefaultClearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

				constexpr float kBlurPower = 1.0f;	//!< ブラーの強さ

				const Vector3 kLightTargetPos = Vector3::Zero;	//!< ライトのターゲットの座標
				constexpr float kLightMaxHeight = 5000.0f;		//!< ライトの最大の高さ
				constexpr float kLightMinFar = 1.0f;	//!< ライトの最小ファー
				constexpr float kLightMaxFar = 15000.0f;	//!< ライトの最大ファー

				constexpr float kNormalScreenWidth = 2.0f;	//!< 正規化スクリーン座標の横幅
				constexpr float kNormalScreenHeight = 2.0f;	//!< 正規化スクリーン座標の縦幅
			}
		}
	}
}
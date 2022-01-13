#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief ライト関連のネームスペース
	*/
	namespace nsLight
	{
		/**
		 * @brief ライトの定数データのネームスペース
		*/
		namespace nsLightConstData
		{
			constexpr int kMaxDirectionalLightNum = 4;	//!< ディレクションライトの最大数
			constexpr int kMaxPointLightNum = 16;		//!< ポイントライトの最大数
			constexpr int kMaxSpotLightNum = 16;		//!< スポットライトの最大数

			/**
			 * @brief ディレクションライトのデフォルトの方向を得る
			 * @return ディレクションライトのデフォルトの方向
			*/
			static inline Vector3& GetDefaultDirection()
			{
				static Vector3 defaultDirection = { -1.0f,-1.0f,1.0f };
				defaultDirection.Normalize();
				return defaultDirection;
			}

			const Vector4 kDefaultDirectionalLightColor = { 1.0f,1.0f,1.0f,1.0f };	//!< デフォルトのディレクションライトのカラー

			const Vector4 kDefaultPointLightColor = { 10.0f,10.0f,10.0f,1.0f };	//!< デフォルトのポイントライトカラー

			constexpr float kDefaultPointLightRange = 100.0f;	//!< デフォルトのポイントライトの影響範囲

			constexpr float kDefaultSpotLightAngle = Math::PI * 0.5f;	//!< デフォルトのスポットライトの射出角度（radian単位）

			/**
			 * @brief アンビエントライトのデフォルトの値を得る
			 * @return アンビエントライトのデフォルトの値
			*/
			static inline Vector3& GetDefaultAmbientLight()
			{
				static Vector3 ambientLight = { 1.0f,1.0f,1.0f };
				ambientLight.Normalize();
				float power = 1.0f;
				ambientLight *= power;
				return ambientLight;
			}
		}
	}
}

#pragma once

namespace nsMyGame
{
	/**
	 * @brief デバック関連のネームスペース
	*/
	namespace nsDebug
	{
		/**
		 * @brief デバックの定数データ
		*/
		namespace nsDebugConstData
		{
			//!< ベクトルのデフォルトの名前
			constexpr const char* const kDefaultVectorName = "vector";
		
			//!< ベクトルのモデルの表示
			constexpr const char* const kVectorModel = "Assets/modelData/preset/vector.tkm";

			constexpr float kVectorFontShadowOffset = 1.0f;	//!< ベクトルのフォントのシャドウオフセット
			static const Vector4 kVectorFontColor = Vector4::Black;	//!< ベクトルのフォントのカラー
			constexpr float kVectorFontScale = 0.4f;	//!< ベクトルのフォントの拡大率
			constexpr float kVectorModelLength = 100.0f;	//!< ベクトルのモデルの長さ
			constexpr float kVectorModelWidthAndHeightScale = 0.2f;	//!< ベクトルのモデルの幅と高さ倍率
		}
	}
}
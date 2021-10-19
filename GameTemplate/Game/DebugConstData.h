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
			//////// ベクトル描画クラスの定数 ////////

			//!< ベクトルのデフォルトの名前
			constexpr const char* const kDefaultVectorName = "vector";
		
			//!< ベクトルのモデルの表示
			constexpr const char* const kVectorModel = "Assets/modelData/preset/vector.tkm";

			constexpr float kVectorFontShadowOffset = 1.0f;	//!< ベクトルのフォントのシャドウオフセット
			constexpr float kVectorFontScale = 0.4f;	//!< ベクトルのフォントの拡大率
			constexpr float kVectorModelLength = 100.0f;	//!< ベクトルのモデルの長さ
			constexpr float kVectorModelWidthAndHeightScale = 0.2f;	//!< ベクトルのモデルの幅と高さ倍率


			//////// デバックテキスト描画クラスの定数 ////////

			constexpr int kTextSize = 128;	//!< テキストのサイズ
			//!<　デフォルトのテキストの座標
			static const Vector2 kDefaultTextPosition = { 400.0f,300.0f };
			constexpr float kDefaultTextScale = 0.5f;	//!< デフォルトのテキストの拡大率
			constexpr float kDefaultTextShadowOffset = 1.0f;	//!< デフォルトのテキストのシャドウオフセット
		}
	}
}
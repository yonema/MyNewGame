#pragma once

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief スプライト関連のネームスペース
		*/
		namespace nsSprite
		{
			/**
			 * @brief スプライトの定数データ
			*/
			namespace nsSpriteConstData
			{
				const Vector2 kDefaultPivot = { 0.5f,0.5f };							//!< デフォルトのピボット
				constexpr const char* kDefaultFxFilePath = "Assets/shader/sprite.fx";	//!< デフォルトのシェーダーのファイルパス
				constexpr const char* const kVsEntryPointFunc = "VSMain";				//1< 頂点シェーダーのエントリーポイント
				constexpr const char* const kPsEntryPointFunc = "PSMain";				//1< ピクセルシェーダーのエントリーポイント
			}
		}
	}
}
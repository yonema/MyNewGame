#pragma once

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief モデル関連のネームスペース
		*/
		namespace nsModel
		{
			/**
			 * @brief モデルの定数データ
			*/
			namespace nsModelConstData
			{
				constexpr const char* const kTkmFileExtensinon = ".tkm";	//!< tkmファイルの拡張子
				constexpr const char* const kTksFileExtensinon = ".tks";	//!< tksファイルの拡張子
				constexpr size_t kFileExtensionLength = 4;					//!< 拡張子の長さ

				constexpr const char* const kVsEntryPointFunc = "VSMain";					//!< 頂点シェーダのエントリーポイント
				constexpr const char* const kVsSkinEntryPointFunc = "VSSkinMain";			//!< スキンありの頂点シェーダーのエントリーポイント
				constexpr const char* const kPsEntryPointFunc = "PSMain";					//!< ピクセルシェーダのエントリーポイント
				constexpr const char* const kDefaultFxFilePath = "Assets/shader/model.fx";	//!< デフォルトのシェーダーファイルパス

			}
		}
	}
}
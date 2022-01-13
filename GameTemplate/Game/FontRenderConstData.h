#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief フォント関連のネームスペース
		*/
		namespace nsFont
		{
			/**
			 * @brief フォントの定数データ
			*/
			namespace nsFontConstData
			{
				constexpr int kTextMaxSize = 256;	//!< テキストの最大サイズ
				const Vector4 kDefaltTextColor = Vector4::White;	//!< フォントのデフォルトのカラー
			}
		}
	}
}
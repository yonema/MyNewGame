#pragma once
namespace nsNinjaAttract
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief ガウシアンブラーの定数データ
		*/
		namespace nsGaussianBlurConstData
		{
			constexpr int kWeightsNum = 8;		//!< 重みの数

			//!< fxファイルパス
			constexpr const char* const kfxFilePath = "Assets/shader/gaussianBlur.fx";

			//!< ピクセルシェーダーのエントリーポイント
			constexpr const char* const kPsEntryFunc = "PSBlur";
			//!< アルファチャンネルにもブラーをかけるときのピクセルシェーダーのエントリーポイント
			constexpr const char* const kPsEntryFunc_alpha = "PSBlur_Alpha";

			//!< 横ブラーをかけるときの頂点シェーダーのエントリーポイント
			constexpr const char* const kVsEntryFunc_xBlur = "VSXBlur";
			//!< 縦ブラーをかけるときの頂点シェーダーのエントリーポイント
			constexpr const char* const kVsEntryFunc_yBlur = "VSYBlur";

		}
	}
}
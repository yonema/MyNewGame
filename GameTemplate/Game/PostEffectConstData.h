#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief ポストエフェクト関連のネームスペース
		*/
		namespace nsPostEffect
		{
			/**
			 * @brief ポストエフェクトの定数データ
			*/
			namespace nsPostEffectConstDat
			{

			}

			/**
			 * @brief ブルームの定数データ
			*/
			namespace nsBloomConstData
			{
				constexpr int kGaussianBlurNum = 4;		//!< ガウシアンブラーの数
				constexpr int kGaussianBulrPower = 10;	//!< ガウシアンブラーのブラーの強さ

				//!< fxファイルパス
				constexpr const char* const kfxFilePath = "Assets/shader/bloom.fx";

				//!< 輝度抽出時のピクセルシェーダーのエントリーポイント
				constexpr const char* const kPsEntryFunc_samplingLuminance = "PSSamplingLuminance";
				//!< 最終合成時のピクセルシェーダーのエントリーポイント
				constexpr const char* const kPsEntryFunc_bloomFinal = "PSBloomFinal";
			}
		}
	}
}
#pragma once

namespace nsMyGame
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
#pragma once

namespace nsNinjaAttract
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

				//!< 頂点シェーダのエントリーポイント
				constexpr const char* const kVsEntryPointFunc = "VSMain";
				//!< スキンありの頂点シェーダーのエントリーポイント
				constexpr const char* const kVsSkinEntryPointFunc = "VSSkinMain";
				//!< インスタンシング描画用の頂点シェーダーのエントリーポイント
				constexpr const char* const kVsInstancingEntryPointFunc = "VSMainInstancing";
				//!< スキンあり、インスタンシング描画用の頂点シェーダーのエントリーポイント
				constexpr const char* const kVsSkinInstancingEntryPointFunc = "VSMainSkinInstancing";
				//!< ピクセルシェーダのエントリーポイント
				constexpr const char* const kPsEntryPointFunc = "PSMain";
				//!< GBufferに描画するモデルのシェーダーファイルパス
				constexpr const char* const kRenderToGBufferFxFilePath = "Assets/shader/RenderToGBufferFor3DModel.fx";
				//!< 半透明描画するモデルのシェーダーファイルパス
				constexpr const char* const kTranslucentModelFxFilePath = "Assets/shader/TranslucentModel.fx";
				//!< シャドウマップに描画するモデルのシェーダーファイルパス
				constexpr const char* const kDrawShadowMapFxFilePath = "Assets/shader/DrawShadowMap.fx";
				//!< デフォルトのシェーダーファイルパス
				constexpr const char* const kDefaultFxFilePath = kRenderToGBufferFxFilePath;

				constexpr float kAnimationInterpolateTime = 0.2f;	//!< アニメーション保管時間

				constexpr float kDefaultDistanceLOD = 10000.0f;		//!< デフォルトのLODの距離

			}
		}
	}
}
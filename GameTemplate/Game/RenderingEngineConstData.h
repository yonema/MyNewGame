#pragma once

namespace nsMyGame
{

	/**
	 * @brief このゲーム用のゲームエンジンネームスペース
	*/
	namespace nsMyEngine
	{
		/**
		 * @brief レンダリングエンジンの定数データ
		*/
		namespace nsRenderingEngineConstData
		{
			constexpr int m_kRenderObjectsCapacityNum = 512;	//!< 描画オブジェクトのキャパシティ

			//!< スプライトのfxファイルパス
			constexpr const char* const m_kSpriteFxFilePath = "Assets/shader/sprite.fx";
			//!< ディファ―ドライティングを行うためのスプライトのfxファイスパス
			constexpr const char* const m_kDefferdLightingSpriteFxFilePath = "Assets/shader/DeferredLighting.fx";

			/**
			 * @brief GBufferの列挙型
			 * @warning RenderToGBuffer3DModel.fxのSPSOutと同じ内容にする。（num以外）
			*/
			enum EnGBuffer
			{
				enGBufferAlbedoDepth,           // アルベドと深度値。αに深度値が記憶されています。
				enGBufferNormal,                // 法線
				enGBufferMetaricShadowSmooth,   // メタリック、影パラメータ、スムース。
												// メタリックがr、影パラメータがg、スムースがa。gは未使用。
				enGBufferNum,                   // G-Bufferの数
			};

			/**
			 * @brief ディファードライティング用の定数バッファの構造体
			 * @warning ここを変更したら、DefferdLighting.hのdefferdLightingCbも一緒に変更する
			*/
			struct SDefferdLightingCB
			{
				Matrix mViewProjInv;	//!< ビュープロジェクション行列の逆行列
			};

			/**
			 * @brief IBLに使用する定数バッファの構造体
			*/
			struct SIBLCB
			{
				bool isIBL = false;		//!< IBLを行うか？
				float IBLLuminance = 1.0f;	//!< IBLの明るさ
			};

		}
	}
}
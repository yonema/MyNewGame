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
			 * @brief モデルで使うデータのネームスペース
			*/
			namespace nsModelData
			{
				/**
				 * @brief 拡張定数バッファの構造体
				*/
				struct SExpandConstantBuffer
				{
					bool isShadowReciever = true;	//!< シャドウレシーバー？
				};
			}
		}
	}
}
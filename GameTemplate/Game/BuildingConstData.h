#pragma once

namespace nsMyGame
{

	/**
	 * @brief 建物関連のネームスペース
	*/
	namespace nsBuilding
	{
		/**
		 * @brief 建物関連の定数データ
		*/
		namespace nsBuildingConstData 
		{
			constexpr UINT kDefaultWidthSegments = 5;				//!< デフォルトの幅セグメント
			constexpr UINT kDefaultHeightSegments = 5;				//!< デフォルトの高さセグメント
			constexpr UINT kDefaultLengthSegments = 5;				//!< デフォルトの長さセグメント
			//constexpr float kDefaultHeightLowerLimit = 1000.0f;		//!< デフォルトの高さの下限
			//constexpr float kDefaultHeightLowerLimit = -10.0f;
			constexpr float kDefaultHeightLowerLimit = 0.0f;
		}
	}
}
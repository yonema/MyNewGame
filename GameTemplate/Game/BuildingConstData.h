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
			constexpr UINT kDefaultWidthSegments = 10;				//!< デフォルトの幅セグメント
			constexpr UINT kDefaultHeightSegments = 20;				//!< デフォルトの高さセグメント
			constexpr UINT kDefaultLengthSegments = 10;				//!< デフォルトの長さセグメント
			constexpr float kDefaultHeightLowerLimit = 1000.0f;		//!< デフォルトの高さの下限

			
			/**
			 * @brief 建物のタイプ
			*/
			enum EnBuildingType
			{
				enBuilding_L_01a,
				enBuilding_L_06b,
				enBuildingTypeNum
			};

			constexpr const wchar_t* const kBuildingForwardName = L"Building";	//!< 建物の先頭の名前

			/**
			 * @brief 建物の名前
			*/
			constexpr const char* const kBuildingNames[enBuildingTypeNum]
			{
				"Building_L_01a",
				"Building_L_06b"
			};

			/**
			 * @brief 建物のモデルのファイルパス
			*/
			constexpr const char* const kBuildingModelFilePath[enBuildingTypeNum]
			{
				"Assets/modelData/building/Building_L_01a.tkm",
				"Assets/modelData/building/Building_L_06b.tkm"
			};

		}
	}
}
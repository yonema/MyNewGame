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
				enBuilding_S_01a,
				enBuilding_S_01b,
				enBuilding_S_02a,
				enBuilding_S_02b,
				enBuilding_S_03a,
				enBuilding_S_03b,
				enBuilding_S_04a,
				enBuilding_S_04b,
				enBuilding_S_05a,
				enBuilding_S_05b,
				enBuilding_S_06a,
				enBuilding_S_06b,
				enBuilding_S_07a,
				enBuilding_S_08,
				enBuilding_S_09,
				enBuilding_S_10,
				enBuilding_S_11,

				enBuilding_L_01a,
				enBuilding_L_01b,
				enBuilding_L_02a,
				enBuilding_L_02b,
				enBuilding_L_03a,
				enBuilding_L_03b,
				enBuilding_L_04a,
				enBuilding_L_04b,
				enBuilding_L_05a,
				enBuilding_L_05b,
				enBuilding_L_06a,
				enBuilding_L_06b,

				enBuildingTypeNum	//!< 建物のタイプの数
			};

			constexpr const wchar_t* const kBuildingForwardName = L"Building";	//!< 建物の先頭の名前
			constexpr const char* const kBuildingObjectName = "Building";	//!< 建物のオブジェクトの名前

			//constexpr std::map< const EnBuildingType, const char* const> kBuildingNamesMap =
			//{

			//};
			/**
			 * @brief 建物の名前
			*/
			constexpr const char* const kBuildingNames[enBuildingTypeNum]
			{
				"Building_S_01a",
				"Building_S_01b",
				"Building_S_02a",
				"Building_S_02b",
				"Building_S_03a",
				"Building_S_03b",
				"Building_S_04a",
				"Building_S_04b",
				"Building_S_05a",
				"Building_S_05b",
				"Building_S_06a",
				"Building_S_06b",
				"Building_S_07a",
				"Building_S_08",
				"Building_S_09",
				"Building_S_10",
				"Building_S_11",

				"Building_L_01a",
				"Building_L_01b",
				"Building_L_02a",
				"Building_L_02b",
				"Building_L_03a",
				"Building_L_03b",
				"Building_L_04a",
				"Building_L_04b",
				"Building_L_05a",
				"Building_L_05b",
				"Building_L_06a",
				"Building_L_06b"
			};

			/**
			 * @brief 建物のモデルのファイルパス
			*/
			constexpr const char* const kBuildingModelFilePath[enBuildingTypeNum]
			{
				"Assets/modelData/buildings/Building_S_01a.tkm",
				"Assets/modelData/buildings/Building_S_01b.tkm",
				"Assets/modelData/buildings/Building_S_02a.tkm",
				"Assets/modelData/buildings/Building_S_02b.tkm",
				"Assets/modelData/buildings/Building_S_03a.tkm",
				"Assets/modelData/buildings/Building_S_03b.tkm",
				"Assets/modelData/buildings/Building_S_04a.tkm",
				"Assets/modelData/buildings/Building_S_04b.tkm",
				"Assets/modelData/buildings/Building_S_05a.tkm",
				"Assets/modelData/buildings/Building_S_05b.tkm",
				"Assets/modelData/buildings/Building_S_06a.tkm",
				"Assets/modelData/buildings/Building_S_06b.tkm",
				"Assets/modelData/buildings/Building_S_07a.tkm",
				"Assets/modelData/buildings/Building_S_08.tkm",
				"Assets/modelData/buildings/Building_S_09.tkm",
				"Assets/modelData/buildings/Building_S_10.tkm",
				"Assets/modelData/buildings/Building_S_11.tkm",

				"Assets/modelData/buildings/Building_L_01a.tkm",
				"Assets/modelData/buildings/Building_L_01b.tkm",
				"Assets/modelData/buildings/Building_L_02a.tkm",
				"Assets/modelData/buildings/Building_L_02b.tkm",
				"Assets/modelData/buildings/Building_L_03a.tkm",
				"Assets/modelData/buildings/Building_L_03b.tkm",
				"Assets/modelData/buildings/Building_L_04a.tkm",
				"Assets/modelData/buildings/Building_L_04b.tkm",
				"Assets/modelData/buildings/Building_L_05a.tkm",
				"Assets/modelData/buildings/Building_L_05b.tkm",
				"Assets/modelData/buildings/Building_L_06a.tkm",
				"Assets/modelData/buildings/Building_L_06b.tkm"
			};

		}
	}
}
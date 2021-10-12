#pragma once

namespace nsMyGame
{
	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		/**
		 * @brief マップの定数データ
		*/
		namespace nsMapConstData
		{
			constexpr const char* const kPlayerName = "player";				//!< プレイヤーの名前
			constexpr const char* const kBuildingName = "testBuilding";		//!< 建物の名前
			constexpr const char* const kGoalName = "goal";					//!< ゴールの名前

			/**
			 * @brief レベルの種類
			*/
			enum EnLevelType
			{
				enLevelTest,	//!< テスト用レベル
				enLevelProto,	//!< プロト用レベル
				enLevelTypeNum	//!< レベルの種類の数
			};

			/**
			 * @brief レベルのファイルパス
			*/
			constexpr const char* const kLevelFilePath[enLevelTypeNum]
			{
				"Assets/levelData/testLevel.tkl",
				"Assets/levelData/level_proto.tkl"
			};

		}
	}
}
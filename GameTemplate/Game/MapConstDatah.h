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
			constexpr const char* const kGoalName = "goal";					//!< ゴールの名前

			/**
			 * @brief レベルの種類
			*/
			enum EnLevelType
			{
				enLevelProto,	//!< プロト用レベル
				enLevelProto2,	//!< プロト用レベル2
				enLevelProto3,	//!< プロト用レベル3
				enLevelAlpha,	//!< アルファ用レベル
				enLevelAlpha2,	//!< アルファ2用レベル
				enLevelmStreetLight,	//!< 電灯用のレベル
				enLevelTrafficLight,	//!< 信号用のレベル
				enLevelPedestrianLight,	//!< 歩行者信号用のレベル
				enLevelStreetTree,	//!< 木用のレベル
				enLevelStreetTreeBranch,//!< 木の枝用のレベル
				enLevelTypeNum	//!< レベルの種類の数
			};

			/**
			 * @brief レベルのファイルパス
			*/
			constexpr const char* const kLevelFilePath[enLevelTypeNum]
			{
				"Assets/levelData/level_proto.tkl",
				"Assets/levelData/level_proto2.tkl",
				"Assets/levelData/level_proto3.tkl",
				"Assets/levelData/level_alpha.tkl",
				"Assets/levelData/level_alpha2.tkl",
				"Assets/levelData/level_StreetLight.tkl",
				"Assets/levelData/level_TrafficLight.tkl",
				"Assets/levelData/level_PedestrianLight.tkl",
				"Assets/levelData/level_StreetTree.tkl",
				"Assets/levelData/level_StreetTree_Branch.tkl"

			};

			/**
			 * @brief マップチップの予約数の種類
			*/
			enum EnNumMapChipReserve
			{
				enReserveStreetLight,
				enReserveTrafficLight,
				enReservePedestrianLight,
				enReserveStreetTree,
				enReserveNum,
			};

			//!< マップチップの予約数テーブル
			constexpr int kNumMapChipReserveTbl[enReserveNum] =
			{
				604,
				146,
				143,
				458
			};

		}
	}
}
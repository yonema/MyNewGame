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
				"Assets/levelData/level_alpha2.tkl"
			};

			/**
			 * @brief 小物の種類
			*/
			enum EnPropsType
			{
				enPropsStreetLight,			//!< 街灯用のレベル
				enPropsTrafficLight,		//!< 信号機用のレベル
				enPropsPedestrianLight,		//!< 歩行者信号機用のレベル
				enPropsStreetTree,			//!< 街路樹用のレベル
				enPropsStreetTreeBranch,	//!< 街路樹の枝用のレベル
				enPropsTypeNum,				//!< 小物の種類の数
			};

			/**
			 * @brief 小物のレベルのファイルパス
			*/
			constexpr const char* const kPropsLevelFilePath[enPropsTypeNum] =
			{
				"Assets/levelData/level_StreetLight.tkl",
				"Assets/levelData/level_TrafficLight.tkl",
				"Assets/levelData/level_PedestrianLight.tkl",
				"Assets/levelData/level_StreetTree.tkl",
				"Assets/levelData/level_StreetTree_Branch.tkl"
			};

			//!< マップチップの予約数テーブル
			constexpr int kNumMapChipReserveTbl[enPropsTypeNum] =
			{
				604,
				146,
				143,
				458,
				458
			};

		}
	}
}
#pragma once

namespace nsNinjaAttract
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
				enLevelPlateAndBuildings,	//!< 地面と建物用レベル
				enLevelPlayer,	//!< プレイヤー用レベル
				enLevelCar,		//!< 車用レベル
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
				"Assets/levelData/plateAndBuildings.tkl",
				"Assets/levelData/playerLevel.tkl",
				"Assets/levelData/carLevel.tkl"
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

			//!< 小物のLOD用のモデルのファイルパス
			constexpr const char* const kPropsLODModelFilePath[enPropsTypeNum] =
			{
				"Assets/modelData/levelSource/StreetLight_LOD.tkm",
				"Assets/modelData/levelSource/TrafficLight_LOD.tkm",
				nullptr,
				"Assets/modelData/levelSource/StreetTree_LOD.tkm",
				"Assets/modelData/levelSource/StreetTree_Branch_LOD.tkm"
			};

			//!< 小物のLOD用の距離
			constexpr float kPropsLODDistance[enPropsTypeNum] =
			{
				1000.0f,
				1000.0f,
				-1.0f,
				-1.0f,
				10000.0f
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

			//!< タイトル中のプレイヤーのレベル上の名前
			constexpr const char* const kPlayerLevelNameInTitle = "player_inTitle";
			//!< ゲーム中のプレイヤーのレベル上の名前
			constexpr const char* const kPlayerLevelNameInGame = "player_inGame";

		}
	}
}
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

			//!< ブロッキングボリュームの座標
			static const Vector3 kBlockingVolumePosition[4]
			{
				{0.0f,0.0f,-3300.0f},
				{35500.0f,0.0,32200.0f},
				{0.0f,0.0f,67700.0f},
				{-35000.0f,0.0,32200.0f},
			};

			//!< ブロッキングボリュームの拡大率
			static const Vector3 kBlockingVolumeScale = { 70000.0f + 1000.0f,20000.0f,1000.0f };

			//!< タイトル中のプレイヤーのレベル上の名前
			constexpr const char* const kPlayerLevelNameInTitle = "player_inTitle";
			//!< ゲーム中のプレイヤーのレベル上の名前
			constexpr const char* const kPlayerLevelNameInGame = "player_inGame";

			//!< 決定音のサウンドのファイルパス
			constexpr const wchar_t* const kDecisionSoundFilePath = L"Assets/sound/decision.wav";

			//!< タイトルのスプライトのファイスパス
			constexpr const char* const kTitleSpriteFilePath = "Assets/Image/title/title.DDS";
			//!< スタートのスプライトのファイスパス
			constexpr const char* const kTitleStartSpriteFilePath = "Assets/Image/title/title_start.DDS";
			//!< タイトルのスプライトの幅
			constexpr int kTitleSpriteWidth = static_cast<int>(512.0f + 1.5f);
			//!< タイトルのスプライトの高さ
			constexpr int kTitleSpriteHeight = static_cast<int>(512.0f + 1.5f);
			//!< スタートのスプライトの幅
			constexpr int kTitleStartSpriteWidth = static_cast<int>(512.0f + 0.4f);
			//!< スタートのスプライトの高さ
			constexpr int kTitleStartSpriteHeight = static_cast<int>(512.0f + 0.4f);
			//!< タイトルのスプライトの座標
			static const Vector2 kTitleSpritePosition = { -200.0f,100.0f };
			//!< スタートのスプライトの座標
			static const Vector2 kTitleStartSpritePosition = { kTitleSpritePosition.x,kTitleSpritePosition .y - 100.0f};
			//!< タイトルのスプライトの最初の高さ
			constexpr float kTitleSpriteInitialHeight = 50.0f;

			/**
			 * @brief タイトルのステート
			*/
			enum EnTitleState
			{
				enTS_beforeFadeIn,
				enTS_fadeIn,
				enTS_titleIn,
				enTS_titleIdle,
				enTS_titleOut,
				enTS_swingDirecting,
				enTS_fadeOut
			};

			constexpr float kTitleInTime = 1.0f;		//!< タイトルインのタイム
			constexpr float kTitleIdleLoopTime = 5.0f;	//!< タイトルアイドルのループタイム
			constexpr float kTitleOutTime = 1.0f;		//!< タイトルアウトのタイム

			constexpr float kStartDirectingTime = 4.0f;				//!< 開始演出のタイム
			constexpr float kFadeOutToClearDirectingTime = 0.5f;	//!< クリア演出の前のフェードアウトのタイム
			constexpr float kFadeInToClearDirectingTime = 0.5f;		//!< クリア演出の前のフェードインのタイム
			constexpr float kFadeOutGoTitleMinActiveTime = 1.5f;	//!< タイトルへ遷移するためのフェードアウトの最小有効タイム
			constexpr float kFadeOutGoTitleMaxActiveTime = 1.7f;	//!< タイトルへ遷移するためのフェードアウトの最大有効タイム
			constexpr float kFadeOutGoTitleTime = 0.5f;				//!< タイトルへ遷移するためのフェードアウトのタイム
			constexpr float kGoTitleDirectingTime = 2.5f;			//< タイトルへ行くための演出のタイム
		}
	}
}
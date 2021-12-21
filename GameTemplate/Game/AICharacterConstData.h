#pragma once

namespace nsMyGame
{

	/**
	 * @brief AIキャラクター関連のネームスペース
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief AIキャラクターの定数データ
		*/
		namespace nsAICharacterConstData
		{
			// 車のモデルデータのファイルパス
			constexpr const char* const kCarModelFilePath = "Assets/modelData/car/sedan_car.tkm";

			// 実際のターゲットポイントの間隔
			constexpr float kRealBetweenTargetPoint = 11200.0f;
			// ターゲットポイントの感覚のゆとり
			constexpr float kBetweenTargetPointBuff = 1000.0f;
			// 使用するターゲットポイントの間隔
			constexpr float kBetweenTargetPoint = kRealBetweenTargetPoint + kBetweenTargetPointBuff;
			// 使用するターゲットポイントの最低間隔
			constexpr float kMinBetweenTargetPoint = 100.0f;
			// 角度のしきい値
			// 1.22rad≒70.0deg
			constexpr float kAngleThreshold = 1.22f;
			// 直進用の角度のしきい値
			// 0.52rad≒30.0deg
			constexpr float kStraightAngleThreshold = 0.52f;

			/**
			 * @brief ターゲットポイントの候補の種類
			*/
			enum EnCandidateTargetPointType
			{
				enTurnRight,					//!< 右折
				enTurnLeft,						//!< 左折
				enGoStraight,					//!< 直進
				enCandidateTargetPointTypeNum	//!< 候補の種類の数
			};
		}
	}
}
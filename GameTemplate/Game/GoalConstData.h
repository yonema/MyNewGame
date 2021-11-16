#pragma once


namespace nsMyGame
{
	/**
	 * @brief ゴール関連のネームスペース
	*/
	namespace nsGoal
	{
		/**
		 * @brief ゴールクラスの定数データ
		*/
		namespace nsGoalConstData
		{
			//!< ゴールのモデルファイルパス
			constexpr const char* const kGoalModelFilePath = "Assets/modelData/props/Scroll.tkm";

			constexpr float kRotateSpeed = 1.0f;	//!< 回転スピード

			constexpr float kTiltAngle = 45.0f;	//!< 傾きの角度

			constexpr float kGoalThreshold = 75.0f;	//!< ゴールのしきい値

		}
	}
}
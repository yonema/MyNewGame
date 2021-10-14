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
			constexpr const char* const kGoalModelFilePath = "Assets/modelData/levelSource/goal.tkm";

			constexpr float kGoalThreshold = 75.0f;	//!< ゴールのしきい値

		}
	}
}
#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief 共通データ
	*/
	namespace nsCommonData
	{

		/**
		 * @brief ゲームオブジェクトの名前の種類
		*/
		enum EnGameObjectName
		{
			enGN_MainGameState,
			enGN_Car,
			enGameObjectNameNum
		};

		/**
		 * @brief ゲームオブジェクトの名前
		*/
		constexpr const char* const kGameObjectName[enGameObjectNameNum]
		{
			"MainGameState",
			"Car"
		};
	}
}
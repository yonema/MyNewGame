#pragma once

namespace nsMyGame
{
	/**
	 * @brief 共通データ
	*/
	namespace nsCommonData
	{
		/**
		 * @brief GameObjectをNewGOするときの優先度(priority)の列挙型
		*/
		enum EnPrioriry
		{
			enPriorityFirst = 0,	//!< 一番目。優先度0
			enPrioritySecond = 10,	//!< 二番目。優先10
			enPriorityThird = 20,	//!< 三番目。優先度20
			enPriorityFourth = 30,	//!< 四番目。優先度30
			enPriorityFinal,		//!< 最後の優先度
		};

		/**
		 * @brief ゲームオブジェクトの名前の種類
		*/
		enum EnGameObjectName
		{
			enMainGameState,
			enGameObjectNameNum
		};

		/**
		 * @brief ゲームオブジェクトの名前
		*/
		constexpr const char* const kGameObjectName[enGameObjectNameNum]
		{
			"MainGameState"
		};
	}
}
#pragma once

namespace nsMyGame
{
	/**
	 * @brief 興津データ
	*/
	namespace nsCommonData
	{
		/**
		 * @brief GameObjectをNewGOするときの優先度(priority)の列挙型
		*/
		enum EnPrioriry
		{
			enPriorityFirst = 0,	//一番目。優先度0
			enPrioritySecond = 10,	//二番目。優先10
			enPriorityThird = 20,	//三番目。優先度20
			enPriorityFourth = 30,	//四番目。優先度30
		};
	}
}
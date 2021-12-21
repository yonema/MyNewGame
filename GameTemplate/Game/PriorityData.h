#pragma once

namespace nsMyGame
{
	/**
	 * @brief ���ʃf�[�^
	*/
	namespace nsCommonData
	{
		/**
		 * @brief GameObject��NewGO����Ƃ��̗D��x(priority)�̗񋓌^
		*/
		enum EnPrioriry
		{
			enPriorityFirst = 0,	//!< ��ԖځB�D��x0
			enPrioritySecond = 10,	//!< ��ԖځB�D��10
			enPriorityThird = 20,	//!< �O�ԖځB�D��x20
			enPriorityFourth = 30,	//!< �l�ԖځB�D��x30
			enPriorityFinal,		//!< �Ō�̗D��x
		};

		/**
		 * @brief �Q�[���I�u�W�F�N�g�̖��O�̎��
		*/
		enum EnGameObjectName
		{
			enMainGameState,
			enGameObjectNameNum
		};

		/**
		 * @brief �Q�[���I�u�W�F�N�g�̖��O
		*/
		constexpr const char* const kGameObjectName[enGameObjectNameNum]
		{
			"MainGameState"
		};
	}
}
#include "stdafx.h"
#include "PlayerCatchEnemy.h"

namespace nsMyGame
{

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{

		/**
		 * @brief ����������
		 * @param[in] playerRef �v���C���[�̎Q��
		*/
		void CPlayerCatchEnemy::Init(const CPlayer& playerRef)
		{
			m_playerRef = &playerRef;

			return;
		}

		/**
		 * @brief ���C���[���G��߂܂��鏈���N���X�̃A�b�v�f�[�g�����s
		*/
		void CPlayerCatchEnemy::ExecuteUpdate()
		{


			return;
		}

	}
}
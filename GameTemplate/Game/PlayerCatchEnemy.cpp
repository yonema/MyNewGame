#include "stdafx.h"
#include "PlayerCatchEnemy.h"
#include "Player.h"

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
		void CPlayerCatchEnemy::Init(CPlayer* playerRef)
		{
			m_playerRef = playerRef;

			return;
		}

		/**
		 * @brief ���C���[���G��߂܂��鏈���N���X�̃A�b�v�f�[�g�����s
		*/
		void CPlayerCatchEnemy::ExecuteUpdate()
		{
			if (m_playerRef->GetInputData().actionCatchEnemy)
			{
				m_playerRef->ChangeOnEnemyState();
			}

			return;
		}

	}
}
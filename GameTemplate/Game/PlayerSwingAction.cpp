#include "stdafx.h"
#include "PlayerSwingAction.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "StringActionTargetManager.h"

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		/**
 * @brief �v���C���[�̓����֘A�̃l�[���X�y�[�X
*/
		namespace nsPlayerMovenent
		{

			/**
			 * @brief ������
			 * @param[in,out] player �v���C���[
			 * @param[in,out] playerMovement �v���C���[�ړ��N���X�̎Q��
			*/
			void CPlayerSwingAction::Init(
				CPlayer* player,
				CPlayerMovement* playerMovement
			)
			{
				// �v���C���[�̎Q�Ƃ��Z�b�g
				m_playerRef = player;
				// ���Z�ړ��x�N�g���̎Q�Ƃ��Z�b�g
				m_playerMovementRef = playerMovement;

				return;
			}

			/**
			 * @brief �X�C���O�A�N�V���������s
			*/
			void CPlayerSwingAction::Execute()
			{
				const Vector3* target = nullptr;
				if (m_playerRef->GetInputData().actionSwing)
				{
					target = nsStringActionTarget::GetNearestSwintTargetPointWithinScope(
						m_playerRef->GetPosition(), 2000.0f
					);
					m_playerMovementRef->SetAddMoveVec(m_playerRef->GetPosition());

					if (target != nullptr)
					{
						//m_playerMovementRef->SetAddMoveVec(*target);
						m_playerRef->StartStringStretchToPos(*target);
					}
					//else
					//{
					//	m_playerMovementRef->SetAddMoveVec(m_playerRef->GetPosition());
					//}

					
				}


				return;
			}
		}
	}
}
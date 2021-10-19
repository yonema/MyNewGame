#include "stdafx.h"
#include "PlayerMovement.h"
#include "Player.h"

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
			// �v���C���[�ړ��N���X�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsPlayerConstData::nsPlayerMoveConstData;

			/**
			 * @brief ������
			 * @param[in] radius �J�v�Z���R���C�_�[�̔��a
			 * @param[in] height �J�v�Z���R���C�_�[�̍���
			 * @param[in,out] player �v���C���[�̎Q��
			*/
			void CPlayerMovement::Init(
				const float radius,
				const float height,
				CPlayer* player
			)
			{
				// �v���C���[�̎Q�Ƃ��Z�b�g����
				m_playerRef = player;

				// �L�����N�^�[�R���g���[��������
				m_charaCon.Init(radius, height, m_playerRef->GetPosition());

				// �v���C���[�̒ʏ�̓����N���X�̏�����
				m_playerNormalMovement.Init(*m_playerRef, this);

				// �v���C���[�̃X�C���O�A�N�V�����N���X�̏�����
				m_playerSwingAction.Init(m_playerRef, this);

				return;
			}

			/**
			 * @brief �v���C���[�̈ړ��N���X�̃A�b�v�f�[�g�����s����
			*/
			void CPlayerMovement::ExecuteUpdate()
			{
				// �X�C���O�g���K�[��true�ŁA���A�󒆂�������
				if (m_playerRef->GetInputData().triggerSwing && IsAir())
				{
					// �X�C���O���
					m_playerRef->SetState(nsPlayerConstData::enSwing);
				}
				else
				{
					// �����Ƒ�����
					//m_playerRef->SetState(nsPlayerConstData::enWalkAndRun);
				}

				// �v���C���[�̈ړ����X�V
				UpdateMovePlayer();

				// �v���C���[�̉�]���X�V
				UpdateTurnPlayer();

				return;
			}

			/**
			 * @brief �L�����N�^�[�R���g���[���[���g�����ړ�
			*/
			void CPlayerMovement::MoveWithCharacterController()
			{
				// �L�����N�^�[�R���g���[���[���s
				m_playerRef->SetPosition(
					m_charaCon.Execute(m_moveVec, nsTimer::GameTime().GetFrameDeltaTime())
				);

				return;
			}


			/**
			 * @brief �v���C���[�̈ړ����X�V
			*/
			void CPlayerMovement::UpdateMovePlayer()
			{
				bool executeWalkAndRunFlag = true;

				// �X�e�[�g�ŏ�����U�蕪����
				switch (m_playerRef->GetState())
				{
				// �X�C���O
				case nsPlayerConstData::enSwing:

					// �X�C���O�A�N�V���������s
					executeWalkAndRunFlag = m_playerSwingAction.Execute();

					break;

				// �����g�����A�N�V�����̌�̋󒆏��
				case nsPlayerConstData::enAirAfterStringAction:
					// �n�ʂɂ�������
					if (!IsAir())
					{
						// �X�e�[�g������Ƒ����ԂɑJ��
						m_playerRef->SetState(nsPlayerConstData::enWalkAndRun);
					}
					break;
				}

				if (executeWalkAndRunFlag)
				{
					// �ʏ�̓��������s
					m_playerNormalMovement.Execute();
				}


				nsDebug::DrawVector(m_playerRef->GetPosition(), m_moveVec, "playerVec");


				return;
			}

			/**
			 * @brief �v���C���[�̉�]���X�V
			*/
			void CPlayerMovement::UpdateTurnPlayer()
			{
				// X,Z���ʂł̈ړ������邩�H
				if (fabsf(m_moveVec.x) < kMoveVecMin && fabsf(m_moveVec.z) < kMoveVecMin)
				// �����͂����邩�H
				//if (fabsf(m_playerRef->GetInputData().axisMoveForward) < 0.001f &&
				//	fabsf(m_playerRef->GetInputData().axisMoveRight) < 0.001f)
				{
					// �ړ����Ă��Ȃ�
					// �������^�[��
					return;
				}

				// tan�� = m_moveVec.x / m_moveVec.y
				const float radAngle = atan2(m_moveVec.x, m_moveVec.z);
				// ���W�A���P�ʂŉ�
				Quaternion nexrQRot;
				nexrQRot.SetRotation(Vector3::AxisY, radAngle);
				// ���݂̉�]�Ǝ��̉�]�����ʐ��`��Ԃ��s���A���f�������X�ɉ�]������B
				nexrQRot.Slerp(kModelRotRate, m_playerRef->GetRotation(), nexrQRot);
				m_playerRef->SetRotation(nexrQRot);

				return;
			}
		}
	}
}
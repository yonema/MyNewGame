#include "stdafx.h"
#include "PlayerWalkAndRun.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "PlayerConstData.h"

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
			// �v���C���[�̕����Ƒ���N���X�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsPlayerConstData::nsPlayerWalkAndRunConstData;
			// �����͂̍ŏ��l���g�p�\�ɂ���
			using nsPlayerConstData::nsPlayerInputConstData::kInputAxisMin;

			/**
			 * @brief ������
			 * @param[in] player �v���C���[
			 * @param[in,out] playerMovement �v���C���[�ړ��N���X�̎Q��
			*/
			void CPlayerWalkAndRun::Init(
				const CPlayer& player,
				CPlayerMovement* playerMovement
			)
			{
				// �v���C���[�̎Q�Ƃ��Z�b�g
				m_playerRef = &player;
				// ���Z�ړ��x�N�g���̎Q�Ƃ��Z�b�g
				m_playerMovementRef = playerMovement;

				return;
			}

			/**
			 * @brief �����Ƒ���̏��������s
			*/
			void CPlayerWalkAndRun::Execute()
			{
				// �����͒l���X�V����
				UpdateInputAxisParam();

				// ���������肩�����߂�
				WalkOrRun();

				// �ړ����x����������
				Acceleration();

				// ���C���v�Z����
				Friction();

				// �ړ����x�ɐ�����������
				LimitSpeed();

				// �O�̃t���[���̑��x���X�V
				m_oldVelocity = m_playerMovementRef->GetAddMoveVec().Length();

				return;
			}

			/**
			 * @brief �����͒l���X�V
			*/
			void CPlayerWalkAndRun::UpdateInputAxisParam()
			{
				// �O�A��ړ��̎�����
				m_inputMoveF = m_playerRef->GetInputData().axisMoveForward;
				// �E�A���ړ��̎�����
				m_inputMoveR = m_playerRef->GetInputData().axisMoveRight;

				// �O�A��ړ��̎����͂̐�Βl
				m_absInputMoveF = fabsf(m_inputMoveF);
				// �E�A���ړ��̎����͂̐�Βl
				m_absInputMoveR = fabsf(m_inputMoveR);

				return;
			}

			/**
			 * @brief ���������肩�����߂�
			*/
			void CPlayerWalkAndRun::WalkOrRun()
			{
				// �_�b�V�����͂�����Ă��Ȃ�
				if (m_playerRef->GetInputData().actionDush != true)
				{
					// �������

					// �����x��ݒ�
					m_acceleration = kWalkAcceleration;
					// �ō����x��ݒ�
					m_maxSpeed = kWalkMaxSpeed;
				}
				else
				{
					// �_�b�V�����
										
					// �����x��ݒ�
					m_acceleration = kRunAcceleration;
					// �ō����x��ݒ�
					m_maxSpeed = kRunMaxSpeed;
				}

				return;
			}

			/**
			 * @brief �ړ����x������������
			*/
			void CPlayerWalkAndRun::Acceleration()
			{
				// �ړ��̑O����
				Vector3 moveForward = m_playerRef->GetCamera().GetCameraForward();
				// �ړ��̉E����
				Vector3 moveRight = m_playerRef->GetCamera().GetCameraRight();
				// Y������������XZ���ʂł̑O�����ƉE�����ɕϊ�����
				moveForward.y = 0.0f;
				moveForward.Normalize();
				moveRight.y = 0.0f;
				moveRight.Normalize();

				//�������ւ̈ړ����x�����Z�B
				m_playerMovementRef->SetAddMoveVec(
					m_playerMovementRef->GetAddMoveVec() + moveForward * m_inputMoveF * m_acceleration
				);
				//�������ւ̈ړ����x�����Z�B
				m_playerMovementRef->SetAddMoveVec(
					m_playerMovementRef->GetAddMoveVec() + moveRight * m_inputMoveR * m_acceleration
				);

				return;
			}

			/**
			 * @brief ���C�̌v�Z
			*/
			void CPlayerWalkAndRun::Friction()
			{
				// ���C��
				float friction = 0.0f;

				// �󒆂��H
				if (m_playerMovementRef->IsAir())
				{
					// �󒆂̖��C��
					friction = kAirFriction;
				}
				else
				{
					// �n�ʏ�̖��C��
					friction = kGroundFriction;
				}

				// �ړ��x�N�g���̑傫��
				const float moveVecLen = m_playerMovementRef->GetAddMoveVec().Length();

				// ���C���v�Z����
				if (moveVecLen <= kMinSpeed)
				{
					// �ړ����x���Œᑬ�x�ȉ��Ȃ�
					// �ړ��[���ɂ���
					m_playerMovementRef->SetAddMoveVec(Vector3::Zero);
				}
				else if (m_absInputMoveF <= kInputAxisMin && m_absInputMoveR <= kInputAxisMin)
				{
					// ���͂��Ȃ�������
					// ���C�Č�������
					m_playerMovementRef->SetAddMoveVec(
						m_playerMovementRef->GetAddMoveVec() * friction
					);
				}
				else if (moveVecLen < m_oldVelocity)
				{
					// �O�̃t���[����葬�x�������Ă�����
					// ���C�Č�������
					m_playerMovementRef->SetAddMoveVec(
						m_playerMovementRef->GetAddMoveVec() * friction
					);
				}

				return;
			}

			/**
			 * @brief �ړ����x�ɑ��x������������
			*/
			void CPlayerWalkAndRun::LimitSpeed()
			{
				// �����͂����邩�H
				if (m_absInputMoveF <= kInputAxisMin && m_absInputMoveR <= kInputAxisMin)
				{
					// �Ȃ��ꍇ�͑��x�����������Ȃ�
					// �������^�[��
					return;
				}


				// �ړ����x���ō����x���I�[�o�[���Ă��邩�H
				if (m_playerMovementRef->GetAddMoveVec().Length() > m_maxSpeed)
				{
					// �I�[�o�[���Ă�����ō����x���ێ�
					m_playerMovementRef->NormalizeAddMoveVec();
					m_playerMovementRef->SetAddMoveVec(m_playerMovementRef->GetAddMoveVec() * m_maxSpeed);
				}

				return;
			}

		}
	}
}
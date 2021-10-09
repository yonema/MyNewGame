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
				// �O�A��ړ��̎�����
				const float inputAxisMoveForward = m_playerRef->GetInputData().axisMoveForward;
				// �E�A���ړ��̎�����
				const float inputAxisMoveRight = m_playerRef->GetInputData().axisMoveRight;

				// �O�A��ړ��̎����͂̐�Βl
				const float absInputAxisMoveForward = fabsf(inputAxisMoveForward);
				// �E�A���ړ��̎����͂̐�Βl
				const float absInputAxisMoveRight = fabsf(inputAxisMoveRight);

				// �ړ����x����������
				Acceleration(inputAxisMoveForward, inputAxisMoveRight);

				// ���C���v�Z����
				Friction(absInputAxisMoveForward, absInputAxisMoveRight);

				// �ړ����x�ɐ�����������
				LimitSpeed(absInputAxisMoveForward, absInputAxisMoveRight);

				// �O�̃t���[���̑��x���X�V
				m_oldVelocity = m_playerMovementRef->GetAddMoveVec().Length();

				return;
			}

			/**
			 * @brief �ړ����x������������
			 * @param inputMoveF �O�A��̎�����
			 * @param inputMoveR �E�A���̎�����
			*/
			void CPlayerWalkAndRun::Acceleration(const float inputMoveF, const float inputMoveR)
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
					m_playerMovementRef->GetAddMoveVec() + moveForward * inputMoveF * kAcceleration
				);
				//�������ւ̈ړ����x�����Z�B
				m_playerMovementRef->SetAddMoveVec(
					m_playerMovementRef->GetAddMoveVec() + moveRight * inputMoveR * kAcceleration
				);

				return;
			}

			/**
			 * @brief ���C�̌v�Z
			 * @param[in] absInputMoveF �O�A��̎����͂̐�Βl
			 * @param[in] absInputMoveR �E�A���̎����͂̐�Βl
			*/
			void CPlayerWalkAndRun::Friction(const float absInputMoveF, const float absInputMoveR)
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
				else if (absInputMoveF <= 0.001f && absInputMoveR <= 0.001f)
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
			 * @param[in] absInputMoveF �O�A��̎����͂̐�Βl
			 * @param[in] absInputMoveR �E�A���̎����͂̐�Βl
			*/
			void CPlayerWalkAndRun::LimitSpeed(const float absInputMoveF, const float absInputMoveR)
			{
				// �����͂����邩�H
				if (absInputMoveF <= 0.001f && absInputMoveR <= 0.001f)
				{
					// �Ȃ��ꍇ�͑��x�����������Ȃ�
					// �������^�[��
					return;
				}


				// �ړ����x���ō����x���I�[�o�[���Ă��邩�H
				if (m_playerMovementRef->GetAddMoveVec().Length() > kMaxSpeed)
				{
					// �I�[�o�[���Ă�����ō����x���ێ�
					m_playerMovementRef->NormalizeAddMoveVec();
					m_playerMovementRef->SetAddMoveVec(m_playerMovementRef->GetAddMoveVec() * kMaxSpeed);
				}

				return;
			}

		}
	}
}
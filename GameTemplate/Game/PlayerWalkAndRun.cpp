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
			// �W�����v�͂��g�p�\�ɂ���
			using nsPlayerConstData::nsPlayerMoveConstData::kJumpForce;

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

				// �������v�Z
				CalcAcceleration();

				// ���C���v�Z
				CalcFriction();

				// ���x�����̌v�Z
				CalcLimitSpeed();

				// ���ۂɈړ�������
				Move();

				// �O�̃t���[���̑��x���X�V
				m_oldVelocity = m_addMoveVec.Length();

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
			 * @brief �������v�Z
			*/
			void CPlayerWalkAndRun::CalcAcceleration()
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
				m_addMoveVec += moveForward * m_inputMoveF * m_acceleration;
				//�������ւ̈ړ����x�����Z�B
				m_addMoveVec += moveRight * m_inputMoveR * m_acceleration;

				return;
			}

			/**
			 * @brief ���C�̌v�Z
			*/
			void CPlayerWalkAndRun::CalcFriction()
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
				const float moveVecLen = m_addMoveVec.Length();

				// ���C���v�Z����
				if (moveVecLen <= kMinSpeed)
				{
					// �ړ����x���Œᑬ�x�ȉ��Ȃ�
					// �ړ��[���ɂ���
					m_addMoveVec = Vector3::Zero;
				}
				else if (m_absInputMoveF <= kInputAxisMin && m_absInputMoveR <= kInputAxisMin)
				{
					// ���͂��Ȃ�������
					// ���C�Č�������
					m_addMoveVec.Scale(friction);
				}
				else if (moveVecLen < m_oldVelocity)
				{
					// �O�̃t���[����葬�x�������Ă�����
					// ���C�Č�������
					m_addMoveVec.Scale(friction);
				}

				return;
			}

			/**
			 * @brief ���x�����̌v�Z
			*/
			void CPlayerWalkAndRun::CalcLimitSpeed()
			{
				// �����͂����邩�H
				if (m_absInputMoveF <= kInputAxisMin && m_absInputMoveR <= kInputAxisMin)
				{
					// �Ȃ��ꍇ�͑��x�����������Ȃ�
					// �������^�[��
					return;
				}


				// �ړ����x���ō����x���I�[�o�[���Ă��邩�H
				if (m_addMoveVec.Length() > m_maxSpeed)
				{
					// �I�[�o�[���Ă�����ō����x���ێ�
					m_addMoveVec.Normalize();
					m_addMoveVec.Scale(m_maxSpeed);
				}

				return;
			}

			/**
			 * @brief ���ۂɈړ�������
			*/
			void CPlayerWalkAndRun::Move()
			{
				// �ړ��x�N�g����X,Z������������
				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecZ();

				// �ړ��x�N�g���ɁA���Z�ړ��x�N�g�������Z����
				m_playerMovementRef->AddMoveVec(m_addMoveVec);

				// �d�͂�������
				m_playerMovementRef->ApplyGravity();

				// �W�����v
				// �W�����v�{�^����������Ă���A���A�n�ʂɂ��Ă���
				if (m_playerRef->GetInputData().actionJump/* && m_charaCon.IsOnGround()*/)
				{
					m_playerMovementRef->AddMoveVec({ 0.0f, kJumpForce,0.0f });
				}

				// �L�����N�^�[�R���g���[���[���g���Ĉړ�������
				m_playerMovementRef->MoveWithCharacterController();

				// �n�ʂɂ��Ă�����A�d�̓��Z�b�g
				if (m_playerMovementRef->IsAir() == false)
				{
					m_playerMovementRef->ResetMoveVecY();
				}


				return;
			}

		}
	}
}
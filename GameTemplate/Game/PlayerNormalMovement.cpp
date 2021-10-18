#include "stdafx.h"
#include "PlayerNormalMovement.h"
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
			// �v���C���[�̒ʏ�̓����N���X�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsPlayerConstData::nsPlayerNormalMovementConstData;
			// �����͂̍ŏ��l���g�p�\�ɂ���
			using nsPlayerConstData::nsPlayerInputConstData::kInputAxisMin;
			// �W�����v�͂��g�p�\�ɂ���
			using nsPlayerConstData::nsPlayerMoveConstData::kJumpForce;

			/**
			 * @brief ������
			 * @param[in] player �v���C���[
			 * @param[in,out] playerMovement �v���C���[�ړ��N���X�̎Q��
			*/
			void CPlayerNormalMovement::Init(
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
			 * @brief �ʏ�̓����̏��������s
			*/
			void CPlayerNormalMovement::Execute()
			{
				if (!m_playerMovementRef->IsAir())
				{

				}
				// �X�e�[�g�����A�N�V������̋󒆂̏������H
				if (m_playerRef->GetState() == nsPlayerConstData::enAirAfterStringAction)
				{
					// ���A�N�V������̋󒆂̏���
					AirAfterStringAction();
				}
				else
				{
					// ����������̈ړ��̏���
					WalkOrRunMove();
				}
			}

			/**
			 * @brief ����������̈ړ��̏���
			*/
			void CPlayerNormalMovement::WalkOrRunMove()
			{
				// �����͒l���X�V����
				UpdateInputAxisParam();

				// ���������肩�����߂�
				WalkOrRun();

				// �ړ��������X�V����
				UpdateMoveDir();

				// �������v�Z
				CalcAcceleration();

				// ���C���v�Z
				CalcFriction();

				// ���x�����̌v�Z
				CalcLimitSpeed();

				// ���ۂɈړ�������
				Move();

				// �O�̃t���[���̑��x���X�V
				m_oldVelocity = m_velocity;

				return;

			}

			/**
			 * @brief �����g�����A�N�V�����̌�̋󒆂̏���
			*/
			void CPlayerNormalMovement::AirAfterStringAction()
			{
				// �����͒l���X�V����
				UpdateInputAxisParam();

				// �ړ��������X�V����
				UpdateMoveDir();


				m_velocity = m_playerMovementRef->GetMoveVec().Length();

				// ���ۂɈړ�������
				Move();

				return;
			}

			/**
			 * @brief �����͒l���X�V
			*/
			void CPlayerNormalMovement::UpdateInputAxisParam()
			{
				// �O�A��ړ��̎�����
				m_inputMoveF = m_playerRef->GetInputData().axisMoveForward;
				// �E�A���ړ��̎�����
				m_inputMoveR = m_playerRef->GetInputData().axisMoveRight;

				// �O�A��ړ��̎����͂̐�Βl
				if (fabsf(m_inputMoveF) <= kInputAxisMin && fabsf(m_inputMoveR) <= kInputAxisMin)
				{
					m_isInputMove = false;
				}
				else
				{
					m_isInputMove = true;
				}


				return;
			}

			/**
			 * @brief �����g�����A�N�V�����̌�̋󒆏�Ԃ����ׂ�
			*/
			void CPlayerNormalMovement::CheckIsAirAfterStringAction()
			{


				return;
			}


			/**
			 * @brief ���������肩�����߂�
			*/
			void CPlayerNormalMovement::WalkOrRun()
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
			 * @brief �ړ��������X�V����
			*/
			void CPlayerNormalMovement::UpdateMoveDir()
			{
				// �����͂��Ȃ����H
				if (m_isInputMove != true)
				{
					// �Ȃ��B�������^�[���B
					return;
				}

				// �ړ��̑O����
				Vector3 moveForward = m_playerRef->GetCamera().GetCameraForward();
				// �ړ��̉E����
				Vector3 moveRight = m_playerRef->GetCamera().GetCameraRight();
				// Y������������XZ���ʂł̑O�����ƉE�����ɕϊ�����
				moveForward.y = 0.0f;
				moveForward.Normalize();
				moveRight.y = 0.0f;
				moveRight.Normalize();

				//���A��O�����ւ̈ړ����������Z�B
				Vector3 newMoveDir = moveForward * m_inputMoveF;	// �V�����ړ�����
				// �E�A�������ւ̈ړ����������Z
				newMoveDir += moveRight * m_inputMoveR;

				// �ړ������ƐV�����ړ������̂Ȃ��p
				float radAngle = acosf(Dot(m_moveDir, newMoveDir));

				// �p�x�Ƒ��x���������l�ȏォ�H
				if (radAngle >= Math::DegToRad(kBreakThresholdAngle) &&
					m_velocity >= kBreakThresholdVelocity)
				{
					// �u���[�L��������
					m_velocity = 0.0f;
				}
				else
				{
					// �ړ�����������
					m_moveDir = newMoveDir;
				}

				return;
			}


			/**
			 * @brief �������v�Z
			*/
			void CPlayerNormalMovement::CalcAcceleration()
			{

				// �����͂����邩�H
				if (m_isInputMove)
				{
					// ����
					// ��������
					m_velocity += m_acceleration;
				}

				return;
			}

			/**
			 * @brief ���C�̌v�Z
			*/
			void CPlayerNormalMovement::CalcFriction()
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

				// ���C���v�Z����
				if (m_velocity <= kMinSpeed)
				{
					// �ړ����x���Œᑬ�x�ȉ��Ȃ�
					// �ړ����x�[���ɂ���
					m_velocity = 0.0f;
				}
				else if (m_isInputMove != true)
				{
					// ���͂��Ȃ�������
					// ���C�Č�������
					m_velocity *= friction;
				}
				else if (m_velocity < m_oldVelocity)
				{
					// �O�̃t���[����葬�x�������Ă�����
					// ���C�Č�������
					m_velocity *= friction;
				}

				return;
			}

			/**
			 * @brief ���x�����̌v�Z
			*/
			void CPlayerNormalMovement::CalcLimitSpeed()
			{
				// �ړ����x���ō����x���I�[�o�[���Ă��邩�H
				if (m_velocity > m_maxSpeed)
				{
					// �I�[�o�[���Ă�����ō����x���ێ�
					m_velocity = m_maxSpeed;
				}

				return;
			}

			/**
			 * @brief ���ۂɈړ�������
			*/
			void CPlayerNormalMovement::Move()
			{
				// �ړ��x�N�g����X,Z������������
				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecZ();

				// �ړ��x�N�g���ɁA���Z�ړ��x�N�g�������Z����
				m_playerMovementRef->AddMoveVec(m_moveDir * m_velocity);

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


				return;
			}

		}
	}
}
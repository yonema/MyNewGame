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
				nsDebug::DrawTextPanel(L"[WalkAndRun:Execute]");

				// ����������̈ړ��̏���
				WalkOrRunMove();
				
				return;
			}

			/**
			 * @brief ����������̈ړ��̏���
			*/
			void CPlayerWalkAndRun::WalkOrRunMove()
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
			void CPlayerWalkAndRun::AirAfterStringAction()
			{

				// �����͒l���X�V����
				UpdateInputAxisParam();

				// �ړ��������X�V����
				UpdateMoveDir();

				// �������v�Z
				CalcAcceleration();

				// ���x�����̌v�Z
				CalcLimitSpeed();

				
				m_velocity += m_playerMovementRef->GetPlayerSwingAction().GetSwingSpeed();

				// ���ۂɈړ�������
				Move();

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

				return;
			}

			/**
			 * @brief �����g�����A�N�V�����̌�̋󒆏�Ԃ����ׂ�
			*/
			void CPlayerWalkAndRun::CheckIsAirAfterStringAction()
			{


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
			 * @brief �ړ��������X�V����
			*/
			void CPlayerWalkAndRun::UpdateMoveDir()
			{
				// �����͂��Ȃ����H
				if (m_playerRef->GetInputData().inputMoveAxis != true)
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
			void CPlayerWalkAndRun::CalcAcceleration()
			{

				// �����͂����邩�H
				if (m_playerRef->GetInputData().inputMoveAxis)
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

				// ���C���v�Z����
				if (m_velocity <= kMinSpeed)
				{
					// �ړ����x���Œᑬ�x�ȉ��Ȃ�
					// �ړ����x�[���ɂ���
					m_velocity = 0.0f;
				}
				else if (m_playerRef->GetInputData().inputMoveAxis != true)
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
			void CPlayerWalkAndRun::CalcLimitSpeed()
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
			void CPlayerWalkAndRun::Move()
			{
				// �ړ��x�N�g����X,Z������������
				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecZ();

				// �ړ��x�N�g���ɁA���Z�ړ��x�N�g�������Z����
				m_playerMovementRef->AddMoveVec(m_moveDir * m_velocity);


				// �W�����v
				// �W�����v�{�^����������Ă���A���A�n�ʂɂ��Ă���
				if (m_playerRef->GetInputData().actionJump/* && m_charaCon.IsOnGround()*/)
				{
					m_playerMovementRef->AddMoveVec({ 0.0f, kJumpForce,0.0f });
				}

				return;
			}

		}
	}
}
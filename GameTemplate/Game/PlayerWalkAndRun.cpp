#include "stdafx.h"
#include "PlayerWalkAndRun.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "PlayerConstData.h"
#include "SoundCue.h"

namespace nsNinjaAttract
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
			using namespace nsPlayerConstData::nsWalkAndRunConstData;

			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerWalkAndRun::CPlayerWalkAndRun()
			{
				// �T�E���h�̏�����
				InitSound();

				return;
			}
			/**
			 * @brief �f�X�g���N�^
			*/
			CPlayerWalkAndRun::~CPlayerWalkAndRun()
			{
				DeleteGO(m_walkSC);
				DeleteGO(m_runSC);
				DeleteGO(m_jumpSC);

				return;
			}

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
			 * @brief ���݂̓����ɁA�ړ��p�����[�^�����킹��
			*/
			void CPlayerWalkAndRun::MuchMoveParam()
			{
				// ���݂̈ړ��x�N�g�����擾
				m_moveDir = m_playerMovementRef->GetMoveVec();
				// Y����������
				m_moveDir.y = 0.0f;

				// ���x��XZ���ʂł̈ړ��x�N�g������v�Z
				m_velocity = m_moveDir.Length();
				// �O�̃t���[���̑��x��ݒ�
				m_oldVelocity = m_velocity;

				// �ړ��x�N�g�����ړ������x�N�g���ɂ���
				m_moveDir.Normalize();	// ���K������

				return;
			}

			/**
			 * @brief ���̃N���X�̃T�E���h���~����
			*/
			void CPlayerWalkAndRun::StopSound()
			{
				// �Đ����̃T�E���h�����ׂĒ�~����

				if (m_walkSC->IsPlaying())
				{
					m_walkSC->Stop();
				}
				if (m_runSC->IsPlaying())
				{
					m_runSC->Stop();
				}

				// �W�����v�͎~�߂Ȃ�
				//if (m_jumpSC->IsPlaying())
				//{
				//	m_jumpSC->Stop();
				//}

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

				// �T�E���h�̍X�V
				UpdateSound();
				
				return;
			}

			/**
			 * @brief �T�E���h�̏�����
			*/
			void CPlayerWalkAndRun::InitSound()
			{
				m_walkSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_walkSC->Init(kWalkSoundFilePath, nsSound::CSoundCue::enSE);
				m_walkSC->SetVolume(kWalkSoundVolume);

				m_runSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_runSC->Init(kRunSoundFilePath, nsSound::CSoundCue::enSE);
				m_runSC->SetVolume(kRunSoundVolume);

				m_jumpSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_jumpSC->Init(kJumpSoundFilePath, nsSound::CSoundCue::enSE);
				m_jumpSC->SetVolume(kJumpSoundVolume);

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

				// �W�����v�̍X�V
				UpdateJump();

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
					// �󒆂̖��C
					friction = kAirFriction;
				}
				else
				{
					// �n�ʏ�̖��C
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

				return;
			}


			/**
			 * @brief �W�����v�̍X�V
			*/
			void CPlayerWalkAndRun::UpdateJump()
			{
				// �W�����v
				// �W�����v�{�^����������Ă���A���A
				// �W�����v�̏㏸���ł͂Ȃ��A���A
				// �n�ʂɂ��Ă���
				if (m_playerRef->GetInputData().actionJump && !m_isJumpUp/* && m_charaCon.IsOnGround()*/)
				{
					m_isJumpUp = true;
					m_jumpUpTimer = 0.0f;
				}

				if (m_isJumpUp)
				{
					// �W�����v�̏㏸��

					// �W�����v�̏㏸���̃^�C�}�[��i�߂�
					m_jumpUpTimer += m_playerRef->GetDeltaTime();

					if (m_jumpUpTimer <= kJumpChargeTime)
					{
						// �^�C�}�[�����߂̎��Ԉȉ��Ȃ�A�������Ȃ��B�������^�[���B
						return;
					}

					if (m_jumpSC->IsPlaying() != true)
					{
						// �W�����v�̏㏸���n�߂�ŏ������s������
						// �W�����v��SE���Đ�
						m_jumpSC->Play(false);
						// �W�����v�͂�0����X�^�[�g���ƁA�x�����邵�A�����̎d�������т�����A
						// ������x�A�ŏ��ɗ͂������Ă����B
						m_playerMovementRef->AddMoveVec({ 0.0f, kInitialJumpForce * m_playerRef->GetDeltaTime(),0.0f });
					}

					// �W�����v�͂�������
					m_playerMovementRef->AddMoveVec({ 0.0f, kJumpForce * m_playerRef->GetDeltaTime(),0.0f });
					
					if (m_jumpUpTimer >= kJumpUpTime)
					{
						// �^�C�}�[���㏸���̎��Ԃ𒴂�����A�W�����v�̏㏸���I������B
						m_isJumpUp = false;
					}
				}

				return;
			}


			/**
			 * @brief �T�E���h�̍X�V
			*/
			void CPlayerWalkAndRun::UpdateSound()
			{
				// �W�����v
				// �W�����v�{�^����������Ă���A���A�n�ʂɂ��Ă���
				if (m_playerRef->GetInputData().actionJump/* && m_charaCon.IsOnGround()*/)
				{
					//m_jumpSC->Play(false);
				}

				if (m_velocity <= 0.001f || m_playerMovementRef->IsAir())
				{
					// �ړ����Ă��Ȃ��A�܂��͋󒆁B�T�E���h���~���āA�������^�[���B
					StopSound();
					return;
				}

				if (m_playerRef->GetInputData().actionDush != true)
				{
					// ����
					m_walkSC->Play(true);
					m_runSC->Stop();
				}
				else
				{
					// ����
					m_runSC->Play(true);
					m_walkSC->Stop();
				}

				return;
			}

		}
	}
}
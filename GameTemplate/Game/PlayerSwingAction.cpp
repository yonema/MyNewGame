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
			// �v���C���[�̃X�C���O�A�N�V�����N���X�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsPlayerConstData::nsPlayerSwingActionConstData;


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
				nsDebug::DrawTextPanel(L"[SwingAction:Execute]");

				// �X�C���O�^�[�Q�b�g��T�����H
				if (m_swingActionState == enFindSwingTarget)
				{
					// �X�C���O�^�[�Q�b�g��T��
					FindSwingTarget();
				}

				// �X�C���O�A�N�V��������߂����H
				if (m_playerRef->GetInputData().actionSwing != true)
				{
					if (m_swingActionState == EnSwingActionState::enIsSwinging)
					{
						m_accelerationAfterSwing = kInitialVelocityOfAterSwingAcceleration;
						
						m_velocityAfterSwing = m_playerMovementRef->GetVelocity();
						m_inputMoveDirXZ = Vector3::Zero;
					}
					// �X�e�[�g���X�C���O��̋󒆏�ԂɑJ�ڂ���
					m_swingActionState = EnSwingActionState::enIsAirAfterSwing;
					// ���ɏI����m�点��
					m_playerRef->EndStringStretchToPos();
				}

				// �n�ʂɂ��Ă��܂������H
				if (m_playerMovementRef->IsAir() != true)
				{
					// �X�C���O�A�N�V�������I������
					m_swingActionState = EnSwingActionState::enEnd;
				}

				// �X�C���O���̏������X�e�[�g�Ŋ���U��
				switch (m_swingActionState)
				{
				// ����L�΂��Ă���Œ�
				case enIsStringStretching:
					// ����L�΂��Ă���ŏI�̏���
					StringStretching();
					break;

				// �X�C���O��
				case enIsSwinging:
					// �X�C���O���̏���
					SwingAction();
					break;

				// �X�C���O��̋�
				case enIsAirAfterSwing:
					// �X�C���O��̋󒆂̏���
					AirAfterSwing();
					break;

				// �X�C���O�I��
				case enEnd:
					// �X�C���O�����̏I��
					EndSwing();
					break;
				}

				return;
			}


			/**
			 * @brief �X�C���O�^�[�Q�b�g��T��
			*/
			void CPlayerSwingAction::FindSwingTarget()
			{
				// �X�C���O�^�[�Q�b�g�ւ̋���
				float dist = FLT_MAX;

				// �X�C���O�^�[�Q�b�g��������
				m_swingTargetPos = nullptr;

				// �����̍��W����A�X�C���O�^�[�Q�b�g��T��
				for (int i = 0; i < kFindSwintTargetNum; i++)
				{
					// �X�C���O�^�[�Q�b�g��T�����W�ւ̃x�N�g��
					Vector3 toFindSwingTargetVec = kToFindSwingTargetVecs[i];
					// �v���C���[�̃��f���̌����ɉ�]������
					m_playerRef->GetRotation().Apply(toFindSwingTargetVec);

					// �w�肳�ꂽ���W����L���͈͓��ɂ���X�C���O�^�[�Q�b�g�̍��W�̒��ň�ԋ߂����W�𓾂�
					const Vector3* swingTargetPos = 
						nsStringActionTarget::GetNearestSwintTargetPointWithinScope(
						m_playerRef->GetPosition() + toFindSwingTargetVec, kFindSwingTargetScope
					);

					// �X�C���O�^�[�Q�b�g�̍��W��nullptr���H
					if (swingTargetPos == nullptr)
					{
						// ������Ȃ������B���ցB
						continue;
					}

					// �v���C���[����X�C���O�^�[�Q�b�g�ւ̃x�N�g��
					Vector3 toSwingTargetVec = *swingTargetPos - m_playerRef->GetPosition();
					// �v���C���[����X�C���O�^�[�Q�b�g�ւ̕����x�N�g��
					Vector3 toSwingTargetDir = toSwingTargetVec;
					toSwingTargetDir.Normalize();	// ���K������

					// �O�����x�N�g��
					Vector3 forward = Vector3::Front;
					// �v���C���[�̃��f���̌����ɉ�]������
					m_playerRef->GetRotation().Apply(forward);
					forward.Normalize();	// ���K������

					// �X�C���O�^�[�Q�b�g�ւ̕����x�N�g���ƁA�O�����x�N�g���̓��ς������H
					if (Dot(toSwingTargetDir, forward) <= 0.0f)
					{
						// ���B�X�C���O�^�[�Q�b�g���v���C���[�̌��ɂ��邩��p�X�B���ցB
						continue;
					}

					// �D��x�������X�C���O�^�[�Q�b�g���H
					if (i < kHighPriorityFindSwintTargetNum)
					{
						// �D��x����������A�������瑦���ɏI���B
						// �X�C���O�^�[�Q�b�g�̍��W��ێ�
						m_swingTargetPos = swingTargetPos;
						break;
					}
					else
					{
						// ���ʂ̗D��x�́A�v���C���[���狗�����߂��X�C���O�^�[�Q�b�g���D�悳���B

						// ����̋����x�N�g��
						const Vector3 newDistVec = *swingTargetPos - m_playerRef->GetPosition();
						// ����̋���
						const float newDist = newDistVec.Length();

						// ����̋��������܂ł̍ŒZ�������傫�����H
						if (newDist > dist)
						{
							// �傫���B�D��x��B���ցB
							continue;
						}
						// �߂��X�C���O�^�[�Q�b�g�̂��߁A�D��x���B
						// �ŒZ������ێ�
						dist = newDist;
						// �X�C���O�^�[�Q�b�g�̍��W��ێ�
						m_swingTargetPos = swingTargetPos;
					}

				}

				// �X�C���O�^�[�Q�b�g�������������H
				if (m_swingTargetPos != nullptr)
				{
					// ��������
					// �����X�C���O�^�[�Q�b�g�Ɍ������ĐL�΂��n�߂�
					m_playerRef->StartStringStretchToPos(*m_swingTargetPos);
					// �X�e�[�g������L�΂����֑J�ڂ���
					m_swingActionState = EnSwingActionState::enIsStringStretching;
				}
				else
				{
					// ������Ȃ�����
					// �X�e�[�g���I���֑J�ڂ���
					m_swingActionState = EnSwingActionState::enIsAirAfterSwing;
				}

				return;
			}

			/**
			 * @brief ����L�΂��Ă���Œ��̏���
			*/
			void CPlayerSwingAction::StringStretching()
			{
				nsDebug::DrawTextPanel(L"StringStretching");
				// �L�т����ĂȂ����H
				if (m_playerRef->IsStringStretched() != true)
				{
					// �܂��L�т����ĂȂ��B�������^�[��
					return;
				}

				//////// �����L�т������B�X�C���O�����鏀�������� ////////

				//////// �X�e�[�g�J�� ////////
				// �L�΂��؂�������A�X�e�[�g���X�C���O���ɑJ��
				m_swingActionState = EnSwingActionState::enIsSwinging;

				//////// �p�����[�^�̃��Z�b�g ////////
				// �������n�߂�X�C���O�X�s�[�h������������
				m_startDecelerateSwingSpeed = kStartDecelerateSwingSpeedInitialValue;
				// ���͂ɂ���Đ�����XZ���ʂł̈ړ�����������������
				m_inputMoveDirXZ = Vector3::Zero;

				if (m_playerRef->GetPosition().y < m_swingTargetPos->y)
				{
					m_swingStartYPos = m_playerRef->GetPosition().y;
				}
				else
				{
					m_swingStartYPos = m_swingTargetPos->y;
				}
				m_swingMinYPos = max(100.0f, m_swingStartYPos - 500.0f);
				m_swingStringLen = m_playerRef->GetStringLength();

				//////// �X�C���O�X�s�[�h�̏������x�̌v�Z ////////
				// �X�C���O�X�s�[�h���������x���x�����H
				//if (m_swingSpeed <= kInitialSwingSpeed)
				{
					// �x���B�������x�̃X�C���O�X�s�[�h��ݒ肷��B�������x�������ƃX�C���O�X�s�[�h�������Ȃ�B
					m_swingSpeed = kInitialSwingSpeed +
						fabsf(m_playerMovementRef->GetMoveVec().y) * kFallImpactRateForInitialSwingSpeed;

					// �������^�[��
					return;
				}

				// ����ω������Ȃ��A���̉��ʂ�Ȃ��B
				//////// �X�C���O�X�s�[�h�̉��������X�s�[�h���v�Z ////////

				// �O��̃X�C���O�X�s�[�h����̉���
				float acceleration = m_playerMovementRef->GetVelocity() - m_swingSpeed;
				// �O�񂩂�A���������邩�H
				if (acceleration >= 0.0f)
				{
					// ����������B�����̈ꕔ�������p���ŉ��Z����
					m_swingSpeed += acceleration * kTakeOverSwingAccelerationRate;
				}
				else
				{
					// �������Ă���
					// ���݂̑��x���X�C���O�X�s�[�h�ɂ���
					m_swingSpeed = m_playerMovementRef->GetVelocity();
				}

				// �X�C���O�X�s�[�h���ő呬�x�𒴂��Ă��邩�H
				if (m_swingSpeed > kMaxSwingSpeed)
				{
					// �����Ă���B�ő呬�x�ɐݒ肷��B
					m_swingSpeed = kMaxSwingSpeed;
				}

				return;
			}

			/**
			 * @brief �X�C���O�A�N�V�����̏���
			*/
			void CPlayerSwingAction::SwingAction()
			{
				nsDebug::DrawTextPanel(L"SwingAction");
				
				// �v���C���[����X�C���O�^�[�Q�b�g�܂ł�XZ���ʂł̃x�N�g��
				Vector3 playerToTargetVecXZ = *m_swingTargetPos - m_playerRef->GetPosition();
				// Y����������
				playerToTargetVecXZ.y = 0.0f;

				// XZ���ʂł̑O����
				Vector3 forwardDirXZ = Vector3::Front;
				// ���f���̌����ɉ�
				m_playerRef->GetRotation().Apply(forwardDirXZ);
				// ���K������
				forwardDirXZ.Normalize();				

				// XZ���ʂ̑O�����ɁA�v���C���[����X�C���O�^�[�Q�b�g�ւ�XZ���ʂ̃x�N�g�����ˉe����
				float projectToTargetVecXZToForwardDirXZ = Dot(forwardDirXZ, playerToTargetVecXZ);

				// XZ���ʂł́A�O�����݂̂́A�X�C���O�^�[�Q�b�g�ւ̃x�N�g��
				Vector3 toTargetFowardVecXZ = forwardDirXZ;
				// �ˉe���ċ��߂������܂ŐL�΂�
				toTargetFowardVecXZ.Scale(projectToTargetVecXZToForwardDirXZ);

				// XZ���ʂł́A�O�����݂̂́A�X�C���O�^�[�Q�b�g�̍��W
				const Vector3 toTargetForwardPosXZ = m_playerRef->GetPosition() + toTargetFowardVecXZ;

				// �O�����Ə�����݂̂́A�X�C���O�^�[�Q�b�g�̍��W
				Vector3 toTargetForwardUpPos = toTargetForwardPosXZ;
				// Y���W�����ۂ̃X�C���O�^�[�Q�b�g�̍��W�Ɠ����ɂ���
				toTargetForwardUpPos.y = m_swingTargetPos->y;

				// �O�����Ə�����݂̂̃X�C���O�^�[�Q�b�g�̍��W����v���C���[�ւ̃x�N�g��
				const Vector3 toTargetForwardUpToPlayerVec = m_playerRef->GetPosition() - toTargetForwardUpPos;
				
				// �O�����Ə�����݂̂̃X�C���O�^�[�Q�b�g�̍��W����v���C���[�ւ̕���
				Vector3 targetUptoPlayerDir = toTargetForwardUpToPlayerVec;
				// ���K������
				targetUptoPlayerDir.Normalize();

				// XZ���ʂł̑O�����ƁA�X�C���O�^�[�Q�b�g����v���C���[�ւ̕����̓���
				float dotFowardDirXZAndToPlayerDir = 
					Dot(forwardDirXZ, targetUptoPlayerDir);

				// ���Z�ړ������x�N�g��
				Vector3 addMoveDir = targetUptoPlayerDir;

				// ���Z�ړ������x�N�g������]�������]�N�H�[�^�j�I��
				Quaternion qRotForAddMoveDir;

				// ���Z�ړ������x�N�g������]�������]���B
				Vector3 rotAxisForAddMoveDir = Cross(targetUptoPlayerDir, forwardDirXZ);

				// �v���C���[���X�C���O�^�[�Q�b�g����ɂ��邩�H
				if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
				{
					// ��ɂ���Ƃ��A��]���̋��ߕ���ς���
					rotAxisForAddMoveDir = Cross(Vector3::Down, forwardDirXZ);
				}

				// ���߂���]���𐳋K������
				rotAxisForAddMoveDir.Normalize();

				// ���ς������H�A�܂�A�X�C���O�^�[�Q�b�g����O���ɂ��邩�H
				if (dotFowardDirXZAndToPlayerDir < 0.0f)
				{
					// ��O��

					float rate = dotFowardDirXZAndToPlayerDir + 1.0f;
					rate = pow(rate, 5.0f);
/*					if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
					{
						addMoveDir = forwardDirXZ;
						addMoveDir.y -= 10.0f;

					}
					else */if (m_playerRef->GetPosition().y > 100.0f)
					{
						//// ��]�N�H�[�^�j�I����90�x��]������
						//qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, 3.14f * 0.5f);
						//// ���Z�ړ������x�N�g������]������
						//qRotForAddMoveDir.Apply(addMoveDir);

						addMoveDir = forwardDirXZ;
						rate = 1.0f - rate;
						float radAngle = 3.14f * 0.1f * -rate;
						// ��]�N�H�[�^�j�I����90�x��]������
						qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle);
						// ���Z�ړ������x�N�g������]������
						qRotForAddMoveDir.Apply(addMoveDir);
					}
					else
					{
						addMoveDir = forwardDirXZ;
					}


				}
				else
				{
					// ����

					float radAngle = 3.14f * 0.5f;
					Vector3 to = m_playerRef->GetPosition() - toTargetForwardPosXZ;
					//float rate = to.Length() / toTargetForwardUpToPlayerVec.Length();
					float rate = toTargetFowardVecXZ.Length() / m_swingStringLen;
					rate = min(rate, 1.0f);
					rate = pow(rate, 4.0f);
					//rate = min(rate, 3.14f * 0.5f);

					if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
					{
						if (m_startDecelerateSwingSpeed <= -50.0f)
						{
							m_startDecelerateSwingSpeed = m_swingSpeed;
						}
						float dot = Dot(forwardDirXZ, targetUptoPlayerDir);
						float rad = acosf(dot);
						float angleRate = rad / (3.14f * 0.1f);
						m_swingSpeed = Math::Lerp<float>(min(1.0f,angleRate), m_startDecelerateSwingSpeed, 0.0f);
						addMoveDir = forwardDirXZ;
						radAngle = 3.14f * 0.5f * min(1.0f, angleRate);
						radAngle = Math::Lerp<float>(min(1.0f, angleRate), 3.14f * 0.4f, 3.14f * 0.5f);


					}
					else
					{

						addMoveDir = forwardDirXZ;
						radAngle = 3.14f * 0.4f * rate;

					}

					// ��]�N�H�[�^�j�I����90�x��]������
					qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle);
					// ���Z�ړ������x�N�g������]������
					qRotForAddMoveDir.Apply(addMoveDir);

				}

				nsDebug::DrawTextPanel(std::to_wstring(m_swingStringLen), L"stringLen");

				Vector3 rightDirXZ = m_playerRef->GetCamera().GetCameraRight();
				rightDirXZ.y = 0.0f;
				rightDirXZ.Normalize();
				float rightPower = m_playerRef->GetInputData().axisMoveRight / 5.0f;
				rightDirXZ.Scale(rightPower);
				m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, rightDirXZ);

				addMoveDir += m_inputMoveDirXZ;

				addMoveDir.Normalize();

				// ���Z�ړ��x�N�g��
				Vector3 addMoveVec = addMoveDir;
				addMoveVec.Scale(m_swingSpeed);

				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecY();
				m_playerMovementRef->ResetMoveVecZ();

				// �ړ��x�N�g���ɁA���Z�ړ��x�N�g�������Z����
				m_playerMovementRef->AddMoveVec(addMoveVec);

				Vector3 dist = m_playerRef->GetPosition() - *m_swingTargetPos;
				nsDebug::DrawTextPanel(std::to_wstring(dist.Length()), L"toSwintTargetLen:");

				return;
			}


			/**
			 * @brief �X�C���O��̋󒆂̏���
			*/
			void CPlayerSwingAction::AirAfterSwing()
			{
				nsDebug::DrawTextPanel(L"AirAfterSwing");
				if (m_playerRef->GetInputData().triggerSwing == true)
				{
					m_swingActionState = enFindSwingTarget;
				}

				Vector3 addVec = Vector3::Zero;
				Vector3 moveVec = m_playerMovementRef->GetMoveVec();
				Vector3 moveVecXZ = moveVec;
				moveVecXZ.y = 0.0f;
				Vector3 addMoveDir = moveVecXZ;
				addMoveDir.Normalize();
				m_accelerationAfterSwing *= 0.99f;
				if (m_accelerationAfterSwing < kMinVelocityOfAfterSwingAcceleration)
				{
					m_accelerationAfterSwing = kMinVelocityOfAfterSwingAcceleration;
				}
				float velocity = m_velocityAfterSwing + m_accelerationAfterSwing;


				if (velocity < nsPlayerConstData::nsPlayerWalkAndRunConstData::kWalkMaxSpeed)
				{
					velocity = nsPlayerConstData::nsPlayerWalkAndRunConstData::kWalkMaxSpeed;
				}

				if (m_playerRef->GetInputData().inputMoveAxis)
				{

					Vector3 rightDirXZ = m_playerRef->GetCamera().GetCameraRight();
					rightDirXZ.y = 0.0f;
					rightDirXZ.Normalize();
					float rightPower = m_playerRef->GetInputData().axisMoveRight / 7.0f;
					rightDirXZ.Scale(rightPower);
					m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, rightDirXZ);
					addMoveDir += m_inputMoveDirXZ;

					addMoveDir.Normalize();
					addVec = addMoveDir;
					addVec.Scale(velocity);
				}
				else
				{
					Vector3 moveDirXZ = moveVecXZ;
					moveDirXZ.Normalize();
					moveDirXZ.Scale(velocity);
					addVec = moveDirXZ;
				}

				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecZ();

				m_playerMovementRef->AddMoveVec(addVec);
				return;
			}

			/**
			 * @brief �X�C���O�����̏I��
			*/
			void CPlayerSwingAction::EndSwing()
			{
				// �X�e�[�g���X�C���O�^�[�Q�b�g��T����Ԃɖ߂��Ă���
				m_swingActionState = enFindSwingTarget;
				// ���ɏI����m�点��
				m_playerRef->EndStringStretchToPos();
				// �v���C���[�̃X�e�[�g������Ƒ���ɂ���
				m_playerRef->ChangeWalkAndRunState();
				m_swingSpeed = 0.0f;

				return;
			}

		}
	}
}
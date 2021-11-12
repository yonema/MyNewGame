#include "stdafx.h"
#include "PlayerSwingAction.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "StringActionTargetManager.h"
#include "PlayerCamera.h"

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
			using namespace nsPlayerConstData::nsSwingActionConstData;


			/**
			 * @brief ������
			 * @param[in,out] player �v���C���[
			 * @param[in,out] playerMovement �v���C���[�ړ��N���X�̎Q��
			 * @param[in,out] playerCamera �v���C���[�J�����N���X�̎Q��
			 * @param[in.out] playerModelAnimation �v���C���[���f���A�j���[�V�����̎Q��
			*/
			void CPlayerSwingAction::Init(
				CPlayer* player,
				CPlayerMovement* playerMovement,
				CPlayerCamera* playerCamera,
				CPlayerModelAnimation* playerModelAnimation
			)
			{
				// �e�Q�Ƃ��Z�b�g
				m_playerRef = player;
				m_playerMovementRef = playerMovement;
				m_playerCameraRef = playerCamera;
				m_playerModelAnimationRef = playerModelAnimation;
				return;
			}

			/**
			 * @brief �X�C���O�A�N�V���������s
			*/
			void CPlayerSwingAction::Execute()
			{
				nsDebug::DrawTextPanel(L"[SwingAction:Execute]");

				// �X�C���O�A�N�V�����̑O�ɍs������
				PreSwingAction();

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
			 * @brief �X�C���O�A�N�V�����̑O�ɍs������
			*/
			void CPlayerSwingAction::PreSwingAction()
			{
				// �X�C���O�^�[�Q�b�g��T�����H
				if (m_swingActionState == enFindSwingTarget)
				{
					// �X�C���O�^�[�Q�b�g��T��
					FindSwingTarget();

					// �������^�[��
					return;
				}

				// �n�ʂɂ��Ă��܂������H
				if (m_playerMovementRef->IsAir() != true)
				{
					// �X�C���O�A�N�V�������I������
					m_swingActionState = EnSwingActionState::enEnd;

					// �������^�[��
					return;
				}


				// �X�C���O�A�N�V��������߂����H����
				// �X�C���O��̋󒆏�Ԃł͂Ȃ����H
				if (m_playerRef->GetInputData().actionSwing != true &&
					m_swingActionState != EnSwingActionState::enIsAirAfterSwing)
				{
						m_playerModelAnimationRef->SetSwingAnimState(
						nsPlayerConstData::nsModelAnimationConstData::enSwingAnim_airAfterSwing
					);
					// �X�C���O���ɃX�C���O����߂����H
					if (m_swingActionState == EnSwingActionState::enIsSwinging)
					{
						// �X�C���O��̉����̏��������Z�b�g
						m_accelerationAfterSwing = kInitialVelocityOfAterSwingAcceleration;
						// �X�C���O��̑��x��ێ�
						m_velocityAfterSwing = m_playerMovementRef->GetVelocity();
						// ���͂ɂ���Đ�����XZ���ʂł̈ړ����������Z�b�g
						m_inputMoveDirXZ = Vector3::Zero;
						m_playerModelAnimationRef->SetSwingAnimState(
							nsPlayerConstData::nsModelAnimationConstData::enSwingAnim_swingRoll
						);

					}
					// �X�e�[�g���X�C���O��̋󒆏�ԂɑJ�ڂ���
					m_swingActionState = EnSwingActionState::enIsAirAfterSwing;
					// ���ɏI����m�点��
					m_playerRef->EndStringStretchToPos();

					// �������^�[��
					return;
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

				// �X�C���O�^�[�Q�b�g��T���x�N�g�����񂷉�]
				Quaternion swingRotationQRot = Quaternion::Identity;

				// �����o�����������߂�
				if (m_playerRef->GetInputData().inputMoveAxis)
				{
					// �ړ��̓��͂���������ړ����͂̕����Ɏ����o���B
					Vector3 inputDir = m_playerCameraRef->GetCameraForward() * m_playerRef->GetInputData().axisMoveForward;
					inputDir += m_playerCameraRef->GetCameraRight() * m_playerRef->GetInputData().axisMoveRight;
					inputDir.y = 0.0f;
					inputDir.Normalize();
					swingRotationQRot.SetRotation(Vector3::Front, inputDir);
				}
				else
				{
					// ���͂��Ȃ�������J�����̑O�����Ɏ����o���B
					Vector3 cameraDir = m_playerRef->GetCamera().GetCameraForward();
					cameraDir.y = 0.0f;
					cameraDir.Normalize();
					swingRotationQRot.SetRotation(Vector3::Front, cameraDir);
				}

				// �����̍��W����A�X�C���O�^�[�Q�b�g��T��
				for (int i = 0; i < kFindSwintTargetNum; i++)
				{
					// �X�C���O�^�[�Q�b�g��T�����W�ւ̃x�N�g��
					Vector3 toFindSwingTargetVec = kToFindSwingTargetVecs[i];
					// �����o�������ɉ�]������
					swingRotationQRot.Apply(toFindSwingTargetVec);

					// ��ԋ߂��X�C���O�^�[�Q�b�g�̃|�C���g�̍��W��T�������̃f�[�^
					nsStringActionTarget::SFindNearestSwingTargetPointData findData;
					// �X�C���O�^�[�Q�b�g��T���f�[�^��ݒ�
					findData.findSwingTargetOrigin = m_playerRef->GetPosition() + toFindSwingTargetVec;
					findData.findSwingTargetScopeRadius = kFindSwingTargetScope;
					// �X�C���O�^�[�Q�b�g�̃|�C���g��T���f�[�^��ݒ�
					findData.findSwingTargetPointOrigin = m_playerRef->GetPosition() + toFindSwingTargetVec;
					findData.findSwingTargetPointScopeRadius = kFindSwingTargetScope;
					// �X�C���O�^�[�Q�b�g�̃|�C���g��T��������ݒ�
					findData.heightLowerLimit = m_playerRef->GetPosition().y + 500.0f;
					findData.forwardLimitDir = Vector3::Front;
					m_playerRef->GetRotation().Apply(findData.forwardLimitDir);
					findData.forwardLimitOrigin = m_playerRef->GetPosition();

					// �w�肳�ꂽ���W����L���͈͓��ɂ���X�C���O�^�[�Q�b�g�̍��W�̒��ň�ԋ߂����W�𓾂�
					const Vector3* swingTargetPos =
						nsStringActionTarget::FindNearestSwingTargetPoint(findData);

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
					m_playerModelAnimationRef->SetSwingAnimState(
						nsPlayerConstData::nsModelAnimationConstData::enSwingAnim_swingStart
					);
				}
				else
				{
					// ������Ȃ�����
					// �X�e�[�g���X�C���O��̋󒆂ɑJ��
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

				m_counter = 0;
				m_afterSwing = true;
				//////// �X�C���O�X�s�[�h�̏������x�̌v�Z ////////
				// �X�C���O�X�s�[�h���������x���x�����H
				if (m_swingSpeed <= kInitialSwingSpeed)
				{
					// �x���B�������x�̃X�C���O�X�s�[�h��ݒ肷��B�������x�������ƃX�C���O�X�s�[�h�������Ȃ�B
					//m_swingSpeed = kInitialSwingSpeed +
					//	fabsf(m_playerMovementRef->GetMoveVec().y) * kFallImpactRateForInitialSwingSpeed;
					m_swingSpeed = 2000.0f;
					m_g = 700.0f;

					// �������^�[��
					return;
				}
				m_g = m_playerMovementRef->GetVelocity();
				// �X�C���O�X�s�[�h���ő呬�x�𒴂��Ă��邩�H
				const float maxG = 1000.0f;
				if (m_g > maxG)
				{
					// �����Ă���B�ő呬�x�ɐݒ肷��B
					m_g = maxG;
				}
				return;
			}

			/**
			 * @brief �X�C���O�A�N�V�����̏���
			*/
			void CPlayerSwingAction::SwingAction()
			{
				nsDebug::DrawTextPanel(L"SwingAction");

				const int counterMax = 30;
				if (m_counter < counterMax)
					m_counter++;
				m_playerCameraRef->LerpDampingRate(/*0.5f - 0.5f*/1.0f - 1.0f * m_counter / counterMax);
				//////// 1.�K�v�ȃx�N�g����p�� ////////
				
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
				Vector3 addMoveDir = Vector3::Zero;

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


				//////// 2.�X�C���O�̊p�x���v�Z ////////

				// ���ς������H�A�܂�A�X�C���O�^�[�Q�b�g����O���ɂ��邩�H
				if (dotFowardDirXZAndToPlayerDir < 0.0f)
				{
					// ��O��

					// �v���C���[���Œ�X�C���O���x����ɂ��邩�H
					if (m_playerRef->GetPosition().y > 100.0f)
					{
						// ��ɂ���

						// 0.0f�`1.0f�ɕω����郌�[�g
						float rate = dotFowardDirXZAndToPlayerDir + 1.0f;

						// pattern1
						// �w���֐��I�ȕω��ɂ���
						//rate = pow(rate, 5.0f);
						// 1.0f�`0.0f�ɕω�����悤�ɂ���
						//rate = 1.0f - rate;
						//float radAngle = 3.14f * 0.1f * -rate;

						// pattern2
						// �񎟊֐��I�ȕω��ɂ���
						rate = pow(rate - 0.5f, 3.0f);
						// ��ɓʂɂ���
						rate *= -1.0f;
						// rate���Œ�l�ƍō��l��0.0f�ɂȂ�悤�ȐؕЂɂ���
						rate += pow(0.5f, 3.0f);
						// 90�x�Ƀ��[�g��������
						float radAngle = 3.14f * 0.5f * -rate;

						// �i�ޕ������܂������O�ɂ���
						addMoveDir = forwardDirXZ;
						// ��]�N�H�[�^�j�I����90�x��]������
						qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle/*3.14f * 0.5f*/);
						// ���Z�ړ������x�N�g������]������
						qRotForAddMoveDir.Apply(addMoveDir);
					}
					else
					{
						// �Œፂ�x�ɂ��邽�߁A�܂������i�ށB
						addMoveDir = forwardDirXZ;
					}

				}
				else
				{
					// ����

					float radAngle = 3.14f * 0.5f;
					m_playerModelAnimationRef->SetSwingAnimState(
						nsPlayerConstData::nsModelAnimationConstData::enSwingAnim_swingRaiseLeg
					);

					// �v���C���[�̍������X�C���O�^�[�Q�b�g��荂���Ȃ�����B
					if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
					{
						// ���񃋁[�v�����������n�߂�X�s�[�h��ݒ�
						if (m_startDecelerateSwingSpeed <= -50.0f)
						{
							m_startDecelerateSwingSpeed = m_swingSpeed;
						}
						float rad = acosf(dotFowardDirXZAndToPlayerDir);
						float angleRate = rad / (3.14f * 0.1f);
						addMoveDir = forwardDirXZ;
						// radAngle = 3.14f * 0.5f * min(1.0f, angleRate);
						radAngle = Math::Lerp<float>(min(1.0f, angleRate), 3.14f * 0.4f, 3.14f * 0.5f);


					}

					addMoveDir = targetUptoPlayerDir;
					// ��]�N�H�[�^�j�I����90�x��]������
					qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle);
					// ���Z�ړ������x�N�g������]������
					qRotForAddMoveDir.Apply(addMoveDir);
					
				}

				//////// 3.�X�C���O�X�s�[�h���v�Z ////////

				// �C�ӂ̈ʒu�ł̐U��q�̑��x
				// g = �d�͉����x�Al = �U��q�̒����Av�C = �C�ӂ̏ꏊ�ł̐U��q�̑��x�A
				// cos���C = �C�ӂ̏ꏊ�̊p�x�Acos���� = ��ԏ�̎��̊p�x
				// v�C = ��2gl(cos���C - cos����)

				// cos���C = �C�ӂ̏ꏊ�̊p�x
				float anyCos = Dot(targetUptoPlayerDir, Vector3::Down);
				// cos���� = ��ԏ�̎��̊p�x�i��ԏ�̎��̊p�x��90�x�j
				float highestCos = 0.0f;
				// g = �d�͉����x
				m_g += 980.0f * nsTimer::CGameTime().GetFrameDeltaTime() * 0.25f;
				const float maxG = 1200.0f;
				if (m_g > maxG)
				{
					m_g = maxG;
				}
				// l = �U��q�̒���
				const float l = toTargetForwardUpToPlayerVec.Length();
				// v�C = ��2gl(cos���C - cos����)
				m_swingSpeed = 2.0f * m_g * l * (anyCos - highestCos);
				m_swingSpeed = std::sqrtf(m_swingSpeed);


				//////// 4.�X�C���O���̍��E�ւ̕����]�� ////////
				Vector3 rightDirXZ = m_playerCameraRef->GetCameraRight();
				rightDirXZ.y = 0.0f;
				rightDirXZ.Normalize();
				float rightPower = m_playerRef->GetInputData().axisMoveRight / 7.0f;
				rightDirXZ.Scale(rightPower);
				m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, rightDirXZ);

				addMoveDir += m_inputMoveDirXZ;

				addMoveDir.Normalize();

				// ���Z�ړ��x�N�g��
				Vector3 addMoveVec = addMoveDir;
				addMoveVec.Scale(m_swingSpeed);


				// �ړ��x�N�g���ɁA���Z�ړ��x�N�g�������Z����
				m_playerMovementRef->AddMoveVec(addMoveVec);
				m_playerMovementRef->LimitMoveVec(m_swingSpeed);

				return;
			}


			/**
			 * @brief �X�C���O��̋󒆂̏���
			*/
			void CPlayerSwingAction::AirAfterSwing()
			{
				nsDebug::DrawTextPanel(L"AirAfterSwing");

				// ���̃X�C���O�ւ̑J�ڏ���
				if (m_playerRef->GetInputData().actionSwing == true)
				{
					// ���~���Ȃ�X�C���O�ֈڍs�ł���
					if (m_playerMovementRef->GetMoveVec().y < 0.0f)
					{
						m_swingActionState = enFindSwingTarget;
					}
				}


				m_playerCameraRef->LerpDampingRate(1.0f);

				m_accelerationAfterSwing *= 0.99f;
				if (m_accelerationAfterSwing < kMinVelocityOfAfterSwingAcceleration)
				{
					m_accelerationAfterSwing = kMinVelocityOfAfterSwingAcceleration;
				}
				float velocity = m_velocityAfterSwing + m_accelerationAfterSwing;


				if (velocity < nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed)
				{
					velocity = nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed;
				}

				Vector3 addMoveVec = m_playerMovementRef->GetMoveVec();

				if (m_afterSwing)
				{
					m_afterSwing = false;
				}
				else
				{
					addMoveVec.y = 0.0f;
				}

				addMoveVec.Normalize();
				addMoveVec.Scale(velocity);

				if (m_playerRef->GetInputData().inputMoveAxis)
				{

					//Vector3 rightDirXZ = m_playerCameraRef->GetCameraRight();
					//rightDirXZ.y = 0.0f;
					//rightDirXZ.Normalize();
					//float rightPower = m_playerRef->GetInputData().axisMoveRight / 7.0f;
					//rightDirXZ.Scale(rightPower);
					//m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, rightDirXZ);
					//addMoveDir += m_inputMoveDirXZ;

					//addMoveDir.Normalize();
					//addVec = addMoveDir;
					//addVec.Scale(velocity);
				}


				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecZ();

				m_playerMovementRef->AddMoveVec(addMoveVec);
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
				// �X�C���O�X�s�[�h�����Z�b�g
				m_swingSpeed = 0.0f;

				return;
			}

		}
	}
}
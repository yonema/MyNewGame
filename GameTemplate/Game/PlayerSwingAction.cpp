#include "stdafx.h"
#include "PlayerSwingAction.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "StringActionTargetManager.h"
#include "PlayerCamera.h"
#include "SoundCue.h"

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
			// �v���C���[�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsPlayerConstData;
			// �v���C���[�̃X�C���O�A�N�V�����N���X�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsSwingActionConstData;

			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerSwingAction::CPlayerSwingAction()
			{

				// �T�E���h�̏�����
				InitSound();

				return;
			}

			/**
			 * @brief �f�X�g���N�^
			*/
			CPlayerSwingAction::~CPlayerSwingAction()
			{
				DeleteGO(m_chainPutOutSC);
				DeleteGO(m_chainBendingSC);
				DeleteGO(m_chainReleaseSC);
				DeleteGO(m_swingLeaveSC);
				DeleteGO(m_swingRollLeaveSC);

				return;
			}

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
				nsDebug::DrawTextPanel(m_g,L"m_g");
				nsDebug::DrawTextPanel(m_swingSpeed,L"m_swingSpeed");


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
					ChangeState(enEnd);

					// �������^�[��
					return;
				}


				// �W�����v���͂����邩�H�A����
				// �X�C���O�����H
				if (m_playerRef->GetInputData().actionJump &&
					m_swingActionState == enIsSwinging)
				{
					m_swingRollFlag = true;

					// �X�e�[�g���X�C���O��̋󒆏�ԂɑJ�ڂ���
					ChangeState(enIsAirAfterSwing);

					// �������^�[��
					return;
				}

				// �X�C���O�A�N�V��������߂����H����
				// �X�C���O��̋󒆏�Ԃł͂Ȃ����H
				if (m_playerRef->GetInputData().actionSwing != true &&
					m_swingActionState != EnSwingActionState::enIsAirAfterSwing)
				{
					// �X�e�[�g���X�C���O��̋󒆏�ԂɑJ�ڂ���
					ChangeState(enIsAirAfterSwing);

					// �������^�[��
					return;
				}


				return;
			}

			/**
			 * @brief �T�E���h��������
			*/
			void CPlayerSwingAction::InitSound()
			{
				m_chainPutOutSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_chainPutOutSC->Init(kChainPutOutSoundFilePath, nsSound::CSoundCue::enSE);
				m_chainPutOutSC->SetVolume(kChainPutOutSoundVolume);

				m_chainBendingSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_chainBendingSC->Init(kChainBendingSoundFilePath, nsSound::CSoundCue::enSE);
				m_chainBendingSC->SetVolume(kChainBendingSoundVolume);

				m_chainReleaseSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_chainReleaseSC->Init(kChainReleaseSoundFilePath, nsSound::CSoundCue::enSE);
				m_chainReleaseSC->SetVolume(kChainReleaseSoundVolume);

				m_swingLeaveSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_swingLeaveSC->Init(kSwingLeaveSoundFilePath, nsSound::CSoundCue::enSE);
				m_swingLeaveSC->SetVolume(kSwingLeaveSoundVolume);

				m_swingRollLeaveSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_swingRollLeaveSC->Init(kSwingRollLeaveSoundFilePath, nsSound::CSoundCue::enSE);
				m_swingRollLeaveSC->SetVolume(kSwingRollLeaveSoundVolume);

				return;
			}


			/**
			 * @brief �X�C���O�^�[�Q�b�g��T��
			*/
			void CPlayerSwingAction::FindSwingTarget()
			{
				//////// 1.�����o�����������߂� ////////

				// �����o�������ւ̉�]�N�H�[�^�j�I��
				Quaternion swingRotationQRot = Quaternion::Identity;

				// �ړ����͕������J�����̑O�������玅���o�����������߂�
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
				// ��]���玅���o�����������߂�
				m_swingForwardDir = Vector3::Front;
				swingRotationQRot.Apply(m_swingForwardDir);
				m_swingForwardDir.Normalize();

				//////// 2.�X�C���O�^�[�Q�b�g��T�� ////////

				// �X�C���O�^�[�Q�b�g�ւ̋���
				float dist = FLT_MAX;
				// �X�C���O�^�[�Q�b�g��������
				m_swingTargetPos = nullptr;
				// ��ԋ߂��X�C���O�^�[�Q�b�g�̃|�C���g�̍��W��T�������̃f�[�^
				nsStringActionTarget::SFindNearestSwingTargetPointData findData;
				// ���ʂ̃f�[�^��ݒ肷��
				// �L���͈͂�ݒ�
				findData.findSwingTargetScopeRadius = kFindSwingTargetScope;
				findData.findSwingTargetPointScopeRadius = kFindSwingTargetScope;
				// �X�C���O�^�[�Q�b�g�̃|�C���g��T��������ݒ�
				findData.heightLowerLimit = m_playerRef->GetPosition().y + 500.0f;
				// �O�����̐����̕����x�N�g����ݒ�
				findData.forwardLimitDir = m_swingForwardDir;
				// �O�����̐����̌��_��ݒ�
				findData.forwardLimitOrigin = m_playerRef->GetPosition();

				// �����̍��W����A�X�C���O�^�[�Q�b�g��T��
				for (int i = 0; i < kFindSwintTargetNum; i++)
				{
					// �X�C���O�^�[�Q�b�g��T�����W�ւ̃x�N�g��
					Vector3 toFindSwingTargetVec = kToFindSwingTargetVecs[i];
					// �����o�������ɉ�]������
					swingRotationQRot.Apply(toFindSwingTargetVec);

					// �ʂ̃f�[�^��ݒ肷��
					// �X�C���O�^�[�Q�b�g��T���f�[�^��ݒ�
					findData.findSwingTargetOrigin = m_playerRef->GetPosition() + toFindSwingTargetVec;
					// �X�C���O�^�[�Q�b�g�̃|�C���g��T���f�[�^��ݒ�
					findData.findSwingTargetPointOrigin = m_playerRef->GetPosition() + toFindSwingTargetVec;

					// �w�肳�ꂽ���W����L���͈͓��ɂ���X�C���O�^�[�Q�b�g�̍��W�̒��ň�ԋ߂����W�𓾂�
					const Vector3* swingTargetPos =
						nsStringActionTarget::FindNearestSwingTargetPoint(findData);

					// �X�C���O�^�[�Q�b�g�̍��W��nullptr���H
					if (swingTargetPos == nullptr)
					{
						// ������Ȃ������B���ցB
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

				//////// 3.�X�C���O�^�[�Q�b�g�������������ǂ����ŏ�����U�蕪���� ////////

				// �X�C���O�^�[�Q�b�g�������������H
				if (m_swingTargetPos != nullptr)
				{
					// ��������
					// �X�e�[�g������L�΂����֑J�ڂ���
					ChangeState(enIsStringStretching);

				}
				else
				{
					// ������Ȃ�����
					// �X�e�[�g���X�C���O��̋󒆂ɑJ��
					ChangeState(enIsAirAfterSwing);
				}

				return;
			}

			/**
			 * @brief ����L�΂��Ă���Œ��̏���
			*/
			void CPlayerSwingAction::StringStretching()
			{
				// �J�����̒l����`�ω�������
				CameraChangeLinearly();

				// �L�т����ĂȂ����H
				if (m_playerRef->IsStringStretched() != true)
				{
					// �܂��L�т����ĂȂ��B�������^�[��
					return;
				}

				// �L�΂��؂�������A�X�e�[�g���X�C���O���ɑJ��
				ChangeState(enIsSwinging);

				return;
			}

			/**
			 * @brief �X�C���O�A�N�V�����̏���
			*/
			void CPlayerSwingAction::SwingAction()
			{
				// �J�����̒l����`�ω�������
				// StringStretching()�ł��Ă�ł邪�A�ω�������O�Ɏ����L�т����Ă��܂�����
				// ����Ȃ����߁A������ł��Ă�
				CameraChangeLinearly();

				//////// 1.�K�v�ȃx�N�g����p�� ////////
				
				// �v���C���[����X�C���O�^�[�Q�b�g�܂ł�XZ���ʂł̃x�N�g��
				Vector3 playerToTargetVecXZ = *m_swingTargetPos - m_playerRef->GetPosition();
				// Y����������
				playerToTargetVecXZ.y = 0.0f;

				// XZ���ʂ̑O�����ɁA�v���C���[����X�C���O�^�[�Q�b�g�ւ�XZ���ʂ̃x�N�g�����ˉe����
				float projectToTargetVecXZToSwingForwardDir = Dot(m_swingForwardDir, playerToTargetVecXZ);

				// XZ���ʂł́A�O�����݂̂́A�X�C���O�^�[�Q�b�g�ւ̃x�N�g��
				Vector3 toTargetFowardVecXZ = m_swingForwardDir;
				// �ˉe���ċ��߂������܂ŐL�΂�
				toTargetFowardVecXZ.Scale(projectToTargetVecXZToSwingForwardDir);

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
				float doSwingForwardDirAndToPlayerDir = 
					Dot(m_swingForwardDir, targetUptoPlayerDir);

				// ���Z�ړ������x�N�g��
				Vector3 addMoveDir = Vector3::Zero;

				// ���Z�ړ������x�N�g������]�������]�N�H�[�^�j�I��
				Quaternion qRotForAddMoveDir;

				// ���Z�ړ������x�N�g������]�������]���B
				Vector3 rotAxisForAddMoveDir = Cross(targetUptoPlayerDir, m_swingForwardDir);

				// �v���C���[���X�C���O�^�[�Q�b�g����ɂ��邩�H
				if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
				{
					// ��ɂ���Ƃ��A��]���̋��ߕ���ς���
					rotAxisForAddMoveDir = Cross(Vector3::Down, m_swingForwardDir);
				}

				// ���߂���]���𐳋K������
				rotAxisForAddMoveDir.Normalize();


				//////// 2.�X�C���O�̊p�x���v�Z ////////

				// ���ς������H�A�܂�A�X�C���O�^�[�Q�b�g����O���ɂ��邩�H
				if (doSwingForwardDirAndToPlayerDir < 0.0f)
				{
					// ��O��

					// �v���C���[���Œ�X�C���O���x����ɂ��邩�H
					if (m_playerRef->GetPosition().y > 100.0f)
					{
						// ��ɂ���

						// 0.0f�`1.0f�ɕω����郌�[�g
						float rate = doSwingForwardDirAndToPlayerDir + 1.0f;

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
						addMoveDir = m_swingForwardDir;
						// ��]�N�H�[�^�j�I����90�x��]������
						qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle/*3.14f * 0.5f*/);
						// ���Z�ړ������x�N�g������]������
						qRotForAddMoveDir.Apply(addMoveDir);
					}
					else
					{
						// �Œፂ�x�ɂ��邽�߁A�܂������i�ށB
						addMoveDir = m_swingForwardDir;
					}

				}
				else
				{
					// ����

					float radAngle = 3.14f * 0.5f;
					m_playerModelAnimationRef->SetSwingAnimState(
						nsModelAnimationConstData::enSwingAnim_swingRaiseLeg
					);

					// �v���C���[�̍������X�C���O�^�[�Q�b�g��荂���Ȃ�����B
					if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
					{
						// ���񃋁[�v�����������n�߂�X�s�[�h��ݒ�
						if (m_startDecelerateSwingSpeed <= -50.0f)
						{
							m_startDecelerateSwingSpeed = m_swingSpeed;
						}
						float rad = acosf(doSwingForwardDirAndToPlayerDir);
						float angleRate = rad / (3.14f * 0.1f);
						addMoveDir = m_swingForwardDir;
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
				const float maxG = 2500.0f;
				if (m_g > maxG)
				{
					m_g = maxG;
				}
				// l = �U��q�̒���
				const float l = toTargetForwardUpToPlayerVec.Length();
				// v�C = ��2gl(cos���C - cos����)
				if (doSwingForwardDirAndToPlayerDir < 0.0f)
				{
					// ��O���Ȃ�p�x�ɂ�錸�����Ȃ�
					m_swingSpeed = 2.0f * m_g * l;
				}
				else
				{
					// �����Ȃ�A��ɏオ��قǌ�������
					m_swingSpeed = 2.0f * m_g * l * (anyCos - highestCos);
				}
				m_swingSpeed = std::sqrtf(m_swingSpeed);


				//////// 4.�X�C���O���̍��E�ւ̕����]�� ////////
				Vector3 rightDirXZ = m_playerCameraRef->GetCameraRight();
				rightDirXZ.y = 0.0f;
				rightDirXZ.Normalize();
				float rightPower = m_playerRef->GetInputData().axisMoveRight / 7.0f;
				rightDirXZ.Scale(rightPower);
				m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, rightDirXZ);
				addMoveDir += m_inputMoveDirXZ;

				// ���K�����ĕ����x�N�g���ɂ���
				addMoveDir.Normalize();

				// ���Z�ړ��x�N�g��
				Vector3 addMoveVec = addMoveDir;
				addMoveVec.Scale(m_swingSpeed);

				// ���݂̈ړ������ƁA�X�C���O�̉��Z�ړ��������t�����Ȃ�
				// ���݂̈ړ��x�N�g�������Z�b�g����B
				Vector3 moveDir = m_playerMovementRef->GetMoveVec();
				moveDir.Normalize();
				if (Dot(addMoveDir, moveDir) <= 0.0f)
				{
					m_playerMovementRef->ResetMoveVecX();
					m_playerMovementRef->ResetMoveVecZ();
				}


				// �ړ��x�N�g���ɁA���Z�ړ��x�N�g�������Z����
				m_playerMovementRef->AddMoveVec(addMoveVec);
				m_playerMovementRef->LimitMoveVec(m_swingSpeed);

				// �X�C���O�̑O�������X�V
				m_swingForwardDir = m_playerMovementRef->GetMoveVec();
				// Y����������
				m_swingForwardDir.y = 0.0f;
				m_swingForwardDir.Normalize();

				return;
			}


			/**
			 * @brief �X�C���O��̋󒆂̏���
			*/
			void CPlayerSwingAction::AirAfterSwing()
			{
				// ���̃X�C���O�ւ̑J�ڏ���
				if (m_playerRef->GetInputData().actionSwing == true)
				{
					// ���~�����H�A����
					// �A�j���[�V�������󒆃A�C�h����Ԃ��H
					if (m_playerMovementRef->GetMoveVec().y < 0.0f &&
						m_playerRef->GetPlayerModelAnimation().GetAnimationState() ==
						nsModelAnimationConstData::enAnim_airIdle)
					{
						ChangeState(enFindSwingTarget);
					}
				}

				// �X�C���O��̉����̌���
				m_accelerationAfterSwing *= 0.99f;
				if (m_accelerationAfterSwing < kMinVelocityOfAfterSwingAcceleration)
				{
					m_accelerationAfterSwing = kMinVelocityOfAfterSwingAcceleration;
				}

				// �X�C���O��̑��x�ǃX�C���O��̉��������킹�đ��x�Ƃ���
				float velocity = m_velocityAfterSwing + m_accelerationAfterSwing;

				// ���Z�ړ��x�N�g��
				Vector3 addMoveVec = m_playerMovementRef->GetMoveVec();

				// �X�C���O�̒��ォ�H
				if (m_afterSwing)
				{
					// �X�C���O�̒���ł͂Ȃ�����
					m_afterSwing = false;
					// �X�C���O���[�������邩�H
					if (m_swingRollFlag)
					{
						// �X�C���O���[�����s����
						m_swingRollFlag = false;
						// Y�����ɃW�����v��������
						addMoveVec.y += nsMovementConstData::kJumpForce;
						m_swingRollLeaveSC->Play(false);
					}
					else
					{
						// �X�C���O��Ԃ��痣���̃T�E���h�𗬂�
						m_swingLeaveSC->Play(false);
					}
				}
				else
				{
					// �ŏ��ȊO��Y�����͉����Ȃ�
					addMoveVec.y = 0.0f;
				}

				// ���x�ŐL�΂�
				addMoveVec.Normalize();
				addMoveVec.Scale(velocity);

				// �ړ����͏���
				if (m_playerRef->GetInputData().inputMoveAxis)
				{
					Vector3 forwardDirXZ = m_playerCameraRef->GetCameraForward();
					forwardDirXZ.y = 0.0f;
					forwardDirXZ.Normalize();
					Vector3 rightDirXZ = m_playerCameraRef->GetCameraRight();
					rightDirXZ.y = 0.0f;
					rightDirXZ.Normalize();
					Vector3 inputDirXZ = forwardDirXZ * m_playerRef->GetInputData().axisMoveForward;
					inputDirXZ += rightDirXZ * m_playerRef->GetInputData().axisMoveRight;
					float radAngle = acosf(Dot(inputDirXZ, m_swingForwardDir));
					if (radAngle <= 3.14f * 0.25f || radAngle >= 3.14f * 0.7f)
					{
						if (radAngle >= 3.14f * 0.7f)
						{
							m_velocityAfterSwing -= nsWalkAndRunConstData::kWalkAcceleration;
							if (m_velocityAfterSwing <= nsWalkAndRunConstData::kWalkMaxSpeed)
							{
								ChangeState(enEnd);
							}
						}
					}
					else
					{
						inputDirXZ *= nsWalkAndRunConstData::kWalkMaxSpeed / 5.0f;
						m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, inputDirXZ);
					}

					addMoveVec += m_inputMoveDirXZ;

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
				ChangeState(enFindSwingTarget);
				// ���ɏI����m�点��
				m_playerRef->EndStringStretchToPos();
				// �v���C���[�̃X�e�[�g������Ƒ���ɂ���
				m_playerRef->ChangeWalkAndRunState();
				// �X�C���O�X�s�[�h�����Z�b�g
				m_swingSpeed = 0.0f;
				// �J�����̌��������f�t�H���g�ɖ߂�
				m_playerCameraRef->LerpDampingRate(0.0f);

				return;
			}

			/**
			 * @brief �X�C���O�A�N�V�����X�e�[�g��ύX����
			 * @param swingActionState
			*/
			void CPlayerSwingAction::ChangeState(const EnSwingActionState swingActionState)
			{
				if (m_swingActionState == swingActionState)
				{
					// �����Ȃ�ύX����K�v�Ȃ��B�������^�[���B
					return;
				}

				// �X�C���O�A�N�V�����X�e�[�g��ύX
				m_swingActionState = swingActionState;

				//////// �ύX��������x�����Ă΂�鏈�� ////////

				// �e��A�ύX��������x�����Ă΂��C�x���g
				switch (m_swingActionState)
				{
				case enFindSwingTarget:
					break;
				case enIsStringStretching:
					IsStringStretchingEvent();
					break;
				case enIsSwinging:
					IsSwingingEvent();
					break;
				case enIsAirAfterSwing:
					IsAirAfterSwingEvent();
					break;
				case enEnd:
					break;
				}

				return;
			}

			/**
			 * @brief ����L�΂����ɑJ�ڂ���Ƃ��Ɉ�x�����Ă΂��֐�
			*/
			void CPlayerSwingAction::IsStringStretchingEvent()
			{
				// �����o���̃T�E���h�𗬂�
				m_chainPutOutSC->Play(false);

				// �����X�C���O�^�[�Q�b�g�Ɍ������ĐL�΂��n�߂�
				m_playerRef->StartStringStretchToPos(*m_swingTargetPos);
				// ����L�΂��n�߂�A�j���[�V�������Đ�����
				m_playerModelAnimationRef->SetSwingAnimState(
					nsModelAnimationConstData::enSwingAnim_swingStart
				);
				// �J�����̒l����`�ω�������^�C�}�[�����Z�b�g����
				m_cameraChangeLinearlyTimer = 0.0f;

				return;
			}

			/**
			 * @brief �X�C���O���ɑJ�ڂ���Ƃ��Ɉ�x�����Ă΂��֐�
			*/
			void CPlayerSwingAction::IsSwingingEvent()
			{
				// �������Ȃ�̃T�E���h�𗬂�
				m_chainBendingSC->Play(false);

				// �������n�߂�X�C���O�X�s�[�h������������
				m_startDecelerateSwingSpeed = kStartDecelerateSwingSpeedInitialValue;
				// ���͂ɂ���Đ�����XZ���ʂł̈ړ�����������������
				m_inputMoveDirXZ = Vector3::Zero;

				m_afterSwing = true;


				// �X�C���O�X�s�[�h���������x���x�����H
				if (m_swingSpeed <= kInitialSwingSpeed)
				{
					// �x���B�������x�̃X�C���O�X�s�[�h��ݒ肷��B�������x�������ƃX�C���O�X�s�[�h�������Ȃ�B
					m_g = Math::Lerp<float>(
							min(1.0f, fabsf(m_playerMovementRef->GetMoveVec().y) / 5000.0f),
							700.0f,
							1200.0f
							);

					// �������^�[��
					return;
				}
				m_g = m_playerMovementRef->GetXZSpeed();
				// �X�C���O�X�s�[�h���ő呬�x�𒴂��Ă��邩�H
				const float maxG = 2000.0f;
				if (m_g > maxG)
				{
					// �����Ă���B�ő呬�x�ɐݒ肷��B
					m_g = maxG;
				}

				return;
			}

			/**
			 * @brief �X�C���O��̋󒆂ɑJ�ڂ���Ƃ��Ɉ�x�����Ă΂��C�x���g
			*/
			void CPlayerSwingAction::IsAirAfterSwingEvent()
			{
				// ���ɏI����m�点��
				m_playerRef->EndStringStretchToPos();

				if (m_swingRollFlag)
				{
					// �X�C���O���[�����s��
					// �X�C���O���[���̃A�j���[�V�������Đ�����
					m_playerModelAnimationRef->SetSwingAnimState(
						nsModelAnimationConstData::enSwingAnim_swingRoll
					);
				}
				else
				{
					// �X�C���O���[�����s��Ȃ�
					// �X�C���O��̋󒆏�Ԃ̃A�j���[�V�������Đ�����
					m_playerModelAnimationRef->SetSwingAnimState(
						nsModelAnimationConstData::enSwingAnim_airAfterSwing
					);
				}


				// �X�C���O���ɃX�C���O����߂����H
				if (m_afterSwing)
				{
					// �X�C���O��̉����̏��������Z�b�g
					m_accelerationAfterSwing = kInitialVelocityOfAterSwingAcceleration;
					// �X�C���O��̑��x��ێ�
					m_velocityAfterSwing = m_playerMovementRef->GetXZSpeed();
					// ���͂ɂ���Đ�����XZ���ʂł̈ړ����������Z�b�g
					m_inputMoveDirXZ = Vector3::Zero;
					// �J�����̒l�����Z�b�g
					m_playerCameraRef->LerpDampingRate(1.0f);
					m_playerCameraRef->LerpTargetOffsetUp(0.0f);
					m_swingForwardDir = m_playerMovementRef->GetMoveVec();
					m_swingForwardDir.y = 0.0f;
					m_swingForwardDir.Normalize();

					if (m_chainBendingSC->IsPlaying())
					{
						// �������Ȃ�̃T�E���h���܂�����Ă�����A�~�߂�
						m_chainBendingSC->Stop();
					}

					// ���𗣂��̃T�E���h�𗬂�
					m_chainReleaseSC->Play(false);

				}
				else
				{
					// �X�C���O���ł͂Ȃ�
					m_accelerationAfterSwing = 0.0f;
					m_velocityAfterSwing = 0.0f;
					ChangeState(enEnd);
				}

				return;
			}

			/**
			 * @brief �J�����̒l����`�ω�������
			*/
			void CPlayerSwingAction::CameraChangeLinearly()
			{
				if (m_cameraChangeLinearlyTimer >= kCameraLerpTime)
				{
					// �^�C�}�[���K��l�𒴂��Ă�����A�������^�[���B
					return;
				}

				// �^�C�}�[��i�߂�
				m_cameraChangeLinearlyTimer += nsTimer::GameTime().GetFrameDeltaTime();

				// �J�����̒l����`�⊮���ĕω�������
				m_playerCameraRef->LerpDampingRate(1.0f - 1.0f * m_cameraChangeLinearlyTimer / kCameraLerpTime);
				m_playerCameraRef->LerpTargetOffsetUp(1.0f * m_cameraChangeLinearlyTimer / kCameraLerpTime);

				return;
			}

		}
	}
}
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
			 * @return �ʏ�̓����̏������s�����H
			*/
			bool CPlayerSwingAction::Execute()
			{
				// �ʏ�̓����̏������s�����H
				bool executeNormalMovementFlag = false;

				// �X�C���O�^�[�Q�b�g��T�����H
				if (m_swingActionState == enFindSwingTarget)
				{
					// �X�C���O�^�[�Q�b�g��T��
					FindSwingTarget();
				}

				if (m_playerRef->GetInputData().actionSwing != true)
				{
					m_swingActionState = enEnd;
					m_playerRef->SetState(nsPlayerConstData::enAirAfterStringAction);
				}

				switch (m_swingActionState)
				{
				case enIsStringStretching:

					executeNormalMovementFlag = true;

					if (m_playerMovementRef->IsAir() != true)
					{
						m_swingActionState = EnSwingActionState::enEnd;
					}
					else if (m_playerRef->IsStringStretched())
					{
						m_swingActionState = EnSwingActionState::enIsSwinging;
						if (m_swingSpeed <= FLT_EPSILON)
						{
							m_swingSpeed = 500.0f + fabsf(m_playerMovementRef->GetMoveVec().y) * 0.5f;
						}
						m_swingSpeed = 1500.0f;
					}
					
					break;
				case enIsSwinging:

					executeNormalMovementFlag = false;

					SwingAction();
					if (m_playerMovementRef->IsAir() != true)
					{
						m_swingActionState = EnSwingActionState::enEnd;
					}

					break;
				case enEnd:

					executeNormalMovementFlag = true;
					m_swingActionState = enFindSwingTarget;
					m_playerRef->EndStringStretchToPos();
					m_playerRef->SetState(nsPlayerConstData::EnPlayerState::enWalkAndRun);
					//m_playerRef->SetState(nsPlayerConstData::enAirAfterStringAction);

					break;
				}

				return executeNormalMovementFlag;
			}


			/**
			 * @brief �X�C���O�^�[�Q�b�g��T��
			*/
			void CPlayerSwingAction::FindSwingTarget()
			{

				Vector3 toFindSwintTargetPos = { 0.0f,2000.0f,1000.0f };
				Vector3 forward = m_playerRef->GetCamera().GetCameraForward();
				forward.y = 0.0f;
				forward.Normalize();
				float rad = acosf(Dot(forward, Vector3::Front));
				if (forward.x < 0.0f)
				{
					rad = -rad;
				}
				Quaternion qRot;
				qRot.SetRotationY(rad);
				qRot.Apply(toFindSwintTargetPos);

				// �w�肳�ꂽ���W����L���͈͓��ɂ���X�C���O�^�[�Q�b�g�̍��W�̒��ň�ԋ߂����W�𓾂�
				m_swingTargetPos = nsStringActionTarget::GetNearestSwintTargetPointWithinScope(
					m_playerRef->GetPosition() + toFindSwintTargetPos, 2000.0f
				);

				// �X�C���O�^�[�Q�b�g�������������H
				if (m_swingTargetPos != nullptr)
				{
					// ��������
					// �����X�C���O�^�[�Q�b�g�Ɍ������ĐL�΂��n�߂�
					m_playerRef->StartStringStretchToPos(*m_swingTargetPos);
					m_swingActionState = EnSwingActionState::enIsStringStretching;
				}
				else
				{
					m_swingActionState = EnSwingActionState::enEnd;
				}


				return;
			}

			/**
			 * @brief �X�C���O�A�N�V�����̏���
			*/
			void CPlayerSwingAction::SwingAction()
			{
				// ���Z�ړ��x�N�g��
				Vector3 addMoveVec = Vector3::Zero;
				
				// �v���C���[����X�C���O�^�[�Q�b�g�܂ł�XZ���ʂł̃x�N�g��
				Vector3 playerToTargetVecXZ = *m_swingTargetPos - m_playerRef->GetPosition();
				// Y����������
				playerToTargetVecXZ.y = 0.0f;

				// XZ���ʂł̑O����
				Vector3 forwardDirXZ = m_playerRef->GetCamera().GetCameraForward();
				// Y����������
				forwardDirXZ.y = 0.0f;
				// ���K��
				forwardDirXZ.Normalize();

				// XZ���ʂ̑O�����ɁA�v���C���[����X�C���O�^�[�Q�b�g�ւ�XZ���ʂ̃x�N�g�����ˉe����
				float projectToTargetVecXZToForwardDirXZ = Dot(forwardDirXZ, playerToTargetVecXZ);

				// XZ���ʂł́A�O�����݂̂́A�X�C���O�^�[�Q�b�g�ւ̃x�N�g��
				Vector3 toTargetFowardVecXZ = forwardDirXZ;
				toTargetFowardVecXZ.Scale(projectToTargetVecXZToForwardDirXZ);
				// XZ���ʂł́A�O�����݂̂́A�X�C���O�^�[�Q�b�g�̍��W
				const Vector3 toTargetForwardPosXZ = m_playerRef->GetPosition() + toTargetFowardVecXZ;
				// �O�����Ə�����݂̂́A�X�C���O�^�[�Q�b�g�̍��W
				Vector3 toTargetForwardUpPos = toTargetForwardPosXZ;
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
				// ���Z�ړ������x�N�g������]�������]��
				Vector3 rotAxisForAddMoveDir = Cross(targetUptoPlayerDir, forwardDirXZ);
				if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
				{
					rotAxisForAddMoveDir = Cross(Vector3::Down, forwardDirXZ);
				}
				rotAxisForAddMoveDir.Normalize();

				// ���ς������H�A�܂�A�X�C���O�^�[�Q�b�g����O���ɂ��邩�H
				if (dotFowardDirXZAndToPlayerDir < 0.0f)
				{
					// ��O��

					float rate = dotFowardDirXZAndToPlayerDir + 1.0f;
					rate = pow(rate, 2.0f);

					// ��]�N�H�[�^�j�I����90�x��]������
					qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, 3.14f * 0.5f);
					// ���Z�ړ������x�N�g������]������
					qRotForAddMoveDir.Apply(addMoveDir);
					addMoveVec = addMoveDir;
					//m_swingSpeed += 5.0f;
					addMoveVec.Scale(m_swingSpeed);

				}
				else
				{
					// ����

					float radAngle = 3.14f * 0.5f;
					Vector3 to = m_playerRef->GetPosition() - toTargetForwardPosXZ;
					float rate = to.Length() / toTargetForwardUpToPlayerVec.Length();
					rate = pow(rate, 2.0f);
					rate = min(rate, 3.14f * 0.5f);

					if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
					{
						if (m_swingSpeed2 <= -50.0f)
						{
							m_swingSpeed2 = m_swingSpeed;
						}
						float dot = Dot(forwardDirXZ, targetUptoPlayerDir);
						float rad = acosf(dot);
						float angleRate = rad / (3.14f * 0.1f);
						m_swingSpeed = Math::Lerp<float>(min(1.0f,angleRate), m_swingSpeed2, 0.0f);
					}
					else
					{

						//m_swingSpeed += 5.0f;
						addMoveDir = forwardDirXZ;
						radAngle = 3.14f * 0.5f * rate;

					}

					// ��]�N�H�[�^�j�I����90�x��]������
					qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle);
					// ���Z�ړ������x�N�g������]������
					qRotForAddMoveDir.Apply(addMoveDir);
					addMoveDir.Normalize();
					addMoveVec = addMoveDir;
					rate += 0.5f;
					addMoveVec.Scale(m_swingSpeed /*+= 5.0f * min(rate, 1.0f)*/);

				}


				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecY();
				m_playerMovementRef->ResetMoveVecZ();

				// �ړ��x�N�g���ɁA���Z�ړ��x�N�g�������Z����
				m_playerMovementRef->AddMoveVec(addMoveVec);

				// �L�����N�^�[�R���g���[���[���g���Ĉړ�������
				m_playerMovementRef->MoveWithCharacterController();


				return;
			}

		}
	}
}
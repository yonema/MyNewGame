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
			 * @return �����Ƒ���̏������s�����H
			*/
			bool CPlayerSwingAction::Execute()
			{
				// �����Ƒ���̏������s�����H
				bool executeWalkAndRunFlag = false;

				// �X�C���O�^�[�Q�b�g��T�����H
				if (m_swingActionState == enFindSwingTarget)
				{
					// �X�C���O�^�[�Q�b�g��T��
					FindSwingTarget();
				}

				if (m_playerRef->GetInputData().actionSwing != true)
				{
					m_swingActionState = enEnd;
				}

				switch (m_swingActionState)
				{
				case enIsStringStretching:

					executeWalkAndRunFlag = true;

					if (m_playerMovementRef->IsAir() != true)
					{
						m_swingActionState = EnSwingActionState::enEnd;
					}
					else if (m_playerRef->IsStringStretched())
					{
						m_swingActionState = EnSwingActionState::enIsSwinging;
					}
					
					break;
				case enIsSwinging:

					executeWalkAndRunFlag = false;

					SwingAction();

					break;
				case enEnd:

					executeWalkAndRunFlag = true;
					m_swingActionState = enFindSwingTarget;
					m_playerRef->EndStringStretchToPos();
					m_playerRef->SetState(nsPlayerConstData::EnPlayerState::enWalkAndRun);

					break;
				}

				return executeWalkAndRunFlag;
			}


			/**
			 * @brief �X�C���O�^�[�Q�b�g��T��
			*/
			void CPlayerSwingAction::FindSwingTarget()
			{

				Vector3 toFindSwintTargetPos = { 0.0f,1000.0f,500.0f };
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
				// �X�C���O�^�[�Q�b�g�i�x�_�j���玩�g�̍��W�i�d��j�ւ̃x�N�g��
				Vector3 fromSwingTargetToPosVec = m_playerRef->GetPosition() - *m_swingTargetPos;
				// �ړ��������
				Vector3 moveDir = m_playerRef->GetCamera().GetCameraForward();
				moveDir.y = 0.5f;
				moveDir.Normalize();

				// �x�_����d��ւ̃x�N�g����90�x�񂷂��߂̎�
				Vector3 rotAxis = Vector3::Down;//fromSwingTargetToPosVec;
				rotAxis.Normalize();
				rotAxis.Cross(moveDir);

				// �x�_����d��ւ̃x�N�g����90�x�񂵂������x�N�g��
				Vector3 verticalDir = fromSwingTargetToPosVec;
				// 90�x�񂷉�]�N�H�[�^�j�I��
				Quaternion qRot;
				qRot.SetRotation(rotAxis, 3.14f * 0.5f);
				qRot.Apply(verticalDir);
				verticalDir.Normalize();
				verticalDir.Scale(500.0f);

				Vector3 next = m_playerRef->GetPosition() + verticalDir;

				Vector3 toNextVec = next - *m_swingTargetPos;
				Vector3 toNextDir = toNextVec;
				toNextDir.Normalize();
				Vector3 toOldVec = fromSwingTargetToPosVec;
				toOldVec.Normalize();
				float rad = acosf(Dot(toNextDir, toOldVec));

				qRot.SetRotation(rotAxis, rad);
				qRot.Apply(toNextVec);

				Vector3 add = *m_swingTargetPos + toNextVec - m_playerRef->GetPosition();



				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecY();
				m_playerMovementRef->ResetMoveVecZ();

				// �ړ��x�N�g���ɁA���Z�ړ��x�N�g�������Z����
				m_playerMovementRef->AddMoveVec(add);

				// �L�����N�^�[�R���g���[���[���g���Ĉړ�������
				m_playerMovementRef->MoveWithCharacterController();

				Vector3 debugVec = m_playerRef->GetPosition() - *m_swingTargetPos;
				wchar_t text[128];
				swprintf_s(text, L"StringLen:%2.2f", debugVec.Length());
				m_debugFont->SetText(text);
				return;
			}

		}
	}
}
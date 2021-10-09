#include "stdafx.h"
#include "PlayerMovement.h"
#include "Player.h"
#include "PlayerConstData.h"
#include "GameTime.h"

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
			// �v���C���[�ړ��N���X�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsPlayerConstData::nsPlayerMoveConstData;

			/**
			 * @brief ������
			 * @param[in] radius �J�v�Z���R���C�_�[�̔��a
			 * @param[in] height �J�v�Z���R���C�_�[�̍���
			 * @param[in,out] player �v���C���[�̎Q��
			*/
			void CPlayerMovement::Init(
				const float radius,
				const float height,
				CPlayer* player
			)
			{
				// �v���C���[�̎Q�Ƃ��Z�b�g����
				m_playerRef = player;

				// �L�����N�^�[�R���g���[��������
				m_charaCon.Init(radius, height, m_playerRef->GetPosition());

				// �v���C���[�̕����Ƒ���̃N���X�̏�����
				m_playerWalkAndRun.Init(*m_playerRef, this);

				// �v���C���[�̃X�C���O�A�N�V�����N���X�̏�����
				m_playerSwingAction.Init(m_playerRef, this);


#ifdef MY_DEBUG
				m_moveVecFont = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFirst);
				m_moveVecFont->SetPosition({ -200.0f,200.0f });
#endif

				return;
			}

			/**
			 * @brief �v���C���[�̈ړ��N���X�̃A�b�v�f�[�g�����s����
			*/
			void CPlayerMovement::ExecuteUpdate()
			{
				// �X�C���O�A�N�V������true�ŁA���A�󒆂�������
				if (m_playerRef->GetInputData().actionSwing && IsAir())
				{
					// �X�C���O���
					m_playerRef->SetState(nsPlayerConstData::enSwing);
				}
				else
				{
					// �����Ƒ�����
					m_playerRef->SetState(nsPlayerConstData::enWalkAndRun);
				}

				// �v���C���[�̈ړ����X�V
				UpdateMovePlayer();

				// �v���C���[�̉�]���X�V
				UpdateTurnPlayer();

				return;
			}


			/**
			 * @brief �v���C���[�̈ړ����X�V
			*/
			void CPlayerMovement::UpdateMovePlayer()
			{
				// �X�e�[�g�ŏ�����U�蕪����
				switch (m_playerRef->GetState())
				{
				case nsPlayerConstData::enWalkAndRun:	// �����Ƒ���
					// �����Ƒ�������s
					m_playerWalkAndRun.Execute();

					// �ړ��x�N�g����X,Z������������
					m_moveVec.x = 0.0f;
					m_moveVec.z = 0.0f;


					// �ړ��x�N�g���ɁA���Z�ړ��x�N�g�������Z����
					m_moveVec += m_addMoveVec;

					// �d��
					m_moveVec.y -= kGravityScale * nsTimer::GameTime().GetFrameDeltaTime();

					// �W�����v
					// �W�����v�{�^����������Ă���A���A�n�ʂɂ��Ă���
					if (m_playerRef->GetInputData().actionJump/* && m_charaCon.IsOnGround()*/)
					{
						m_moveVec.y += kJumpForce;
					}

					// �L�����N�^�[�R���g���[���[���s
					m_playerRef->SetPosition(m_charaCon.Execute(m_moveVec, nsTimer::GameTime().GetFrameDeltaTime()));

					// �n�ʂɂ��Ă�����A�d�̓��Z�b�g
					if (m_charaCon.IsOnGround())
					{
						m_moveVec.y = 0.0f;
					}
					break;
				case nsPlayerConstData::enSwing:	// �X�C���O

					// �X�C���O�A�N�V���������s
					m_playerSwingAction.Execute();

					m_playerRef->SetPosition(m_addMoveVec);
					m_charaCon.SetPosition(m_addMoveVec);

					m_moveVec = Vector3::Zero;
					m_addMoveVec = Vector3::Zero;

					break;
				}

#ifdef MY_DEBUG
				wchar_t text[256];
				//swprintf_s(text, L"x:%2.2f,y:%2.2f,z:%2.2f\nlen:%2.2f",
				//	m_moveVec.x, m_moveVec.y, m_moveVec.z,m_moveVec.Length());
				//swprintf_s(text, L"x:%2.2f,y:%2.2f,z:%2.2f",m_moveVec.x, m_moveVec.y, m_moveVec.z);
				swprintf_s(text, L"forward:%2.2f,right:%2.2f\nlen:%2.2f",
					m_playerRef->GetInputData().axisMoveForward,
					m_playerRef->GetInputData().axisMoveRight,
					m_moveVec.Length());
				m_moveVecFont->SetText(text);
#endif

				return;
			}

			/**
			 * @brief �v���C���[�̉�]���X�V
			*/
			void CPlayerMovement::UpdateTurnPlayer()
			{
				// X,Z���ʂł̈ړ������邩�H
				if (fabsf(m_moveVec.x) < kMoveVecMin && fabsf(m_moveVec.z) < kMoveVecMin)
				// �����͂����邩�H
				//if (fabsf(m_playerRef->GetInputData().axisMoveForward) < 0.001f &&
				//	fabsf(m_playerRef->GetInputData().axisMoveRight) < 0.001f)
				{
					// �ړ����Ă��Ȃ�
					// �������^�[��
					return;
				}

				// tan�� = m_moveVec.x / m_moveVec.y
				const float radAngle = atan2(m_moveVec.x, m_moveVec.z);
				// ���W�A���P�ʂŉ�
				Quaternion nexrQRot;
				nexrQRot.SetRotation(Vector3::AxisY, radAngle);
				// ���݂̉�]�Ǝ��̉�]�����ʐ��`��Ԃ��s���A���f�������X�ɉ�]������B
				nexrQRot.Slerp(kModelRotRate, m_playerRef->GetRotation(), nexrQRot);
				m_playerRef->SetRotation(nexrQRot);

				return;
			}
		}
	}
}
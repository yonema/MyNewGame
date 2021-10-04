#include "stdafx.h"
#include "PlayerMovement.h"
#include "PlayerConstData.h"
#include "PlayerCamera.h"
#include "PlayerInput.h"
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
			using namespace nsPlayerMoveConstData;

			/**
			 * @brief ������
			 * @param[in] radius �J�v�Z���R���C�_�[�̔��a
			 * @param[in] height �J�v�Z���R���C�_�[�̍���
			 * @param[in,out] position �v���C���[�̍��W�̎Q��
			 * @param[in,out] rotation �v���C���[�̉�]�̎Q��
			 * @param[in] playerCamera �v���C���[�J����
			 * @param[in] playerInputData �v���C���[���͏��
			*/
			void CPlayerMovement::Init(
				const float radius,
				const float height,
				Vector3* position,
				Quaternion* rotation,
				const CPlayerCamera& playerCamera,
				const SPlayerInputData& playerInputData
			)
			{
				// �v���C���[�̍��W�̎Q�Ƃ��Z�b�g����
				m_playerPosition = position;
				// �v���C���[�̉�]�̎Q�Ƃ��Z�b�g����
				m_playerRotation = rotation;
				// �v���C���[�J�������Z�b�g����
				m_playerCamera = &playerCamera;
				// �v���C���[�̓��͏����Z�b�g����
				m_playerInputData = &playerInputData;

				// �L�����N�^�[�R���g���[��������
				m_charaCon.Init(radius, height, *m_playerPosition);

				// �v���C���[�̕����Ƒ���̃N���X�̏�����
				m_playerWalkAndRun.Init(&m_moveVecForward, &m_moveVecRight, playerCamera, playerInputData);


#ifdef MY_DEBUG
				m_moveVecFont = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFirst);
				m_moveVecFont->SetPosition({ -100.0f,100.0f });
#endif

				return;
			}

			/**
			 * @brief �v���C���[�̈ړ��N���X�̃A�b�v�f�[�g�����s����
			*/
			void CPlayerMovement::ExecuteUpdate()
			{
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
				// �����Ƒ�������s
				m_playerWalkAndRun.Execute(m_charaCon.IsJump());

				// �ړ��x�N�g����X,Z������������
				m_moveVec.x = 0.0f;
				m_moveVec.z = 0.0f;
				// �ړ��x�N�g���ɁA�O�����ƈړ��x�N�g���ƉE�����̈ړ��x�N�g�������Z����
				m_moveVec += m_moveVecForward + m_moveVecRight;

				// �d��
				m_moveVec.y -= kGravityScale * nsTimer::GameTime().GetFrameDeltaTime();

				// �W�����v
				// �W�����v�{�^����������Ă���A���A�n�ʂɂ��Ă���
				if (m_playerInputData->actionJump && m_charaCon.IsOnGround())
				{
					m_moveVec.y += kJumpForce;
				}

				// �L�����N�^�[�R���g���[���[���s
				*m_playerPosition = m_charaCon.Execute(m_moveVec, nsTimer::GameTime().GetFrameDeltaTime());

				// �n�ʂɂ��Ă�����A�d�̓��Z�b�g
				if (m_charaCon.IsOnGround())
				{
					m_moveVec.y = 0.0f;
				}

#ifdef MY_DEBUG
				wchar_t text[256];
				swprintf_s(text, L"x:%2.2f,y:%2.2f,z:%2.2f", m_moveVec.x, m_moveVec.y, m_moveVec.z);
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
				if (fabs(m_moveVec.x) < 0.001f && fabs(m_moveVec.y) < 0.001f)
				{
					// �ړ����Ă��Ȃ�
					// �������^�[��
					return;
				}

				// tan�� = m_moveVec.x / m_moveVec.y
				const float radAngle = atan2(m_moveVec.x, m_moveVec.z);
				// ���W�A���P�ʂŉ�
				m_playerRotation->SetRotation(Vector3::AxisY, radAngle);

				return;
			}
		}
	}
}
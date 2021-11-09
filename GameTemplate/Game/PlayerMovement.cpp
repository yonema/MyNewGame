#include "stdafx.h"
#include "PlayerMovement.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "PlayerModelAnimation.h"


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
			using namespace nsPlayerConstData::nsMovementConstData;

			/**
			 * @brief ������
			 * @param[in] radius �J�v�Z���R���C�_�[�̔��a
			 * @param[in] height �J�v�Z���R���C�_�[�̍���
			 * @param[in,out] player �v���C���[�̎Q��
			 * @param[in,out] playerCamera �v���C���[�J�����̎Q��
			 * @param[in.out] playerModelAnimation �v���C���[���f���A�j���[�V�����̎Q��
			*/
			void CPlayerMovement::Init(
				const float radius,
				const float height,
				CPlayer* player,
				CPlayerCamera* playerCamera,
				CPlayerModelAnimation* playerModelAnimation
			)
			{
				// �v���C���[�̎Q�Ƃ��Z�b�g����
				m_playerRef = player;

				// �L�����N�^�[�R���g���[��������
				m_charaCon.Init(radius, height, m_playerRef->GetPosition());

				// �v���C���[�̕����Ƒ���N���X�̏�����
				m_playerWalkAndRun.Init(*m_playerRef, this);

				// �v���C���[�̃X�C���O�A�N�V�����N���X�̏�����
				m_playerSwingAction.Init(m_playerRef, this, playerCamera, playerModelAnimation);

				// �v���C���[�̕ǂ𑖂鏈���N���X�̏�����
				m_playerWallRun.Init(m_playerRef, this);

				return;
			}

			/**
			 * @brief �v���C���[�̈ړ��N���X�̃A�b�v�f�[�g�����s����
			*/
			void CPlayerMovement::ExecuteUpdate()
			{
				// �X�e�[�g���X�V
				UpdateState();

				// �v���C���[�̈ړ����X�V
				UpdateMovePlayer();

				// �v���C���[�̉�]���X�V
				UpdateTurnPlayer();

				nsDebug::DrawVector(m_playerRef->GetPosition(), m_moveVec, "playerVec");
				nsDebug::DrawTextPanel(std::to_wstring(GetVelocity()), L"speed");

				return;
			}

			/**
			 * @brief ���Z�ړ��x�N�g���ɐ�����������
			 * @param[in] limit ����
			*/
			void CPlayerMovement::LimitMoveVec(const float limit)
			{
				if (m_moveVec.Length() <= limit)
				{
					// ���Z�ړ��x�N�g����������菬����������A�������|���Ȃ��B�������^�[���B
					return;
				}

				// ���������ς��̑傫���̃x�N�g���ɒ���
				m_moveVec.Normalize();
				m_moveVec.Scale(limit);

				return;
			}

			/**
			 * @brief �d�͂�������
			*/
			void CPlayerMovement::ApplyGravity()
			{
				if (m_useGravity != true)
				{
					// �d�͂������Ȃ��B�������^�[��
					return;
				}

				// �d�͂�������
				m_moveVec.y -= nsPlayerConstData::nsMovementConstData::kGravityScale *
					nsTimer::GameTime().GetFrameDeltaTime();

				// �d�͂̐���
				//if (m_moveVec.y < -nsPlayerConstData::nsMovementConstData::kMaxFallSpeed)
				//{
				//	m_moveVec.y = -nsPlayerConstData::nsMovementConstData::kMaxFallSpeed;
				//}

				return;
			}

			/**
			 * @brief �L�����N�^�[�R���g���[���[���g�����ړ�
			*/
			void CPlayerMovement::MoveWithCharacterController()
			{
				// �L�����N�^�[�R���g���[���[���s
				m_playerRef->SetPosition(
					m_charaCon.Execute(m_moveVec, nsTimer::GameTime().GetFrameDeltaTime())
				);

				return;
			}

			/**
			 * @brief �X�e�[�g���X�V����
			*/
			void CPlayerMovement::UpdateState()
			{

				//if (IsOnWall())
				//{
				//	m_playerRef->ChangeWallRun();
				//	return;
				//}

				// �X�C���O�g���K�[��true�ŁA���A�󒆂�������
				if (m_playerRef->GetInputData().actionSwing && IsAir())
				{
					// �X�C���O��Ԃ֑J��
					m_playerRef->ChangeSwingState();
				}

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
				// �����Ƒ���
				case nsPlayerConstData::enWalkAndRun:
					// �����Ƒ�������s
					m_playerWalkAndRun.Execute();
					break;
				// �X�C���O
				case nsPlayerConstData::enSwing:
					// �X�C���O�A�N�V���������s
					m_playerSwingAction.Execute();
					break;
				// �Ǒ���
				case nsPlayerConstData::enWallRun:
					// �Ǒ�������s
					m_playerWallRun.Execute();
					break;
				}

				ApplyGravity();

				MoveWithCharacterController();


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
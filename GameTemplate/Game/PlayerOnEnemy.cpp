#include "stdafx.h"
#include "PlayerOnEnemy.h"
#include "Player.h"
#include "AICar.h"
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

			using namespace nsPlayerConstData::nsOnEnemyConstData;

			/**
			 * @brief ������
			 * @param[in.out] player �v���C���[
			 * @param[in,out] playerMovement �v���C���[�ړ��N���X�̎Q��
			*/
			void CPlayerOnEnemy::Init(
				CPlayer* player,
				CPlayerMovement* playerMovement
			)
			{
				m_playerRef = player;
				m_playerMovementRef = playerMovement;

				return;
			}

			/**
			 * @brief �v���C���[�̓G�̏�ɏ�鏈�������s
			*/
			void CPlayerOnEnemy::Execute()
			{
				// �X�e�[�g�ɂ���ď�����U�蕪����
				switch (m_onEnemyState)
				{
				case enStartStretchToEnemy:
					StartStretchToEnemy();
					break;
				case enStretchingToEnemy:
					StretchingToEnemy();
					break;
				case enGoOnEnemy:
					GoOnEnemy();
					break;
				case enOnEnemy:
					OnEnemy();
					break;
				}

				return;
			}

			/**
			 * @brief ������Ԃɖ߂�
			*/
			void CPlayerOnEnemy::Reset()
			{
				m_onEnemyState = enStartStretchToEnemy;
				m_enemyRef = nullptr;
				m_playerRef->SetIsOnEnemyCamera(false);
				m_playerMovementRef->SetUseGravity(true);

				return;
			}


			/**
			 * @brief �G�Ɍ������Ď���L�΂��n�߂鏈��
			*/
			void CPlayerOnEnemy::StartStretchToEnemy()
			{
				// �^�[�Q�b�g�̓G�̎Q�Ƃ�����Ă���
				m_enemyRef = m_playerRef->GetCatchEnemy().GetTargetEnemy();

				if (m_enemyRef)
				{
					// �G�̕�����������
					LookAtEnemy();

					// �G������΁A�X�e�[�g��J�ڂ���B
					ChangeState(enStretchingToEnemy);
				}

				return;
			}


			/**
			 * @brief �G�l�~�[�Ɍ������Ď���L�΂�����
			*/
			void CPlayerOnEnemy::StretchingToEnemy()
			{
				// �^�[�Q�b�g���W�́A�G�̍��W
				Vector3 targetPos = m_enemyRef->GetPosition();
				// ������������Əグ��
				targetPos.y += kOnEnemyHeight;
				
				// �G�Ɍ������Ď���L�΂�
				m_playerRef->SetStringStretchPos(targetPos);

				if (m_playerRef->IsStringStretched())
				{
					// �L�΂��؂�����A�X�e�[�g�J�ځB
					ChangeState(enGoOnEnemy);
				}

				return;
			}

			/**
			 * @brief �G�̏�ɍs������
			*/
			void CPlayerOnEnemy::GoOnEnemy()
			{
				// �^�[�Q�b�g���W�́A�G�̍��W
				Vector3 targetPos = m_enemyRef->GetPosition();
				// ������������Əグ��
				targetPos.y += kOnEnemyHeight;

				// ���̐L�΂�����X�V����B
				m_playerRef->SetStringStretchPos(targetPos);

				// �ړ��x�N�g�������Z�b�g����
				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecY();
				m_playerMovementRef->ResetMoveVecZ();

				// �ړ���̍��W
				Vector3 pos = Vector3::Zero;
				// �ŏ��̍��W����A�^�[�Q�b�g���W�܂ŁA���`�⊮�ŏ��X�ɋ߂Â��B
				pos.Lerp(m_goOnEnemyRate, m_startGoOnEnemyPos, targetPos);
				// �v���C���[�̍��W�𒼐ڐݒ肷��
				m_playerMovementRef->SetDirectPosition(pos);

				// ��ԗ���i�߂�
				m_goOnEnemyRate += nsTimer::GameTime().GetFrameDeltaTime();

				if (m_goOnEnemyRate >= 1.0f)
				{
					// �G�̏�܂Œ�������A�X�e�[�g��J�ڂ���B
					ChangeState(enOnEnemy);
				}

				return;
			}

			/**
			 * @brief �G�̏�ɂ��鏈��
			*/
			void CPlayerOnEnemy::OnEnemy()
			{
				// �^�[�Q�b�g�̓G�̎Q�Ƃ�����Ă���
				m_enemyRef = m_playerRef->GetCatchEnemy().GetTargetEnemy();

				if (m_enemyRef == nullptr)
				{
					// �G�����Ȃ������瑁�����^�[���B
					return;
				}

				// �^�[�Q�b�g���W�́A�G�̍��W
				Vector3 targetPos = m_enemyRef->GetPosition();
				// ������������Əグ��
				targetPos.y += kOnEnemyHeight;

				// ����Ă���G�Ɠ�����]�ɂ���
				Quaternion qRot = m_enemyRef->GetRotation();

				m_playerMovementRef->SetDirectPosition(targetPos);
				m_playerRef->SetRotation(qRot);


				return;
			}

			/**
			 * @brief �G�̕�����������
			*/
			void CPlayerOnEnemy::LookAtEnemy()
			{
				// �v���C���[����G�ւ̕����x�N�g��
				Vector3 playerToEnemyNorm = m_enemyRef->GetPosition() - m_playerRef->GetPosition();
				playerToEnemyNorm.Normalize();	// ���K������
				// ��]
				Quaternion qRot;
				qRot.SetRotation(Vector3::Front, playerToEnemyNorm);
				// �v���C���[�ɉ�]��ݒ肷��
				m_playerRef->SetRotation(qRot);

				return;
			}

			/**
			 * @brief �X�e�[�g��ύX����
			 * @param[in] newState �V�����X�e�[�g
			*/
			void CPlayerOnEnemy::ChangeState(const nsPlayerConstData::nsOnEnemyConstData::EnOnEnemyState newState)
			{
				if (m_onEnemyState == newState)
				{
					// ���݂̃X�e�[�g�Ɠ����Ȃ�A�����Ȃ��B�������^�[���B
					return;
				}

				// �X�e�[�g��ύX
				m_onEnemyState = newState;

				// �ύX�����u�ԂɁA��x�����Ă΂�鏈���B
				switch (m_onEnemyState)
				{
				case enStartStretchToEnemy:
					break;

				case enStretchingToEnemy:
					// ����L�΂��n�߂āA�d�͂�؂��āA�G�̏�ɏ���Ă���J�����ɂ���B
					m_playerRef->SetIsOnEnemyCamera(true);
					m_playerRef->StartStringStretchToPos(m_enemyRef->GetPosition());
					m_playerMovementRef->SetUseGravity(false);
					break;

				case enGoOnEnemy:
					m_startGoOnEnemyPos = m_playerRef->GetPosition();
					break;

				case enOnEnemy:
					// �⊮�������Z�b�g���āA���̐L�΂����I����B
					m_goOnEnemyRate = 0.0f;
					m_playerRef->EndStringStretchToPos();
					break;
				}


				return;
			}


		}
	}
}
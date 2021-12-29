#include "stdafx.h"
#include "PlayerCatchEnemy.h"
#include "PlayerConstData.h"
#include "Player.h"
#include "GameMainState.h"
#include "AICar.h"

namespace nsMyGame
{

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		using namespace nsPlayerConstData::nsCatchEnemyConstData;

		/**
		 * @brief ����������
		 * @param[in,out] playerRef �v���C���[�̎Q��
		*/
		void CPlayerCatchEnemy::Init(CPlayer* playerRef)
		{
			m_playerRef = playerRef;

			// �Q�[���X�e�[�g�̎Q�Ƃ����������Ă���
			m_gameState = FindGO<nsGameState::CGameMainState>(
				nsCommonData::kGameObjectName[nsCommonData::enMainGameState]
				);

			// �Ԃ����̎Q�Ƃ����������Ă���
			m_aiCarsRef = m_gameState->GetAICar();

			return;
		}

		/**
		 * @brief ���C���[���G��߂܂��鏈���N���X�̃A�b�v�f�[�g�����s
		*/
		void CPlayerCatchEnemy::ExecuteUpdate()
		{
			if (m_playerRef->GetState() == nsPlayerConstData::enOnEnemy)
			{
				// �G�̏�ɏ���Ă��鎞�̍X�V
				OnEnemyUpdate();
				// �������^�[��
				return;
			}

			// �^�[�Q�b�g��T��
			FindTarget();

			// ���̃N���X�Ŏg�����ʃf�[�^�̍X�V
			UpdateCommonData();

			// �G��߂܂��邩�ǂ������ׂ�
			CheckCatchEnemy();

			return;
		}

		/**
		 * @brief �G�̏�ɏ���Ă��鎞�̍X�V
		*/
		void CPlayerCatchEnemy::OnEnemyUpdate()
		{
			if (m_playerRef->GetCamera().IsOnEnemyCamera() != true)
			{
				// �G�̏�ɂ܂�����Ă��Ȃ�������A�������^�[���B
				return;
			}

			m_onEnemyTimer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_onEnemyTimer >= 5.0f)
			{
				m_onEnemyTimer = 0.0f;
				m_targetRef = nullptr;
				m_playerRef->ResetOnEnemy();
				m_playerRef->ChangeWalkAndRunState();
			}

			return;
		}

		/**
		 * @brief �^�[�Q�b�g��T��
		*/
		void CPlayerCatchEnemy::FindTarget()
		{
			if (m_targetRef)
			{
				// �^�[�Q�b�g������ꍇ�A�^�[�Q�b�g�Ƃ��Čp�����Ă悢�����ׂ�B

				// �^�[�Q�b�g��Ԃ��p�����邩���ׂ�
				CheckContinueTarget();
			}

			if (m_targetRef != nullptr)
			{
				// �^�[�Q�b�g������ꍇ�͒T���Ȃ��̂ŁA�������^�[���B
				return;
			}

			// �G�l�~�[1�̂����ׂ�
			for (auto& enemy : *m_aiCarsRef)
			{
				// �v���C���[����G�l�~�[�ւ̃x�N�g��
				Vector3 playerToEnemyVec = enemy->GetPosition() - m_gameState->GetPlayer().GetPosition();
				// �v���C���[����G�l�~�[�ւ̋���
				const float playerToEnemyLen = playerToEnemyVec.Length();

				if (playerToEnemyLen >= kTargetMaxLength)
				{
					// ���������ȏ㗣��Ă��邽�߁A�^�[�Q�b�g�ɂ͂Ȃ�Ȃ��B���ցB
					continue;
				}

				// �J��������G�l�~�[�ւ̃x�N�g��
				Vector3 cameraToEnemyVec =
					enemy->GetPosition() - m_gameState->GetPlayer().GetCamera().GetCameraPosition();
				// �J��������G�l�~�[�ւ̕���
				Vector3 cameraToEnemyNorm = cameraToEnemyVec;
				cameraToEnemyNorm.Normalize();	// ���K������
				// �J��������G�l�~�[�ւ̕����ƁA�J�����̑O�����́A�p�x��
				float angleDiff =
					Dot(cameraToEnemyNorm, m_gameState->GetPlayer().GetCamera().GetCameraForward());
				angleDiff = acosf(angleDiff);	// ���ς��p�x�ɕϊ�

				if (angleDiff >= kTargetMaxAngle)
				{
					// �p�x�������ȏ�̑傫���̂��߁A�^�[�Q�b�g�ɂ͂Ȃ�Ȃ��B���ցB
					continue;
				}

				// @todo
				// �G�l�~�[�ւ̎������A�����ɎՂ��Ă��Ȃ������ׂ鏈����ǉ�����B
				// ����OBB�̓����蔻��Ōv�Z����B


				// �S�Ă̏����𖞂����Ă��邽�߁A�^�[�Q�b�g�Ƃ���B
				m_targetRef = enemy;
				// �^�[�Q�b�g�������������߁A���[�v���I������B
				break;
			}

			return;
		}

		/**
		 * @brief �^�[�Q�b�g��Ԃ��p�����邩���ׂ�
		*/
		void CPlayerCatchEnemy::CheckContinueTarget()
		{
			// �v���C���[����^�[�Q�b�g�ւ̃x�N�g��
			Vector3 playerToTargetVec = m_targetRef->GetPosition() - m_gameState->GetPlayer().GetPosition();
			// �v���C���[����^�[�Q�b�g�܂ł̋���
			const float playerToTargetLen = playerToTargetVec.Length();
			if (playerToTargetLen >= kTargetMaxLength)
			{
				// ���������ȏ㗣��Ă�����A�^�[�Q�b�g�����������B
				m_targetRef = nullptr;
				// ���������̂ŁA�������^�[��
				return;
			}

			// �J��������^�[�Q�b�g�ւ̃x�N�g��
			Vector3 cameraToTargetVec =
				m_targetRef->GetPosition() - m_gameState->GetPlayer().GetCamera().GetCameraPosition();
			// �J��������^�[�Q�b�g�ւ̕���
			Vector3 cameraToTargetNorm = cameraToTargetVec;
			cameraToTargetNorm.Normalize();	// ���K������
			// �J��������^�[�Q�b�g�ւ̕����ƁA�J�����̑O�����́A�p�x��
			float angleDiff =
				Dot(cameraToTargetNorm, m_gameState->GetPlayer().GetCamera().GetCameraForward());
			angleDiff = acosf(angleDiff);	// ���ς���p�x�֕ϊ�

			if (angleDiff >= kTargetMaxAngle)
			{
				// �p�x�������ȏ�̑傫���Ȃ�A�^�[�Q�b�g�����������B
				m_targetRef = nullptr;
				// ���������̂ŁA�������^�[��
				return;
			}

			return;
		}


		/**
		 * @brief ���̃N���X�Ŏg�����ʃf�[�^�̍X�V
		*/
		void CPlayerCatchEnemy::UpdateCommonData()
		{
			if (m_targetRef == nullptr)
			{
				// �^�[�Q�b�g���������Ă��Ȃ��ꍇ�́A����ȉ��̏����͍s��Ȃ��B
				return;
			}

			// �v���C���[����A�^�[�Q�b�g�ւ̋������v�Z����
			Vector3 playerToDiff = m_targetRef->GetPosition() - m_playerRef->GetPosition();
			m_targetLength = playerToDiff.Length();

			return;
		}

		/**
		 * @brief �G��߂܂��邩�ǂ������ׂ�
		*/
		void CPlayerCatchEnemy::CheckCatchEnemy()
		{
			if (m_targetRef == nullptr ||
				m_targetLength > kCanCatchMaxLength)
			{
				// �^�[�Q�b�g���������Ă��Ȃ��A�܂��́A
				// �^�[�Q�b�g�Ƃ̋��������ȉ��ł͂Ȃ��Ƃ�
				
				// �G��߂܂��邱�Ƃ��ł��Ȃ��B�������^�[���B
				return;
			}

			// �߂܂��邱�Ƃ��ł���

			if (m_playerRef->GetInputData().actionCatchEnemy)
			{
				// �G��߂܂�����͏�񂪂���
				// �G�̏�ɏ���Ԃ֑J�ڂ���
				m_playerRef->ChangeOnEnemyState();
			}

			return;
		}


	}
}
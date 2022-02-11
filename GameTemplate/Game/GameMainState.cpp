#include "stdafx.h"
#include "GameMainState.h"
#include "GameMainUI.h"
#include "GameTime.h"
#include "Player.h"
#include "AICar.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �Q�[���X�e�[�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGameState
	{
		using namespace nsGameMainStateConstData;
		CGameMainState* CGameMainState::m_instance = nullptr;


		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CGameMainState::Start()
		{
			if (m_playerRef->IsTitleMode() != true)
			{
				m_gameMainUI = NewGO<nsUI::CGameMainUI>(nsCommonData::enPriorityFirst);
			}

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CGameMainState::OnDestroy()
		{
			DeleteGO(m_gameMainUI);
			m_instance = nullptr;

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CGameMainState::Update()
		{
			switch (m_gameMainStateState)
			{
			case enGS_startDirecting:
				break;
			case enGS_inGame:
				// �Q�[���̃^�C�����v������
				TimeGame();
				break;
			case enGS_beforeClearDirecting:
				m_directingTimer += nsTimer::GameTime().GetFrameDeltaTime();
				if (m_directingTimer >= kStartClearDirectingTime)
				{
					ChangeState(enGS_startFadeOutToClearDirecting);
				}
				break;
			case enGS_startFadeOutToClearDirecting:
				break;
			case enGS_fadeOutToClearDirecting:
				break;
			case enGS_clearDirecting:
				m_directingTimer += nsTimer::GameTime().GetFrameDeltaTime();

				if (m_directingTimer >= 2.0f)
				{
					ChangeState(enGS_result);
				}
				break;
			case enGS_result:
				m_directingTimer += nsTimer::GameTime().GetFrameDeltaTime();

				if (m_directingTimer >= 2.0f)
				{
					ChangeState(enGS_lastJump);
				}
				break;
			case enGS_lastJump:
				break;
			case enGS_goTitle:
				break;
			}

			return;
		}


		/**
		 * @brief �Q�[���̃^�C�����v������
		*/
		void CGameMainState::TimeGame()
		{
			// �Q�[���̃^�C�����v��Ȃ����H
			if (m_isTimeGame != true)
			{
				// �v��Ȃ��B�������^�[���B
				return;
			}

			// �X�g�b�v�E�H�b�`���~�߂�
			m_stopWatch.Stop();
			// �^�C�}�[��i�߂�
			m_gameTimer += static_cast<float>(m_stopWatch.GetElapsed());
			// �X�g�b�v�E�H�b�`���ĊJ����
			m_stopWatch.Start();

			// UI�̃^�C�}�[�̃e�L�X�g��ݒ肷��
			m_gameMainUI->SetTimerText(m_gameTimer);

			return;
		}

		/**
		 * @brief �Ԃ�ǉ�����
		 * @param[in,out] aiCar �ǉ������
		*/
		void CGameMainState::AddAICar(nsAICharacter::CAICar* aiCar)
		{

			m_aiCarsRef.emplace_back(aiCar);

			return;
		}

		/**
		 * @brief �~�b�V��������N���A����
		 * @param[in] missionType �N���A����~�b�V�����̃^�C�v
		*/
		void CGameMainState::ClearOneMission(const nsUI::nsMissionUIConstData::EnMissionType missionType)
		{
			if (missionType < 0 || missionType >= nsUI::nsMissionUIConstData::enMissionTypeNum)
			{
				// �͈͊O�B�������^�[���B
				return;
			}

			// �N���A�����t���O�����Ă�
			m_missionClearFlag[missionType] = true;

			// �N���A�J�E���^�[�����Z����
			m_clearCounter++;

			if (m_clearCounter >= nsAICharacter::CAICar::GetCarTotalNumber())
			{
				// ���C���~�b�V����������
				ChangeState(enGS_beforeClearDirecting);
			}

			return;
		}

		/**
		 * @brief �~�b�V������\������
		*/
		void CGameMainState::ShowMission()
		{
			m_gameMainUI->ShowMission();
			return;
		}


		/**
		 * @brief �X�e�[�g��J��
		 * @param[in] newState �V�����X�e�[�g
		*/
		void CGameMainState::ChangeState(const nsGameMainStateConstData::EnGameMainStateState newState)
		{
			if (m_gameMainStateState == newState)
			{
				return;
			}

			m_gameMainStateState = newState;

			m_directingTimer = 0.0f;
			switch (m_gameMainStateState)
			{
			case enGS_startDirecting:
				break;
			case enGS_inGame:
				// �Q�[���̃^�C���̌v���J�n
				StartTimingGame();
				break;
			case enGS_beforeClearDirecting:
				// �Q�[���̃^�C���̌v���I��
				StopTimingGame();
				break;
			case enGS_clearDirecting:
				// UI���B��
				m_gameMainUI->HideUI();
				break;
			case enGS_goTitle:
				break;
			}
		}

	}
}
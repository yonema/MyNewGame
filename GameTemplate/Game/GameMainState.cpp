#include "stdafx.h"
#include "GameMainState.h"
#include "GameMainUI.h"
#include "GameTime.h"

namespace nsMyGame
{
	/**
	 * @brief �Q�[���X�e�[�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGameState
	{
		CGameMainState* CGameMainState::m_instance = nullptr;


		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CGameMainState::Start()
		{
			m_gameMainUI = NewGO<nsUI::CGameMainUI>(nsCommonData::enPriorityFirst);


			return true;
		}

		/**
		 * @brief �X�V����
		*/
		void CGameMainState::OnDestroy()
		{
			DeleteGO(m_gameMainUI);

			return;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CGameMainState::Update()
		{
			// �Q�[���̃^�C�����v������
			TimeGame();

			return;
		}

		/**
		 * @brief �S�[������
		*/
		void CGameMainState::Goal()
		{
			m_isGoal = true;
			// �^�C�}�[�̌v�����I������
			StopTimingGame();
			// UI�ɃS�[����`����
			m_gameMainUI->Goal();
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
	}
}
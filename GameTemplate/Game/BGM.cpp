#include "stdafx.h"
#include "BGM.h"
#include "Player.h"
#include "SoundCue.h"
#include "GameTime.h"

namespace nsMyGame
{
	namespace nsBGM
	{
		using namespace nsBGMConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CBGM::Start()
		{

			for (int i = 0; i < enBGMTypeNum; i++)
			{
				m_bgmSC[i] = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_bgmSC[i]->Init(kBGMSoundFilePaht[i], nsSound::CSoundCue::enBGM);
				m_bgmSC[i]->SetVolume(0.0f);
				m_bgmSC[i]->Play(true);
			}

			m_currentBGMType = enBT_Normal;
			m_bgmState = enBT_Normal;
			m_bgmSC[m_currentBGMType]->SetVolume(kBGMSoundVolume[m_currentBGMType]);

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CBGM::OnDestroy()
		{
			for (int i = 0; i < enBGMTypeNum; i++)
			{
				DeleteGO(m_bgmSC[i]);
			}

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CBGM::Update()
		{
			// BGM�̃X�e�[�g���X�V����
			UpdateBGMState();

			// �N���X�t�F�[�h���X�V
			UpdateCrossFade();

			return;
		}

		/**
		 * @brief ������
		 * @param[in] player �v���C���[�̎Q��
		*/
		void CBGM::Init(const nsPlayer::CPlayer& player)
		{
			m_playerRef = &player;

			return;
		}

		/**
		 * @brief �N���X�t�F�[�h��ݒ肷��
		 * @param[in] bgmType ����BGM�̎��
		*/
		void CBGM::SetCrossFade(const nsBGMConstData::EnBGMType bgmType)
		{
			m_nextBGMType = bgmType;
			m_crossFadeTimer = 0.0f;
			m_isCrossFade = true;
			return;
		}

		/**
		 * @brief BGM�̃X�e�[�g���X�V����
		*/
		void CBGM::UpdateBGMState()
		{
			if (m_playerRef->GetState() == nsPlayer::nsPlayerConstData::enSwing)
			{
				ChangeState(enBT_Swing);
			}
			else
			{
				ChangeState(enBT_Normal);
			}
			return;
		}

		/**
		 * @brief �N���X�t�F�[�h���X�V
		*/
		void CBGM::UpdateCrossFade()
		{
			if (m_isCrossFade != true)
			{
				return;
			}

			if (m_crossFadeTimer >= kCrossFadeTime)
			{
				m_isCrossFade = false;
				m_bgmSC[m_currentBGMType]->SetVolume(0.0f);
				m_bgmSC[m_nextBGMType]->SetVolume(1.0f);

				m_currentBGMType = m_nextBGMType;
			}

			const float rate = m_crossFadeTimer / kCrossFadeTime;

			float volume = Math::Lerp<float>(rate, kBGMSoundVolume[m_currentBGMType], 0.0f);
			m_bgmSC[m_currentBGMType]->SetVolume(volume);

			volume = Math::Lerp<float>(rate, 0.0f, kBGMSoundVolume[m_nextBGMType]);
			m_bgmSC[m_nextBGMType]->SetVolume(volume);

			m_crossFadeTimer += nsTimer::GameTime().GetFrameDeltaTime();

			return;
		}

		/**
		 * @brief �X�e�[�g�J��
		 * @param[in] newState �V�����X�e�[�g
		*/
		void CBGM::ChangeState(const nsBGMConstData::EnBGMType newState)
		{
			if (m_bgmState == newState)
			{
				return;
			}

			m_bgmState = newState;

			SetCrossFade(m_bgmState);

			switch (m_bgmState)
			{
			case enBT_Normal:
				break;
			case enBT_Swing:
				break;
			}

		}

	}
}
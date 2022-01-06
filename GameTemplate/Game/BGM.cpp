#include "stdafx.h"
#include "BGM.h"
#include "Player.h"
#include "SoundCue.h"
#include "BGMConstData.h"

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
			m_swingBGMSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_swingBGMSC->Init(kSwingBGMSoundFilePath, nsSound::CSoundCue::enBGM);
			m_swingBGMSC->SetVolume(kSwingBGMSoundVolume);
			m_swingBGMSC->Play(true);
			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CBGM::OnDestroy()
		{
			DeleteGO(m_swingBGMSC);
			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CBGM::Update()
		{
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
	}
}
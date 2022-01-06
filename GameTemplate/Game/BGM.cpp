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
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
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
		 * @brief 消去される時に呼ばれる処理
		*/
		void CBGM::OnDestroy()
		{
			DeleteGO(m_swingBGMSC);
			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CBGM::Update()
		{
			return;
		}

		/**
		 * @brief 初期化
		 * @param[in] player プレイヤーの参照
		*/
		void CBGM::Init(const nsPlayer::CPlayer& player)
		{
			m_playerRef = &player;

			return;
		}
	}
}
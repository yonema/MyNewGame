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
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
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
		 * @brief 消去される時に呼ばれる処理
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
		 * @brief 更新処理
		*/
		void CBGM::Update()
		{
			// BGMのステートを更新する
			UpdateBGMState();

			// クロスフェードを更新
			UpdateCrossFade();

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

		/**
		 * @brief クロスフェードを設定する
		 * @param[in] bgmType 次のBGMの種類
		*/
		void CBGM::SetCrossFade(const nsBGMConstData::EnBGMType bgmType)
		{
			m_nextBGMType = bgmType;
			m_crossFadeTimer = 0.0f;
			m_isCrossFade = true;
			return;
		}

		/**
		 * @brief BGMのステートを更新する
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
		 * @brief クロスフェードを更新
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
		 * @brief ステート遷移
		 * @param[in] newState 新しいステート
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
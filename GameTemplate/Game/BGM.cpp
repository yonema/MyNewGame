#include "stdafx.h"
#include "BGM.h"
#include "Player.h"
#include "SoundCue.h"
#include "GameTime.h"

namespace nsNinjaAttract
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
				// 各BGMを生成して初期化
				m_bgmSC[i] = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_bgmSC[i]->Init(kBGMSoundFilePath[i], nsSound::CSoundCue::enBGM);
				// 音量を0にして再生しておく
				m_bgmSC[i]->SetVolume(0.0f);
				m_bgmSC[i]->Play(true);
			}

			// 現在のBGMとステートをノーマルにしておく
			m_currentBGMType = enBT_Normal;
			m_bgmState = enBT_Normal;
			m_bgmSC[m_currentBGMType]->SetVolume(kBGMSoundVolume[m_currentBGMType]);

			m_resultSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_resultSC->Init(kResultSoundFilePath, nsSound::CSoundCue::enSE);

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
			DeleteGO(m_resultSC);
			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CBGM::Update()
		{
			if (m_isResultSound)
			{
				// リザルトサウンド中は、更新しない。早期リターン。
				return;
			}
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
		 * @brief リザルトのサウンドの再生
		*/
		void CBGM::PlayResultSound()
		{
			// リザルトのサウンドをワンショット再生
			m_resultSC->Play(false);
			// リザルトのサウンドの音量を設定
			m_resultSC->SetVolume(kResultSoundVolume);

			// リザルトサウンド中にする
			m_isResultSound = true;

			// リザルトサウンド中のBGMをループ再生再生
			m_bgmSC[enBT_Normal]->Play(true);
			// リザアルトサウンド中のBGMの音量を設定
			m_bgmSC[enBT_Normal]->SetVolume(kBGMSoundVolume[m_currentBGMType] * kResultBGMVolume);

			return;
		}

		/**
		 * @brief リザルトのサウンドの音量を設定
		 * @param[in] volume 音量
		*/
		void CBGM::SetResultSoundVolume(const float volume)
		{
			m_bgmSC[enBT_Normal]->SetVolume(kBGMSoundVolume[m_currentBGMType] * kResultBGMVolume * volume);

			return;
		}

		/**
		 * @brief クロスフェードを設定する
		 * @param[in] bgmType 次のBGMの種類
		*/
		void CBGM::SetCrossFade(const nsBGMConstData::EnBGMType bgmType)
		{
			if (m_currentBGMType == bgmType)
			{
				m_currentBGMType = m_nextBGMType;
			}
			// 次のBGMを設定
			m_nextBGMType = bgmType;
			// クロスフェードのタイマーとタイムを初期化する
			m_crossFadeTimer = 0.0f;
			m_crossFadeTime = kCrossFadeTime[m_nextBGMType];
			// クロスフェード、オン
			m_isCrossFade = true;

			for (int i = 0; i < enBGMTypeNum; i++)
			{
				// クロスフェードに使うBGM以外は音量0にする
				if (i != m_currentBGMType &&
					i != m_nextBGMType)
				{
					m_bgmSC[i]->SetVolume(0.0f);
				}
			}

			return;
		}

		/**
		 * @brief BGMのステートを更新する
		*/
		void CBGM::UpdateBGMState()
		{
			if (m_playerRef->GetState() == nsPlayer::nsPlayerConstData::enSwing)
			{
				// スイング状態
				ChangeState(enBT_Swing);
			}
			else if (m_playerRef->GetState() == nsPlayer::nsPlayerConstData::enOnEnemy)
			{
				// QTE状態
				ChangeState(enBT_qte);
			}
			else if (m_bgmState == enBT_Swing &&
				m_playerRef->GetPlayerMovement().IsAir())
			{
				// スイング中に、空中でスイング中ではなくなっても、地面に降りるまでは
				// ノーマルに戻らず、スイング状態を維持するためのif
			}
			else
			{
				// ノーマル状態
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
				// クロスフェードしない。早期リターン。
				return;
			}

			if (m_crossFadeTimer >= m_crossFadeTime)
			{
				// タイマーがタイムをオーバーしたら終了

				// クロスフェード、オフ。
				m_isCrossFade = false;
				// 現在のBGMの音量と次のBGMの音量を完全に切り替える
				m_bgmSC[m_currentBGMType]->SetVolume(0.0f);
				m_bgmSC[m_nextBGMType]->SetVolume(1.0f);

				// 次のBGMを現在のBGMにする
				m_currentBGMType = m_nextBGMType;
			}

			// タイマーの進んでいる率
			const float rate = m_crossFadeTimer / m_crossFadeTime;

			// 指数関数的変化にさせる。音が大きい状態が長くなるようにする。
			float t = pow(rate, 2.0f);	// 補完率
			// 音量を線形補完
			float volume = Math::Lerp<float>(t, kBGMSoundVolume[m_currentBGMType], 0.0f);
			// 現在のBGMの音量を設定
			m_bgmSC[m_currentBGMType]->SetVolume(volume);

			// 指数関数的変化にさせる。音が大きい状態が長くなるようにする。
			t = 1.0f - pow((1.0f - rate), 2.0f);
			// 音量を線形補完
			volume = Math::Lerp<float>(rate, 0.0f, kBGMSoundVolume[m_nextBGMType]);
			// 次のBGMの音量を設定
			m_bgmSC[m_nextBGMType]->SetVolume(volume);

			// タイマーを進める
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

			// ステートが遷移したら、クロスフェードを行う
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
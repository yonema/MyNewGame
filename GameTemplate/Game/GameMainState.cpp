#include "stdafx.h"
#include "GameMainState.h"
#include "GameMainUI.h"
#include "GameTime.h"
#include "Player.h"
#include "AICar.h"

namespace nsNinjaAttract
{
	/**
	 * @brief ゲームステート関連のネームスペース
	*/
	namespace nsGameState
	{
		using namespace nsGameMainStateConstData;
		CGameMainState* CGameMainState::m_instance = nullptr;


		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
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
		 * @brief 消去される時に呼ばれる処理
		*/
		void CGameMainState::OnDestroy()
		{
			DeleteGO(m_gameMainUI);
			m_instance = nullptr;

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CGameMainState::Update()
		{
			switch (m_gameMainStateState)
			{
			case enGS_startDirecting:
				break;
			case enGS_inGame:
				// ゲームのタイムを計測する
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
		 * @brief ゲームのタイムを計測する
		*/
		void CGameMainState::TimeGame()
		{
			// ゲームのタイムを計らないか？
			if (m_isTimeGame != true)
			{
				// 計らない。早期リターン。
				return;
			}

			// ストップウォッチを止める
			m_stopWatch.Stop();
			// タイマーを進める
			m_gameTimer += static_cast<float>(m_stopWatch.GetElapsed());
			// ストップウォッチを再開する
			m_stopWatch.Start();

			// UIのタイマーのテキストを設定する
			m_gameMainUI->SetTimerText(m_gameTimer);

			return;
		}

		/**
		 * @brief 車を追加する
		 * @param[in,out] aiCar 追加する車
		*/
		void CGameMainState::AddAICar(nsAICharacter::CAICar* aiCar)
		{

			m_aiCarsRef.emplace_back(aiCar);

			return;
		}

		/**
		 * @brief ミッションを一つクリアする
		 * @param[in] missionType クリアするミッションのタイプ
		*/
		void CGameMainState::ClearOneMission(const nsUI::nsMissionUIConstData::EnMissionType missionType)
		{
			if (missionType < 0 || missionType >= nsUI::nsMissionUIConstData::enMissionTypeNum)
			{
				// 範囲外。早期リターン。
				return;
			}

			// クリアしたフラグをたてる
			m_missionClearFlag[missionType] = true;

			// クリアカウンターを加算する
			m_clearCounter++;

			if (m_clearCounter >= nsAICharacter::CAICar::GetCarTotalNumber())
			{
				// メインミッションを完了
				ChangeState(enGS_beforeClearDirecting);
			}

			return;
		}

		/**
		 * @brief ミッションを表示する
		*/
		void CGameMainState::ShowMission()
		{
			m_gameMainUI->ShowMission();
			return;
		}


		/**
		 * @brief ステートを遷移
		 * @param[in] newState 新しいステート
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
				// ゲームのタイムの計測開始
				StartTimingGame();
				break;
			case enGS_beforeClearDirecting:
				// ゲームのタイムの計測終了
				StopTimingGame();
				break;
			case enGS_clearDirecting:
				// UIを隠す
				m_gameMainUI->HideUI();
				break;
			case enGS_goTitle:
				break;
			}
		}

	}
}
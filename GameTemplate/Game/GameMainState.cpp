#include "stdafx.h"
#include "GameMainState.h"
#include "GameMainUI.h"
#include "GameTime.h"

namespace nsMyGame
{
	/**
	 * @brief ゲームステート関連のネームスペース
	*/
	namespace nsGameState
	{
		CGameMainState* CGameMainState::m_instance = nullptr;


		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CGameMainState::Start()
		{
			m_gameMainUI = NewGO<nsUI::CGameMainUI>(nsCommonData::enPriorityFirst);


			return true;
		}

		/**
		 * @brief 更新処理
		*/
		void CGameMainState::OnDestroy()
		{
			DeleteGO(m_gameMainUI);

			return;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CGameMainState::Update()
		{
			// ゲームのタイムを計測する
			TimeGame();

			return;
		}

		/**
		 * @brief ゴールした
		*/
		void CGameMainState::Goal()
		{
			m_isGoal = true;
			// タイマーの計測を終了する
			StopTimingGame();
			// UIにゴールを伝える
			m_gameMainUI->Goal();
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
	}
}
#include "stdafx.h"
#include "GameMainUI.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "MiniMap.h"
#include "EnemyCatchUI.h"
#include "MissionUI.h"
#include "UIConstData.h"

namespace nsNinjaAttract
{
	/**
	 * @brief UI関連のネームスペース
	*/
	namespace nsUI
	{
		// ゲームメインの定数データを使用可能にする
		using namespace nsGameMainUIConstData;

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CGameMainUI::Start()
		{
			// タイマーのフォントを初期化
			InitTimerFont();

			// ゴールのスプライトの初期化
			InitGoalSprite();

			m_miniMap = NewGO<CMiniMap>(nsCommonData::enPriorityFirst);

			m_enemyCatchUI = NewGO<CEnemyCatchUI>(nsCommonData::enPriorityFirst);

			m_missionUI = NewGO<CMissionUI>(nsCommonData::enPriorityFirst);

			return true;
		}

		/**
		 * @brief 更新処理
		*/
		void CGameMainUI::OnDestroy()
		{
			DeleteGO(m_timerFR);

			DeleteGO(m_goalSR);

			DeleteGO(m_miniMap);

			DeleteGO(m_enemyCatchUI);

			DeleteGO(m_missionUI);

			return;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CGameMainUI::Update()
		{
			return;
		}

		/**
		 * @brief タイマーのテキストを設定する
		 * @param[in] time タイム
		*/
		void CGameMainUI::SetTimerText(const float time)
		{
			// 分
			int min = static_cast<int>(time) / 60;
			// 秒
			int sec = static_cast<int>(time) - 60 * min;
			// テキスト
			wchar_t text[64];
			// タイム部分のテキスト
			wchar_t timeText[64];

			// テキストを設定
			swprintf_s(text, kTimerFontTextHead);
			swprintf_s(timeText, kTimerFontTextEnd, min, sec);
			wcscat(text, timeText);	

			// フォントレンダラーにテキストをセット
			m_timerFR->SetText(text);

			return;
		}

		/**
		 * @brief ゴールした
		*/
		void CGameMainUI::Goal()
		{
			// ゴールのスプライトの有効化
			m_goalSR->Activate();

			return;
		}

		/**
		 * @brief タイマーのフォントを初期化
		*/
		void CGameMainUI::InitTimerFont()
		{
			// タイマーのフォントを生成
			m_timerFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFirst);
			// パラメータを設定
			m_timerFR->SetParam(
				kTimerFontTextHead,		// テキスト
				kTimerFontPosition,		// 座標
				kTimerFontColor			// カラー
			);
			// シャドウパラメータを設定
			m_timerFR->SetShadowParam(true, kTimerFontShadowOffset, kTimerFontShadowColor);
			m_timerFR->Deactivate();
			return;
		}

		/**
		 * @brief ゴールのスプライトの初期化
		*/
		void CGameMainUI::InitGoalSprite()
		{
			// ゴールのスプライトの生成
			m_goalSR = NewGO <nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFirst);
			// 座標を設定
			m_goalSR->SetPosition(kGoalSpritePosition);
			// 初期化
			m_goalSR->Init(kGoalSpriteFilePath, kGoalSpriteWidth, kGoalSpriteHeight);
			// 無効化
			m_goalSR->Deactivate();

			return;
		}

		/**
		 * @brief ミッションを表示する
		*/
		void CGameMainUI::ShowMission()
		{
			m_missionUI->ShowMission();

			return;
		}

		/**
		 * @brief UIを隠す
		*/
		void CGameMainUI::HideUI()
		{
			m_miniMap->HideUI();

			return;
		}
	}
}
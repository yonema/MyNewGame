#include "stdafx.h"
#include "MissionUI.h"
#include "SpriteRender.h"
#include "SpriteRenderConstData.h"
#include "FontRender.h"
#include "../../ExEngine/level2D/Level2D.h"
#include "GameTime.h"
#include "GameMainState.h"
#include "GameMainStateConstData.h"

namespace nsNinjaAttract
{
	/**
	 * @brief UI関連のネームスペース
	*/
	namespace nsUI
	{
		using namespace nsMissionUIConstData;

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CMissionUI::Start()
		{
			// ゲームステートの参照を引っ張ってくる
			m_gameState = FindGO<nsGameState::CGameMainState>(
				nsCommonData::kGameObjectName[nsCommonData::enGN_MainGameState]
				);

			// スプライトの初期化
			InitSprite();

			// フォントの初期化
			InitFont();

			// クリアフラグをチェックするステートへ遷移
			ChangeState(enMS_checkClearFlag);

			return true;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CMissionUI::OnDestroy()
		{
			// スプライトの破棄
			QueryAllMissionSRs(
				[](nsGraphic::nsSprite::CSpriteRender* spriteRender) 
				{
					DeleteGO(spriteRender);
				}
			);

			// フォントんの破棄
			DeleteGO(m_clearTimeFR);
			DeleteGO(m_numOfMissFR);

			return;
		}
		
		/**
		 * @brief ミッションを表示する
		*/
		void CMissionUI::ShowMission()
		{
			ChangeState(enMS_showMission);
			return;
		}

		/**
		 * @brief ミッションを一つクリアした時の処理
		 * @param[in] missionType クリアしたミッションのタイプ
		*/
		void CMissionUI::ClearOneMission(const nsMissionUIConstData::EnMissionType missionType)
		{
			m_currentClearMissionType = missionType;

			ChangeState(enMS_clearOneMission);

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CMissionUI::Update()
		{
			if (nsGameState::CGameMainState::GetInstance()->GetGameMainStateState() ==
				nsGameState::nsGameMainStateConstData::enGS_result)
			{
				ChangeState(enMS_result);
			}
			switch (m_missionState)
			{
			case enMS_none:
				break;
			case enMS_checkClearFlag:
				// クリアフラグをチェックする処理を更新
				UpdateCheckClearFlag();
				break;
			case enMS_showMission:
				// ミッションを表示する処理を更新
				UpdataShowMission(kMissionStartPosition, kMissionPosition);
				break;
			case enMS_hideMission:
				// ミッションを非表示にする処理を更新
				UpdateHideMission();
				break;
			case enMS_clearOneMission:
				// ミッションを表示する処理を更新
				UpdataShowMission(kMissionStartPosition, kMissionPosition);
				// ミッションを一つクリアした時の処理の更新
				UpdateClearOneMission();
				break;
			case enMS_result:
				// リザルトの時の処理の更新
				UpdateResult();
				break;
			}
			return;
		}

		/**
		 * @brief スプライトの初期化
		*/
		void CMissionUI::InitSprite()
		{
			// レベル2Dのインスタンスを生成
			m_missionLevel = new Level2D;

			m_missionLevel->Init(
				kMissionLevel2DFilePath, 
				[&](Level2DObjectData& objData)->bool
				{
					if (objData.EqualObjectName(kMissionLevelObjName))
					{
						// ミッションウィンドウのスプライトの生成と初期化
						m_missionWindowSR = 
							NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird + objData.numOfLyaer);
						m_missionWindowSR->Init(
							kMissionSpriteFilePath,
							objData.width,
							objData.height,
							nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
							AlphaBlendMode_Trans
						);
						// 座標を設定
						m_missionWindowSR->SetPosition(objData.position + kMissionStartPosition);

						return true;
					}

					for (int missionType = 0; missionType < enMissionTypeNum; missionType++)
					{
						// 各ミッションの種類全てを調べる
						if (objData.EqualObjectName(kCheckMarkLevelObjNames[missionType]))
						{
							m_checkMarkSRs[missionType] = 
								NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird + objData.numOfLyaer);
							
							// チェックマークの初期化
							m_checkMarkSRs[missionType]->Init(
								kCheckMarkSpriteFilePath,
								objData.width * objData.scale.x,
								objData.height * objData.scale.y,
								nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
								AlphaBlendMode_Trans
							);

							// オフセットを格納
							m_checkMarkOffsets[missionType] = objData.position;
							// クリアフラグをセット
							m_checkMarkFlag[missionType] = false;

							// 座標を設定
							m_checkMarkSRs[missionType]->SetPosition(m_checkMarkOffsets[missionType] + kMissionPosition);

							return true;
						}
					}

					// マップチップで表示する予定がないため、すべてtrueを戻す。
					return true;
				});

			// もう使用しないので、インスタンスを破棄。
			delete m_missionLevel;

			// ミッションリザルトの枠のスプライトの生成と初期化
			m_missionResultFrameSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFourth);
			m_missionResultFrameSR->Init(
				kMissionResultFrameFilePath,
				kMissionResultFrameSpriteWidth,
				kMissionResultFrameSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			m_missionResultFrameSR->SetAlphaValue(0.0f);
			m_missionResultFrameSR->SetPosition(kMissionResultPosition);

			// ミッションリザルトのテキストのスプライトの生成と初期化
			for (int mrt = 0; mrt < enMissionResultTypeNum; mrt++)
			{
				m_missionResultTextSRs[mrt] = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFourth);
				m_missionResultTextSRs[mrt]->Init(
					kMissionResultTextSpriteFilepath[mrt],
					kMissionResultTextSpriteWidth,
					kMissionResultTextSpriteHeight,
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
				m_missionResultTextSRs[mrt]->SetPosition(kMissionResultPosition);
			}

			// 全てのスプライトを非表示
			QueryAllMissionSRs(
				[](nsGraphic::nsSprite::CSpriteRender* spriteRender)
				{
					spriteRender->Deactivate();
					spriteRender->SetAlphaValue(0.0f);
					return;
				}
			);

			return;
		}

		/**
		 * @brief フォントの初期化
		*/
		void CMissionUI::InitFont()
		{
			m_clearTimeFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFifth);
			m_clearTimeFR->SetParam(
				L"01:00",
				{ 100.0f,-20.0f },
				Vector4::White,
				0.0,
				0.5f
			);
			m_clearTimeFR->Deactivate();

			m_numOfMissFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFifth);
			m_numOfMissFR->SetParam(
				L"0",
				{ 100.0f,-55.0f },
				Vector4::White,
				0.0,
				0.5f
			);
			m_numOfMissFR->Deactivate();


			return;
		}


		/**
		 * @brief クリアフラグをチェックする処理を更新
		*/
		void CMissionUI::UpdateCheckClearFlag()
		{
			// ステートから、現在のミッションクリア情報を取ってくる。
			const bool* clearFlag = m_gameState->GetClearFlag();

			for (int missionType = 0; missionType < enMissionTypeNum; missionType++)
			{
				if (m_checkMarkFlag[missionType] == clearFlag[missionType])
				{
					// 差異がないフラグはスキップ
					continue;
				}

				// 差異があるフラグは、新しくクリアしたミッションのため、クリア処理へ。
				ClearOneMission(static_cast<EnMissionType>(missionType));

				// クリアできるのは一度に一つまで
				break;
			}

			return;
		}

		/**
		 * @brief ミッションを表示する処理を更新
		 * @param[in] startPos 表示アニメーションの開始座標
		 * @param[in] endPos 表示アニメーションの終了座標
		*/
		void CMissionUI::UpdataShowMission(const Vector3& startPos, const Vector3& endPos)
		{
			// タイマーを進める
			m_timer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_timer <= kStartShowMissionTime)
			{
				const float rate = m_timer / kStartShowMissionTime;
				Vector3 pos = Vector3::Zero;
				pos.Lerp(rate, startPos, endPos);

				m_missionWindowSR->SetPosition(pos);
				m_missionWindowSR->SetAlphaValue(rate);

				for (int i = 0; i < enMissionTypeNum ; i++)
				{
					if (m_checkMarkFlag[i] != true)
					{
						// クリアしてないミッションは更新しない
						continue;
					}
					m_checkMarkSRs[i]->SetPosition(pos + m_checkMarkOffsets[i]);
					m_checkMarkSRs[i]->SetAlphaValue(rate);
				}
				m_missionResultFrameSR->SetPosition(pos);
				m_missionResultFrameSR->SetAlphaValue(rate);

				return;
			}
			else if (m_timer <= kShowMissionTime)
			{
				m_missionWindowSR->SetPosition(endPos);
				m_missionWindowSR->SetAlphaValue(1.0f);
				for (int i = 0; i < enMissionTypeNum; i++)
				{
					if (m_checkMarkFlag[i] != true)
					{
						// クリアしてないミッションは更新しない
						continue;
					}
					m_checkMarkSRs[i]->SetPosition(endPos + m_checkMarkOffsets[i]);
					m_checkMarkSRs[i]->SetAlphaValue(1.0f);
				}
				m_missionResultFrameSR->SetPosition(endPos);
				m_missionResultFrameSR->SetAlphaValue(1.0f);
				return;
			}
			else
			{
				ChangeState(enMS_hideMission);
				return;
			}

			return;
		}

		/**
		 * @brief ミッションを非表示にする処理を更新
		*/
		void CMissionUI::UpdateHideMission()
		{
			// タイマーを進める
			m_timer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_timer <= kHideMissionTime)
			{
				const float rate = m_timer / kHideMissionTime;
				Vector3 pos = Vector3::Zero;
				pos.Lerp(rate, kMissionPosition, kMissionStartPosition);

				const float alphaValue = 1.0f - rate;

				m_missionWindowSR->SetPosition(pos);
				m_missionWindowSR->SetAlphaValue(alphaValue);

				for (int i = 0; i < enMissionTypeNum; i++)
				{
					if (m_checkMarkFlag[i] != true)
					{
						// クリアしてないミッションは更新しない
						continue;
					}
					m_checkMarkSRs[i]->SetPosition(pos + m_checkMarkOffsets[i]);
					m_checkMarkSRs[i]->SetAlphaValue(alphaValue);
				}
				m_missionResultFrameSR->SetPosition(pos);
				m_missionResultFrameSR->SetAlphaValue(alphaValue);
				return;
			}
			else
			{
				m_missionWindowSR->SetPosition(kMissionStartPosition);
				m_missionWindowSR->SetAlphaValue(0.0f);
				m_missionWindowSR->Deactivate();

				for (int i = 0; i < enMissionTypeNum; i++)
				{
					if (m_checkMarkFlag[i] != true)
					{
						// クリアしてないミッションは更新しない
						continue;
					}
					m_checkMarkSRs[i]->SetPosition(kMissionStartPosition + m_checkMarkOffsets[i]);
					m_checkMarkSRs[i]->SetAlphaValue(0.0f);
				}
				m_missionResultFrameSR->SetPosition(kMissionStartPosition);
				m_missionResultFrameSR->SetAlphaValue(0.0f);
				ChangeState(enMS_checkClearFlag);
				return;
			}

			return;
		}

		/**
		 * @brief ミッションを一つクリアした時の処理の更新
		*/
		void CMissionUI::UpdateClearOneMission()
		{

			if (m_timer <= kWaitStartClearOneMissionTime)
			{
				if (m_timer <= 0.0f + FLT_EPSILON)
				{
					m_checkMarkFlag[m_currentClearMissionType] = true;
				}
				return;
			}
			else if (m_timer <= kInCheckMarkTime)
			{
				const float rate = (m_timer - kWaitStartClearOneMissionTime) / 
					(kInCheckMarkTime - kWaitStartClearOneMissionTime);
				const float scale = Math::Lerp<float>(rate, kInCheckMarkStartScale, kInCheckMarkEndScale);
				const float alphaValue = Math::Lerp<float>(rate, 0.0f, kInCheckMarkEndAlphaValue);
				m_checkMarkSRs[m_currentClearMissionType]->SetScale(scale);
				m_checkMarkSRs[m_currentClearMissionType]->SetAlphaValue(alphaValue);
				return;
			}
			else if (m_timer <= kScaleUpCheckMarkTime)
			{
				const float rate = 
					(m_timer - kInCheckMarkTime) / (kScaleUpCheckMarkTime - kInCheckMarkTime);
				const float scale = Math::Lerp<float>(rate, 1.0f, kScaleUpCheckMarkEndScale);
				m_checkMarkSRs[m_currentClearMissionType]->SetScale(scale);
				m_checkMarkSRs[m_currentClearMissionType]->SetAlphaValue(1.0f);
				return;
			}
			else if (m_timer <= kScaleuDownCheckMarkTime)
			{
				const float rate = 
					(m_timer - kScaleUpCheckMarkTime) / (kScaleuDownCheckMarkTime - kScaleUpCheckMarkTime);
				const float scale = Math::Lerp<float>(rate, kScaleUpCheckMarkEndScale, 1.0f);
				m_checkMarkSRs[m_currentClearMissionType]->SetScale(scale);
				return;
			}
			else if (m_timer <= kShowMissionTime)
			{
				m_checkMarkSRs[m_currentClearMissionType]->SetScale(1.0f);
				return;
			}

			return;
		}

		/**
		 * @brief リザルトの時の処理の更新
		*/
		void CMissionUI::UpdateResult()
		{
			float buff = 0.0f;
			switch (m_missionResultState)
			{
			case enMRS_showMission:
				UpdataShowMission(kMissionResultStartPosition, kMissionResultPosition);

				if (m_timer >= 1.0f)
				{
					m_missionResultState = enMRS_subMission;
					m_timer = 0.0f;
					// サブミッションをクリアしたかどうかを調べる
					CheckClearSubMission();
				}
				break;
			case enMRS_subMission:
				m_timer += nsTimer::GameTime().GetFrameDeltaTime();
				buff = m_timer;
				for (int i = enMT_clearTime; i < enMissionTypeNum; i++)
				{
					if (m_timer <= 0.5f * (i - enMT_clearTime))
					{
						continue;
					}

					if (i == enMT_clearTime)
					{
						m_clearTimeFR->Activate();
						m_clearTimeTimer += nsTimer::GameTime().GetFrameDeltaTime();
						m_timer = m_clearTimeTimer;
					}
					else
					{
						m_numOfMissFR->Activate();
						m_numOfMissTimer += nsTimer::GameTime().GetFrameDeltaTime();
						m_timer = m_numOfMissTimer;
					}

					if (m_checkMarkFlag[i] == true)
					{
						m_currentClearMissionType = static_cast<EnMissionType>(i);
						UpdateClearOneMission();

					}
					else
					{

					}

				}
				m_timer = buff;
				if (m_timer >= 2.0f)
				{
					m_missionResultState = enMRS_showResult;
					m_timer = 0.0f;
				}
				break;

			case enMRS_showResult:
				m_timer += nsTimer::GameTime().GetFrameDeltaTime();

				if (m_timer <= kStartShowMissionTime)
				{
					const float rate = m_timer / kStartShowMissionTime;
					Vector3 pos = Vector3::Zero;
					pos.Lerp(rate, kMissionResultStartPosition, kMissionResultPosition);

					for (auto& mrtSR : m_missionResultTextSRs)
					{
						mrtSR->SetPosition(pos);
						mrtSR->SetAlphaValue(rate);
					}

					return;
				}
				else
				{
					for (auto& mrtSR : m_missionResultTextSRs)
					{
						mrtSR->SetPosition(kMissionResultPosition);
						mrtSR->SetAlphaValue(1.0f);
					}
					return;
				}

				break;
			}

			return;
		}

		/**
		 * @brief サブミッションをクリアしたかどうかを調べる
		*/
		void CMissionUI::CheckClearSubMission()
		{
			int clearCount = 0;
			const int clearTime = static_cast<int>(nsGameState::CGameMainState::GetInstance()->GetGameTime());
			if (clearTime <= 120)
			{
				m_checkMarkFlag[enMT_clearTime] = true;
				clearCount++;
			}
			else
			{
				m_clearTimeFR->SetColor(Vector4::Red);
			}

			{
				const int min = clearTime / 60;
				const int sec = clearTime - (60 * min);
				wchar_t text[16];
				swprintf_s(text, L"%02d:%02d", min, sec);
				m_clearTimeFR->SetText(text);
			}

			const int numOfCommandMiss = nsGameState::CGameMainState::GetInstance()->GetNumOfCommandMiss();
			if (numOfCommandMiss <= 0)
			{
				m_checkMarkFlag[enMT_noCommandMiss] = true;
				clearCount++;
			}
			else
			{
				m_numOfMissFR->SetColor(Vector4::Red);
			}
			{
				wchar_t text[16];
				swprintf_s(text, L"%5d", numOfCommandMiss);
				m_numOfMissFR->SetText(text);
			}

			m_missionResultTextSRs[clearCount]->Activate();

			return;
		}

		/**
		 * @brief ミッションのスプライトレンダラー全てにクエリを行う
		 * @param[in] func 実行する関数
		*/
		void CMissionUI::QueryAllMissionSRs(
			const std::function<void(nsGraphic::nsSprite::CSpriteRender* spriteRender)> func
		)
		{
			for (auto& checkMarkSR : m_checkMarkSRs)
			{
				func(checkMarkSR);
			}
			func(m_missionWindowSR);
			func(m_missionResultFrameSR);
			for (auto& mrtSR : m_missionResultTextSRs)
			{
				func(mrtSR);
			}

			return;
		}

		/**
		 * @brief ステートを遷移する
		 * @param[in] newState 新しいステート
		*/
		void CMissionUI::ChangeState(nsMissionUIConstData::EnMissionState newState)
		{
			if (m_missionState == newState)
			{
				return;
			}

			m_missionState = newState;

			m_timer = 0.0f;

			switch (m_missionState)
			{
			case enMS_none:
				break;
			case enMS_checkClearFlag:
				break;
			case enMS_showMission:
				m_missionWindowSR->Activate();
				break;
			case enMS_hideMission:
				break;
			case enMS_clearOneMission:
				m_missionWindowSR->Activate();
				m_checkMarkSRs[m_currentClearMissionType]->Activate();
				break;
			case enMS_result:
				m_missionWindowSR->Activate();
				m_missionResultFrameSR->Activate();
				for (int i = enMT_clearTime; i < enMissionTypeNum; i++)
				{
					m_checkMarkSRs[i]->SetPosition(kMissionResultPosition + m_checkMarkOffsets[i]);
					m_checkMarkSRs[i]->Activate();
				}
				break;
			}

			return;
		}
	}
}
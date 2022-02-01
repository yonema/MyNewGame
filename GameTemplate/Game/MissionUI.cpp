#include "stdafx.h"
#include "MissionUI.h"
#include "SpriteRender.h"
#include "SpriteRenderConstData.h"
#include "FontRender.h"
#include "../../ExEngine/level2D/Level2D.h"
#include "GameTime.h"
#include "GameMainState.h"
#include "GameMainStateConstData.h"
#include "SoundCue.h"

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

			// サウンドの初期化
			InitSound();

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

			// フォントの破棄
			DeleteGO(m_clearTimeFR);
			DeleteGO(m_numOfMissFR);

			// サウンドの破棄
			DeleteGO(m_opneMission);
			DeleteGO(m_closeMission);
			DeleteGO(m_clearOneMission);
			DeleteGO(m_clearAllMission);
			DeleteGO(m_showResult);
			DeleteGO(m_resultPerfect);

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

			ChangeState(enMS_beforedClearOneMission);

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
				UpdateShowMission(kMissionStartPosition, kMissionPosition);
				break;
			case enMS_hideMission:
				// ミッションを非表示にする処理を更新
				UpdateHideMission();
				break;
			case enMS_beforedClearOneMission:
				m_timer += nsTimer::GameTime().GetFrameDeltaTime();
				if (m_timer >= kBeforeShowMission)
				{
					ChangeState(enMS_clearOneMission);
				}
				break;
			case enMS_clearOneMission:
				// ミッションを表示する処理を更新
				UpdateShowMission(kMissionStartPosition, kMissionPosition);
				// ミッションを一つクリアした時の処理の更新
				UpdateClearOneMission();
				break;
			case enMS_clearAllMission:
				// 全てのミッションをクリアした時の処理の更新
				UpdateClearAllMission();
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
					else if (objData.EqualObjectName(kMissionAllClearTextLevelObjName))
					{
						// ミッションをすべてクリアした時のテキストのスプライトの生成と初期化
						m_missionAllClearTextSR =
							NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird + objData.numOfLyaer);
						m_missionAllClearTextSR->Init(
							kMissionAllClearTextSpriteFilePath,
							objData.width,
							objData.height,
							nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
							AlphaBlendMode_Trans
						);
						m_missionAllClearTextSpriteOffset = objData.position;
						// 座標を設定
						m_missionAllClearTextSR->SetPosition(objData.position + kMissionStartPosition);

						return true;
					}
					else if (objData.EqualObjectName(kMissionAllClearFrameLevelObjName))
					{
						// ミッションをすべてクリアした時のフレームのスプライトの生成と初期化
						m_missionAllClearFrameSR =
							NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird + objData.numOfLyaer);
						m_missionAllClearFrameSR->Init(
							kMissionAllClearFrameSpriteFilePath,
							objData.width,
							objData.height,
							nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
							AlphaBlendMode_Trans
						);
						m_missionAllClearFrameSpriteOffset = objData.position;
						// 座標を設定
						m_missionAllClearFrameSR->SetPosition(objData.position + kMissionStartPosition);
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
				m_missionResultTextSRs[mrt]->SetPosition(kMissionResultTextPosition);
			}

			m_toEndSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
			m_toEndSR->Init(
				kToEndSpriteFilePath,
				kToEndSpriteWidth,
				kToEndSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);
			m_toEndSR->SetPosition(kToEndSpritePosition);


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
		 * @brief サウンドの初期化
		*/
		void CMissionUI::InitSound()
		{
			m_opneMission = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_opneMission->Init(kOpenMissionSoundFilePath, nsSound::CSoundCue::enSE);

			m_closeMission = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_closeMission->Init(kCloseMissionSoundFilePath, nsSound::CSoundCue::enSE);

			m_clearOneMission = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_clearOneMission->Init(kClearOneMissionSoundFilePath, nsSound::CSoundCue::enSE);

			m_clearAllMission = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_clearAllMission->Init(kClearAllMissionSoundFilePath, nsSound::CSoundCue::enSE);

			m_showResult = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_showResult->Init(kShowResultSoundFilePath, nsSound::CSoundCue::enSE);

			m_resultPerfect = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
			m_resultPerfect->Init(kResultPerfectSoundFilePath, nsSound::CSoundCue::enSE);

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
		void CMissionUI::UpdateShowMission(const Vector3& startPos, const Vector3& endPos)
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
				m_missionAllClearTextSR->SetPosition(pos + m_missionAllClearTextSpriteOffset);
				m_missionAllClearTextSR->SetAlphaValue(rate);
				m_missionAllClearFrameSR->SetPosition(pos + m_missionAllClearFrameSpriteOffset);
				m_missionAllClearFrameSR->SetAlphaValue(rate);

				if (rate <= 0.25f && m_opneMission->IsPlaying() != true)
				{
					// ミッションを表示するときのサウンドを再生
					m_opneMission->Play(false);
				}

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
				m_missionAllClearTextSR->SetPosition(endPos + m_missionAllClearTextSpriteOffset);
				m_missionAllClearTextSR->SetAlphaValue(1.0f);
				m_missionAllClearFrameSR->SetPosition(endPos + m_missionAllClearFrameSpriteOffset);
				m_missionAllClearFrameSR->SetAlphaValue(1.0f);
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
			if (m_timer <= 0.0f + FLT_EPSILON)
			{
				// ミッションを非表示するときのサウンドを再生
				m_closeMission->Play(false);
			}

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
				m_missionAllClearTextSR->SetPosition(pos + m_missionAllClearTextSpriteOffset);
				m_missionAllClearTextSR->SetAlphaValue(alphaValue);
				m_missionAllClearFrameSR->SetPosition(pos + m_missionAllClearFrameSpriteOffset);
				m_missionAllClearFrameSR->SetAlphaValue(alphaValue);
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
				m_missionAllClearTextSR->SetPosition(kMissionStartPosition + m_missionAllClearTextSpriteOffset);
				m_missionAllClearTextSR->SetAlphaValue(0.0f);
				m_missionAllClearFrameSR->SetPosition(kMissionStartPosition + m_missionAllClearFrameSpriteOffset);
				m_missionAllClearFrameSR->SetAlphaValue(0.0f);
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
				if (m_checkMarkSRs[m_currentClearMissionType]->GetAlphaValue() <= 0.0f + FLT_EPSILON)
				{
					m_clearOneMission->MultiOneShotPlay();
				}
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
				if (nsGameState::CGameMainState::GetInstance()->GetGameMainStateState() ==
					nsGameState::nsGameMainStateConstData::enGS_beforeClearDirecting)
				{
					m_checkMarkFlag[m_currentClearMissionType] = true;
					ChangeState(enMS_clearAllMission);
				}
				return;
			}

			return;
		}

		/**
		 * @brief 全てのミッションをクリアした時の処理の更新
		*/
		void CMissionUI::UpdateClearAllMission()
		{
			m_timer += nsTimer::GameTime().GetFrameDeltaTime();

			if (m_timer < kInMissionAllClearTextSpriteTime)
			{
				const float rate = m_timer / kInMissionAllClearTextSpriteTime;
				const float scale = Math::Lerp<float>(rate, kInMissionAllClearStartScale, kInMissionAllClearEndScale);
				const float alphaValue = rate;
				m_missionAllClearTextSR->SetScale(scale);
				m_missionAllClearTextSR->SetAlphaValue(alphaValue);
			}
			else if (m_timer <= kMissionAllClearTextSpriteTime)
			{
				if (std::fabsf(m_missionAllClearTextSR->GetAlphaValue() - 1.0f) > FLT_EPSILON)
				{
					m_clearAllMission->Play(false);
				}
				m_missionAllClearTextSR->SetScale(1.0f);
				m_missionAllClearTextSR->SetAlphaValue(1.0f);
				// 全てのミッションをクリアした時にフレームのスプライトを点滅させる
				BlinkSprite(m_missionAllClearFrameSR, &m_blinkTimer, kMissionAllClearFrameBlinkTime);
			}
			else
			{
				ChangeState(enMS_hideMission);
			}

			return;
		}

		/**
		 * @brief リザルトの時の処理の更新
		*/
		void CMissionUI::UpdateResult()
		{
			// 全てのミッションをクリアした時にフレームのスプライトを点滅させる
			BlinkSprite(m_missionAllClearFrameSR, &m_blinkTimer, kMissionAllClearFrameBlinkTime);

			float buff = 0.0f;
			switch (m_missionResultState)
			{
			case enMRS_showMission:
				UpdateShowMission(kMissionResultStartPosition, kMissionResultPosition);
				// ミッション表示途中では、フレームは表示させない。
				m_missionAllClearFrameSR->SetAlphaValue(0.0f);
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

				if (m_timer < kMissionResultTextTime)
				{
					const float rate = m_timer / kStartShowMissionTime;
					const float scale = Math::Lerp<float>(rate, kMissionResultTextStartSclae, kMissionResultTextEndSclae);

					for (auto& mrtSR : m_missionResultTextSRs)
					{
						mrtSR->SetAlphaValue(rate);
						mrtSR->SetScale(scale);
					}

					return;
				}
				else
				{
					for (auto& mrtSR : m_missionResultTextSRs)
					{
						if (mrtSR->IsActive() != true)
						{
							continue;
						}
						if (std::fabsf(mrtSR->GetAlphaValue() - 1.0f) > FLT_EPSILON)
						{
							m_showResult->Play(false);
							if (mrtSR == m_missionResultTextSRs[enMRT_perfect])
							{
								m_resultPerfect->Play(false);
							}
						}						
						mrtSR->SetAlphaValue(1.0f);
						mrtSR->SetScale(1.0f);
					}

					if (m_toEndSR->IsActive() != true)
					{
						m_toEndSR->Activate();
					}

					// 終わりへと行くスプライトの点滅の点滅処理
					BlinkSprite(m_toEndSR, &m_toEndBlinkTimer, kToEndBlinkTime);

					if (nsGameState::GameMainState()->GetGameMainStateState() ==
						nsGameState::nsGameMainStateConstData::enGS_goTitle)
					{
						// ゲームステートがタイトルへ行く状態へ遷移したら

						// リザルトのステートをフェードアウトへと遷移
						m_missionResultState = enMRS_fadeOutToEnd;
						// タイマーをリセット
						m_toEndBlinkTimer = 0.0f;
					}
					

					return;
				}

				break;
			case enMRS_fadeOutToEnd:
				m_toEndBlinkTimer += nsTimer::GameTime().GetFrameDeltaTime();

				if (m_toEndBlinkTimer > kToEndFadeOutTime)
				{
					// タイマーが上限より進んでいる
					// スプライトを完全に透明にする
					m_toEndSR->SetAlphaValue(0.0f);
					return;
				}

				// タイマーの進み率
				const float scale = m_toEndBlinkTimer / kToEndFadeOutTime;
				// アルファ値。だんだんと透明にしていく。
				const float alphaValue = 1.0f - scale;
				m_toEndSR->SetAlphaValue(alphaValue);

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
		 * @brief 全てのミッションをクリアした時のフレームの点滅処理
		*/
		void CMissionUI::BlinkClearAllMissionFrame()
		{
			if (m_missionAllClearFrameSR->IsActive() != true)
			{
				return;
			}
			m_blinkTimer += nsTimer::GameTime().GetFrameDeltaTime();
			constexpr float halfTime = kMissionAllClearFrameBlinkTime * 0.5f;

			if (m_blinkTimer <= halfTime)
			{
				const float alphaValue = m_blinkTimer / halfTime;
				m_missionAllClearFrameSR->SetAlphaValue(alphaValue);
			}
			else if (m_blinkTimer <= kMissionAllClearFrameBlinkTime)
			{
				const float alphaValue = 1.0f - (m_blinkTimer - halfTime) / halfTime;
				m_missionAllClearFrameSR->SetAlphaValue(alphaValue);
			}
			else
			{
				m_blinkTimer -= kMissionAllClearFrameBlinkTime;
				m_missionAllClearFrameSR->SetAlphaValue(0.0f);

			}

			return;
		}

		/**
		 * @brief スプライトを点滅させる
		 * @param[in,out] sprite 点滅させるスプライト
		 * @param[in,out] timer タイマー
		 * @param[in] blinkTime 点滅間隔タイム
		*/
		void CMissionUI::BlinkSprite(nsGraphic::nsSprite::CSpriteRender* sprite, float* timer, const float blinkTime)
		{
			if (sprite->IsActive() != true)
			{
				// 有効ではないなら、点滅処理を行わない。早期リターン。
				return;
			}

			// タイマーを進める
			*timer += nsTimer::GameTime().GetFrameDeltaTime();

			// 点滅間隔タイムの半分のタイム
			const float halfTime = blinkTime * 0.5f;

			if (*timer <= halfTime)
			{
				// タイマーが点滅間隔の半分のタイム以下のとき

				// アルファ値。だんだんと不透明にしていく。
				const float alphaValue = *timer / halfTime;
				sprite->SetAlphaValue(alphaValue);
				return;
			}
			else if (*timer <= blinkTime)
			{
				// タイマーが点滅間隔の半分より大きく、点滅間隔のタイム以下のとき

				// アルファ値。だんだんと透明にしていく。
				const float alphaValue = 1.0f - (*timer - halfTime) / halfTime;
				sprite->SetAlphaValue(alphaValue);
				return;
			}
			else
			{
				// タイマーが点滅間隔のタイムより大きいとき

				// タイマーとアルファ値をリセットする
				*timer -= blinkTime;
				sprite->SetAlphaValue(0.0f);
				return;
			}

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
			func(m_missionAllClearTextSR);
			func(m_missionAllClearFrameSR);
			func(m_toEndSR);

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
			case enMS_clearAllMission:
				m_missionAllClearTextSR->Activate();
				m_missionAllClearTextSR->SetAlphaValue(0.0f);
				m_missionAllClearFrameSR->Activate();
				m_missionAllClearFrameSR->SetAlphaValue(0.0f);
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
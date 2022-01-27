#include "stdafx.h"
#include "MissionUI.h"
#include "UIConstData.h"
#include "SpriteRender.h"
#include "SpriteRenderConstData.h"
#include "FontRender.h"
#include "../../ExEngine/level2D/Level2D.h"

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
			// スプライトの初期化
			InitSprite();

			// フォントの初期化
			InitFont();

			return true;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CMissionUI::OnDestroy()
		{
			// スプライトの破棄
			for (auto& checkMarkSR : m_checkMarkSRs)
			{
				DeleteGO(checkMarkSR);
			}
			DeleteGO(m_missionWindowSR);
			DeleteGO(m_missionResultFrameSR);
			for (auto& missionResultTextSR : m_missionResultTextSRs)
			{
				DeleteGO(missionResultTextSR);
			}

			// フォントんの破棄
			DeleteGO(m_clearTimeFR);
			DeleteGO(m_numOfMissFR);

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CMissionUI::Update()
		{
			return;
		}

		/**
		 * @brief スプライトの初期化
		*/
		void CMissionUI::InitSprite()
		{
			// ミッションの種類の数だけリサーブ
			m_checkMarkSRs.reserve(enMissionTypeNum);
			for (int missionType = 0; missionType < enMissionTypeNum; missionType++)
			{
				// 先にインスタンスを生成しておく。初期化はレベルの初期化の中で行う。
				//m_checkMarkSRs.emplace_back(NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFourth));
				m_checkMarkSRs.emplace_back(nullptr);
			}

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
						m_missionWindowSR->SetPosition(objData.position);

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
							// 座標を設定
							m_checkMarkSRs[missionType]->SetPosition(objData.position);
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

			// ミッションリザルトのテキストのスプライトの生成と初期化
			m_missionResultTextSRs.reserve(enMissionResultTypeNum);
			for (int mrt = 0; mrt < enMissionResultTypeNum; mrt++)
			{
				m_missionResultTextSRs.emplace_back(NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFourth));
				m_missionResultTextSRs[mrt]->Init(
					kMissionResultTextSpriteFilepath[mrt],
					kMissionResultTextSpriteWidth,
					kMissionResultTextSpriteHeight,
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);

			}

			return;
		}

		/**
		 * @brief フォントの初期化
		*/
		void CMissionUI::InitFont()
		{
			m_clearTimeFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFinal);
			m_clearTimeFR->SetText(L"01:00");
			m_clearTimeFR->SetPosition({ 100.0f,-100.0f });

			m_numOfMissFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFinal);
			m_numOfMissFR->SetText(L"0");
			m_numOfMissFR->SetPosition({ 100.0f,-120.0f });

			return;
		}
	}
}
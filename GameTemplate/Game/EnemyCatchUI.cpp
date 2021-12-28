#include "stdafx.h"
#include "EnemyCatchUI.h"
#include "SpriteRender.h"
#include "GameMainState.h"
#include "Player.h"
#include "AICar.h"
#include "GameTime.h"


namespace nsMyGame
{
	/**
	 * @brief UI関連のネームスペース
	*/
	namespace nsUI
	{
		using namespace nsEnemyCatchUIConstData;

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CEnemyCatchUI::Start()
		{
			// ゲームステートの参照を引っ張ってくる
			m_gameState = FindGO<nsGameState::CGameMainState>(
				nsCommonData::kGameObjectName[nsCommonData::enMainGameState]
				);

			// 車たちの参照を引っ張ってくる
			m_aiCarsRef = m_gameState->GetAICar();

			// 敵を捕まえることができる合図のスプライトの初期化
			InitCanCatchEnemeySprite();

			return true;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CEnemyCatchUI::OnDestroy()
		{
			for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
			{
				DeleteGO(m_canCatchEnemySR[i]);
			}


			return;
		}

		/**
		 * @brief 更新処理
		*/
		void CEnemyCatchUI::Update()
		{
			// 敵を捕まえることができる合図のスプライトの更新
			UpdateCanCatchEnemySprite();

			return;
		}


		/**
		 * @brief 敵を捕まえることができる合図のスプライトの初期化
		*/
		void CEnemyCatchUI::InitCanCatchEnemeySprite()
		{
			for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
			{
				m_canCatchEnemySR[i] = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);
				m_canCatchEnemySR[i]->Init(
					kCanCatchEnemySpriteFilePath[i],
					kCanCatchEnemySpriteWidht[i],
					kCanCatchEnemySpriteHeight[i],
					nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
					AlphaBlendMode_Trans
				);
			}

			return;
		}

		/**
		 * @brief 敵を捕まえることができる合図のスプライトの更新
		*/
		void CEnemyCatchUI::UpdateCanCatchEnemySprite()
		{
			// 補間率
			float t = 0.0f;
			// 枠1の角度
			float Frame1angle = 0.0f;
			// 枠2の拡大率
			float Frame2scale = 1.0f;
			
			if (m_canCatchEnemyTimer <= kCanCatchTime1)
			{
				// タイマーがタイム1以下の時
				// ゆっくりと、時計回りに回転して、拡大していく。
				t = m_canCatchEnemyTimer / kCanCatchTime1;
				Frame1angle = Math::Lerp<float>(t, 0.0f, 360.0f);
				Frame2scale = Math::Lerp<float>(t, 1.0f, kCanCatchMaxScale);
			}
			else if (m_canCatchEnemyTimer <= (kCanCatchTime1 + kCanCatchTime2))
			{
				// タイマーがタイム2以下の時
				// 速く、反時計回りに回転して、縮小していく。
				t = (m_canCatchEnemyTimer - kCanCatchTime1) / kCanCatchTime2;
				Frame1angle = Math::Lerp<float>(t, 360.0f, 0.0f);
				Frame2scale = Math::Lerp<float>(t, kCanCatchMaxScale, 1.0f);
			}
			else
			{
				// タイムを過ぎたら、タイマーをリセット
				m_canCatchEnemyTimer = 0.0f;
			}

			// タイマーを進める
			m_canCatchEnemyTimer += nsTimer::GameTime().GetFrameDeltaTime();

			// 敵を捕まえることができる合図のスプライトを更新する
			for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
			{
				// スプライトの回転
				Quaternion qRot = Quaternion::Identity;
				// スプライトの拡大率
				Vector3 scale = Vector3::One;
				if (i == enCanCatchEnemyFrame1)
				{
					// 枠1の時は、回転を変更する。
					qRot.SetRotationDegZ(Frame1angle);
				}
				else if (i == enCanCatchEnemyFrame2)
				{
					// 枠2の時は、拡大率を変更する。
					scale.Scale(Frame2scale);
				}
				m_canCatchEnemySR[i]->SetPosition(Vector2::Zero);
				m_canCatchEnemySR[i]->SetRotation(qRot);
				m_canCatchEnemySR[i]->SetScale(scale);
			}

		}

	}
}
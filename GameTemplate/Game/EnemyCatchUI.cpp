#include "stdafx.h"
#include "EnemyCatchUI.h"
#include "SpriteRender.h"
#include "GameMainState.h"
#include "UIConstData.h"
#include "Player.h"
#include "AICar.h"


namespace nsMyGame
{
	/**
	 * @brief UI関連のネームスペース
	*/
	namespace nsUI
	{
		using namespace nsGameMainUIConstData;

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool EnemyCatchUI::Start()
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
		void EnemyCatchUI::OnDestroy()
		{
			DeleteGO(m_canCatchEnemySR);

			return;
		}

		/**
		 * @brief 更新処理
		*/
		void EnemyCatchUI::Update()
		{
			return;
		}


		/**
		 * @brief 敵を捕まえることができる合図のスプライトの初期化
		*/
		void EnemyCatchUI::InitCanCatchEnemeySprite()
		{
			m_canCatchEnemySR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);


			return;
		}


	}
}
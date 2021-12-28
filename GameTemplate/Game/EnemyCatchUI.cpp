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
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		using namespace nsGameMainUIConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool EnemyCatchUI::Start()
		{
			// �Q�[���X�e�[�g�̎Q�Ƃ����������Ă���
			m_gameState = FindGO<nsGameState::CGameMainState>(
				nsCommonData::kGameObjectName[nsCommonData::enMainGameState]
				);

			// �Ԃ����̎Q�Ƃ����������Ă���
			m_aiCarsRef = m_gameState->GetAICar();

			// �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̏�����
			InitCanCatchEnemeySprite();

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void EnemyCatchUI::OnDestroy()
		{
			DeleteGO(m_canCatchEnemySR);

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void EnemyCatchUI::Update()
		{
			return;
		}


		/**
		 * @brief �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̏�����
		*/
		void EnemyCatchUI::InitCanCatchEnemeySprite()
		{
			m_canCatchEnemySR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityThird);


			return;
		}


	}
}
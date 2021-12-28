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
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		using namespace nsEnemyCatchUIConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CEnemyCatchUI::Start()
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
		void CEnemyCatchUI::OnDestroy()
		{
			for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
			{
				DeleteGO(m_canCatchEnemySR[i]);
			}


			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CEnemyCatchUI::Update()
		{
			// �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̍X�V
			UpdateCanCatchEnemySprite();

			return;
		}


		/**
		 * @brief �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̏�����
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
		 * @brief �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̍X�V
		*/
		void CEnemyCatchUI::UpdateCanCatchEnemySprite()
		{
			// ��ԗ�
			float t = 0.0f;
			// �g1�̊p�x
			float Frame1angle = 0.0f;
			// �g2�̊g�嗦
			float Frame2scale = 1.0f;
			
			if (m_canCatchEnemyTimer <= kCanCatchTime1)
			{
				// �^�C�}�[���^�C��1�ȉ��̎�
				// �������ƁA���v���ɉ�]���āA�g�債�Ă����B
				t = m_canCatchEnemyTimer / kCanCatchTime1;
				Frame1angle = Math::Lerp<float>(t, 0.0f, 360.0f);
				Frame2scale = Math::Lerp<float>(t, 1.0f, kCanCatchMaxScale);
			}
			else if (m_canCatchEnemyTimer <= (kCanCatchTime1 + kCanCatchTime2))
			{
				// �^�C�}�[���^�C��2�ȉ��̎�
				// �����A�����v���ɉ�]���āA�k�����Ă����B
				t = (m_canCatchEnemyTimer - kCanCatchTime1) / kCanCatchTime2;
				Frame1angle = Math::Lerp<float>(t, 360.0f, 0.0f);
				Frame2scale = Math::Lerp<float>(t, kCanCatchMaxScale, 1.0f);
			}
			else
			{
				// �^�C�����߂�����A�^�C�}�[�����Z�b�g
				m_canCatchEnemyTimer = 0.0f;
			}

			// �^�C�}�[��i�߂�
			m_canCatchEnemyTimer += nsTimer::GameTime().GetFrameDeltaTime();

			// �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g���X�V����
			for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
			{
				// �X�v���C�g�̉�]
				Quaternion qRot = Quaternion::Identity;
				// �X�v���C�g�̊g�嗦
				Vector3 scale = Vector3::One;
				if (i == enCanCatchEnemyFrame1)
				{
					// �g1�̎��́A��]��ύX����B
					qRot.SetRotationDegZ(Frame1angle);
				}
				else if (i == enCanCatchEnemyFrame2)
				{
					// �g2�̎��́A�g�嗦��ύX����B
					scale.Scale(Frame2scale);
				}
				m_canCatchEnemySR[i]->SetPosition(Vector2::Zero);
				m_canCatchEnemySR[i]->SetRotation(qRot);
				m_canCatchEnemySR[i]->SetScale(scale);
			}

		}

	}
}
#include "stdafx.h"
#include "EnemyCatchUI.h"
#include "SpriteRender.h"
#include "FontRender.h"
#include "GameMainState.h"
#include "Player.h"
#include "PlayerConstData.h"
#include "AICar.h"
#include "GameTime.h"


namespace nsNinjaAttract
{
	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		using namespace nsEnemyCatchUIConstData;

		// �K�v�ȃv���C���[�p�̒萔���g�p�\�ɂ���
		using nsPlayer::nsPlayerConstData::nsCatchEnemyConstData::kCanCatchMaxLength;
		using nsPlayer::nsPlayerConstData::nsCatchEnemyConstData::kTargetMaxLength;
		using nsPlayer::nsPlayerConstData::nsCatchEnemyConstData::kTargetMaxAngle;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CEnemyCatchUI::Start()
		{
			// �Q�[���X�e�[�g�̎Q�Ƃ����������Ă���
			m_gameState = FindGO<nsGameState::CGameMainState>(
				nsCommonData::kGameObjectName[nsCommonData::enGN_MainGameState]
				);

			// �Ԃ����̎Q�Ƃ����������Ă���
			m_aiCarsRef = m_gameState->GetAICar();

			// �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̏�����
			InitCanCatchEnemeySprite();

			// �^�[�Q�b�g�������X�v���C�g�̏�����
			InitTargetSprite();

			// �^�[�Q�b�g�Ƃ̋����������t�H���g�̏�����
			InitTargetLengthFont();

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
			DeleteGO(m_targetSR);
			DeleteGO(m_targetLengthFR);

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CEnemyCatchUI::Update()
		{
			if (m_gameState->GetPlayer().GetState() == nsPlayer::nsPlayerConstData::enOnEnemy)
			{
				// �G�l�~�[�̏�ɂ���Ƃ��̍X�V
				OnEnemyUpdate();
				// �������^�[��
				return;
			}

			// ���̃N���X�Ďg�p���鋤�ʂ̃f�[�^���X�V
			UpdateTargetLength();

			// �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̍X�V
			UpdateCanCatchEnemySprite();

			// �^�[�Q�b�g�������X�v���C�g�̍X�V
			UpdateTargetSprite();

			// �^�[�Q�b�g�Ƃ̋����������t�H���g�̍X�V
			UpdateTargetLengthFont();

			return;
		}


		/**
		 * @brief �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̏�����
		*/
		void CEnemyCatchUI::InitCanCatchEnemeySprite()
		{
			for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
			{
				m_canCatchEnemySR[i] = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond);
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
		 * @brief �^�[�Q�b�g�������X�v���C�g�̏�����
		*/
		void CEnemyCatchUI::InitTargetSprite()
		{
			m_targetSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPrioritySecond);
			m_targetSR->Init(
				kTargetSpriteFilePath,
				kTargetSpriteWidth,
				kTargetSpriteHeight,
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			return;
		}

		/**
		 * @brief �^�[�Q�b�g�Ƃ̋����������t�H���g�̏�����
		*/
		void CEnemyCatchUI::InitTargetLengthFont()
		{
			m_targetLengthFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPrioritySecond);
			m_targetLengthFR->SetParam(
				L"",
				Vector2::Zero,
				Vector4::White,
				0.0f,
				0.5f
			);
			return;
		}

		/**
		 * @brief �G�l�~�[�̏�ɂ���Ƃ��̍X�V
		*/
		void CEnemyCatchUI::OnEnemyUpdate()
		{
			// �S�ẴX�v���C�g���\���ɂ���
			for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
			{
				// ���ׂĔ�\��
				m_canCatchEnemySR[i]->Deactivate();
			}
			m_targetSR->Deactivate();
			m_targetLengthFR->Deactivate();

			return;
		}

		/**
		 * @brief ���̃N���X�Ďg�p���鋤�ʂ̃f�[�^���X�V
		*/
		void CEnemyCatchUI::UpdateTargetLength()
		{
			// �^�[�Q�b�g�̎Q�Ƃ𓾂�
			m_targetRef = m_gameState->GetPlayer().GetCatchEnemy().GetTargetEnemy();

			if (m_targetRef == nullptr)
			{
				// �^�[�Q�b�g���������Ă��Ȃ��ꍇ�́A����ȉ��̏����͍s��Ȃ��B
				return;
			}

			// �v���C���[����A�^�[�Q�b�g�ւ̋������v�Z����
			m_targetLength = m_gameState->GetPlayer().GetCatchEnemy().GetTargetLength();

			// �^�[�Q�b�g�̉�ʏ��2D���W���v�Z����
			g_camera3D->CalcScreenPositionFromWorldPosition(m_taraget2DPos, m_targetRef->GetPosition());

			return;
		}

		/**
		 * @brief �G��߂܂��邱�Ƃ��ł��鍇�}�̃X�v���C�g�̍X�V
		*/
		void CEnemyCatchUI::UpdateCanCatchEnemySprite()
		{
			if (m_targetRef == nullptr || 
				m_targetLength > kCanCatchMaxLength)
			{
				// �^�[�Q�b�g���������Ă��Ȃ��A�܂��́A
				// �^�[�Q�b�g�Ƃ̋��������ȉ��ł͂Ȃ��Ƃ�

				for (int i = 0; i < enCanCatchEnemySpriteNum; i++)
				{
					// ���ׂĔ�\��
					m_canCatchEnemySR[i]->Deactivate();
				}
				// �������^�[��
				return;
			}


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
				m_canCatchEnemySR[i]->SetPosition(m_taraget2DPos);
				m_canCatchEnemySR[i]->SetRotation(qRot);
				m_canCatchEnemySR[i]->SetScale(scale);

				// �\������
				m_canCatchEnemySR[i]->Activate();
			}

			return;
		}

		/**
		 * @brief �^�[�Q�b�g�������X�v���C�g�̍X�V
		*/
		void CEnemyCatchUI::UpdateTargetSprite()
		{
			if (m_targetRef == nullptr ||
				m_targetLength <= kCanCatchMaxLength)
			{
				// �^�[�Q�b�g���������Ă��Ȃ��A�܂��́A
				// �^�[�Q�b�g�Ƃ̋��������ȉ��̂Ƃ�

				// ��\��
				m_targetSR->Deactivate();
				// �������^�[��
				return;
			}

			// ��ԗ�
			float t = (m_targetLength - kCanCatchMaxLength) / (kTargetMaxLength - kCanCatchMaxLength);
			// �g�嗦���Ⴂ�l�̎��������Ȃ�悤�ɁA���]������B
			t = 1.0f - t;
			// �w���֐��I�ω��ɂ�����B
			t = pow(t, kTargetSpritePowPower);
			// �g�嗦����`�⊮�ŋ��߂�
			float scale = Math::Lerp<float>(t, kTargetSpriteMinScale, kTargetSpriteMaxScale);

			// �\������
			m_targetSR->Activate();

			m_targetSR->SetPosition(m_taraget2DPos);
			m_targetSR->SetScale(scale);

			return;
		}

		/**
		 * @brief �^�[�Q�b�g�Ƃ̋����������t�H���g�̍X�V
		*/
		void CEnemyCatchUI::UpdateTargetLengthFont()
		{
			if (m_targetRef == nullptr ||
				m_targetLength <= kCanCatchMaxLength)
			{
				// �^�[�Q�b�g���������Ă��Ȃ��A�܂��́A
				// �^�[�Q�b�g�Ƃ̋��������ȉ��̂Ƃ�

				// ��\��
				m_targetLengthFR->Deactivate();
				// �������^�[��
				return;
			}

			// �\��
			m_targetLengthFR->Activate();

			// �\������e�L�X�g
			wchar_t text[16];
			// �P�ʁi m�j
			wchar_t unit[16];
			// �^�[�Q�b�g�܂ł̋����B�f�t�H���g�ł�cm�Ȃ̂�m�ɒ����B
			const float targetLength = m_targetLength * 0.1f;
			// 4���A�����Ȃ��̐���
			swprintf_s(text, L"%4.0f", targetLength);
			// �P��
			swprintf_s(unit, L" m");
			// �P�ʂ��e�L�X�g�ɑ����B
			wcscat(text, unit);
			// �e�L�X�g���t�H���g�����_���[�ɐݒ�
			m_targetLengthFR->SetText(text);
			// ���W���v�Z����B
			Vector2 pos = m_taraget2DPos;
			pos.x += kTargetLengthFontRenderPosBuff.x;
			pos.y += kTargetLengthFontRenderPosBuff.y;

			// �t�H���g�̍��W��ݒ肷��
			m_targetLengthFR->SetPosition(pos);

			return;
		}

	}
}
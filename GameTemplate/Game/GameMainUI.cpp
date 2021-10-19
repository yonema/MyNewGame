#include "stdafx.h"
#include "GameMainUI.h"
#include "UIConstData.h"
#include "FontRender.h"
#include "SpriteRender.h"

namespace nsMyGame
{
	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		// �Q�[�����C���̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsGameMainUIConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CGameMainUI::Start()
		{
			// �^�C�}�[�̃t�H���g��������
			InitTimerFont();

			// �S�[���̃X�v���C�g�̏�����
			InitGoalSprite();

			return true;
		}

		/**
		 * @brief �X�V����
		*/
		void CGameMainUI::OnDestroy()
		{
			DeleteGO(m_timerFR);

			DeleteGO(m_goalSR);

			return;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CGameMainUI::Update()
		{
			return;
		}

		/**
		 * @brief �^�C�}�[�̃e�L�X�g��ݒ肷��
		 * @param[in] time �^�C��
		*/
		void CGameMainUI::SetTimerText(const float time)
		{
			// ��
			int min = static_cast<int>(time) / 60;
			// �b
			int sec = static_cast<int>(time) - 60 * min;
			// �e�L�X�g
			wchar_t text[64];
			// �^�C�������̃e�L�X�g
			wchar_t timeText[64];

			// �e�L�X�g��ݒ�
			swprintf_s(text, kTimerFontTextHead);
			swprintf_s(timeText, kTimerFontTextEnd, min, sec);
			wcscat(text, timeText);	

			// �t�H���g�����_���[�Ƀe�L�X�g���Z�b�g
			m_timerFR->SetText(text);

			return;
		}

		/**
		 * @brief �S�[������
		*/
		void CGameMainUI::Goal()
		{
			// �S�[���̃X�v���C�g�̗L����
			m_goalSR->Activate();

			return;
		}

		/**
		 * @brief �^�C�}�[�̃t�H���g��������
		*/
		void CGameMainUI::InitTimerFont()
		{
			// �^�C�}�[�̃t�H���g�𐶐�
			m_timerFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFirst);
			// �p�����[�^��ݒ�
			m_timerFR->SetParam(
				kTimerFontTextHead,			// �e�L�X�g
				kTimerFontPosition,		// ���W
				kTimerFontColor			// �J���[
			);
			// �V���h�E�p�����[�^��ݒ�
			m_timerFR->SetShadowParam(true, kTimerFontShadowOffset, kTimerFontShadowColor);

			return;
		}

		/**
		 * @brief �S�[���̃X�v���C�g�̏�����
		*/
		void CGameMainUI::InitGoalSprite()
		{
			// �S�[���̃X�v���C�g�̐���
			m_goalSR = NewGO <nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFirst);
			// ���W��ݒ�
			m_goalSR->SetPosition(kGoalSpritePosition);
			// ������
			m_goalSR->Init(kGoalSpriteFilePath, kGoalSpriteWidth, kGoalSpriteHeight);
			// ������
			m_goalSR->Deactivate();

			return;
		}



	}
}
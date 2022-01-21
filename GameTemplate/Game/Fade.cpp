#include "stdafx.h"
#include "Fade.h"
#include "SpriteRender.h"
#include "GameTime.h"

namespace nsNinjaAttract
{

	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		const char* const CFade::kFadeSpriteFilePath = "Assets/Image/black.dds";
		const float CFade::kDefaultFadeTime = 2.0f;

		/**
		 * @brief �R���X�g���N�^
		*/
		CFade::CFade()
		{
			// �t�F�[�h�̃X�v���C�g�����_���[�̃I�u�W�F�N�g�𐶐�����
			m_fadeSR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::enPriorityFourth);
		}

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CFade::Start()
		{

			// �t�F�[�h�p�̃X�v���C�g�̏�����
			m_fadeSR->Init(
				kFadeSpriteFilePath,
				static_cast<float>(g_graphicsEngine->GetFrameBufferWidth()),
				static_cast<float>(g_graphicsEngine->GetFrameBufferHeight()),
				nsGraphic::nsSprite::nsSpriteConstData::kDefaultPivot,
				AlphaBlendMode_Trans
			);

			if (m_fadeState == enFS_blackOut)
			{
				// �u���b�N�A�E�g��ԂȂ�A�A���t�@�l��1�ɂ���B
				m_fadeSR->SetAlphaValue(1.0f);
			}
			else
			{
				// �ʏ��ԂȂ�A�A���t�@�l��0�ɂ���B
				m_fadeSR->SetAlphaValue(0.0f);
			}

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CFade::OnDestroy()
		{
			DeleteGO(m_fadeSR);

			return;
		}

		/**
		 * @brief �X�V����
		*/
		void CFade::Update()
		{
			if (m_fadeState == enFS_fadeEnd)
			{
				// �t�F�[�h���I����Ă�����A�������Ȃ��B�������^�[���B
				return;
			}

			// �t�F�[�h�̃^�C�}�[��i�߂�
			m_fadeTimer += nsTimer::GameTime().GetFrameDeltaTime();
			// �t�F�[�h�^�C�}�[�̐i�ݗ�
			const float scale = min(1.0f, m_fadeTimer / m_fadeTime);
			// �A���t�@�l
			float alphaValue = 0.0f;

			if (m_fadeState == enFS_fadeIn)
			{
				// �t�F�[�h�C���Ȃ�A�A���t�@�l�����񂾂񏬂������Ă����B
				alphaValue = 1.0f - 1.0f * scale;
			}
			else if (m_fadeState == enFS_fadeOut)
			{
				// �t�F�[�h�A�E�g�Ȃ�A�A���t�@�l�����񂾂�傫�����Ă����B
				alphaValue = 1.0f * scale;
			}
			else
			{
				// �u���b�N�A�E�g�Ȃ�A�A���t�@�l��1�ɂ��ăt�F�[�h���I������B
				alphaValue = 1.0f;
				m_fadeState = enFS_fadeEnd;
			}

			// �A���t�@�l��ݒ肷��
			m_fadeSR->SetAlphaValue(alphaValue);

			if (scale >= 1.0f)
			{
				// �^�C�}�[�̐i�ݗ���1�𒴂�����t�F�[�h���I������
				m_fadeState = enFS_fadeEnd;
			}

			return;
		}

	}
}
#include "stdafx.h"
#include "TextPanelRender.h"
#include "RenderingEngine.h"

namespace nsMyGame
{
	/**
	 * @brief �f�o�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsDebug
	{

		// �f�o�b�N�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsDebugConstData;

		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CTextPanelRender::Start()
		{
			// �e�L�X�g�p�l���̃f�[�^�W�̃L���p�V�e�B���m��
			m_textPanelDatas.reserve(kTextPanelDatasNum);

			// �����_���[�N���X��2D�`��֐���ݒ肷��
			m_render.SetOnRender2DFunc([&](RenderContext& rc) { this->Render2D(rc); });

			// �t�H���g�̃V���h�E�p�����[�^��ݒ�
			m_font.SetShadowParam(true, m_shadowOffset, Vector4::Black);

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CTextPanelRender::OnDestroy()
		{
			return;
		}

		/**
		 * @brief �`��I�u�W�F�N�g�o�^�̓���
		*/
		void CTextPanelRender::AddRenderEntrance()
		{
			// �`��I�u�W�F�N�g�ɓo�^����
			nsMyEngine::CRenderingEngine::GetInstance()->AddRenderObject(&m_render);
		}


		/**
		 * @brief 2D�`��֐�
		 * @param[in,out] rc �����_�����O�R���e�L�X�g
		*/
		void CTextPanelRender::Render2D(RenderContext& rc)
		{
			m_font.Begin(rc);

			Vector2 pos = m_textPanelPosition;

			for (const auto& textPanelData : m_textPanelDatas)
			{
				// �e�L�X�g
				wchar_t text[kTextSize];
				// char��wchar�ɕϊ�
				mbstowcs(text, textPanelData, kTextSize);
				// �e�L�X�g�̏c�̊�
				const float between = 40.0f * m_textScale;
				// ��_
				const Vector2 pivot = { 0.0f,1.0f };

				// �e�L�X�g��`��
				m_font.Draw(text, pos, Vector4::White, 0.0f, m_textScale, pivot);

				// ���W�����ɉ�����	
				pos.y -= between;

			}



			// �t�H���g�̕`��I��
			m_font.End(rc);

			// �x�N�g���̃f�[�^�W���N���A����
			m_textPanelDatas.clear();

			return;
		}

	}
}
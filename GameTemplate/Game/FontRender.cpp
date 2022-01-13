#include "stdafx.h"
#include "FontRender.h"
#include "RenderingEngine.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �t�H���g�֘A�̃l�[���X�y�[�X
		*/
		namespace nsFont
		{
			/**
			 * @brief �R���X�g���N�^
			*/
			CFontRender::CFontRender()
			{
				m_render.SetOnRender2DFunc([&](RenderContext& rc) { this->Render2D(rc); });

				return;
			}

			/**
			 * @brief �j�����ꂽ�Ƃ��̏���
			*/
			void CFontRender::OnDestroy()
			{
				return;
			}

			/**
			 * @brief �`��I�u�W�F�N�g�o�^�̓���
			*/
			void CFontRender::AddRenderEntrance()
			{
				// �`��I�u�W�F�N�g�ɓo�^����
				nsMyEngine::CRenderingEngine::GetInstance()->AddRenderObject(&m_render);
			}

			/**
			 * @brief 2D�`��֐�
			 * @param rc �����_�����O�R���e�L�X�g
			*/
			void CFontRender::Render2D(RenderContext& rc)
			{
				//�`��J�n
				m_font.Begin(rc);

				//�`�揈��
				m_font.Draw(
					m_text,
					m_position,
					m_color,
					m_rotation,
					m_scale,
					m_pivot);

				//�`��I��
				m_font.End(rc);

				return;
			}

			/**
			 * @brief �t�H���g�̃p�����[�^��ݒ�
			 * @param[in] text �e�L�X�g
			 * @param[in] pos ���W
			 * @param[in] color �J���[
			 * @param[in] rotation ��]
			 * @param[in] scale �g�嗦
			 * @param[in] pivot �s�{�b�g
			*/
			void CFontRender::SetParam(
				const wchar_t* text,
				const Vector2& pos,
				const Vector4& color,
				const float rotation,
				const float scale,
				const Vector2& pivot
			)
			{
				//�e�t�H���g�̃p�����[�^��ݒ肷��

				SetText(text);
				SetPosition(pos);
				m_color = color;
				m_rotation = rotation;
				m_scale = scale;
				m_pivot = pivot;

				return;
			}
		}
	}
}
#include "stdafx.h"
#include "FontRender.h"

namespace nsMyGame
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
			 * @brief �j�����ꂽ�Ƃ��̏���
			*/
			void CFontRender::OnDestroy()
			{
				return;
			}

			/**
			 * @brief �`�揈���֐�
			 * @param[in] rc �����_�[�R���e�L�X�g
			*/
			void CFontRender::Render(RenderContext& rc)
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
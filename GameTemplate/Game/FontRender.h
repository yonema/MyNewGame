#pragma once
#include "FontRenderConstData.h"
#include "Render.h"

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
			 * @brief �t�H���g�`��N���X
			*/
			class CFontRender : public IGameObject
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CFontRender();

				/**
				 * @brief �f�X�g���N�^
				*/
				~CFontRender() = default;

			public:		// �I�[�o�[���C�h���������o�֐�
				/**
				 * @brief �X�^�[�g�֐�
				 * @return �A�b�v�f�[�g���s�����H
				*/
				bool Start() override final { return true; };

				/**
				 * @brief �j�����ꂽ�Ƃ��̏���
				*/
				void OnDestroy() override final;

				/**
				 * @brief �`��I�u�W�F�N�g�o�^�̓���
				*/
				void AddRenderEntrance() override final;

			public:		// �����o�֐�

				/**
				 * @brief �t�H���g�̃p�����[�^��ݒ�
				 * @param[in] text �e�L�X�g
				 * @param[in] pos ���W
				 * @param[in] color �J���[
				 * @param[in] rotation ��]
				 * @param[in] scale �g�嗦
				 * @param[in] pivot �s�{�b�g
				*/
				void SetParam(
					const wchar_t* text,
					const Vector2& pos = Vector2::Zero,
					const Vector4& color = nsFontConstData::kDefaltTextColor,
					const float rotation = 0.0f,
					const float scale = 1.0f,
					const Vector2& pivot = Vector2::Zero
				);

				/**
				 * @brief ���W��ݒ�
				 * @param[on] pos 
				*/
				void SetPosition(const Vector2& pos)
				{
					m_position = pos;
				}
				

				/**
				 * @brief ���W���擾
				 * @return ���W
				*/
				const Vector2& GetPosition() const
				{
					return m_position;
				}

				/**
				 * @brief �e�L�X�g��ݒ�
				 * @param[in] text �e�L�X�g
				*/
				void SetText(const wchar_t* text)
				{
					swprintf_s(m_text, text);
				}

				/**
				 * @brief �t�H���g�̃V���h�E�̐ݒ�
				 * @param isDrawShadow �V���h�E��`�悷�邩�H
				 * @param shadowOffset �V���h�E�̃s�N�Z���̃I�t�Z�b�g
				 * @param shadowColor �V���h�E�̃J���[
				*/
				void SetShadowParam(
					const bool isDrawShadow,
					const float shadowOffset,
					const Vector4& shadowColor
				)
				{
					m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
				}

				/**
				 * @brief �s�{�b�g��ݒ肷��
				 * @param[in] pivot �s�{�b�g
				*/
				void SetPivot(const Vector2& pivot)
				{
					m_pivot = pivot;
				}

			private:	// private�ȃ����o�֐�
				/**
				 * @brief 2D�`��֐�
				 * @param rc �����_�����O�R���e�L�X�g
				*/
				void Render2D(RenderContext & rc);

			private:	// �f�[�^�����o
				Font m_font;											//!< �t�H���g�N���X
				CRender m_render;										//!< �����_���[�N���X
				wchar_t m_text[nsFontConstData::kTextMaxSize] = {};		//!< �\������e�L�X�g
				Vector2 m_position = Vector2::Zero;						//!< ���W
				Vector4 m_color = nsFontConstData::kDefaltTextColor;	//!< �J���[
				float m_rotation = 0.0f;								//!< ��]
				float m_scale = 1.0f;									//!< �g�嗦
				Vector2 m_pivot = Vector2::Zero;						//!< �s�{�b�g
			};

		}
	}
}
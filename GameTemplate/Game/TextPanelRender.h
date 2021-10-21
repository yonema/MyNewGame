#pragma once
#include "Render.h"
#include "DebugConstData.h"

namespace nsMyGame
{
	/**
	 * @brief �f�o�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsDebug
	{
		/**
		 * @brief �f�o�b�N�p�̃e�L�X�g�p�l���`��N���X
		*/
		class CTextPanelRender : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CTextPanelRender() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CTextPanelRender() = default;

		public:		// �I�[�o�[���C�h���������o�֐�

			/**
			 * @brief Update�̒��O�ŌĂ΂��J�n����
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool Start() override final;

			/**
			 * @brief ��������鎞�ɌĂ΂�鏈��
			*/
			void OnDestroy() override final;

			/**
			 * @brief �`��I�u�W�F�N�g�o�^�̓���
			*/
			void AddRenderEntrance() override final;

		public:		// �����o�֐�

			/**
			 * @brief �`�悷��e�L�X�g���e�L�X�g�p�l���ɒǉ�
			 * @param[in] text �e�L�X�g
			 * @param[in] headText �擪�ɕt����e�L�X�g
			*/
			void AddTextPanel(const std::wstring& text, const std::wstring& headText)
			{
				m_textPanelDatas.emplace_back(std::move(headText + text));
			}

			/**
			 * @brief 2D�`��֐�
			 * @param[in,out] rc �����_�����O�R���e�L�X�g
			*/
			void Render2D(RenderContext& rc);

		private:	// �f�[�^�����o
			std::vector<std::wstring> m_textPanelDatas;						//!< �e�L�X�g�p�l���̃f�[�^�W
			nsGraphic::CRender m_render;									//!< �����_���[�N���X
			float m_textScale = nsDebugConstData::kDefaultTextScale;		//!< �e�L�X�g�̊g�嗦
			Font m_font;													//!< �t�H���g
			Vector2 m_textPanelPosition = nsDebugConstData::kDefaultTextPosition;	//!< �e�L�X�g�p�l���̍��W
			bool m_isShadow = false;										//!< �V���h�E��`�悷�邩�H
			float m_shadowOffset = nsDebugConstData::kDefaultTextShadowOffset;	//!< �V���h�E�̃I�t�Z�b�g
		};

	}
}
#pragma once

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �����_���[�N���X
		*/
		class CRender
		{
		private:	// �G�C���A�X�錾
			using RenderFunc = std::function<void(RenderContext&)>;		// �����_�����O�p�֐��̌^

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			CRender() = default;	// �R���X�g���N�^
			~CRender() = default;	// �f�X�g���N�^

		public:		// �����o�֐�

			/**
			 * @brief GBuffer�ɕ`�悷��֐���ݒ肷��
			 * @param[in] function GBuffer�ɕ`�悷��֐�
			*/
			void SetOnRenderToBGuuferFunc(const RenderFunc& function)
			{
				m_onRenderToGBufferFunc = function;
			}

			/**
			 * @brief �t�H���[�h�����_�����O������֐���ݒ肷��
			 * @param[in] function �t�H���[�h�����_�����O������֐�
			*/
			void SetOnForwardRenderFunc(const RenderFunc& function)
			{
				m_onForwardRenderFunc = function;
			}

			/**
			 * @brief 2D��`�������֐���ݒ�
			 * @param[in] function 2D��`�������֐�
			*/
			void SetOnRender2DFunc(const RenderFunc& function)
			{
				m_onRender2DFunc = function;
			}


			/**
			 * @brief GBuffer�ɕ`�悷��֐������s
			 * @param[in] rc �����_�����O�R���e�L�X�g
			*/
			void OnRenderToGBuffer(RenderContext& rc);

			/**
			 * @brief �t�H���[�h�����_�����O����֐������s
			 * @param[in] rc �����_�����O�R���e�L�X�g
			*/
			void OnForwardRender(RenderContext& rc);

			/**
			 * @brief 2D��`�������֐������s
			 * @param[in] rc �����_�����O�R���e�L�X�g
			*/
			void OnRender2D(RenderContext& rc);

		private:
			RenderFunc m_onRenderToGBufferFunc = [](RenderContext&) {};	// GBuffer�ɕ`�悷��֐�
			RenderFunc m_onForwardRenderFunc = [](RenderContext&) {};	// �t�H���[�h�����_�����O������֐�
			RenderFunc m_onRender2DFunc = [](RenderContext&) {};		// 2D��`�������֐�
		};
	}
}



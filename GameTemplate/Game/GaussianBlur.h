#pragma once
#include "GaussianBlurConstData.h"


namespace nsNinjaAttract
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{

		/**
		 * @brief �K�E�V�A���u���[�N���X
		*/
		class CGaussianBlur
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CGaussianBlur() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CGaussianBlur() = default;

		public:		// �����o�֐�

			/**
			 * @brief �������֐�
			 * @param[in,out] originalTexture �K�E�V�A���u���[��������I���W�i���e�N�X�`��
			 * @param[in] isBlurAlpha �A���t�@�l�`�����l���ɂ��u���[�������邩�H
			*/
			void Init(Texture* originalTexture, const bool isBlurAlpha = false);

			/**
			 * @brief �K�E�V�A���u���[��GPU��Ŏ��s
			 * @warning �{�֐��̌Ăяo���́ADirectX12�𗘗p�����`��R�}���h�������ɌĂяo���K�v������܂��B
			 * @param[in] rc �����_�����O�R���e�L�X�g
			 * @param[in] blurPower �u���[�̋����B�l���傫���قǃ{�P�������Ȃ�B
			*/
			void ExecuteOnGPU(RenderContext& rc, const float blurPower);

			/**
			 * @brief �{�P�e�N�X�`�����擾
			 * @return �{�P�e�N�X�`��
			*/
			Texture& GetBokeTexture()
			{
				return m_yBlurRenderTarget.GetRenderTargetTexture();
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �����_�����O�^�[�Q�b�g��������
			*/
			void InitRenderTargets();

			/**
			 * @brief �X�v���C�g��������
			 * @param isBlurAlpha �A���t�@�`�����l���ɂ��u���[�������邩�H
			*/
			void InitSprites(const bool isBlurAlpha);

			/**
			 * @brief �d�݃e�[�u�����X�V����
			 * @param blurPower �u���[�̋����B�l���傫���قǃ{�P�������Ȃ�B
			*/
			void UpdateWeightsTable(const float blurPower);

		private:	// �f�[�^�����o
			float m_weights[nsGaussianBlurConstData::kWeightsNum];	//!< �d�݃e�[�u��
			Texture* m_originalTexture = nullptr;	//!< �I���W�i���e�N�X�`��
			RenderTarget m_xBlurRenderTarget;		//!< ���{�P�摜��`�悷�郌���_�����O�^�[�Q�b�g
			RenderTarget m_yBlurRenderTarget;		//!< �c�{�P�摜��`�悷�郌���_�����O�^�[�Q�b�g
			Sprite m_xBlurSprite;					//!< ���{�P�摜��`�悷�邽�߂̃X�v���C�g
			Sprite m_yBlurSprite;					//!< �c�{�P�摜��`�悷�邽�߂̃X�v���C�g
		};

	}
}

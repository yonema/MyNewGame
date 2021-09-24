#pragma once
#include "PostEffectConstData.h"
#include "GaussianBlur.h"

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �|�X�g�G�t�F�N�g�֘A�̃l�[���X�y�[�X
		*/
		namespace nsPostEffect
		{
			/**
			 * @brief �u���[���N���X
			*/
			class CBloom : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CBloom() = default;
				/**
				 * @brief �f�X�g���N�^
				*/
				~CBloom() = default;

			public:		// �����o�֐�

				/**
				 * @brief �������֐�
				 * @param[in] mainRenderTarget ���C�������_�����O�^�[�Q�b�g
				*/
				void OnInit(RenderTarget& mainRenderTarget);

				/**
				 * @brief �`��֐�
				 * @param[in] rc �����_�����O�R���e�L�X�g
				 * @param[in] mainRenderTarget ���C�������_�����O�^�[�Q�b�g
				*/
				void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget);

			private:	// private�ȃ����o�֐�

				/**
				 * @brief �����_�����O�^�[�Q�b�g��������
				 * @param[in] mainRenderTarget ���C�������_�����O�^�[�Q�b�g
				*/
				void InitRenderTarget(const RenderTarget& mainRenderTarget);

				/**
				 * @brief �X�v���C�g�ƃK�E�V�A���u���[�̏�����
				 * @param[in] mainRenderTarget ���C�������_�����O�^�[�Q�b�g
				*/
				void InitSpriteAndGaussianBlur(RenderTarget& mainRenderTarget);

			private:	// �f�[�^�����o
				RenderTarget m_luminanceRenderTarget;	//!< �P�x���o�p�����_�����O�^�[�Q�b�g
				Sprite m_luminanceSprite;				//!< �P�x���o�p�X�v���C�g
				Sprite m_finalSprite;					//!< �ŏI�����p�X�v���C�g
				//!< �K�E�V�A���u���[�N���X
				CGaussianBlur m_gaussianBlur[nsBloomConstData::kGaussianBlurNum];
			};
		}
	}
}

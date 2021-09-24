#pragma once
#include "Noncopyable.h"
#include "PostEffectConstData.h"
#include "Bloom.h"

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
			 * @brief �|�X�g�G�t�F�N�g�N���X
			*/
			class CPostEffect : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CPostEffect() = default;
				/**
				 * @brief �f�X�g���N�^
				*/
				~CPostEffect() = default;

			public:		// �����o�֐�

				/**
				 * @brief �������֐�
				 * @param mainRenderTarget 
				*/
				void Init(RenderTarget& mainRenderTarget);

				/**
				 * @brief �`��֐�
				 * @param[in] rc �����_�����R���e�L�X�g
				 * @param[in] mainRenderTarget ���C�������_�����O�^�[�Q�b�g
				*/
				void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

			private:	// �f�[�^�����o
				CBloom m_bloom;		//!< �u���[���N���X
			};
		}
	}
}


#include "stdafx.h"
#include "PostEffect.h"

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
			 * @brief �������֐�
			 * @param mainRenderTarget
			*/
			void CPostEffect::Init(RenderTarget& mainRenderTarget)
			{
				// �u���[����������
				m_bloom.OnInit(mainRenderTarget);

				return;
			}


			/**
			 * @brief �`��֐�
			 * @param[in] rc �����_�����R���e�L�X�g
			 * @param[in] mainRenderTarget ���C�������_�����O�^�[�Q�b�g
			*/
			void CPostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
			{
				// �u���[����`��
				m_bloom.OnRender(rc, mainRenderTarget);


				return;
			}
		}
	}
}
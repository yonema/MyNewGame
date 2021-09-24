#include "stdafx.h"
#include "Render.h"

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief GBuffer�ɕ`�悷��֐������s
		 * @param[in] rc �����_�����O�R���e�L�X�g
		*/
		void CRender::OnRenderToGBuffer(RenderContext& rc)
		{
			m_onRenderToGBufferFunc(rc);

			return;
		}

		/**
		 * @brief �t�H���[�h�����_�����O����֐������s
		 * @param[in] rc �����_�����O�R���e�L�X�g
		*/
		void CRender::OnForwardRender(RenderContext& rc)
		{
			m_onForwardRenderFunc(rc);

			return;
		}

		/**
		 * @brief 2D��`�������֐������s
		 * @param[in] rc �����_�����O�R���e�L�X�g
		*/
		void CRender::OnRender2D(RenderContext& rc)
		{
			m_onRender2DFunc(rc);

			return;
		}
	}
}
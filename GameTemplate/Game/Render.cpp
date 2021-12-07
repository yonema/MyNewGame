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
		 * @brief �V���h�E�}�b�v��`�悷��֐������s
		 * @param[in] rc �����_�����O�R���e�L�X�g
		 * @param[in] ligNo ���C�g�̔ԍ�
		 * @param[in] shadowMapNo �V���h�E�}�b�v�̔ԍ�
		 * @param[in] lvpMatrix ���C�g�r���[�v���W�F�N�V�����s��
		*/
		void CRender::OnRenderShadowMap(
			RenderContext& rc,
			const int ligNo,
			const int shadowMapNo,
			const Matrix& lvpMatrix
		)
		{
			m_onRenderShadowMapFunc(rc, ligNo, shadowMapNo, lvpMatrix);

			return;
		}

		/**
		 * @brief �v���C���[��p�̃V���h�E�}�b�v��`�悷��֐������s
		 * @param[in] rc �����_�����O�R���e�L�X�g
		 * @param[in] ligNo ���C�g�̔ԍ�
		 * @param[in] lvpMatrix ���C�g�r���[�v���W�F�N�V�����s��
		*/
		void CRender::OnRenderPlayerShadowMap(RenderContext& rc, const int ligNo, const Matrix& lvpMatrix)
		{
			m_onRenderPlayerShadowMapFunc(rc, ligNo, lvpMatrix);
		}

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
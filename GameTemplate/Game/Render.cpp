#include "stdafx.h"
#include "Render.h"

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		void CRender::OnRenderToGBuffer(RenderContext& rc)
		{
			m_onRenderToGBufferFunc(rc);
		}

		void CRender::OnRender2D(RenderContext& rc)
		{
			m_onRender2DFunc(rc);
		}
	}
}
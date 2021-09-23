#include "stdafx.h"
#include "Render.h"

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
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
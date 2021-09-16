#include "stdafx.h"
#include "GameTime.h"

namespace nsMyGame
{
	/**
	 * @brief �^�C�}�[
	*/
	namespace nsTimer
	{

		void CGameTime::DrawFPS(RenderContext& rc, float elapsedTime)
		{
			//�o�ߎ��Ԃ��v������J�E���g
			static int count = 0;

			//�o�ߎ��Ԃ����Z���Ă���
			m_timeTotal += elapsedTime;
			//�J�E���g��i�߂�
			count++;

			//�J�E���g��30�܂Ői�񂾂�
			if (count == 30) {
				//����30�t���[����FPS�𕽋ς��āA
				//�\������FPS���o��
				m_fps = 1.0f / (m_timeTotal / count);

				//�o�ߎ��Ԃ̑��a�ƃJ�E���g�������l�ɖ߂�
				m_timeTotal = 0.0f;
				count = 0;

			}

			//�t�H���g�̕`��J�n
			m_font.Begin(rc);

			//������̐ݒ�
			wchar_t fps[256];
			swprintf_s(fps, L"FPS = %2.4f", m_fps);
			float w = g_camera2D->GetWidth();
			float h = g_camera2D->GetHeight();
			//�`��
			m_font.Draw(
				fps,
				{
					w * -0.5f,
					h * 0.5f
				},
				Vector4::White,
				0.0f,
				1.0f,
				{ 0.0f, 1.0f }
			);
			//�`��I��
			m_font.End(rc);
		}
	}
}
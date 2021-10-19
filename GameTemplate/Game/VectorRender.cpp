#include "stdafx.h"
#include "VectorRender.h"
#include "ModelRender.h"
#include "RenderingEngine.h"


namespace nsMyGame
{
	/**
	 * @brief �f�o�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsDebug
	{
		// �f�o�b�N�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsDebugConstData;
		/**
		 * @brief Update�̒��O�ŌĂ΂��J�n����
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CVectorRender::Start()
		{
			// ���f�������_���[�̐����Ə�����
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			m_modelRender->Init(kVectorModel);
			// ������
			m_modelRender->Deactivate();

			// �����_���[�N���X��2D�`��֐���ݒ肷��
			m_render.SetOnRender2DFunc([&](RenderContext& rc) { this->Render2D(rc); });

			// �t�H���g�̃V���h�E�p�����[�^��ݒ�
			m_font.SetShadowParam(true, kVectorFontShadowOffset, Vector4::Black);

			return true;
		}

		/**
		 * @brief ��������鎞�ɌĂ΂�鏈��
		*/
		void CVectorRender::OnDestroy()
		{
			DeleteGO(m_modelRender);

			return;
		}

		/**
		 * @brief ��ɌĂ΂��X�V����
		*/
		void CVectorRender::AlwaysUpdate()
		{
			// �x�N�g�������_���[�̃f�[�^�W���󂩁H
			if (m_vectorRenderDatas.empty())
			{
				// ��̂��߁A������
				m_modelRender->Deactivate();

				// �������^�[��
				return;
			}

			// �L����
			m_modelRender->Activate();
			
			// �x�N�g�������_���[�̃f�[�^�W���炷�ׂĎ��o���āA���f���̃p�����[�^���X�V����
			for (const auto& vectorRenderData : m_vectorRenderDatas)
			{
				// ��]
				Quaternion rot;
				// �x�N�g���̌����ɉ�]������
				rot.SetRotation(Vector3::AxisZ, vectorRenderData.vector);

				// �g�嗦
				// ���݂̃x�N�g�����f���̒�����100�̂��߁A100�ŏ��Z
				float scale = vectorRenderData.vector.Length() / kVectorModelLength;
				// ���ƍ����̔{���͉�����
				float wideAndHeight = scale * kVectorModelWidthAndHeightScale;

				// �x�N�g�����f���̃g�����X�t�H�[����ݒ肷��
				m_modelRender->SetPosition(vectorRenderData.origin);
				m_modelRender->SetRotation(rot);
				m_modelRender->SetScale({ wideAndHeight, wideAndHeight, scale });
			}

			return;
		}

		/**
		 * @brief �`��I�u�W�F�N�g�o�^�̓���
		*/
		void CVectorRender::AddRenderEntrance()
		{
			// �`��I�u�W�F�N�g�ɓo�^����
			nsMyEngine::CRenderingEngine::GetInstance()->AddRenderObject(&m_render);
		}

		/**
		 * @brief 2D�`��֐�
		 * @param[in,out] rc �����_�����O�R���e�L�X�g
		*/
		void CVectorRender::Render2D(RenderContext& rc)
		{
			// �t�H���g�`��J�n
			m_font.Begin(rc);

			// �x�N�g�������_���[�̃f�[�^�W���炷�ׂĎ��o���āA�t�H���g�̃p�����[�^���X�V����
			for (const auto& vectorRenderData : m_vectorRenderDatas)
			{
				Vector4 pos;
				pos.Set(vectorRenderData.origin);
				const auto& vpMatrix = g_camera3D->GetViewProjectionMatrix();
				vpMatrix.Apply(pos);
				pos.x /= pos.w;
				pos.y /= pos.w;
				pos.z /= pos.w;

				//�X�N���[����Ԃɕϊ�����B
				float half_w = g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
				float half_h = g_graphicsEngine->GetFrameBufferHeight() * 0.5f;

				pos.x *= half_w;
				pos.y *= half_h;

				// �x�N�g���̃e�L�X�g�\���p�p�����[�^
				const Vector2 pivot = { 0.0f,1.0f };
				wchar_t text[256];
				wchar_t wc[126];
				mbstowcs(wc, vectorRenderData.name.c_str(), vectorRenderData.name.length() + 1);
				Vector2 textPos = { pos.x, pos.y - 4.0f };
				float between = 40.0f * m_fontScale;

				// �x�N�g���̖��O��\��
				swprintf_s(text, L"%s", wc);
				m_font.Draw(text, textPos, Vector4::White, 0.0f, m_fontScale, pivot);

				//�x�N�g���f�[�^��\���B
				memset(text, 0, sizeof(text));
				textPos.y -= between;
				swprintf_s(
					text,
					L"vec[ %.3f, %.3f, %.3f ]",
					vectorRenderData.vector.x,
					vectorRenderData.vector.y,
					vectorRenderData.vector.z
				);
				m_font.Draw(text, textPos, Vector4::White, 0.0f, m_fontScale, pivot);

				//�����f�[�^��\���B
				auto dir = vectorRenderData.vector;
				textPos.y -= between;
				dir.Normalize();
				memset(text, 0, sizeof(text));
				swprintf_s(
					text,
					L"dir[ %.3f, %.3f, %.3f ]",
					dir.x,
					dir.y,
					dir.z
				);
				m_font.Draw(text, textPos, Vector4::White, 0.0f, m_fontScale, pivot);

				//�X�J���[��\���B
				memset(text, 0, sizeof(text));
				textPos.y -= between;
				swprintf_s(
					text,
					L"scalar[ %.3f ]",
					vectorRenderData.vector.Length()
				);
				m_font.Draw(text, textPos, Vector4::White, 0.0f, m_fontScale, pivot);
			}

			// �t�H���g�̕`��I��
			m_font.End(rc);
			
			// �x�N�g���̃f�[�^�W���N���A����
			m_vectorRenderDatas.clear();
			return;
		}

	}
}
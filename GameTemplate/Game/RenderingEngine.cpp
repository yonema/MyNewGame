#include "stdafx.h"
#include "RenderingEngine.h"
#include "system/system.h"
#include "Render.h"

namespace nsMyGame
{

	/**
	 * @brief ���̃Q�[���p�̃Q�[���G���W���l�[���X�y�[�X
	*/
	namespace nsMyEngine
	{
		CRenderingEngine* CRenderingEngine::m_instance = nullptr;	// �B��̃C���X�^���X

		/**
		 * @brief �R���X�g���N�^
		*/
		CRenderingEngine::CRenderingEngine()
		{
			m_renderObjects.reserve(m_kRenderObjectsCapacityNum);

			return;
		}
		/**
		 * @brief �f�X�g���N�^
		*/
		CRenderingEngine::~CRenderingEngine()
		{
			return;
		}

		/**
		 * @brief �����_�����O�G���W�������s
		*/
		void CRenderingEngine::Execute()
		{
			// �����_�����O�R���e�L�X�g
			RenderContext& rc = g_graphicsEngine->GetRenderContext();

			// �`��I�u�W�F�N�g�̓o�^
			GameObjectManager::GetInstance()->ExecuteAddRender();

			// GBuffer�ɕ`�悷��
			RenderToGBuffer(rc);

			// �G�t�F�N�g�̕`��
			EffectEngine::GetInstance()->Draw();

			// 2D��`�悷��
			Render2D(rc);

			m_renderObjects.clear();

			return;
		}

		/**
		 * @brief GBuffer�ɕ`�悷��
		 * @param rc �����_�����O�R���e�L�X�g
		*/
		void CRenderingEngine::RenderToGBuffer(RenderContext& rc)
		{
			for (nsGraphic::CRender* renderObject : m_renderObjects)
			{
				renderObject->OnRenderToGBuffer(rc);
			}
			return;
		}

		/**
		 * @brief 2D��`�悷��
		 * @param rc �����_�����O�R���e�L�X�g
		*/
		void CRenderingEngine::Render2D(RenderContext& rc)
		{
			for (nsGraphic::CRender* renderObject : m_renderObjects)
			{
				renderObject->OnRender2D(rc);
			}
			return;
		}
	}
}
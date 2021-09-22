#include "stdafx.h"
#include "RenderingEngine.h"
#include "system/system.h"

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

			// �Q�[���I�u�W�F�N�g�̕`��
			GameObjectManager::GetInstance()->ExecuteRender(rc);
			// �G�t�F�N�g�̕`��
			EffectEngine::GetInstance()->Draw();

			return;
		}
	}
}
#include "stdafx.h"
#include "DebugManager.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �f�o�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsDebug
	{
		CDebugManager* CDebugManager::m_instance = nullptr;

		/**
		 * @brief �R���X�g���N�^
		*/
		CDebugManager::CDebugManager()
		{
#ifdef MY_DEBUG
			// �x�N�g���`��N���X�̐���
			m_vectorRender = NewGO<CVectorRender>(nsCommonData::enPriorityFinal);
			// �e�L�X�g�p�l���`��N���X�̐���
			m_textPanelRender = NewGO<CTextPanelRender>(nsCommonData::enPriorityFinal);
#endif

			return;
		}
		/**
		 * @brief �f�X�g���N�^
		*/
		CDebugManager::~CDebugManager()
		{

			return;
		}

		/**
		 * @brief ���O�̔j������
		*/
		void CDebugManager::PreDelete()
		{
#ifdef MY_DEBUG
			DeleteGO(m_vectorRender);
			DeleteGO(m_textPanelRender);
#endif


			return;
		}

	}
};
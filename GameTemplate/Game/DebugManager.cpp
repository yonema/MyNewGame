#include "stdafx.h"
#include "DebugManager.h"

namespace nsMyGame
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
			m_vectorRender = NewGO<CVectorRender>(nsCommonData::enPriorityFinal);
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
#endif


			return;
		}

	}
};
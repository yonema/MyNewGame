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
			m_vectorRender = NewGO<CVectorRender>(nsCommonData::enPriorityFinal);

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
			DeleteGO(m_vectorRender);


			return;
		}

	}
};
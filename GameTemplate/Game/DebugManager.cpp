#include "stdafx.h"
#include "DebugManager.h"

namespace nsMyGame
{
	/**
	 * @brief デバック関連のネームスペース
	*/
	namespace nsDebug
	{
		CDebugManager* CDebugManager::m_instance = nullptr;

		/**
		 * @brief コンストラクタ
		*/
		CDebugManager::CDebugManager()
		{
			m_vectorRender = NewGO<CVectorRender>(nsCommonData::enPriorityFinal);

			return;
		}
		/**
		 * @brief デストラクタ
		*/
		CDebugManager::~CDebugManager()
		{

			return;
		}

		/**
		 * @brief 事前の破棄処理
		*/
		void CDebugManager::PreDelete()
		{
			DeleteGO(m_vectorRender);


			return;
		}

	}
};
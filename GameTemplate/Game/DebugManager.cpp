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
#ifdef MY_DEBUG
			m_vectorRender = NewGO<CVectorRender>(nsCommonData::enPriorityFinal);
#endif

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
#ifdef MY_DEBUG
			DeleteGO(m_vectorRender);
#endif


			return;
		}

	}
};
#include "stdafx.h"
#include "DebugManager.h"

namespace nsNinjaAttract
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
			// ベクトル描画クラスの生成
			m_vectorRender = NewGO<CVectorRender>(nsCommonData::enPriorityFinal);
			// テキストパネル描画クラスの生成
			m_textPanelRender = NewGO<CTextPanelRender>(nsCommonData::enPriorityFinal);
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
			DeleteGO(m_textPanelRender);
#endif


			return;
		}

	}
};
#include "stdafx.h"
#include "TestMap.h"
#include "ModelRender.h"

namespace nsMyGame
{
	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		/**
		 * @brief スタート関数
		 * @return アップデートを行うか？
		*/
		bool CTestMap::Start()
		{
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			m_modelRender->Init("Assets/modelData/unityChan.tkm");

			return true;
		}

		/**
		 * @brief 破棄した時に呼ばれる
		*/
		void CTestMap::OnDestroy()
		{
			DeleteGO(m_modelRender);
			return;
		}

		/**
		 * @brief アップデート関数
		*/
		void CTestMap::Update()
		{
			return;
		}
	}
}
#include "stdafx.h"
#include "Game.h"
#include "TestMap.h"
#include "TestMapForSpotLight.h"
#include "CTestMapForShadow.h"
#include "DirectionalLight.h"

/**
 * @brief このゲームのネームスペース
 * @todo MyGameをもっとユニークなゲームのタイトル名とかに変更する
*/
namespace nsMyGame
{
	/**
	 * @brief ゲームファイルのネームスペース
	*/
	namespace nsGame
	{
		/**
		 * @brief スタート関数
		 * @return アップデートを行うか？
		*/
		bool CGame::Start()
		{
			m_gameDirectionalLight = NewGO<nsLight::CDirectionalLight>(nsCommonData::enPriorityFirst);
			//m_gameDirectionalLight->SetDirection({ 1.0f,0.0f,0.0f });
			//m_gameDirectionalLight->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			//NewGO<nsMaps::nsTestMaps::CTestMap>(nsCommonData::enPriorityFirst, "testMap");
			//NewGO<nsMaps::nsTestMaps::CTestMapForSpotLight>(nsCommonData::enPriorityFirst, "testMap");
			NewGO<nsMaps::nsTestMaps::CTestMapForShadow>(nsCommonData::enPriorityFirst, "testMap");

			return true;
		}

		/**
		 * @brief 破棄されるときに呼ばれる関数
		*/
		void CGame::OnDestroy()
		{
			//DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMap>("testMap"));
			//DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMapForSpotLight>("testMap"));
			DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMapForShadow>("testMap"));

			DeleteGO(m_gameDirectionalLight);

			return;
		}
	}
}
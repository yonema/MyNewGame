#include "stdafx.h"
#include "Game.h"
#include "TestMap.h"
#include "TestMapForSpotLight.h"
#include "CTestMapForShadow.h"
#include "TestMapForLevel3D.h"
#include "TestMapForPlayerMove.h"
#include "ProtoMap.h"
#include "AlphaMap.h"
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

		const char* const CGame::m_kMapObjName = "map";
		const Vector3 CGame::m_kGameDirectionalLightDir = { -0.3f,-1.0f,0.3f };
		const Vector4 CGame::m_kGameDirectionalLightColor = { 4.0f,4.0f,4.0f,1.0f };

		/**
		 * @brief スタート関数
		 * @return アップデートを行うか？
		*/
		bool CGame::Start()
		{
			m_gameDirectionalLight = NewGO<nsLight::CDirectionalLight>(nsCommonData::enPriorityFirst);
			Vector3 dir = m_kGameDirectionalLightDir;
			dir.Normalize();
			m_gameDirectionalLight->SetDirection(dir);
			m_gameDirectionalLight->SetColor(m_kGameDirectionalLightColor);

			m_mapNum = m_kCurrentMapNum;

			// 一番目の優先度
			using nsCommonData::enPriorityFirst;

			switch (m_mapNum)
			{
			case 0:
				NewGO<nsMaps::nsTestMaps::CTestMap>(enPriorityFirst, m_kMapObjName);
				break;
			case 1:
				NewGO<nsMaps::nsTestMaps::CTestMapForSpotLight>(enPriorityFirst, m_kMapObjName);
				break;
			case 2:
				NewGO<nsMaps::nsTestMaps::CTestMapForShadow>(enPriorityFirst, m_kMapObjName);
				break;
			case 3:
				NewGO<nsMaps::nsTestMaps::CTestMapForLevel3D>(enPriorityFirst, m_kMapObjName);
				break;
			case 4:
				NewGO<nsMaps::nsTestMaps::CTestMapForPlayerMove>(enPriorityFirst, m_kMapObjName);
				break;
			case 5:
				NewGO<nsMaps::nsProtoMaps::CProtoMap>(enPriorityFirst, m_kMapObjName);
				break;
			case 6:
				NewGO<nsMaps::nsAlphaMaps::CAlphaMap>(enPriorityFirst, m_kMapObjName);
				break;
			default:
				break;
			}


			return true;
		}

		/**
		 * @brief 破棄されるときに呼ばれる関数
		*/
		void CGame::OnDestroy()
		{

			DeleteGO(m_gameDirectionalLight);

			switch (m_mapNum)
			{
			case 0:
				DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMap>(m_kMapObjName));
				break;
			case 1:
				DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMapForSpotLight>(m_kMapObjName));
				break;
			case 2:
				DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMapForShadow>(m_kMapObjName));
				break;
			case 3:
				DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMapForLevel3D>(m_kMapObjName));
				break;
			case 4:
				DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMapForPlayerMove>(m_kMapObjName));
				break;
			case 5:
				DeleteGO(FindGO<nsMaps::nsProtoMaps::CProtoMap>(m_kMapObjName));
				break;
			case 6:
				DeleteGO(FindGO<nsMaps::nsAlphaMaps::CAlphaMap>(m_kMapObjName));
				break;
			default:
				break;
			}

			return;
		}
	}
}
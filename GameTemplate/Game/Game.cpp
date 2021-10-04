#include "stdafx.h"
#include "Game.h"
#include "TestMap.h"
#include "TestMapForSpotLight.h"
#include "CTestMapForShadow.h"
#include "TestMapForLevel3D.h"
#include "TestMapForPlayerMove.h"
#include "DirectionalLight.h"

/**
 * @brief ���̃Q�[���̃l�[���X�y�[�X
 * @todo MyGame�������ƃ��j�[�N�ȃQ�[���̃^�C�g�����Ƃ��ɕύX����
*/
namespace nsMyGame
{
	/**
	 * @brief �Q�[���t�@�C���̃l�[���X�y�[�X
	*/
	namespace nsGame
	{
		/**
		 * @brief �X�^�[�g�֐�
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CGame::Start()
		{
			m_gameDirectionalLight = NewGO<nsLight::CDirectionalLight>(nsCommonData::enPriorityFirst);
			//m_gameDirectionalLight->SetDirection({ 1.0f,0.0f,0.0f });
			//m_gameDirectionalLight->SetColor({ 1.0f,1.0f,1.0f,1.0f });

			m_mapNum = 4;

			// ��Ԗڂ̗D��x
			using nsCommonData::enPriorityFirst;

			switch (m_mapNum)
			{
			case 0:
				NewGO<nsMaps::nsTestMaps::CTestMap>(enPriorityFirst, m_mapName);
				break;
			case 1:
				NewGO<nsMaps::nsTestMaps::CTestMapForSpotLight>(enPriorityFirst, m_mapName);
				break;
			case 2:
				NewGO<nsMaps::nsTestMaps::CTestMapForShadow>(enPriorityFirst, m_mapName);
				break;
			case 3:
				NewGO<nsMaps::nsTestMaps::CTestMapForLevel3D>(enPriorityFirst, m_mapName);
				break;
			case 4:
				NewGO<nsMaps::nsTestMaps::CTestMapForPlayerMove>(enPriorityFirst, m_mapName);
				break;
			default:
				break;
			}


			return true;
		}

		/**
		 * @brief �j�������Ƃ��ɌĂ΂��֐�
		*/
		void CGame::OnDestroy()
		{

			DeleteGO(m_gameDirectionalLight);

			switch (m_mapNum)
			{
			case 0:
				DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMap>(m_mapName));
				break;
			case 1:
				DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMapForSpotLight>(m_mapName));
				break;
			case 2:
				DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMapForShadow>(m_mapName));
				break;
			case 3:
				DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMapForLevel3D>(m_mapName));
				break;
			case 4:
				DeleteGO(FindGO<nsMaps::nsTestMaps::CTestMapForPlayerMove>(m_mapName));
				break;
			default:
				break;
			}

			return;
		}
	}
}
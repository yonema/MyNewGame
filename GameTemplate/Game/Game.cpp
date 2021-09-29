#include "stdafx.h"
#include "Game.h"
#include "TestMap.h"
#include "TestMapForSpotLight.h"
#include "CTestMapForShadow.h"
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
			//NewGO<nsMaps::nsTestMaps::CTestMap>(nsCommonData::enPriorityFirst, "testMap");
			//NewGO<nsMaps::nsTestMaps::CTestMapForSpotLight>(nsCommonData::enPriorityFirst, "testMap");
			NewGO<nsMaps::nsTestMaps::CTestMapForShadow>(nsCommonData::enPriorityFirst, "testMap");

			return true;
		}

		/**
		 * @brief �j�������Ƃ��ɌĂ΂��֐�
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
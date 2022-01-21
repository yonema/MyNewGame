#include "stdafx.h"
#include "Game.h"
#include "TestMap.h"
#include "TestMapForSpotLight.h"
#include "CTestMapForShadow.h"
#include "TestMapForLevel3D.h"
#include "TestMapForPlayerMove.h"
#include "ProtoMap.h"
#include "AlphaMap.h"
#include "MainMap.h"
#include "TitleMap.h"
#include "DirectionalLight.h"
#include "RenderingEngine.h"
#include "Fade.h"

/**
 * @brief ���̃Q�[���̃l�[���X�y�[�X
*/
namespace nsNinjaAttract
{
	/**
	 * @brief �Q�[���t�@�C���̃l�[���X�y�[�X
	*/
	namespace nsGame
	{

		const char* const CGame::m_kMapObjName = "map";
		const Vector3 CGame::m_kGameDirectionalLightDir = { -0.3f,-1.0f,0.3f };
		const Vector4 CGame::m_kGameDirectionalLightColor = { 2.0f,2.0f,2.0f,1.0f };

		/**
		 * @brief �X�^�[�g�֐�....................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................
		 * @return �A�b�v�f�[�g���s�����H
		*/
		bool CGame::Start()
		{
			// �t�F�[�h��^���Âɂ���
			nsMyEngine::CRenderingEngine::GetInstance()->GetFade()->BlackOut();

			// �f�B���N�V�������C�g�𐶐�����
			m_gameDirectionalLight = NewGO<nsLight::CDirectionalLight>(nsCommonData::enPriorityFirst);
			Vector3 dir = m_kGameDirectionalLightDir;
			dir.Normalize();
			m_gameDirectionalLight->SetDirection(dir);
			m_gameDirectionalLight->SetColor(m_kGameDirectionalLightColor);

			// �^�C�g���}�b�v�𐶐�
			NewGO<nsMaps::CTitleMap>(
				nsCommonData::enPriorityFirst,
				nsCommonData::kGameObjectName[nsCommonData::enGN_TitleMap]
				);


			return true;

			m_mapNum = m_kCurrentMapNum;

			// ��Ԗڂ̗D��x
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
			case 7:
				NewGO<nsMaps::CMainMap>(enPriorityFirst, m_kMapObjName);
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

			// �^�C�g�����}�b�v������΁A�j������B
			QueryGOs<nsMaps::CTitleMap>(
				nsCommonData::kGameObjectName[nsCommonData::enGN_TitleMap],
				[&](nsMaps::CTitleMap* titleMap)->bool
				{
					DeleteGO(titleMap);
					return true;
				}
			);

			// ���C���}�b�v������΁A�j������
			QueryGOs<nsMaps::CMainMap>(
				nsCommonData::kGameObjectName[nsCommonData::enGN_MainMap],
				[&](nsMaps::CMainMap* mainMap)->bool
				{
					DeleteGO(mainMap);
					return true;
				}
			);

			return;

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
			case 7:
				DeleteGO(FindGO<nsMaps::CMainMap>(m_kMapObjName));
				break;
			default:
				break;
			}

			return;
		}
	}
}
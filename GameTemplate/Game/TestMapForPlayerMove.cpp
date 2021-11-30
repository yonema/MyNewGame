#include "stdafx.h"
#include "TestMapForPlayerMove.h"
#include "SkyCube.h"
#include "Player.h"
#include "Buildings.h"
#include "BuildingConstData.h"

namespace nsMyGame
{
	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		/**
		 * @brief �e�X�g�}�b�v�i���x���j�p�̃l�[���X�y�[�X
		*/
		namespace nsTestMaps
		{

			const char* const CTestMapForPlayerMove::m_kBuildingName = "testBuilding";
			const char* const CTestMapForPlayerMove::m_kLevelFilePaht = "Assets/levelData/testLevel.tkl";

			/**
			 * @brief �X�^�[�g�֐�
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool CTestMapForPlayerMove::Start()
			{
				// �X�J�C�L���[�u�̐����Ə�����
				m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
				m_skyCube->Init(nsNature::nsSkyCubeConstData::enSkyCubeType_day);

				// �J�����̉����ʂ�ݒ�
				g_camera3D->SetFar(40000.0f);
				int a = 0;

				// �����̖��O�̃��C�h������
				wchar_t buildingNameW[32];
				mbstowcs(buildingNameW, m_kBuildingName, sizeof(m_kBuildingName));

				// ���x���̐���
				m_level3D.Init(
					m_kLevelFilePaht,
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						if (objData.EqualObjectName(buildingNameW))
						{
							if (a != 2)
							{
								a++;
								return false;
							}
							nsBuilding::CBuildings* building = 
								NewGO<nsBuilding::CBuildings>(nsCommonData::enPriorityFirst, m_kBuildingName);
							//building->Init(
							//	nsBuilding::nsBuildingConstData::enBuilding_L_01a,
							//	objData.position,
							//	objData.rotation
							//);
							a++;
							return true;
						}

						return false;
					}
				);

				// �v���C���[�̐���
				m_player = NewGO<nsPlayer::CPlayer>(nsCommonData::enPriorityFirst);
				m_player->SetPosition({ 0.0f,500.0f,0.0f });

				return true;
			}

			/**
			 * @brief �j���������ɌĂ΂��֐�
			*/
			void CTestMapForPlayerMove::OnDestroy()
			{
				DeleteGO(m_skyCube);	// �X�J�C�L���[�u�N���X�̔j��
				DeleteGO(m_player);		// �v���C���[�N���X�̔j��

				// ���������ׂĔj��
				QueryGOs<nsBuilding::CBuildings>(
					m_kBuildingName,
					[&](nsBuilding::CBuildings* building)->bool
					{
						DeleteGO(building);
						return true;
					}
				);

				return;
			}

			/**
			 * @brief �A�b�v�f�[�g�֐�
			*/
			void CTestMapForPlayerMove::Update()
			{
				return;
			}


		}
	}
}
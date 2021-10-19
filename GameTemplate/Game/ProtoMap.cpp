#include "stdafx.h"
#include "ProtoMap.h"
#include "SkyCube.h"
#include "Player.h"
#include "Building.h"
#include "BuildingConstData.h"
#include "MapConstDatah.h"
#include "GameMainState.h"
#include "Goal.h"


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
		namespace nsProtoMaps
		{
			// �}�b�v�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsMapConstData;

			/**
			 * @brief �X�^�[�g�֐�
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool CProtoMap::Start()
			{
				// �Q�[���X�e�[�g�̐���
				m_gameState = NewGO<nsGameState::CGameMainState>(nsCommonData::enPriorityFirst);

				// �X�J�C�L���[�u�̐����Ə�����
				m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
				m_skyCube->Init(nsNature::nsSkyCubeConstData::enSkyCubeType_day);


				// �����̖��O�̃��C�h������
				wchar_t buildingNameW[32];
				mbstowcs(buildingNameW, kBuildingName, sizeof(kBuildingName));

				//m_stage = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
				//m_stage->Init("Assets/modelData/city2/city.tkm");

				// ���x���̐���
				m_level3D.Init(
					kLevelFilePath[enLevelProto],
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						// �����̐���
						if (objData.EqualObjectName(kBuildingName))
						{

							nsBuilding::CBuilding* building =
								NewGO<nsBuilding::CBuilding>(nsCommonData::enPriorityFirst, kBuildingName);
							building->Init(
								nsBuilding::nsBuildingConstData::enTestBuilding,
								objData.position,
								objData.rotation
							);
							return true;
						}
						// �v���C���[�̐���
						else if (objData.EqualObjectName(kPlayerName))
						{
							m_player = NewGO<nsPlayer::CPlayer>(nsCommonData::enPriorityFirst);
							m_player->SetPosition(objData.position);
							m_player->SetRotation(objData.rotation);

							return true;
						}
						// �S�[���̐���
						else if (objData.EqualObjectName(kGoalName))
						{
							m_goal = NewGO <nsGoal::CGoal>(nsCommonData::enPriorityFirst);
							m_goal->Init(objData.position, objData.rotation, objData.scale, *m_player);

							return true;
						}

						return false;
					}
				);


				// �^�C�}�[�̌v�����n�߂�
				m_gameState->StartTimingGame();

				return true;
			}

			/**
			 * @brief �j���������ɌĂ΂��֐�
			*/
			void CProtoMap::OnDestroy()
			{
				DeleteGO(m_skyCube);	// �X�J�C�L���[�u�N���X�̔j��
				DeleteGO(m_player);		// �v���C���[�N���X�̔j��
				DeleteGO(m_goal);		// �S�[����j��

				// ���������ׂĔj��
				QueryGOs<nsBuilding::CBuilding>(
					kBuildingName,
					[&](nsBuilding::CBuilding* building)->bool
					{
						DeleteGO(building);
						return true;
					}
				);

				DeleteGO(m_gameState);

				return;
			}

			/**
			 * @brief �A�b�v�f�[�g�֐�
			*/
			void CProtoMap::Update()
			{
				return;
			}
		}
	}
}
#include "stdafx.h"
#include "AlphaMap.h"
#include "SkyCube.h"
#include "Player.h"
#include "Buildings.h"
#include "BuildingConstData.h"
#include "GameMainState.h"
#include "Goal.h"
#include "AICar.h"
#include "BGM.h"

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
		namespace nsAlphaMaps
		{
			// �}�b�v�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsMapConstData;

			/**
			 * @brief �X�^�[�g�֐�
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool CAlphaMap::Start()
			{
				// �Q�[���X�e�[�g�̐���
				m_gameState = NewGO<nsGameState::CGameMainState>(
					nsCommonData::enPriorityFirst,
					nsCommonData::kGameObjectName[nsCommonData::enMainGameState]
					);

				// �X�J�C�L���[�u�̐����Ə�����
				m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
				m_skyCube->Init(nsNature::nsSkyCubeConstData::enSkyCubeType_day);

				// �v���C���[�̐���
				// �����D��x�����Ȃ��ƁA���̈ʒu�������B
				m_player = NewGO<nsPlayer::CPlayer>(nsCommonData::enPrioritySecond);

				// �����̐���
				m_buildings = NewGO<nsBuilding::CBuildings>(nsCommonData::enPriorityFirst);

				// �Q�[���X�e�[�g�̏�����
				m_gameState->Init(*m_player);

				//constexpr int kCarNum = 1;
				constexpr int kCarNum = 3;
				nsAICharacter::CAICar* cars[kCarNum] = {};
				for (int i = 0; i < kCarNum; i++)
				{
					cars[i] = NewGO<nsAICharacter::CAICar>(nsCommonData::enPriorityFirst, "Car");
					cars[i]->Init(
						m_aiField.GetAICharaInitData(),
						//(*m_aiField.GetAICharaInitData().naviMeshTargetPointsRef)[i],
						{ -520.0f + 11200.0f * i,0.0f,4200.0f },
						Quaternion::Identity,
						Vector3::One
					);
				}


				// ���x���̐���
				m_level3D.Init(
					kLevelFilePath[enLevelAlpha2],
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						// �����̐���
						if (objData.ForwardMatchName(nsBuilding::nsBuildingConstData::kBuildingForwardName))
						{
							//return true;
							// �����̃^�C�v�̐��̕��A������܂őS�����ׂ�
							for (int i = 0; i < nsBuilding::nsBuildingConstData::enBuildingTypeNum; i++)
							{
								if (objData.EqualObjectName(
									nsBuilding::nsBuildingConstData::kBuildingNames[i]) != true
									)
								{
									// ���O����v���Ȃ���Ύ���
									continue;
								}

								// �����̃^�C�v���w�肵�Ēǉ�
								m_buildings->AddBuliding(
									static_cast<nsBuilding::nsBuildingConstData::EnBuildingType>(i),
									objData.position,
									objData.rotation,
									objData.scale
								);
								return true;
							}

							MessageBoxA(nullptr, "Building�̖��O���Ȃ�", "�x��", MB_OK);

							return true;
						}
						// �v���C���[�̐���
						else if (objData.EqualObjectName(kPlayerName))
						{
							m_player->SetPosition(objData.position);
							m_player->SetRotation(objData.rotation);

							return true;
						}
						// �S�[���̐���
						else if (objData.EqualObjectName(kGoalName))
						{
							return true;
							m_goal = NewGO <nsGoal::CGoal>(nsCommonData::enPriorityFirst);
							m_goal->Init(objData.position, objData.rotation, objData.scale, *m_player);

							return true;
						}

						return false;
					}
				);

				// ������������
				m_buildings->Init();

				// ���������x�����琶��
				for (int propsType = 0; propsType < enPropsTypeNum; propsType++)
				{
					break;
					// �����p�̃��x����������
					m_propsLevel3D[propsType].Init(
						kPropsLevelFilePath[propsType],
						[&](nsLevel3D::SLevelObjectData& objData)
						{
							// �}�b�v�`�b�v�̗\�񐔂�ݒ肷��
							objData.numMapChipReserve = kNumMapChipReserveTbl[propsType];
							// ���[�U�[��`�̃R���W���������������p�̑����ɐݒ肷��
							objData.userIndex = EnCollisionAttr::enCollisionAttr_Props;
							// �V���h�E�L���X�^�[�ɂ���
							//objData.shadowCaster = true;


							switch (propsType)
							{
							// �R�R�Ŏw�肵�����̂����������
							case enPropsPedestrianLight:	// ���s�җp�M���@
								return false;
								break;

							case enPropsStreetLight:		// �X��
								objData.lodModelFilePath = "Assets/modelData/levelSource/StreetLight_LOD.tkm";
								objData.distanceLOD = 1000.0f;
								return false;
								break;

							case enPropsTrafficLight:		// �M���@
								objData.lodModelFilePath = "Assets/modelData/levelSource/TrafficLight_LOD.tkm";
								objData.distanceLOD = 1000.0f;
								return false;
								break;

							case enPropsStreetTree:			// �X�H��
								objData.lodModelFilePath = "Assets/modelData/levelSource/StreetTree_LOD.tkm";
								return false;
								break;

							// �X�H���̎}�͔������ŕ`�悷��
							case enPropsStreetTreeBranch:
								objData.isTranslucent = true;
								objData.priority = nsCommonData::enPrioritySecond;
								objData.lodModelFilePath = "Assets/modelData/levelSource/StreetTree_Branch_LOD.tkm";
								objData.distanceLOD = 10000.0f;

								return false;
								break;

							// �w�肵�Ȃ������琶������Ȃ�
							default:
								return true;
								break;
							}
						}
					);					

				}

				// BGM�N���X�̐����Ə�����
				m_bgm = NewGO<nsBGM::CBGM>(nsCommonData::enPriorityFirst);
				m_bgm->Init(*m_player);

				// �^�C�}�[�̌v�����n�߂�
				m_gameState->StartTimingGame();

				return true;
			}

			/**
			 * @brief �j���������ɌĂ΂��֐�
			*/
			void CAlphaMap::OnDestroy()
			{
				DeleteGO(m_skyCube);	// �X�J�C�L���[�u�N���X�̔j��
				DeleteGO(m_player);		// �v���C���[�N���X�̔j��
				DeleteGO(m_goal);		// �S�[����j��
				DeleteGO(m_gameState);	// �Q�[���X�e�[�g��j��
				DeleteGO(m_buildings);	// ������j��
				DeleteGO(m_bgm);
				
				QueryGOs<nsAICharacter::CAICar>(
					"Car",
					[&](nsAICharacter::CAICar* car)->bool
					{
						DeleteGO(car);
						return true;
					}
				);

				return;
			}

			/**
			 * @brief �A�b�v�f�[�g�֐�
			*/
			void CAlphaMap::Update()
			{
				return;
			}
		}
	}
}
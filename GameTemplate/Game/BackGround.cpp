#include "stdafx.h"
#include "BackGround.h"
#include "Level3D.h"
#include "SkyCube.h"
#include "Buildings.h"


namespace nsNinjaAttract
{
	using namespace nsMaps::nsMapConstData;

	/**
	 * @brief �w�i�X�e�[�W�p�̃l�[���X�y�[�X
	*/
	namespace nsBackGround
	{
		/**
		 * @brief �R���X�g���N�^
		*/
		CBackGround::CBackGround()
		{
			// ���x���𐶐�����
			m_buildingLevel3D = std::make_unique<nsLevel3D::CLevel3D>();
			for (auto& propsLevel : m_propsLevel3D)
			{
				propsLevel = std::make_unique<nsLevel3D::CLevel3D>();
			}

			// �X�J�C�L���[�u�̐����Ə�����
			m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
			m_skyCube->Init(nsNature::nsSkyCubeConstData::enSkyCubeType_day);
			m_skyCube->SetPosition(nsLevel3D::CLevel3D::m_kLevelObjectOffset);

			// �����̐���
			m_buildings = NewGO<nsBuilding::CBuildings>(nsCommonData::enPriorityFirst);

			// ���x�������[�h����
			LoadLevel3D();

			// ���[�h�������x���f�[�^�����ƂɌ�����������
			m_buildings->Init();


			return;
		}

		/**
		 * @brief �f�X�g���N�^
		*/
		CBackGround::~CBackGround()
		{
			DeleteGO(m_skyCube);	// �X�J�C�L���[�u�N���X�̔j��
			DeleteGO(m_buildings);	// ������j��

			return;
		}

		/**
		 * @brief �^�C�g����ʂ��H��ݒ�
		 * @param[in] isTitle �^�C�g����ʂ��H
		*/
		void CBackGround::SetIsTitle(const bool isTitle) 
		{
			m_skyCube->SetIsTitle(isTitle);
			return;
		}

		/**
		 * @brief ���x�������[�h����
		*/
		void CBackGround::LoadLevel3D()
		{
			// �����Ń��^�[��������A�����Ə�����S���������Ȃ��B
			//return true;
			// �����p��3D���x���̏�����
			// �}�b�v�`�b�v�̃C���X�^���X�͐������Ȃ�
			m_buildingLevel3D->Init(
				kLevelFilePath[enLevelPlateAndBuildings],
				[&](nsLevel3D::SLevelObjectData& objData)
				{
					// �����̐���
					if (objData.ForwardMatchName(nsBuilding::nsBuildingConstData::kBuildingForwardName) != true)
					{
						// �����̐擪�̖��O�ł͂Ȃ��B�����ł͂Ȃ��f�[�^�������Ă���B
						objData.shadowCaster = true;
						return false;
					}

					// �����̃^�C�v�̐��̕��A������܂őS�����ׂ�
					for (int i = 0; i < nsBuilding::nsBuildingConstData::enBuildingTypeNum; i++)
					{
#ifdef _DEBUG
						if (i != 0)
						{
							// �f�o�b�N�r���h�p�B���[�h���d�����߁A1��ނ̌����������[�h���Ȃ��B
							return true;
						}

#endif

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

					// ���O����v���Ȃ������̃f�[�^������
					MessageBoxA(nullptr, "Building�̖��O���Ȃ�", "�x��", MB_OK);
					return true;
				}
			);

			// ���������x�����琶��
			for (int propsType = 0; propsType < enPropsTypeNum; propsType++)
			{
#ifdef _DEBUG
				// �f�o�b�N�r���h�p�B���[�h���d�����߁A�����͐������Ȃ��B
				break;
#endif
				// �����p�̃��x����������
				m_propsLevel3D[propsType]->Init(
					kPropsLevelFilePath[propsType],
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						// �}�b�v�`�b�v�̗\�񐔂�ݒ肷��
						objData.numMapChipReserve = kNumMapChipReserveTbl[propsType];
						// ���[�U�[��`�̃R���W���������������p�̑����ɐݒ肷��
						objData.userIndex = EnCollisionAttr::enCollisionAttr_Props;
						// LOD�p�̃��f���̃t�@�C���p�X���w��
						objData.lodModelFilePath = kPropsLODModelFilePath[propsType];
						// LOD�p�̋�����ݒ�
						objData.distanceLOD = kPropsLODDistance[propsType];
						// �V���h�E�L���X�^�[�ɂ���
						//objData.shadowCaster = true;


						switch (propsType)
						{
							// �R�R�Ŏw�肵�����̂����������
						case enPropsPedestrianLight:	// ���s�җp�M���@
						case enPropsStreetLight:		// �X��
						case enPropsTrafficLight:		// �M���@
						case enPropsStreetTree:			// �X�H��
							return false;
							break;

							// �X�H���̎}�͔������ŕ`�悷��
						case enPropsStreetTreeBranch:
							objData.isTranslucent = true;
							objData.priority = nsCommonData::enPrioritySecond;
							objData.collisionModelFilePath = 
								"Assets/modelData/levelSource/StreetTree_collision.tkm";
							return false;
							break;

							// �w�肵�Ȃ������琶������Ȃ�
						default:
							return true;
							break;
						}

						return true;
					}
				);

				// ���̏�����
			}


			return;
		}
	}
}
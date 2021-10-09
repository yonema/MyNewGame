#include "stdafx.h"
#include "TestMapForPlayerMove.h"
#include "SkyCube.h"
#include "Player.h"
#include "Building.h"

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
				// ���x���̐���
				m_level3D.Init(
					"Assets/levelData/testLevel.tkl",
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						if (objData.EqualObjectName(L"testBuilding"))
						{
							if (a != 1)
							{
								a++;
								return false;
							}
							nsBuilding::CBuilding* building = 
								NewGO<nsBuilding::CBuilding>(nsCommonData::enPriorityFirst, "testBuilding");
							building->Init(
								"Assets/modelData/levelSource/testBuilding.tkm",
								objData.position,
								objData.rotation
							);
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

				QueryGOs<nsBuilding::CBuilding>(
					"testBuilding",
					[&](nsBuilding::CBuilding* building)->bool
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
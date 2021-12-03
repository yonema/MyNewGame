#include "stdafx.h"
#include "Buildings.h"
#include "ModelRender.h"


namespace nsMyGame
{
	/**
	 * @brief �����֘A�̃l�[���X�y�[�X
	*/
	namespace nsBuilding
	{
		// �����֘A�̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsBuildingConstData;

		/**
		 * @brief �R���X�g���N�^
		*/
		CBuildings::CBuildings()
		{

			return;
		}


		/**
		 * @brief �j���������ɌĂ΂��֐�
		*/
		void CBuildings::OnDestroy()
		{
			// �������N�G�����āA�S���j��
			QueryGOs<nsGraphic::nsModel::CModelRender>(
				kBuildingObjectName,
				[&](nsGraphic::nsModel::CModelRender* modelRender)->bool
				{
					DeleteGO(modelRender);
					return true;
				}
			);


#ifdef MY_DEBUG
			for (auto& debugModel : m_debugModels)
			{
				DeleteGO(debugModel);
			}
#endif

			return;
		}

		/**
		 * @brief �A�b�v�f�[�g�֐�
		*/
		void CBuildings::Update()
		{
			return;
		}

		/**
		 * @brief ������ǉ�����
		 * @param[in] buildingType �����̎��
		 * @param[in] pos ���W
		 * @param[in] rot ��]
		 * @param[in] scale �g�嗦
		*/
		void CBuildings::AddBuliding(
			const nsBuildingConstData::EnBuildingType buildingType,
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3& scale
		)
		{
			// �����̃f�[�^�̃C���X�^���X�𐶐�
			SBuildingDataPtr buildingData(std::make_unique<SBuildingData>());
			// ���W�A��]�A�g�嗦��ݒ�
			buildingData->pos = pos;
			buildingData->rot = rot;
			buildingData->scale = scale;
			// �����̃f�[�^�̃}�b�v�ɓo�^����
			m_buildingDataMap[buildingType].emplace_back(std::move(buildingData));

			return;
		}


		/**
		 * @brief ������
		*/
		void CBuildings::Init()
		{
			// �����̃^�C�v�S�����ׂ�
			for (int buildingType = 0; buildingType < enBuildingTypeNum; buildingType++)
			{
				// �����̃^�C�v
				const EnBuildingType bType = static_cast<EnBuildingType>(buildingType);

				if (m_buildingDataMap.count(bType) == 0)
				{
					// ���̃^�C�v�̌����������������Ă��Ȃ������玟�փX�L�b�v
					continue;
				}

				// ���̃^�C�v�̌����̃��f�������_���[�𐶐�����
				m_modelRenderMap[bType] = NewGO<nsGraphic::nsModel::CModelRender>(
					nsCommonData::enPriorityFirst,
					kBuildingObjectName
					);

				// ���̃^�C�v�̌����̐�
				const int numBuilding = static_cast<int>(m_buildingDataMap[bType].size());

				if (numBuilding == 1)
				{
					// ������Ȃ�A�ʏ�̃��f���̏����������āA���փX�L�b�v�B
					InitModel(bType);
				}
				else
				{
					// ��������Ȃ�A�C���X�^���V���O�`��̃��f���̏������B
					InitModelInstance(bType, numBuilding);
				}

				m_modelRenderMap[bType]->SetIsShadowCaster(true);
			}


#ifdef no
#ifdef MY_DEBUG

			// �v�Z���ꂽ�X�C���O�^�[�Q�b�g�̍��W�R���e�i���擾
			const std::vector<Vector3>& swingTargetPositions = m_swingTarget.GetSwingTargetPositions();

			// �X�C���O�^�[�Q�b�g�̍��W�ԍ�
			int stPosNo = 0;
			// �X�C���O�^�[�Q�b�g�̍��W�̐������L���p�V�e�B���m��
			m_debugModels.reserve(swingTargetPositions.size());
			// �S�Ă̍��W�𒲂ׂ�
			for (auto& stPos : swingTargetPositions)
			{
				nsGraphic::nsModel::CModelRender* debugModel = 
					NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
				debugModel->SetPosition(swingTargetPositions[stPosNo++]);
				debugModel->Init("Assets/modelData/debugModel/testBox.tkm");
				m_debugModels.emplace_back(debugModel);

			}
#endif
#endif

			return;
		}

		/**
		 * @brief �ʏ�̃��f���̏�����
		 * @param[in] buildingType �����̎��
		*/
		void CBuildings::InitModel(const nsBuildingConstData::EnBuildingType buildingType)
		{
			// ���f�������_���[�̍��W�Ɖ�]�Ɗg�嗦��ݒ�
			m_modelRenderMap[buildingType]->SetPosition(m_buildingDataMap[buildingType][0]->pos);
			m_modelRenderMap[buildingType]->SetRotation(m_buildingDataMap[buildingType][0]->rot);
			m_modelRenderMap[buildingType]->SetScale(m_buildingDataMap[buildingType][0]->scale);

			// ���f�������_���[��������
			m_modelRenderMap[buildingType]->Init(kBuildingModelFilePath[buildingType]);

			// �X�C���O�^�[�Q�b�g�̃C���X�^���X�𐶐�
			m_buildingDataMap[buildingType][0]->swingTarget = 
				std::make_unique<nsStringActionTarget::nsSwingTarget::CSwingTarget>();
			// �X�C���O�^�[�Q�b�g�̏�����
			m_buildingDataMap[buildingType][0]->swingTarget->Init(
				*m_modelRenderMap[buildingType]->GetModel().GetTkmFile()
			);
			// �X�C���O�^�[�Q�b�g�̍��W���v�Z����
			m_buildingDataMap[buildingType][0]->swingTarget->CalcSwingingTargetPositions(
				m_modelRenderMap[buildingType]->GetModel().GetWorldMatrix(),
				kDefaultWidthSegments,
				kDefaultHeightSegments,
				kDefaultLengthSegments,
				kDefaultHeightLowerLimit
			);

			// �ÓI�����I�u�W�F�N�g�̃C���X�^���X�𐶐�
			m_buildingDataMap[buildingType][0]->staticObject = std::make_unique<PhysicsStaticObject>();
			// �ÓI�����I�u�W�F�N�g�����f�����琶��
			m_buildingDataMap[buildingType][0]->staticObject->CreateFromModel(
				m_modelRenderMap[buildingType]->GetModel(),
				m_modelRenderMap[buildingType]->GetModel().GetWorldMatrix()
			);

			return;
		}

		/**
		 * @brief �C���X�^���V���O�`��̃��f���̏�����
		 * @param[in] buildingType �����̎��
		 * @param[in] numInstance �C���X�^���X�̐�
		*/
		void CBuildings::InitModelInstance(
			const nsBuildingConstData::EnBuildingType buildingType,
			const int numInstance
		)
		{
			// ���f�������_���[���C���X�^���V���O�`����s���悤�ɏ�����
			m_modelRenderMap[buildingType]->Init(
				kBuildingModelFilePath[buildingType],
				nullptr,
				0,
				numInstance
			);

			// ���̃^�C�v�̌����̂��ׂẴf�[�^��������\�z����
			for (const auto& buildingData : m_buildingDataMap[buildingType])
			{
				// ���f�������_���[�̃C���X�^���V���O�̃f�[�^���X�V
				m_modelRenderMap[buildingType]->UpdateInstancingData(
					buildingData->pos,
					buildingData->rot,
					buildingData->scale
				);

				// ���f���N���X�Ɋ�Â������[���h�s����v�Z
				Matrix worldMatrix = m_modelRenderMap[buildingType]->GetModel().CalcWorldMatrix(
					buildingData->pos,
					buildingData->rot,
					buildingData->scale
				);

				// �X�C���O�^�[�Q�b�g�̃C���X�^���X�𐶐�
				buildingData->swingTarget =
					std::make_unique<nsStringActionTarget::nsSwingTarget::CSwingTarget>();
				// �X�C���O�^�[�Q�b�g�̏�����
				buildingData->swingTarget->Init(
					*m_modelRenderMap[buildingType]->GetModel().GetTkmFile()
				);
				// �X�C���O�^�[�Q�b�g�̍��W���v�Z����
				buildingData->swingTarget->CalcSwingingTargetPositions(
					worldMatrix,
					kDefaultWidthSegments,
					kDefaultHeightSegments,
					kDefaultLengthSegments,
					kDefaultHeightLowerLimit
				);

				// �ÓI�����I�u�W�F�N�g�̃C���X�^���X�𐶐�
				buildingData->staticObject = std::make_unique<PhysicsStaticObject>();
				// �ÓI�����I�u�W�F�N�g�����f�����琶��
				buildingData->staticObject->CreateFromModel(
					m_modelRenderMap[buildingType]->GetModel(),
					worldMatrix
				);
			}

			return;
		}



	}
}
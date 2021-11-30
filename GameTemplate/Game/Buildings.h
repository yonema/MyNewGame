#pragma once
#include "SwingTarget.h"
#include "BuildingConstData.h"

namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic { namespace nsModel { class CModelRender; } }	// ���f�������_���[

	/**
	 * @brief �����֘A�̃l�[���X�y�[�X
	*/
	namespace nsBuilding
	{
		/**
		 * @brief �����̃f�[�^
		*/
		struct SBuildingData
		{
			Vector3 pos = Vector3::Zero;			//!< ���W
			Quaternion rot = Quaternion::Identity;	//!< ��]
			Vector3 scale = Vector3::One;			//!< �g�嗦
			std::unique_ptr<nsStringActionTarget::nsSwingTarget::CSwingTarget> swingTarget;	//!< �X�C���O�^�[�Q�b�g
			std::unique_ptr<PhysicsStaticObject> staticObject;	//!< �ÓI�����I�u�W�F�N�g
		};

		/**
		 * @brief �����N���X
		*/
		class CBuildings : public IGameObject
		{
		private:	// �G�C���A�X�錾
			using SBuildingDataPtr = std::unique_ptr<SBuildingData>;	// �����̃f�[�^�̃X�}�[�g�|�C���^
			using SBuildingDataVector = std::vector<SBuildingDataPtr>;	// �����̃f�[�^�̃R���e�i

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CBuildings();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CBuildings() = default;

		public:		// �I�[�o�[���C�h���������o�֐�
			
			/**
			 * @brief �j���������ɌĂ΂��֐�
			*/
			void OnDestroy() override final;

			/**
			 * @brief �A�b�v�f�[�g�֐�
			*/
			void Update() override final;

		public:		// �����o�֐�

			/**
			 * @brief ������ǉ�����
			 * @param[in] buildingType �����̎��
			 * @param[in] pos ���W
			 * @param[in] rot ��]
			 * @param[in] scale �g�嗦
			*/
			void AddBuliding(
				const nsBuildingConstData::EnBuildingType buildingType,
				const Vector3& pos,
				const Quaternion& rot,
				const Vector3& scale
			);

			/**
			 * @brief ������
			*/
			void Init();

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �ʏ�̃��f���̏�����
			 * @param[in] buildingType �����̎��
			*/
			void InitModel(const nsBuildingConstData::EnBuildingType buildingType);

			/**
			 * @brief �C���X�^���V���O�`��̃��f���̏�����
			 * @param[in] buildingType �����̎��
			 * @param[in] numInstance �C���X�^���X�̐�
			*/
			void InitModelInstance(
				const nsBuildingConstData::EnBuildingType buildingType,
				const int numInstance
			);

		private:	// �f�[�^�����o
			//!< ���f�������_���[�̃}�b�v
			std::unordered_map < nsBuildingConstData::EnBuildingType, nsGraphic::nsModel::CModelRender*> m_modelRenderMap;
			//!< �����̃f�[�^�̃}�b�v
			std::unordered_map<nsBuildingConstData::EnBuildingType, SBuildingDataVector> m_buildingDataMap;

#ifdef MY_DEBUG
			std::vector<nsGraphic::nsModel::CModelRender*> m_debugModels;	//!< �f�o�b�N�p���f��
#endif

		};
	}
}




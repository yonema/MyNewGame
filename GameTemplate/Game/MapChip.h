#pragma once
#include "Noncopyable.h"
#include "ModelRender.h"


namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }	// ���f�������_���[�N���X
	}

	/**
	 * @brief 3D�̃��x���f�[�^�֘A�̃l�[���X�y�[�X
	*/
	namespace nsLevel3D
	{
		// �O���錾
		struct SLevelObjectData;		// ���x���I�u�W�F�N�g�f�[�^

		/**
		 * @brief �}�b�v�`�b�v�̃f�[�^
		*/
		struct SMapChipData
		{
			Vector3 position;		//!< ���W�B
			Vector3 scale;			//!< �傫���B
			Quaternion rotation;	//!< ��]�B
			//std::unique_ptr<PhysicsStaticObject> physicsStaticObject;
		};

		/**
		 * @brief �}�b�v�`�b�v�N���X
		*/
		class CMapChip : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			 * @param[in] objData ���x���I�u�W�F�N�g�f�[�^
			 * @param[in] filePath �t�@�C���p�X
			*/
			CMapChip(const SLevelObjectData& objData, const char* filePath);
			/**
			 * @brief �f�X�g���N�^
			*/
			~CMapChip()
			{
				// ���f�������_���[�̔j��
				DeleteGO(m_modelRender);
			}

		public:		// �����o�֐�

			/**
			 * @brief ����������
			*/
			void Init();

			/**
			 * @brief �}�b�v�`�b�v�f�[�^�̒ǉ�
			 * @param[in] objData ���x���I�u�W�F�N�g�f�[�^
			*/
			void AddMapChipData(const SLevelObjectData& objData);

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �ʏ�̃��f���̏�����
			*/
			void InitModel();

			/**
			 * @brief �C���X�^���V���O�`��̃��f���̏�����
			 * @param[in] numInstance �C���X�^���X��
			*/
			void InitModelInstancing(const int numInstance);

		private:	// �f�[�^�����o
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< ���f�������_���[
			std::vector< std::unique_ptr<SMapChipData>> m_mapChipDataVector;	//!< �}�b�v�`�b�v�f�[�^�̃��X�g
			std::unique_ptr<const char*> m_filePath;					//!< tkm�t�@�C���p�X
			//!< �ÓI�����I�u�W�F�N�g�̔z��B
			std::vector<std::unique_ptr<PhysicsStaticObject>> m_physicsStaticObjectPtrVector;
			bool m_isTranslucent = false;								//!< �������`�悩�H
			int m_priority = nsCommonData::enPriorityFirst;				//!< �D��x
			//!< ���[�U�[��`�̃R���W��������
			EnCollisionAttr m_userIndex = EnCollisionAttr::enCollisionAttr_None;

		};

	}
}
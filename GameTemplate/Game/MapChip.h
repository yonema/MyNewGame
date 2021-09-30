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

		private:	// �f�[�^�����o
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< ���f�������_���[
			PhysicsStaticObject m_physicsStaticObject;					//!< �ÓI�����I�u�W�F�N�g�B
		};

	}
}
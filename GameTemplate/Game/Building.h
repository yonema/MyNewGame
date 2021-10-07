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
		 * @brief �����N���X
		*/
		class CBuilding : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CBuilding();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CBuilding() = default;

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
			 * @brief ������
			 * @param[in] tkmFilePath tkm�t�@�C���p�X
			 * @param[in] pos ���W
			 * @param[in] rot ��]
			 * @param[in] scale �g�嗦
			*/
			void Init(
				const char* tkmFilePath,
				const Vector3& pos,
				const Quaternion rot = Quaternion::Identity,
				const Vector3& scale = Vector3::One
			);

		private:	// �f�[�^�����o
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;			//!< ���f�������_���[
			nsStringActionTarget::nsSwingTarget::CSwingTarget m_swingTarget;	//!< �X�C���O�^�[�Q�b�g

#ifdef MY_DEBUG
			std::vector<nsGraphic::nsModel::CModelRender*> m_debugModels;	//!< �f�o�b�N�p���f��
#endif

		};
	}
}




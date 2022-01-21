#pragma once
#include "Noncopyable.h"
#include "MapConstDatah.h"

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsLevel3D { class CLevel3D; }			// 3D���x���N���X
	namespace nsNature { class CSkyCube; }			// �X�J�C�N���X
	namespace nsBuilding { class CBuildings; }		// �����N���X


	/**
	 * @brief �w�i�X�e�[�W�p�̃l�[���X�y�[�X
	*/
	namespace nsBackGround
	{
		/**
		 * @brief �w�i�X�e�[�W�N���X
		*/
		class CBackGround : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CBackGround();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CBackGround();

		public:		// �����o�֐�

			/**
			 * @brief �^�C�g����ʂ��H��ݒ�
			 * @param[in] isTitle �^�C�g����ʂ��H
			*/
			void SetIsTitle(const bool isTitle);

		private:	// private�ȃ����o�֐�

			/**
			 * @brief ���x�������[�h����
			*/
			void LoadLevel3D();

		private:	// �f�[�^�����o
			std::unique_ptr<nsLevel3D::CLevel3D> m_buildingLevel3D;	//!< �����p3D���x���N���X
			//!< �����p3D���x���N���X
			std::unique_ptr <nsLevel3D::CLevel3D> m_propsLevel3D[nsMaps::nsMapConstData::enPropsTypeNum];
			nsNature::CSkyCube* m_skyCube = nullptr;		//!< �X�J�C�L���[�u
			nsBuilding::CBuildings* m_buildings = nullptr;	//!< �����N���X

		};
	}
}



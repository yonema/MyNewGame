#pragma once
#include "Noncopyable.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �W�I���g���֘A�̃l�[���X�y�[�X
	*/
	namespace nsGeometry
	{

		/**
		 * @brief �u���b�L���O�{�����[���N���X
		*/
		class CBlockingVolume : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^

			/**
			 * @brief �R���X�g���N�^
			*/
			CBlockingVolume() = default;

			/**
			 * @brief �f�X�g���N�^
			*/
			~CBlockingVolume() = default;

		public:		// �����o�֐�

			/**
			 * @brief ������
			 * @param[in] pos ���W
			 * @param[in] rot ��]
			 * @param[in] scale �g��
			*/
			void Init(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

		private:	// static�ȃf�[�^�����o

			//!< �u���b�L���O�{�����[���̃t�@�C���p�X
			static const char* const m_kBlockingVolumeModelFilePath;

		private:	// �f�[�^�����o
			std::unique_ptr<PhysicsStaticObject> m_physicsStaticObject;
		};

	}
}


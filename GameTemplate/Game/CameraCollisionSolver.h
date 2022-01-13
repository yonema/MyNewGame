#pragma once
#include "Noncopyable.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �J�����֘A�̃l�[���X�y�[�X
	*/
	namespace nsCamera
	{
		/**
		 * @brief �J�����R���W���������N���X
		*/
		class CCameraCollisionSolver : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CCameraCollisionSolver() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CCameraCollisionSolver() = default;

		public:		// �����o�֐�

			/**
			 * @brief ������
			 * @param radius �J�����̃o�E���f�B���O�X�t�B�A�̔��a
			*/
			void Init(const float radius);

			/**
			 * @brief �R���W���������̎��s
			 * @param[out] result �R���W�����������s�������_�̍��W���i�[�����
			 * @param[in] position �R���W�����������s���O�̎��_�̍��W
			 * @param[in] target �J�����̒����_
			 * @return �R���W�����������s�������H
			*/
			bool Execute(Vector3& result_out, const Vector3& position, const Vector3& target);

		private:	// �f�[�^�����o
			SphereCollider m_collider;	//!< �R���C�_�[
			float m_radius = 0.0f;		//!< ���a
			bool m_isInited = false;	//!< �������ς݂��H

		};

	}
}
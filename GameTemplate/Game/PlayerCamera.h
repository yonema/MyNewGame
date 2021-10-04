#pragma once
#include "Noncopyable.h"
#include "PlayerInput.h"
#include "SpringCamera.h"

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �O���錾
		struct SPlayerInputData;	// �v���C���[�̓��͏��\����

		/**
		 * @brief �v���C���[�̃J�����N���X
		*/
		class CPlayerCamera : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerCamera() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CPlayerCamera() = default;

		public:		// �����o�֐�

			/**
			 * @brief ������
			 * @param[in] startTargetPos �X�^�[�g�����_
			 * @param[in] inputData ���͏��
			*/
			void Init(const Vector3& startTargetPos, const SPlayerInputData& inputData);

			/**
			 * @brief �v���C���[�J�����N���X�̃A�b�v�f�[�g�����s����
			*/
			void ExecuteUpdate();

			/**
			 * @brief �J�����̑O�����̃x�N�g�����擾
			 * @return �J�����̑O�����̃x�N�g��
			*/
			const Vector3& GetCameraForward() const
			{
				return m_camera->GetForward();
			}

			/**
			 * @brief �J�����̉E�����̃x�N�g�����擾
			 * @return �J�����̉E�����̃x�N�g��
			*/
			const Vector3& GetCameraRight() const
			{
				return m_camera->GetRight();
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �J�����̉�]���v�Z����
			*/
			void CalcCameraRotation();

			/**
			 * @brief �J�����̎��_�ƒ����_���v�Z����
			 * @param[out] pos_out ���_���i�[�����
			 * @param[out] targetPos_out �����_���i�[�����
			*/
			void CalcCameraPositionAndTargetPos(
				Vector3* pos_out,
				Vector3* targetPos_out
			);

			/**
			 * @brief �o�l�J�������X�V����
			 * @param[in] pos ���_
			 * @param[in] targetPos �����_
			*/
			void UpdateSpringCamera(const Vector3& pos, const Vector3& targetPos);

		private:	// �f�[�^�����o
			Camera* m_camera = nullptr;				//!< �J����
			nsCamera::CSpringCamera m_springCamera;	//!< �o�l�J����
			Vector3 m_toCameraVec = Vector3::Zero;	//!< �����_���王�_�ւ̃x�N�g��

			const Vector3* m_playerPosition = nullptr;				//!< �v���C���[�̍��W�̎Q��
			const SPlayerInputData* m_playerInputData = nullptr;	//!< �v���C���[�̓��͏��̎Q��

		};

	}
}
#pragma once
#include "Noncopyable.h"
#include "SpringCamera.h"

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �O���錾
		class CPlayer;	// �v���C���[�̓��͏��\����

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
			 * @param[in] player �v���C���[�̎Q��
			*/
			void Init(const CPlayer& player);

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

			void SetTargetOffsetUp(const float offsetUp)
			{
				m_targetOffsetUp = offsetUp;
			}

			void SetCameraPositionOffsetUp(const float cameraPosUp)
			{
				m_cameraPositionOffsetUp = cameraPosUp;
			}

			void SetToCameraDistance(const float distance)
			{
				m_toCameraDistance = distance;
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �J�����̉�]���v�Z����
			*/
			void CalcCameraRotation();

			/**
			 * @brief �����I�ɃJ�������v���C���[�̈ړ���Ɍ�����
			*/
			void AutoTurnToPlayerDestination();


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
			Camera* m_camera = nullptr;					//!< �J����
			nsCamera::CSpringCamera m_springCamera;		//!< �o�l�J����
			Vector3 m_toCameraVec = Vector3::Zero;		//!< �����_���王�_�ւ̃x�N�g��
			const CPlayer* m_playerRef = nullptr;		//!< �v���C���[�̎Q��
			float m_autoTurnStartTimer = 0.0f;			//!< �����ŃJ��������]���n�߂�^�C�}�[
			float m_autoTurnStartTimerResetTimer = 0.0f;	//!< �����ŃJ��������]���n�߂�^�C�}�[�����Z�b�g����^�C�}�[
			float m_targetOffsetUp = 0.0f;
			float m_cameraPositionOffsetUp = 0.0f;
			float m_toCameraDistance = 0.0f;
		};

	}
}
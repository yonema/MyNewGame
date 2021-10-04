#pragma once
#include "Noncopyable.h"
#include "CameraCollisionSolver.h"

namespace nsMyGame
{
	/**
	 * @brief �J�����֘A�̃l�[���X�y�[�X
	*/
	namespace nsCamera
	{
		/**
		 * @brief �΂˃J�����N���X
		*/
		class CSpringCamera : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CSpringCamera() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CSpringCamera() = default;

		public:		// �����o�֐�

			/**
			 * @brief ������
			 * @param camera �J����
			 * @param maxMoveSpeed �J�����̍ő呬�x
			 * @param isEnableCollisionSolver �R���W����������L���ɂ��邩�H
			 * @param sphereCollisionRadius ���̃R���W�����̔��a�BisEnableCollisionSolver��true�̂Ƃ��̂ݗL���B
			*/
			void Init(
				Camera& camera,
				const float maxMoveSpeed,
				const bool isEnableCollisionSolver = false,
				const float sphereCollisionRadius = 0.0f
			);

			/**
			 * @brief �����_��ݒ�
			 * @param[in] target �����_
			*/
			void SetTarget(const Vector3& target)
			{
				m_target = target;
			}

			/**
			 * @brief ���_��ݒ�
			 * @param[in] position ���_
			*/
			void SetPosition(const Vector3& position)
			{
				m_position = position;
			}

			/**
			 * @brief �����ʂ�ݒ�
			 * @param[in] _far ������
			*/
			void SetFar(const float _far)
			{
				if (m_camera == nullptr) {
					return;
				}
				m_camera->SetFar(_far);
			}

			/**
			 * @brief �ߕ��ʂ�ݒ�
			 * @param[in] _near �ߕ���
			*/
			void SetNear(const float _near)
			{
				if (m_camera == nullptr) {
					return;
				}
				m_camera->SetNear(_near);
			}

			/**
			 * @brief �J�����̉�]�s����擾
			 * @return �J�����̉�]�s��
			*/
			const Matrix& GetCameraRotation() const
			{
				if (m_camera == nullptr) {
					return Matrix::Identity;
				}
				return m_camera->GetCameraRotation();
			}

			/**
			 * @brief �����_���擾
			 * @attention ���̊֐����߂��Ă��钍���_�́A���ۂ̒����_�ł͂Ȃ��̂Œ��ӂ��Ă��������B
			 * ���ۂ̒����_���ق����ꍇ��GetRealTarget���g�p���Ă��������B
			 * @return �����_
			*/
			const Vector3& GetTarget() const
			{
				return m_target;
			}

			/**
			 * @brief ���_���擾
			 * @attention ���̊֐����Ԃ��Ă��鎋�_�́A���ۂ̎��_�ł͂Ȃ��̂Œ��ӂ��Ă��������B
			 * ���ۂ̎n�_���ق����ꍇ��GetRealPosition���g�p���ĉ������B
			 * @return ���_
			*/
			const Vector3& GetPosition() const
			{
				return m_position;
			}

			/**
			 * @brief ���ۂ̒����_���擾�B
			 * @return �����_
			*/
			const Vector3& GetRealTarget() const
			{
				if (m_camera == nullptr) {
					return Vector3::Zero;
				}
				return m_camera->GetTarget();

			}

			/**
			 * @brief ���_���擾
			 * @return ���_
			*/
			const Vector3& GetRealPosition() const
			{
				if (m_camera == nullptr) {
					return Vector3::Zero;
				}
				return m_camera->GetPosition();
			}

			/**
			 * @brief �X�V
			*/
			void Update();

			/**
			 * @brief �o�l�̌�������ݒ�B�l���傫���قǃJ�������x��ĕt���Ă��܂��B
			 * @param[in] dampingRate �o�l�̌�����
			*/
			void SetDampingRate(const float dampingRate)
			{
				m_targetDampingRate = dampingRate;
			}

			/**
			 * @brief ��p��ݒ�
			 * @param[in] angle ��p
			*/
			void SetViewAngle(const float angle)
			{
				if (m_camera == nullptr) {
					return;
				}
				m_camera->SetViewAngle(angle);
			}

			/**
			 * @brief ��p���擾
			 * @return ��p
			*/
			float GetViewAngle() const
			{
				if (m_camera == nullptr) {
					return 0.0f;
				}
				return m_camera->GetViewAngle();
			}

			/**
			 * @brief ���t���b�V��
			 * @details ���̊֐����Ăяo���ƁA�o�l�̈ړ������t���b�V������āAUpdate�֐����Ăяo����
			 * �J�����̈ʒu�������ɖڕW���_�ƖڕW�����_�Ɉړ����܂��B
			 * �V�[�����؂�ւ���ăJ��������C�ɐ؂�ւ������Ƃ��ȂǂɎg�p���Ă��������B
			*/
			void Refresh()
			{
				m_targetMoveSpeed = Vector3::Zero;
				m_positionMoveSpeed = Vector3::Zero;
				m_isRefresh = true;
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �΂˃J�����̍X�V����
			*/
			void UpdateSpringCamera();

			/**
			 * @brief �J�����̍X�V����
			*/
			void UpdateCamera()
			{
				if (m_camera) {
					m_camera->Update();
				}
			}
		private:
			Camera* m_camera = nullptr;						//!< �J�����B
			Vector3 m_target = Vector3::Zero;				//!< �ڕW�ƂȂ钍���_�B
			Vector3	m_position = Vector3::Zero;				//!< �ڕW�ƂȂ鎋�_�B
			Vector3	m_targetMoveSpeed = Vector3::Zero;		//!< �����_�̈ړ����x�B
			Vector3	m_positionMoveSpeed = Vector3::Zero;	//!< ���_�̈ړ����x�B
			float m_maxMoveSpeed = 0.0f;					//!< �ō��ړ����x�B
			float m_targetDampingRate = 1.0f;				//!< �������B�l���傫���قǃJ�������x��t���Ă���B
			float m_dampingRate = 1.0f;						//!< �������B
			float m_dampingRateVel = 0.0f;
			bool m_isEnableCollisionSolver = false;			//!< �R���W�����������L���H
			bool m_isRefresh = true;						//!< ���t���b�V�����K�v�H
			CCameraCollisionSolver m_cameraCollisionSolver;	//!< �J�����R���W���������N���X
		};
	}
}



#pragma once
#include "Noncopyable.h"
#include "SpringCamera.h"
#include "PlayerConstData.h"

// ��������
//#include "SpriteRender.h"

namespace nsNinjaAttract
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

			/**
			 * @brief �J�����̍��W���擾
			 * @return �J�����̍��W
			*/
			const Vector3& GetCameraPosition() const
			{
				return m_camera->GetPosition();
			}

			/**
			 * @brief �o�l�J�����̌���������`�⊮����B
			 * 0.0f���f�t�H���g�ōŏ��̌������B1.0f���ő�̌������B
			 * �������������قǃJ�������x��ĕt���Ă���B
			 * @param[in] rate �������̐��`�⊮��
			*/
			void LerpDampingRate(const float rate)
			{
				m_springCamera.SetDampingRate(Math::Lerp<float>(
					rate,
					nsPlayerConstData::nsCameraConstData::kSpringDampingRate,
					nsPlayerConstData::nsCameraConstData::kMaxSpringDampingRate
					));
			}

			/**
			 * @brief �����_�̏㉺�̃I�t�Z�b�g����`�⊮����B
			 * 0.0f���f�t�H���g�ōŏ��̃I�t�Z�b�g�B1.0f���ő�̃I�t�Z�b�g�B
			 * �I�t�Z�b�g���傫���قǃL�����N�^�[����ʂ̉��ɉf��B
			 * @param[in] rate �����_�̏㉺�̃I�t�Z�b�g�̐��`�⊮��
			*/
			void LerpTargetOffsetUp(const float rate)
			{
				m_targetOffsetUp = Math::Lerp<float>(
					rate,
					nsPlayerConstData::nsCameraConstData::kTargetOffsetUp,
					nsPlayerConstData::nsCameraConstData::kNearTargetOffsetUp
					);
			}

			/**
			 * @brief �G�̏�ɏ���Ă���J�������H��ݒ肷��
			 * @param[in] isOnEnemyCamera �G�̏�ɏ���Ă���J�������H
			*/
			void SetIsOnEnemyCamera(const bool isOnEnemyCamera)
			{
				m_isOnEnemyCamera = isOnEnemyCamera;
			}

			/**
			 * @brief �G�̏�ɏ���Ă���J�������H�𓾂�
			 * @return �G�̏�ɏ���Ă���J�������H
			*/
			bool IsOnEnemyCamera() const
			{
				return m_isOnEnemyCamera;
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
			 * @brief �G�̏�ɏ���Ă��鎞�̃J�����̏���
			*/
			void OnEnemyCamera();


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
			bool m_isOnEnemyCamera = false;				//!< �G�̏�ɏ���Ă���Ƃ��̃J�������H

			// ������������
			//Camera m_dummyCamera;
			//nsGraphic::nsSprite::CSpriteRender* m_titleSR = nullptr;
			//nsGraphic::nsSprite::CSpriteRender* m_titleStartSR = nullptr;
			//bool m_titleFadeOut = false;
			//float m_titleA = 1.0f;
		};

	}
}
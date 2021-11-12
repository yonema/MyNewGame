#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"


namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �O���錾
		class CPlayer;			// �v���C���[�N���X
		class CPlayerCamera;	// �v���C���[�̃J�����N���X
		class CPlayerModelAnimation; // �v���C���[���f���A�j���[�V�����N���X

		/**
		 * @brief �v���C���[�̓����֘A�̃l�[���X�y�[�X
		*/
		namespace nsPlayerMovenent
		{
			// �O���錾
			class CPlayerMovement;	// �v���C���[�̓����N���X

			/**
			 * @brief �v���C���[�̃X�C���O�̃A�N�V�����N���X
			*/
			class CPlayerSwingAction : private nsUtil::Noncopyable
			{
			private:	// �G�C���A�X�錾
				// �X�C���O�A�N�V�����̃X�e�[�g
				using EnSwingActionState = nsPlayerConstData::nsSwingActionConstData::EnSwingActionState;

			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CPlayerSwingAction() = default;
				/**
				 * @brief �f�X�g���N�^
				*/
				~CPlayerSwingAction() = default;

			public:		// �����o�֐�

				/**
				 * @brief ������
				 * @param[in.out] player �v���C���[
				 * @param[in,out] playerMovement �v���C���[�ړ��N���X�̎Q��
				 * @param[in,out] playerCamera �v���C���[�J�����N���X�̎Q��
				 * @param[in.out] playerModelAnimation �v���C���[���f���A�j���[�V�����̎Q��
				*/
				void Init(
					CPlayer* player,
					CPlayerMovement* playerMovement,
					CPlayerCamera* playerCamera,
					CPlayerModelAnimation* playerModelAnimation
				);

				/**
				 * @brief �X�C���O�A�N�V���������s
				*/
				void Execute();

				/**
				 * @brief �X�C���O�A�N�V�����̑O�ɍs������
				*/
				void PreSwingAction();

				/**
				 * @brief �X�C���O���̑��x�����Z�b�g����
				*/
				void ResetSwingSpeed()
				{
					m_swingSpeed = 0.0f;
					m_swingSpeed = -100.0f;
				}

				/**
				 * @brief �X�C���O���̃X�s�[�h�𓾂�
				 * @return �X�C���O���̃X�s�[�h
				*/
				float GetSwingSpeed() const
				{
					return m_swingSpeed;
				}

			private:	// private�ȃ����o�֐�

				/**
				 * @brief �X�C���O�^�[�Q�b�g��T��
				*/
				void FindSwingTarget();

				/**
				 * @brief ����L�΂��Ă���Œ��̏���
				*/
				void StringStretching();

				/**
				 * @brief �X�C���O�A�N�V�����̏���
				*/
				void SwingAction();

				/**
				 * @brief �X�C���O��̋󒆂̏���
				*/
				void AirAfterSwing();

				/**
				 * @brief �X�C���O�����̏I��
				*/
				void EndSwing();

			private:	// �f�[�^�����o
				CPlayer* m_playerRef = nullptr;					//!< �v���C���[�N���X�̎Q��
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< �v���C���[�ړ��N���X�̎Q��
				CPlayerCamera* m_playerCameraRef = nullptr;		//!< �v���C���[�J�����N���X�̎Q��
				CPlayerModelAnimation* m_playerModelAnimationRef = nullptr;	//!< �v���C���[�̃��f���A�j���[�V�����̎Q��
				const Vector3* m_swingTargetPos = nullptr;		//!< �X�C���O�^�[�Q�b�g�̍��W
				//!< �X�C���O�A�N�V�����̃X�e�[�g
				EnSwingActionState m_swingActionState = 
					nsPlayerConstData::nsSwingActionConstData::enFindSwingTarget;
				float m_swingRadAngle = 0.0f;						//!< �X�C���O�̃��W�A���p�x
				float m_swingSpeed = 0.0f;							//!< �X�C���O�X�s�[�h
				//!< �������n�߂�X�C���O�X�s�[�h
				float m_startDecelerateSwingSpeed = 
					nsPlayerConstData::nsSwingActionConstData::kStartDecelerateSwingSpeedInitialValue;
				Vector3 m_inputMoveDirXZ = Vector3::Zero;			//!< ���͂ɂ���Đ�����XZ���ʂł̈ړ�����
				float m_velocityAfterSwing = 0.0f;					//!< �X�C���O��̑��x
				float m_accelerationAfterSwing = 0.0f;				//!< �X�C���O��̉���
				float m_g = 0.0f;
				int m_counter = 0;
				bool m_afterSwing = false;
			};
		}
	}
}

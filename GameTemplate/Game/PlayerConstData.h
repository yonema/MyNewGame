#pragma once

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		/**
		 * @brief �v���C���[�̒萔�f�[�^
		*/
		namespace nsPlayerConstData
		{
			constexpr float kCapsuleRadius = 50.0f;		//!< �J�v�Z���R���C�_�[�̔��a
			constexpr float kDapsuleHeight = 100.0f;	//!< �J�v�Z���R���C�_�[�̍���

			/**
			 * @brief �v���C���[�̃X�e�[�g
			*/
			enum EnPlayerState
			{
				enWalkAndRun,			//!< �����Ƒ���
				enSwing,				//!< �X�C���O
				enAirAfterStringAction,	//!< �����g�����A�N�V�����̌�̋󒆏��

			};


			/**
			 * @brief �v���C���[�ړ��N���X�̒萔�f�[�^
			*/
			namespace nsPlayerMoveConstData
			{
				constexpr float kGravityScale = 980.0f * 2.0f;	//!< �d�͂̋���
				constexpr float kMaxFallSpeed = 2000.0f;		//!< �ō��������x
				constexpr float kJumpForce = 700.0f;	//!< �W�����v��
				constexpr float kModelRotRate = 0.3f;	//!< ���f���̉�]�̕�ԗ�
				constexpr float kMoveVecMin = 0.001f;	//!< �ړ��x�N�g���̍ŏ��l
			}

			/**
			 * @brief �v���C���[�̒ʏ�̓����N���X�̒萔�f�[�^
			*/
			namespace nsPlayerNormalMovementConstData
			{
				constexpr float kWalkAcceleration = 20.0f;	//!< �������̉����x
				constexpr float kRunAcceleration = 40.0f;	//!< ���莞�̉����x
				constexpr float kWalkMaxSpeed = 500.0f;		//!< �������̍ō����x
				constexpr float kRunMaxSpeed = 1000.0f;		//!< ���莞�̍ō����x
				constexpr float kMinSpeed = 2.0f;			//!< �Œᑬ�x
				constexpr float kGroundFriction = 0.9f;		//!< �n�ʂ̖��C
				constexpr float kAirFriction = 1.0f;		//!< �󒆂̖��C
				constexpr float kBreakThresholdAngle = 135.0f;		//!< �u���[�L�̊p�x�̂������l
				constexpr float kBreakThresholdVelocity = 135.0f;	//!< �u���[�L�̑��x�̂������l
			}

			/**
			 * @brief �v���C���[�̃X�C���O�A�N�V�����N���X�̒萔�f�[�^
			*/
			namespace nsPlayerSwingActionConstData
			{
				/**
				 * @brief �X�C���O�A�N�V�����̃X�e�[�g
				*/
				enum EnSwingActionState
				{
					enFindSwingTarget,		//!< �X�C���O�^�[�Q�b�g��T��
					enIsStringStretching,	//!< ����L�΂���
					enIsSwinging,			//!< �X�C���O��
					enEnd,					//!< �I��
				};
			}
			/**
			 * @brief �v���C���[���f���N���X�̒萔�f�[�^
			*/
			namespace nsPlayerModelRenderConstData
			{
				//!< �v���C���[�̃��f���f�[�^�̃t�@�C�X�p�X
				constexpr const char* const kPlayerModelFilePath = "Assets/modelData/unityChan.tkm";

				/**
				 * @brief �A�j���[�V�����N���b�v�X
				*/
				enum EnAnimationClips
				{
					enAnim_idle,	//!< �A�C�h��
					enAnim_walk,	//!< ����
					enAnim_num		//!< �A�j���[�V�����N���b�v�̐�
				};

				//!< �A�C�h����Ԃ̃A�j���[�V�����t�@�C���p�X
				constexpr const char* const kAnimationFilePath_idle = "Assets/animData/idle.tka";
				//!< ������Ԃ̃A�j���[�V�����t�@�C���p�X
				constexpr const char* const kAnimationFilePath_walk = "Assets/animData/walk.tka";

				constexpr float kDefaultAnimInterpolateTime = 0.2f;	//!< �f�t�H���g�̃A�j���[�V�����⊮����

			}

			/**
			 * @brief �v���C���[�̎��̃��f���N���X�̒萔�f�[�^
			*/
			namespace nsPlayerStringModelConstData
			{
				//!< ���̃��f���t�@�C���p�X
				constexpr const char* const kStringModelFilePath = "Assets/modelData/testString.tkm";
				constexpr float kStretchedTime = 0.1f;	//!< �����L�т���܂ł̎���
			}

			/**
			 * @brief �v���C���[���̓N���X�̒萔�f�[�^
			*/
			namespace nsPlayerInputConstData
			{
				constexpr float kDPadInputPower = 1.0f;					//!< �\���L�[���͂̈ړ���
				static const float kSquareRootOfTwo = std::sqrtf(2.0f);	//!< ���[�g2�̒l
				constexpr float kInputAxisMin = 0.001f;					//!< �����͂̍ŏ��l
			}

			/**
			 * @brief �v���C���[�J�����̒萔�f�[�^
			*/
			namespace nsPlayerCameraConstData
			{
				constexpr float kCameraMaxSpeed = 10000.0f;	//!< �J�����̍ő呬�x
				constexpr float kCameraRadius = 5.0f;		//!< �J�����̃R���W�����̔��a

				constexpr float kTargetOffsetUp = 80.0f;		//!< �����_�̏㉺�̃I�t�Z�b�g
				constexpr float kTargetOffsetForward = 20.0f;	//!< �����_�̑O��̃I�t�Z�b�g

				constexpr float kDefaultCameraFar = 40000.0f;	//!< �f�t�H���g�̉�����

				//!< �f�t�H���g�̒����_���王�_�ւ̃x�N�g��
				static const Vector3 kDefaultToCameraVec = { 0.0f,50.0f,-400.0f };

				constexpr float kCameraRotSpeed = 3.0f;		//!< �J�����̉�]����X�s�[�h
				//!< �����_���王�_�ւ̕����x�N�g����Y�̍ő�l�B�J�����̉������̏���B
				constexpr float kMaxToCameraDirY = 0.8f;
				//!< �����_���王�_�ւ̕����x�N�g����Y�̍ŏ��l�B�J�����̏�����̏���B
				constexpr float kMinToCameraDirY = -0.5f;

				//!< �o�l�̌������B�l���傫���قǁA�J�������x��ĒǏ]���Ă���
				constexpr float kSpringDampingRate = 0.7f;

			}
		}
	}
}
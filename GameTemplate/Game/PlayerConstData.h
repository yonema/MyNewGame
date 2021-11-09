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
				enWallRun,				//!< �Ǒ���
			};


			/**
			 * @brief �v���C���[�ړ��N���X�̒萔�f�[�^
			*/
			namespace nsPlayerMoveConstData
			{
				constexpr float kGravityScale = 980.0f * 3.0f;	//!< �d�͂̋���
				constexpr float kMaxFallSpeed = 2000.0f;		//!< �ō��������x
				constexpr float kJumpForce = 1000.0f;	//!< �W�����v��
				constexpr float kModelRotRate = 0.3f;	//!< ���f���̉�]�̕�ԗ�
				constexpr float kMoveVecMin = 0.001f;	//!< �ړ��x�N�g���̍ŏ��l
			}

			/**
			 * @brief �v���C���[�̕����Ƒ���N���X�̒萔�f�[�^
			*/
			namespace nsPlayerWalkAndRunConstData
			{
				constexpr float kWalkAcceleration = 50.0f;	//!< �������̉����x
				constexpr float kRunAcceleration = 100.0f;	//!< ���莞�̉����x
				constexpr float kWalkMaxSpeed = 750.0f;		//!< �������̍ō����x
				constexpr float kRunMaxSpeed = 1200.0f;		//!< ���莞�̍ō����x
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
					enIsAirAfterSwing,		//!< �X�C���O��̋�
					enEnd,					//!< �I��
				};

				constexpr int kFindSwintTargetNum = 4;	//!< �X�C���O�^�[�Q�b�g��T����
				//!< �X�C���O�^�[�Q�b�g��T�����W�ւ̃x�N�g���W
				static const Vector3 kToFindSwingTargetVecs[kFindSwintTargetNum] =
				{
					{ 0.0f,2000.0f,2000.0f },		// �O��
					{ 500.0f,2000.0f,2000.0f },		// �O��
					{ -500.0f,2000.0f,2000.0f },		// �O��
					{ 0.0f,0.0f,0.0f }				// ���S
				};
				//!< �D��x�������X�C���O�^�[�Q�b�g�̐�
				constexpr int kHighPriorityFindSwintTargetNum = 3;
				//!< �X�C���O�^�[�Q�b�g��T���L���͈͂̔��a
				constexpr float kFindSwingTargetScope = 2000.0f;

				// �������n�߂�X�s�[�h�̏����l
				constexpr float kStartDecelerateSwingSpeedInitialValue = -100.0f;

				constexpr float kInitialSwingSpeed = 1500.0f;	//!< �ŏ��̃X�C���O�X�s�[�h
				//!< �������x���ŏ��̃X�C���O�X�s�[�h�ɗ^����e����
				constexpr float kFallImpactRateForInitialSwingSpeed = 0.5f;
				constexpr float kMaxSwingSpeed = 4000.0f;	//!< �ő�X�C���O�X�s�[�h
				//!< �X�C���O�̉����������p������
				constexpr float kTakeOverSwingAccelerationRate = 0.5f;

				//!< �X�C���O��̉����̏���
				constexpr float kInitialVelocityOfAterSwingAcceleration = 1500.0f;
				//!< �X�C���O��̉����̍Œᑬ�x
				constexpr float kMinVelocityOfAfterSwingAcceleration = 
					kInitialVelocityOfAterSwingAcceleration * 0.1f;

				constexpr float kMaxCameraOffsetUp = 40.0f;
			}
			/**
			 * @brief �v���C���[���f���N���X�̒萔�f�[�^
			*/
			namespace nsPlayerModelRenderConstData
			{
				//!< �v���C���[�̃��f���f�[�^�̃t�@�C�X�p�X
				constexpr const char* const kPlayerModelFilePath = 
					"Assets/modelData/kunoichi_ver2.1/kunoichi_ver2.1.tkm";

				/**
				 * @brief �A�j���[�V�����N���b�v�X
				*/
				enum EnAnimationClips
				{
					enAnim_idle,	//!< �A�C�h��
					//enAnim_ninjaIdle,	//!< �E�҃A�C�h��
					enAnim_walk,	//!< ����
					enAnim_run,		//!< ����
					enAnim_jump,	//!< �W�����v
					enAnim_swingStart,	//!< �X�C���O�X�^�[�g
					//enAnim_swinging,	//!< �X�C���M���O
					//enAnim_swingToLand,	//!< �X�C���O���璅�n
					enAnim_num		//!< �A�j���[�V�����N���b�v�̐�
				};

				//!< �A�j���[�V�����̃t�@�C���p�X
				constexpr const char* const kAnimationFilePath[enAnim_num] =
				{
					"Assets/animData/kunoichi/idle.tka",
					"Assets/animData/kunoichi/walk.tka",
					"Assets/animData/kunoichi/run.tka",
					"Assets/animData/kunoichi/jump.tka",
					"Assets/animData/kunoichi/swingStart.tka",

					//"Assets/animData/sayaka_kunoichi_animation/ninjaIdle.tka",
					//"Assets/animData/sayaka_kunoichi_animation/walk.tka",
					//"Assets/animData/sayaka_kunoichi_animation/run.tka",
					//"Assets/animData/sayaka_kunoichi_animation/jump.tka",
					//"Assets/animData/sayaka_kunoichi_animation/swingStart.tka",
					//"Assets/animData/sayaka_kunoichi_animation/swinging.tka",
					//"Assets/animData/sayaka_kunoichi_animation/swingToLand.tka",

				};

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

				constexpr float kTargetOffsetUp = 40.0f;		//!< �����_�̏㉺�̃I�t�Z�b�g
				constexpr float kTargetOffsetForward = 20.0f;	//!< �����_�̑O��̃I�t�Z�b�g

				constexpr float kDefaultCameraFar = 100000.0f;	//!< �f�t�H���g�̉�����

				//!< �f�t�H���g�̒����_���王�_�ւ̃x�N�g��
				static const Vector3 kDefaultToCameraVec = { 0.0f,50.0f,-400.0f };

				//!< �f�t�H���g�̒����_���王�_�ւ̋���
				constexpr float kDefaultToCameraDistance = 400.0f;

				constexpr float kCameraRotSpeed = 3.0f;		//!< �J�����̉�]����X�s�[�h
				//!< �����_���王�_�ւ̕����x�N�g����Y�̍ő�l�B�J�����̉������̏���B
				constexpr float kMaxToCameraDirY = 0.8f;
				//!< �����_���王�_�ւ̕����x�N�g����Y�̍ŏ��l�B�J�����̏�����̏���B
				constexpr float kMinToCameraDirY = -0.5f;

				//!< �o�l�̌������B�l���傫���قǁA�J�������x��ĒǏ]���Ă���
				constexpr float kSpringDampingRate = 0.7f;

				constexpr float kAutoTurnStartTime = 1.0f;	//!< �����ŃJ��������]���n�߂�^�C��
				//!< �����ŃJ��������]���n�߂�^�C�}�[�����Z�b�g����^�C��
				constexpr float kAutoTurnStartTimerResetTime = 0.1f;
				//!< �J�����������ŉ�]����X�s�[�h���̗ݏ�̐�
				constexpr float kAutoTurnSpeedRatePowPower = 2.0f;
				constexpr float kAutoTurnSpeedMin = 0.01f;	//!< �J�����������ŉ�]����ŏ��X�s�[�h
				constexpr float kAutoTurnSpeedMax = 0.1f;	//!< �J�����������ŉ�]����ő�X�s�[�h
				constexpr float kAutoTurnExecuteThreshold = 0.99f;	//!< �J�����������ŉ�]�����s���邵�����l
				constexpr float kAutoTurnYRate = 0.03f;		//!< �J������Y���W�������ŉ�]���闦
			}
		}
	}
}
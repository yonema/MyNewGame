#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsPlayer 
	{
		class CPlayer;			// �v���C���[�N���X
		namespace nsPlayerMovenent { class CPlayerMovement; }
	}
	namespace nsSound { class CSoundCue; }

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		/**
		 * @brief �v���C���[�̓����֘A�̃l�[���X�y�[�X
		*/
		namespace nsPlayerMovenent
		{
			/**
			 * @brief �v���C���[�̕����Ƒ���N���X
			*/
			class CPlayerWalkAndRun : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CPlayerWalkAndRun();
				/**
				 * @brief �f�X�g���N�^
				*/
				~CPlayerWalkAndRun();

			public:		// �����o�֐�

				/**
				 * @brief ������
				 * @param[in] player �v���C���[
				 * @param[in,out] playerMovement �v���C���[�ړ��N���X�̎Q��
				*/
				void Init(
					const CPlayer& player,
					CPlayerMovement* playerMovement
				);

				/**
				 * @brief �����Ƒ���̏��������s
				*/
				void Execute();

				/**
				 * @brief ���݂̓����ɁA�ړ��p�����[�^�����킹��
				*/
				void MuchMoveParam();

				/**
				 * @brief ���̃N���X�̃T�E���h���~����
				*/
				void StopSound();

				/**
				 * @brief �W�����v�̏㏸�����H�𓾂�
				 * @return �W�����v�̏㏸�����H
				*/
				bool IsJumpUp() const
				{
					return m_isJumpUp;
				}


			private:	// private�ȃ����o�֐�

				/**
				 * @brief �T�E���h�̏�����
				*/
				void InitSound();

				/**
				 * @brief ����������̈ړ��̏���
				*/
				void WalkOrRunMove();

				/**
				 * @brief �����g�����A�N�V�����̌�̋󒆂̏���
				*/
				void AirAfterStringAction();

				/**
				 * @brief �����͒l���X�V
				*/
				void UpdateInputAxisParam();

				/**
				 * @brief ���������肩�����߂�
				*/
				void WalkOrRun();

				/**
				 * @brief �ړ��������X�V����
				*/
				void UpdateMoveDir();

				/**
				 * @brief �������v�Z
				*/
				void CalcAcceleration();

				/**
				 * @brief ���C�̌v�Z
				*/
				void CalcFriction();

				/**
				 * @brief ���x�����̌v�Z
				*/
				void CalcLimitSpeed();

				/**
				 * @brief ���ۂɈړ�������
				*/
				void Move();

				/**
				 * @brief �W�����v�̍X�V
				*/
				void UpdateJump();

				/**
				 * @brief �T�E���h�̍X�V
				*/
				void UpdateSound();

			private:	// �f�[�^�����o
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< �ړ��N���X�̎Q��
				const CPlayer* m_playerRef = nullptr;			//!< �v���C���[�̎Q��
				Vector3 m_moveDir = Vector3::Zero;				//!< �ړ�����
				float m_velocity = 0.0f;						//!< �X�s�[�h
				float m_oldVelocity = 0.0f;						//!< �O�̃t���[���̑��x
				bool m_isJumpUp = false;						//!< �W�����v�̏㏸�����H
				float m_jumpUpTimer = 0.0f;						//!< �W�����v�̏㏸���̃^�C�}�[

				// �����͒l
				float m_inputMoveF = 0.0f;			//!< �O�A��ړ��̎����͒l
				float m_inputMoveR = 0.0f;			//!< �E�A���ړ��̎����͒l

				// �����Ƒ���ŕς��p�����[�^
				float m_acceleration = 0.0f;	//!< �����x
				float m_maxSpeed = 0.0f;		//!< �ō����x

				// �T�E���h
				nsSound::CSoundCue* m_walkSC = nullptr;		//!< �����̃T�E���h
				nsSound::CSoundCue* m_runSC = nullptr;		//!< ����̃T�E���h
				nsSound::CSoundCue* m_jumpSC = nullptr;		//!< �W�����v�̃T�E���h
				//!< �W�����v���̃{�C�X�̃T�E���h
				nsSound::CSoundCue* 
					m_jumpVoiceSC[nsPlayerConstData::nsWalkAndRunConstData::kJumpVoiceTypeNum] = {};

				std::unique_ptr<std::mt19937> m_mt;							//!< �����Z���k�c�C�X�^�[��32�r�b�g��
				std::unique_ptr<std::uniform_int_distribution<>> m_rand;	//!< �͈͕t���̈�l����

			};

		}
	}
}
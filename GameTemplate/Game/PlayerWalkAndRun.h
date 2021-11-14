#pragma once
#include "Noncopyable.h"

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �O���錾
		class CPlayer;			// �v���C���[�N���X

		/**
		 * @brief �v���C���[�̓����֘A�̃l�[���X�y�[�X
		*/
		namespace nsPlayerMovenent
		{
			// �O���錾
			class CPlayerMovement;	// �v���C���[�̓����N���X

			/**
			 * @brief �v���C���[�̕����Ƒ���N���X
			*/
			class CPlayerWalkAndRun : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CPlayerWalkAndRun() = default;
				/**
				 * @brief �f�X�g���N�^
				*/
				~CPlayerWalkAndRun() = default;

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


			private:	// private�ȃ����o�֐�

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

			private:	// �f�[�^�����o
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< �ړ��N���X�̎Q��
				const CPlayer* m_playerRef = nullptr;			//!< �v���C���[�̎Q��
				Vector3 m_moveDir = Vector3::Zero;				//!< �ړ�����
				float m_velocity = 0.0f;						//!< �X�s�[�h
				float m_oldVelocity = 0.0f;						//!< �O�̃t���[���̑��x

				// �����͒l
				float m_inputMoveF = 0.0f;			//!< �O�A��ړ��̎����͒l
				float m_inputMoveR = 0.0f;			//!< �E�A���ړ��̎����͒l

				// �����Ƒ���ŕς��p�����[�^
				float m_acceleration = 0.0f;	//!< �����x
				float m_maxSpeed = 0.0f;		//!< �ō����x
			};

		}
	}
}
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

			private:	// private�ȃ����o�֐�

				/**
				 * @brief �����͒l���X�V
				*/
				void UpdateInputAxisParam();

				/**
				 * @brief ���������肩�����߂�
				*/
				void WalkOrRun();

				/**
				 * @brief �ړ����x������������
				*/
				void Acceleration();

				/**
				 * @brief ���C�̌v�Z
				*/
				void Friction();

				/**
				 * @brief �ړ����x�ɑ��x������������
				*/
				void LimitSpeed();

			private:	// �f�[�^�����o
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< �ړ��N���X�̎Q��
				const CPlayer* m_playerRef = nullptr;			//!< �v���C���[�̎Q��
				float m_oldVelocity = 0.0f;						//!< �O�̃t���[���̑��x

				// �����͒l
				float m_inputMoveF = 0.0f;			//!< �O�A��ړ��̎����͒l
				float m_inputMoveR = 0.0f;			//!< �E�A���ړ��̎����͒l
				float m_absInputMoveF = 0.0f;		//!< �O�A��ړ��̎����͂̐�Βl
				float m_absInputMoveR = 0.0f;		//!< �E�A���ړ��̎����͂̐�Βl

				// �����Ƒ���ŕς��p�����[�^
				float m_acceleration = 0.0f;	//!< �����x
				float m_maxSpeed = 0.0f;		//!< �ō����x
			};

		}
	}
}
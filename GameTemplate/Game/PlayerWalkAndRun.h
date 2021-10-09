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
				 * @brief �ړ����x������������
				 * @param inputMoveF �O�A��̎�����
				 * @param inputMoveR �E�A���̎�����
				*/
				void Acceleration(const float inputMoveF, const float inputMoveR);

				/**
				 * @brief ���C�̌v�Z
				 * @param[in] absInputMoveF �O�A��̎����͂̐�Βl
				 * @param[in] absInputMoveR �E�A���̎����͂̐�Βl
				*/
				void Friction(const float absInputMoveF, const float absInputMoveR);

				/**
				 * @brief �ړ����x�ɑ��x������������
				 * @param[in] absInputMoveF �O�A��̎����͂̐�Βl
				 * @param[in] absInputMoveR �E�A���̎����͂̐�Βl
				*/
				void LimitSpeed(const float absInputMoveF, const float absInputMoveR);

			private:	// �f�[�^�����o
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< �ړ��N���X�̎Q��
				const CPlayer* m_playerRef = nullptr;			//!< �v���C���[�̎Q��
				float m_oldVelocity = 0.0f;						//!< �O�̃t���[���̑��x
			};

		}
	}
}
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
				using EnSwingActionState = nsPlayerConstData::nsPlayerSwingActionConstData::EnSwingActionState;

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
				*/
				void Init(
					CPlayer* player,
					CPlayerMovement* playerMovement
				);

				/**
				 * @brief �X�C���O�A�N�V���������s
				 * @return �ʏ�̓����̏������s�����H
				*/
				bool Execute();

				/**
				 * @brief �X�C���O���̑��x�����Z�b�g����
				*/
				void ResetSwingSpeed()
				{
					m_swingSpeed = 0.0f;
					m_swingSpeed = -100.0f;
				}

			private:	// private�ȃ����o�֐�

				/**
				 * @brief �X�C���O�^�[�Q�b�g��T��
				*/
				void FindSwingTarget();

				/**
				 * @brief �X�C���O�A�N�V�����̏���
				*/
				void SwingAction();

			private:	// �f�[�^�����o
				CPlayer* m_playerRef = nullptr;					//!< �v���C���[�N���X�̎Q��
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< �v���C���[�ړ��N���X�̎Q��
				const Vector3* m_swingTargetPos = nullptr;		//!< �X�C���O�^�[�Q�b�g�̍��W
				//!< �X�C���O�A�N�V�����̃X�e�[�g
				EnSwingActionState m_swingActionState = 
					nsPlayerConstData::nsPlayerSwingActionConstData::enFindSwingTarget;
				float m_swingRadAngle = 0.0f;						//!< �X�C���O�̃��W�A���p�x
				int m_swingCounter = 0;
				float m_swingSpeed = 0.0f;
				float m_swingSpeed2 = -100.0f;


			};
		}
	}
}

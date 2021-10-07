#pragma once
#include "Noncopyable.h"
#include "PlayerWalkAndRun.h"

// �f�o�b�N�p
#ifdef MY_DEBUG
#include "FontRender.h"
#endif

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �O���錾
		class CPlayerCamera;	// �v���C���[�J�����N���X
		struct SPlayerInputData;	//�v���C���[�̓��͏��\����

		/**
		 * @brief �v���C���[�̓����֘A�̃l�[���X�y�[�X
		*/
		namespace nsPlayerMovenent
		{
			/**
			 * @brief �v���C���[�̓����N���X
			*/
			class CPlayerMovement : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CPlayerMovement() = default;
				/**
				 * @brief �f�X�g���N�^
				*/
				~CPlayerMovement()
				{
#ifdef MY_DEBUG
					DeleteGO(m_moveVecFont);
#endif
					return;
				}

			public:		// �����o�֐�

				/**
				 * @brief ������
				 * @param[in] radius �J�v�Z���R���C�_�[�̔��a
				 * @param[in] height �J�v�Z���R���C�_�[�̍���
				 * @param[in,out] position �v���C���[�̍��W�̎Q��
				 * @param[in,out] rotation �v���C���[�̉�]�̎Q��
				 * @param[in] playerCamera �v���C���[�J����
				 * @param[in] playerInputData �v���C���[���͏��
				*/
				void Init(
					const float radius,
					const float height,
					Vector3* position,
					Quaternion* rotation,
					const CPlayerCamera& playerCamera,
					const SPlayerInputData& playerInputData
				);

				/**
				 * @brief �v���C���[�̈ړ��N���X�̃A�b�v�f�[�g�����s����
				*/
				void ExecuteUpdate();


			private:	// private�ȃ����o�֐�

				/**
				 * @brief �v���C���[�̈ړ����X�V
				*/
				void UpdateMovePlayer();

				/**
				 * @brief �v���C���[�̉�]���X�V
				*/
				void UpdateTurnPlayer();

			private:	// �f�[�^�����o
				CharacterController m_charaCon;			//!< �L�����N�^�[�R���g���[���[

				CPlayerWalkAndRun m_playerWalkAndRun;	//!< �v���C���[�̕����Ƒ���N���X

				Vector3 m_moveVec = Vector3::Zero;			//!< �ړ��x�N�g��
				Vector3 m_addMoveVec = Vector3::Zero;		//!< ���Z�ړ��x�N�g��

				const CPlayerCamera* m_playerCamera = nullptr;			//!< �v���C���[�J�����̎Q��
				const SPlayerInputData* m_playerInputData = nullptr;	//!< �v���C���[���͏��̎Q��
				Vector3* m_playerPosition = nullptr;					//!< �v���C���[�̍��W�̎Q��
				Quaternion* m_playerRotation = nullptr;					//!< �v���C���[�̉�]�̎Q��

				// �f�o�b�N�p
#ifdef MY_DEBUG
				nsGraphic::nsFont::CFontRender* m_moveVecFont = nullptr;
#endif

			};
		}
	}
}

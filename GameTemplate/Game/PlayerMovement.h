#pragma once
#include "Noncopyable.h"
#include "PlayerWalkAndRun.h"
#include "PlayerSwingAction.h"
#include "PlayerConstData.h"
#include "GameTime.h"

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �O���錾
		class CPlayer;	// �v���C���[�N���X

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
				~CPlayerMovement() = default;

			public:		// �����o�֐�

				/**
				 * @brief ������
				 * @param[in] radius �J�v�Z���R���C�_�[�̔��a
				 * @param[in] height �J�v�Z���R���C�_�[�̍���
				 * @param[in,out] player �v���C���[�̎Q��
				*/
				void Init(
					const float radius,
					const float height,
					CPlayer* player
				);

				/**
				 * @brief �v���C���[�̈ړ��N���X�̃A�b�v�f�[�g�����s����
				*/
				void ExecuteUpdate();

				/**
				 * @brief �ړ��x�N�g���ɉ��Z����
				 * @param[in] addMoveVec ���Z�ړ��x�N�g��
				*/
				void AddMoveVec(const Vector3& addMoveVec)
				{
					m_moveVec += addMoveVec;
				}

				/**
				 * @brief �ړ��x�N�g���𓾂�
				 * @return �ړ��x�N�g��
				*/
				const Vector3& GetMoveVec() const
				{
					return m_moveVec;
				}

				/**
				 * @brief �ړ����x�𓾂�
				 * @return �ړ����x
				*/
				float GetVelocity() const
				{
					Vector3 moveVecXZ = m_moveVec;
					moveVecXZ.y = 0.0f;
					return moveVecXZ.Length();
				}

				/**
				 * @brief �ړ��x�N�g����X�����̃��Z�b�g����
				*/
				void ResetMoveVecX() { m_moveVec.x = 0.0f; }
				/**
				 * @brief �ړ��x�N�g����Y�����̃��Z�b�g����
				*/
				void ResetMoveVecY() { m_moveVec.y = 0.0f; }
				/**
				 * @brief �ړ��x�N�g����Z�����̃��Z�b�g����
				*/
				void ResetMoveVecZ() { m_moveVec.z = 0.0f; }

				/**
				 * @brief �󒆂��H�𓾂�
				 * @return �󒆂��H
				*/
				bool IsAir() const
				{
					return !m_charaCon.IsOnGround();
				}

				/**
				 * @brief �X�C���O�A�N�V�����̎Q�Ƃ𓾂�
				 * @return �X�C���O�A�N�V�����̎Q��
				*/
				const CPlayerSwingAction& GetPlayerSwingAction() const
				{
					return m_playerSwingAction;
				}

			private:	// private�ȃ����o�֐�

				/**
				 * @brief �v���C���[�̈ړ����X�V
				*/
				void UpdateMovePlayer();

				/**
				 * @brief �v���C���[�̉�]���X�V
				*/
				void UpdateTurnPlayer();

				/**
				 * @brief �d�͂�������
				*/
				void ApplyGravity();

				/**
				 * @brief �L�����N�^�[�R���g���[���[���g�����ړ�
				*/
				void MoveWithCharacterController();

			private:	// �f�[�^�����o
				CPlayer* m_playerRef = nullptr;			//!< �v���C���[�̎Q��
				Vector3 m_moveVec = Vector3::Zero;		//!< �ړ��x�N�g��
				CharacterController m_charaCon;			//!< �L�����N�^�[�R���g���[���[

				CPlayerWalkAndRun m_playerWalkAndRun;	//!< �v���C���[�̕����Ƒ���N���X
				CPlayerSwingAction m_playerSwingAction;	//!< �v���C���[�̃X�C���O�A�N�V�����N���X
				bool m_useGravity = true;				//!< �d�͂��g�p���邩�H

			};
		}
	}
}

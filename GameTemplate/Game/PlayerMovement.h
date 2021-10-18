#pragma once
#include "Noncopyable.h"
#include "PlayerNormalMovement.h"
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
				 * @brief �d�͂�������
				*/
				void ApplyGravity()
				{
					m_moveVec.y -= nsPlayerConstData::nsPlayerMoveConstData::kGravityScale *
						nsTimer::GameTime().GetFrameDeltaTime();
					if (m_moveVec.y < -nsPlayerConstData::nsPlayerMoveConstData::kMaxFallSpeed)
					{
						m_moveVec.y = -nsPlayerConstData::nsPlayerMoveConstData::kMaxFallSpeed;
					}
				}

				/**
				 * @brief �L�����N�^�[�R���g���[���[���g�����ړ�
				*/
				void MoveWithCharacterController();

				/**
				 * @brief �󒆂��H�𓾂�
				 * @return �󒆂��H
				*/
				bool IsAir() const
				{
					return !m_charaCon.IsOnGround();
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

			private:	// �f�[�^�����o
				CPlayer* m_playerRef = nullptr;			//!< �v���C���[�̎Q��
				Vector3 m_moveVec = Vector3::Zero;		//!< �ړ��x�N�g��
				CharacterController m_charaCon;			//!< �L�����N�^�[�R���g���[���[

				CPlayerNormalMovement m_playerNormalMovement;	//!< �v���C���[�̒ʏ�̓����N���X
				CPlayerSwingAction m_playerSwingAction;	//!< �v���C���[�̃X�C���O�A�N�V�����N���X


			};
		}
	}
}

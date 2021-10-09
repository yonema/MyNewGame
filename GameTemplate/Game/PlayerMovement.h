#pragma once
#include "Noncopyable.h"
#include "PlayerWalkAndRun.h"
#include "PlayerSwingAction.h"

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
				 * @brief �ړ��x�N�g����ݒ肷��
				 * @param[in] addMoveVec �ړ��x�N�g��
				*/
				void SetAddMoveVec(const Vector3& addMoveVec)
				{
					m_addMoveVec = addMoveVec;
				}

				/**
				 * @brief �ړ��x�N�g���𓾂�
				 * @return �ړ��x�N�g��
				*/
				const Vector3& GetAddMoveVec() const
				{
					return m_addMoveVec;
				}

				/**
				 * @brief �ړ��׃N�g���𐳋K������
				*/
				void NormalizeAddMoveVec()
				{
					m_addMoveVec.Normalize();
				}

				/**
				 * @brief �󒆂��H�𓾂�
				 * @return �󒆂��H
				*/
				bool IsAir() const
				{
					return m_charaCon.IsJump();
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

				CharacterController m_charaCon;			//!< �L�����N�^�[�R���g���[���[

				CPlayerWalkAndRun m_playerWalkAndRun;	//!< �v���C���[�̕����Ƒ���N���X
				CPlayerSwingAction m_playerSwingAction;	//!< �v���C���[�̃X�C���O�A�N�V�����N���X

				Vector3 m_moveVec = Vector3::Zero;		//!< �ړ��x�N�g��
				Vector3 m_addMoveVec = Vector3::Zero;	//!< ���Z�ړ��x�N�g��


				// �f�o�b�N�p
#ifdef MY_DEBUG
				nsGraphic::nsFont::CFontRender* m_moveVecFont = nullptr;
#endif

			};
		}
	}
}

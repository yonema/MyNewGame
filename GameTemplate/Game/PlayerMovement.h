#pragma once
#include "Noncopyable.h"
#include "PlayerWalkAndRun.h"
#include "PlayerSwingAction.h"
#include "PlayerWallRun.h"
#include "PlayerOnEnemy.h"
#include "PlayerConstData.h"
#include "GameTime.h"

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsPlayer
	{
		class CPlayer;			// �v���C���[�N���X
		class CPlayerCamera;	// �v���C���[�J�����N���X
		class CPlayerModelAnimation; // �v���C���[���f���A�j���[�V�����N���X
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
			 * @brief �v���C���[�̓����N���X
			*/
			class CPlayerMovement : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CPlayerMovement();
				/**
				 * @brief �f�X�g���N�^
				*/
				~CPlayerMovement();

			public:		// �����o�֐�

				/**
				 * @brief ������
				 * @param[in] radius �J�v�Z���R���C�_�[�̔��a
				 * @param[in] height �J�v�Z���R���C�_�[�̍���
				 * @param[in,out] player �v���C���[�̎Q��
				 * @param[in,out] playerCamera �v���C���[�J�����̎Q��
				 * @param[in.out] playerModelAnimation �v���C���[���f���A�j���[�V�����̎Q��
				*/
				void Init(
					const float radius,
					const float height,
					CPlayer* player,
					CPlayerCamera* playerCamera,
					CPlayerModelAnimation* playerModelAnimation
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
				 * @brief ���Z�ړ��x�N�g���ɐ�����������
				 * @param[in] limit ����
				*/
				void LimitMoveVec(const float limit);

				/**
				 * @brief �ړ����x�𓾂�
				 * @return �ړ����x
				*/
				float GetXZSpeed() const
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
				 * @brief �ǂɐG��Ă��邩�H�𓾂�
				 * @return �ǂɐG��Ă��邩�H
				*/
				bool IsOnWall() const
				{
					return m_charaCon.IsOnWall();
				}

				/**
				 * @brief ���ڍ��W��ݒ�
				 * @param[in] pos ���W
				*/
				void SetDirectPosition(const Vector3& pos);

				/**
				 * @brief �d�͂��g�p���邩�H��ݒ肷��
				 * @param useGravity �d�͂��g�p���邩�H
				*/
				void SetUseGravity(const bool useGravity)
				{
					m_useGravity = useGravity;
				}

				/**
				 * @brief �����Ƒ���̃N���X�̈ړ��p�����[�^�����킹��
				*/
				void MuchWalkAndRunMoveParam()
				{
					m_playerWalkAndRun.MuchMoveParam();
				}

				/**
				 * @brief �X�C���O�A�N�V�����̎Q�Ƃ𓾂�
				 * @return �X�C���O�A�N�V�����̎Q��
				*/
				const CPlayerSwingAction& GetPlayerSwingAction() const
				{
					return m_playerSwingAction;
				}

				/**
				 * @brief �v���C���[���G�̏�ɏ���Ă��鏈���̎Q�Ƃ𓾂�
				 * @return �v���C���[���G�̏�ɏ���Ă��鏈���̎Q��
				*/
				const CPlayerOnEnemy& GetPlayerOnEnemy() const
				{
					return m_playerOnEnemy;
				}

				/**
				 * @brief �G�̏�ɏ�鏈���N���X��������Ԃɖ߂�
				*/
				void ResetOnEnemy()
				{
					m_playerOnEnemy.Reset();
				}

				/**
				 * @brief �X�C���O�A�N�V�����N���X��������Ԃɖ߂�
				*/
				void ResetSwing()
				{
					m_playerSwingAction.Reset();
				}

				/**
				 * @brief �����Ƒ���̃T�E���h���~����
				*/
				void StopWalkAndRunSound()
				{
					m_playerWalkAndRun.StopSound();
				}

			private:	// private�ȃ����o�֐�

				/**
				 * @brief �X�e�[�g���X�V����
				*/
				void UpdateState();

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

				/**
				 * @brief �T�E���h�̍X�V
				*/
				void UpdateSound();

			private:	// �f�[�^�����o
				CPlayer* m_playerRef = nullptr;			//!< �v���C���[�̎Q��
				Vector3 m_moveVec = Vector3::Zero;		//!< �ړ��x�N�g��
				CharacterController m_charaCon;			//!< �L�����N�^�[�R���g���[���[

				CPlayerWalkAndRun m_playerWalkAndRun;	//!< �v���C���[�̕����Ƒ���N���X
				CPlayerSwingAction m_playerSwingAction;	//!< �v���C���[�̃X�C���O�A�N�V�����N���X
				CPlayerWallRun m_playerWallRun;			//!< �v���C���[�̕ǂ𑖂鏈���N���X
				CPlayerOnEnemy m_playerOnEnemy;			//!< �v���C���[�̓G�̏�ɏ��N���X
				bool m_useGravity = true;				//!< �d�͂��g�p���邩�H

				nsSound::CSoundCue* m_landingSC = nullptr;	//!< ���n�̃T�E���h
				bool m_landingSoundFlag = false;			//!< ���n�̃T�E���h�𗬂��邩�H
			};
		}
	}
}

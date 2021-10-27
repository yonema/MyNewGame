#pragma once
#include "PlayerModelAnimation.h"
#include "PlayerMovement.h"
#include "PlayerInput.h"
#include "PlayerCamera.h"
#include "PlayerStringModel.h"
#include "PlayerConstData.h"

namespace nsMyGame
{

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �O���錾
		class CPlayerModelAnimation;	// �v���C���[���f���A�j���[�V�����N���X

		/**
		 * @brief �v���C���[�N���X
		*/
		class CPlayer : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayer() = default;
			/**
			 * @brief �R���X�g���N�^
			*/
			~CPlayer() = default;

		public:		// �I�[�o�[���C�h���������o�֐�

			/**
			 * @brief �X�^�[�g�֐�
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool Start() override final;

			/**
			 * @brief �j���������ɌĂ΂��֐�
			*/
			void OnDestroy() override final;

			/**
			 * @brief �A�b�v�f�[�g�֐�
			*/
			void Update() override final;

		public:		// �����o�֐�

			/**
			 * @brief ���W��ݒ肷��
			 * @param[in] pos ���W
			*/
			void SetPosition(const Vector3& pos)
			{
				m_position = pos;
			}

			/**
			 * @brief ���W�𓾂�
			 * @return ���W
			*/
			const Vector3& GetPosition() const
			{
				return m_position;
			}

			/**
			 * @brief ��]���Z�b�g����
			 * @param[in] rot ��]
			*/
			void SetRotation(const Quaternion& rot)
			{
				m_rotation = rot;
			}

			/**
			 * @brief ��]�𓾂�
			 * @return ��]
			*/
			const Quaternion& GetRotation() const
			{
				return m_rotation;
			}

			/**
			 * @brief �v���C���[�̓��͏��𓾂�
			 * @return �v���C���[�̓��͏��
			*/
			const SPlayerInputData& GetInputData() const
			{
				return m_playerInput.GetPlayerInputData();
			}

			/**
			 * @brief �v���C���[�̃J�����𓾂�
			 * @return �v���C���[�̃J����
			*/
			const CPlayerCamera& GetCamera() const
			{
				return m_playerCamera;
			}

			/**
 * @brief �v���C���[�̃J�����𓾂�
 * @return �v���C���[�̃J����
*/
			CPlayerCamera& GetCamera()
			{
				return m_playerCamera;
			}

			/**
			 * @brief �v���C���[�̈ړ��N���X�̎Q�Ƃ𓾂�
			 * @return �v���C���[�̈ړ��N���X�̎Q��
			*/
			const nsPlayerMovenent::CPlayerMovement& GetPlayerMovement() const
			{
				return m_playerMove;
			}

			/**
			 * @brief �����Ƒ����Ԃ֑J�ڂ���
			*/
			void ChangeWalkAndRunState();

			/**
			 * @brief �X�C���O��Ԃ֑J�ڂ���
			*/
			void ChangeSwingState();

			/**
			 * @brief �Ǒ����Ԃ֑J�ڂ���
			*/
			void ChangeWallRun();

			/**
			 * @brief �v���C���[�̃X�e�[�g�𓾂�
			 * @return �X�e�[�g
			*/
			nsPlayerConstData::EnPlayerState GetState() const
			{
				return m_playerState;
			}

			/**
			 * @brief �����w�肵�����W�Ɍ������ĐL�т鏈�����J�n����
			 * @param[in] pos �L�т��̍��W
			*/
			void StartStringStretchToPos(const Vector3& pos);

			/**
			 * @brief �����w�肵�����W�Ɍ������ĐL�т鏈�����I������
			*/
			void EndStringStretchToPos();

			/**
			 * @brief �����L�т����Ă��邩�H�𓾂�
			 * @return �����L�т����Ă��邩�H
			*/
			bool IsStringStretched() const
			{
				return m_playerStringModel->IsStretched();
			}

			/**
			 * @brief ���̒����𓾂�
			 * @return ���̒���
			*/
			float GetStringLength() const
			{
				return m_playerStringModel->GetStringLength();
			}

		private:	// private�ȃ����o�֐�

		private:	// �f�[�^�����o
			Vector3 m_position = Vector3::Zero;				//!< ���W
			Quaternion m_rotation = Quaternion::Identity;	//!< ��]

			CPlayerModelAnimation m_playerModelAnimation;	//!< �v���C���[���f���A�j���[�V�����N���X
			CPlayerInput m_playerInput;						//!< �v���C���[���̓N���X
			CPlayerCamera m_playerCamera;					//!< �v���C���[�J�����N���X
			nsPlayerMovenent::CPlayerMovement m_playerMove;	//!< �v���C���[�ړ��N���X
			//!< �v���C���[�̃X�e�[�g
			nsPlayerConstData::EnPlayerState m_playerState = nsPlayerConstData::enWalkAndRun;
			CPlayerStringModel* m_playerStringModel = nullptr;	//!< �v���C���[�̎��̃��f���N���X
		};

	}
}



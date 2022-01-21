#pragma once
#include "PlayerModelAnimation.h"
#include "PlayerMovement.h"
#include "PlayerInput.h"
#include "PlayerCamera.h"
#include "PlayerStringModel.h"
#include "PlayerCatchEnemy.h"
#include "PlayerConstData.h"

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsPlayer { class CPlayerModelAnimation; }
	namespace nsAICharacter { class CAICar; }

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
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
			 * @brief ���͉\���H��ݒ肷��
			 * @param[in] isInputtable ���͉\���H
			*/
			void SetIsInputtable(const bool isInputtable)
			{
				m_isInputtable = isInputtable;
			}

			/**
			 * @brief ���͉\���H�𓾂�
			 * @return ���͉\���H
			*/
			bool IsInputtable() const
			{
				return m_isInputtable;
			}

			/**
			 * @brief �^�C�g�����[�h�ɐݒ�
			*/
			void TitleMode()
			{
				m_isTitleMode = true;
				SetIsInputtable(false);
			}

			/**
			 * @brief �^�C�g�����[�h���H�𓾂�
			 * @return �^�C�g�����[�h���H
			*/
			bool IsTitleMode() const
			{
				return m_isTitleMode;
			}

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
				return m_playerInput->GetPlayerInputData();
			}

			/**
			 * @brief �v���C���[�̃J�����𓾂�
			 * @return �v���C���[�̃J����
			*/
			const CPlayerCamera& GetCamera() const
			{
				return *m_playerCamera;
			}

			/**
			 * @brief �v���C���[�̈ړ��N���X�̎Q�Ƃ𓾂�
			 * @return �v���C���[�̈ړ��N���X�̎Q��
			*/
			const nsPlayerMovenent::CPlayerMovement& GetPlayerMovement() const
			{
				return *m_playerMove;
			}

			/**
			 * @brief �v���C���[���f���A�j���[�V�����N���X�̎Q�Ƃ𓾂�
			 * @return �v���C���[���f���A�j���[�V�����N���X�̎Q��
			*/
			const CPlayerModelAnimation& GetPlayerModelAnimation() const
			{
				return *m_playerModelAnimation;
			}

			/**
			 * @brief �^�[�Q�b�g�̓G�̎Q�Ƃ𓾂�
			 * @return �^�[�Q�b�g�̓G�̎Q��
			*/
			const CPlayerCatchEnemy& GetCatchEnemy() const
			{
				return *m_playerCatchEnemy;
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
			void ChangeWallRunState();

			/**
			 * @brief �G�̏�ɏ���Ă����Ԃ֑J�ڂ���
			*/
			void ChangeOnEnemyState();

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
			 * @brief �L�т��̍��W��ݒ肷��
			 * @param[in] pos ���W
			*/
			void SetStringStretchPos(const Vector3& pos);

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

			/**
			 * @brief �G�̏�ɏ���Ă���J�������H��ݒ�
			 * @param[in] isOnEnemyCamera �G�̏�ɏ���Ă���J�������H
			*/
			void SetIsOnEnemyCamera(const bool isOnEnemyCamera)
			{
				m_playerCamera->SetIsOnEnemyCamera(isOnEnemyCamera);
				if (isOnEnemyCamera)
					m_playerCamera->LerpDampingRate(1.0f);
				else
					m_playerCamera->LerpDampingRate(0.0f);
			}

			/**
			 * @brief �G�̏�ɏ�鏈���N���X��������Ԃɖ߂�
			*/
			void ResetOnEnemy()
			{
				m_playerMove->ResetOnEnemy();
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �X�e�[�g�J��
			 * @param newState[in] �V�����X�e�[�g
			*/
			void ChangeState(nsPlayerConstData::EnPlayerState newState);

		private:	// �f�[�^�����o
			Vector3 m_position = Vector3::Zero;				//!< ���W
			Quaternion m_rotation = Quaternion::Identity;	//!< ��]

			std::unique_ptr<CPlayerModelAnimation> m_playerModelAnimation;	//!< �v���C���[���f���A�j���[�V�����N���X
			std::unique_ptr <CPlayerInput> m_playerInput;						//!< �v���C���[���̓N���X
			std::unique_ptr <CPlayerCamera> m_playerCamera;					//!< �v���C���[�J�����N���X
			std::unique_ptr <nsPlayerMovenent::CPlayerMovement> m_playerMove;	//!< �v���C���[�ړ��N���X
			std::unique_ptr <CPlayerCatchEnemy> m_playerCatchEnemy;			//!< �v���C���[���G��߂܂��鏈���N���X
			//!< �v���C���[�̃X�e�[�g
			nsPlayerConstData::EnPlayerState m_playerState = nsPlayerConstData::enWalkAndRun;
			CPlayerStringModel* m_playerStringModel = nullptr;	//!< �v���C���[�̎��̃��f���N���X

			bool m_isInputtable = true;		//!< ���͉\���H
			bool m_isTitleMode = false;		//!< �^�C�g�����[�h���H
		};

	}
}



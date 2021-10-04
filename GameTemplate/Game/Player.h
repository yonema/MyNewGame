#pragma once
#include "PlayerModel.h"
#include "PlayerMovement.h"
#include "PlayerInput.h"
#include "PlayerCamera.h"

namespace nsMyGame
{

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �O���錾
		class CPlayerModel;	// �v���C���[���f���N���X

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
			 * @brief �����ʒu��ݒ肷��
			 * @param[in] startPos �X�^�[�g���W
			*/
			void SetStartPosition(const Vector3& startPos)
			{
				// �X�^�[�g�֐����܂��������Ă��Ȃ����H
				if (!IsStart())
					m_position = startPos;
			}

		private:	// private�ȃ����o�֐�

		private:	// �f�[�^�����o
			Vector3 m_position = Vector3::Zero;				//!< ���W
			Quaternion m_rotation = Quaternion::Identity;	//!< ��]

			CPlayerModel m_playerModel;						//!< �v���C���[���f���N���X
			CPlayerInput m_playerInput;						//!< �v���C���[���̓N���X
			CPlayerCamera m_playerCamera;					//!< �v���C���[�J�����N���X
			nsPlayerMovenent::CPlayerMovement m_playerMove;						//!< �v���C���[�ړ��N���X

			SPlayerInputData m_playerInputData;				//!< �v���C���[�̓��͏��
		};

	}
}



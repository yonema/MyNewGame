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
		class CPlayer;	// �v���C���[�N���X

		/**
		 * @brief �v���C���[�̓��͏��\����
		*/
		struct SPlayerInputData
		{
			float axisMoveForward = 0.0f;			//!< �O�A��ړ��̎����͏��
			float axisMoveRight = 0.0f;				//!< �E�A���ړ��̎����͏��
			bool inputMoveAxis = false;				//!< �ړ��̎����͂����������H
			float axisCameraRotVertical = 0.0f;		//!< �J�����̏c��]�̎����͏��
			float axisCameraRotHorizontal = 0.0f;	//!< �J�����̉���]�̎����͏��
			bool inputCameraAxis = false;			//!< �J�����̎����͂����������H
			bool actionJump = false;				//!< �W�����v�̓��͏��
			bool actionDush = false;				//!< �_�b�V���̓��͏��
			bool actionSwing = false;				//!< �X�C���O�̓��͏��
			bool actionSeachEnemy = false;			//!< �G��T�m������͏��
		};

		/**
		 * @brief �v���C���[���̓N���X
		*/
		class CPlayerInput : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerInput() = default;
			/**
			 * @brief �R���X�g���N�^
			*/
			~CPlayerInput() = default;

		public:		// �����o�֐�

			/**
			 * @brief ������
			 * @param[in,out] player �v���C���[�̎Q��
			*/
			void Init(CPlayer* player);

			/**
			 * @brief ���͏��������s����
			*/
			void ExecuteUpdate();

			/**
			 * @brief �v���C���[�̓��͏��𓾂�
			 * @return �v���Cy�|�̓��͏��
			*/
			const SPlayerInputData& GetPlayerInputData() const
			{
				return m_playerInputData;
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �����͏����X�V����
			*/
			void UpdateInputAxis();

			/**
			 * @brief �A�N�V�������͏����X�V����
			*/
			void UpdateInputAction();

			/**
			 * @brief �ړ��̎����͏��̍X�V
			*/
			void OnUpdateInputAxisMove();

			/**
			 * @brief �J�����̉�]�̎����͏��̍X�V
			*/
			void OnUpdateInputAxisCameraRot();

		private:	// �f�[�^�����o
			const GamePad* m_pad = nullptr;		//!<  �Q�[���p�b�h
			CPlayer* m_playerRef = nullptr;		//!< �v���C���[�̎Q��
			SPlayerInputData m_playerInputData;	//!< �v���C���[�̓��͏��


		};
	}
}


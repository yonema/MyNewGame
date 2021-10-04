#pragma once
#include "Noncopyable.h"

namespace nsMyGame
{
	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		/**
		 * @brief �v���C���[�̓��͏��\����
		*/
		struct SPlayerInputData
		{
			float axisMoveForward = 0.0f;			//!< �O�A��ړ��̎����͏��
			float axisMoveRight = 0.0f;				//!< �E�A���ړ��̎����͏��
			float axisCameraRotVertical = 0.0f;		//!< �J�����̏c��]�̎����͏��
			float axisCameraRotHorizontal = 0.0f;	//!< �J�����̉���]�̎����͏��
			bool actionJump = false;				//!< �W�����v�̓��͏��
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
			 * @param[in,out] playerInputData �v���C���[�̓��͏��
			*/
			void Init(SPlayerInputData* playerInputData);

			/**
			 * @brief ���͏��������s����
			*/
			void ExecuteInput();

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
			const GamePad* m_pad = nullptr;					//!<  �Q�[���p�b�h
			SPlayerInputData* m_playerInputData = nullptr;	//!< �v���C���[�̓��͏��̎Q��

		};
	}
}


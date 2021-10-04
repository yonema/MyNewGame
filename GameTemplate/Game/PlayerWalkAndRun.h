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
		class CPlayerCamera;		// �v���C���[�J����
		struct SPlayerInputData;	//�v���C���[�̓��͏��\����

		/**
		 * @brief �v���C���[�̓����֘A�̃l�[���X�y�[�X
		*/
		namespace nsPlayerMovenent
		{
			/**
			 * @brief �v���C���[�̕����Ƒ���N���X
			*/
			class CPlayerWalkAndRun : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CPlayerWalkAndRun() = default;
				/**
				 * @brief �f�X�g���N�^
				*/
				~CPlayerWalkAndRun() = default;

			public:		// �����o�֐�

				/**
				 * @brief ������
				 * @param[in,out] moveVecForward �O�����̈ړ��x�N�g���̎Q��
				 * @param[in,out] moveVecRight �E�����̈ړ��x�N�g���̎Q��
				 * @param[in] camera �v���C���[�J����
				 * @param[in] playerInputData �v���C���[���͏��
				*/
				void Init(
					Vector3* moveVecForward,
					Vector3* moveVecRight,
					const CPlayerCamera& camera,
					const SPlayerInputData& playerInputData
				);

				/**
				 * @brief �����Ƒ���̏��������s
				 * @param[in] isAir �󒆂��H
				*/
				void Execute(const bool isAir);

			private:	// private�ȃ����o�֐�

				/**
				 * @brief �ړ����x������������
				 * @param inputMoveF �O�A��̎�����
				 * @param inputMoveR �E�A���̎�����
				*/
				void Acceleration(const float inputMoveF, const float inputMoveR);

				/**
				 * @brief ���C�̌v�Z
				 * @param[in] absInputMoveF �O�A��̎����͂̐�Βl
				 * @param[in] absInputMoveR �E�A���̎����͂̐�Βl
				 * @param[in] isAir �󒆂��H
				*/
				void Friction(const float absInputMoveF, const float absInputMoveR, const bool isAir);

				/**
				 * @brief �ړ����x�ɑ��x������������
				 * @param[in] absInputMoveF �O�A��̎����͂̐�Βl
				 * @param[in] absInputMoveR �E�A���̎����͂̐�Βl
				*/
				void LimitSpeed(const float absInputMoveF, const float absInputMoveR);

			private:	// �f�[�^�����o
				Vector3* m_moveVecForward = nullptr;			//!< �O�����̈ړ��x�N�g��
				Vector3* m_moveVecRight = nullptr;				//!< �E�����̈ړ��x�N�g��
				const CPlayerCamera* m_playerCamera = nullptr;	//!< �v���C���[�J����
				const SPlayerInputData* m_playerInputData = nullptr;	//!< �v���C���[�̓��͏��
			};

		}
	}
}
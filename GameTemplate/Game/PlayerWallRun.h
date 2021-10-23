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
		class CPlayer;			// �v���C���[�N���X

		/**
		 * @brief �v���C���[�̓����֘A�̃l�[���X�y�[�X
		*/
		namespace nsPlayerMovenent
		{
			// �O���錾
			class CPlayerMovement;	// �v���C���[�̓����N���X

			/**
			 * @brief �v���C���[�̕ǂ𑖂鏈���N���X
			*/
			class CPlayerWallRun : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CPlayerWallRun() = default;
				/**
				 * @brief �f�X�g���N�^
				*/
				~CPlayerWallRun() = default;

			public:		// �����o�֐�

				/**
				 * @brief ������
				 * @param[in.out] player �v���C���[
				 * @param[in,out] playerMovement �v���C���[�ړ��N���X�̎Q��
				*/
				void Init(
					CPlayer* player,
					CPlayerMovement* playerMovement
				);

				/**
				 * @brief �E�H�[�����������s
				*/
				void Execute();

			private:	// �f�[�^�����o
				CPlayer* m_playerRef = nullptr;					//!< �v���C���[�N���X�̎Q��
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< �v���C���[�ړ��N���X�̎Q��


			};
		}
	}
}

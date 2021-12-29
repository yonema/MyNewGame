#pragma once
#include "Noncopyable.h"

namespace nsMyGame
{
	// �O���錾

	namespace nsPlayer { class CPlayer; }

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		/**
		 * @brief �v���C���[���G��߂܂��鏈���N���X
		*/
		class CPlayerCatchEnemy : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerCatchEnemy() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CPlayerCatchEnemy() = default;

		public:		// �����o�֐�

			/**
			 * @brief ����������
			 * @param[in] playerRef �v���C���[�̎Q��
			*/
			void Init(CPlayer* playerRef);

			/**
			 * @brief ���C���[���G��߂܂��鏈���N���X�̃A�b�v�f�[�g�����s
			*/
			void ExecuteUpdate();

		private:	// �f�[�^�����o
			CPlayer* m_playerRef = nullptr;	//!< �v���C���[�̎Q��
		};


	}
}

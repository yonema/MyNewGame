#pragma once
#include "AICharacterBase.h"

namespace nsMyGame
{
	/**
	 * @brief AI�L�����N�^�[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief AI�̎ԃN���X
		*/
		class CAICar : public CAICharacterBase
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CAICar() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CAICar() = default;

		public:		// �I�[�o�[���C�h���������o�֐�

			/**
			 * @brief �h���N���X�p��Update�̒��O�ŌĂ΂��J�n����
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool StartSub() override final;

			/**
			 * @brief �h���N���X�p�̏�������鎞�ɌĂ΂�鏈��
			*/
			void OnDestroySub() override final {};

			/**
			 * @brief �h���N���X�p�̍X�V����
			*/
			void UpdateSub() override final;

		public:		// �����o�֐�

		private:	// �f�[�^�����o

			
		};

	}
}
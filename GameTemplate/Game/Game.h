#pragma once

/**
 * @brief ���̃Q�[���̃l�[���X�y�[�X
 * @todo MyGame�������ƃ��j�[�N�ȃQ�[���̃^�C�g�����Ƃ��ɕύX����
*/
namespace nsMyGame
{
	/**
	 * @brief �Q�[���t�@�C���̃l�[���X�y�[�X
	*/
	namespace nsGame
	{
		/**
		 * @brief �Q�[���N���X
		*/
		class CGame : public IGameObject
		{
		public:		//�R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CGame() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CGame() = default;

		public:		//�I�[�o�[���C�h���������o�֐�
			/**
			 * @brief �X�^�[�g�֐�
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool Start() override final;


		private:


		};
	}
}


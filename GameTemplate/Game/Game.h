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
			CGame() = default;	//�R���X�g���N�^
			~CGame() = default;	//�f�X�g���N�^

		public:		//�I�[�o�[���C�h���������o�֐�
			bool Start() override final;		//�X�^�[�g�֐�
			//void Update() override final;		//�A�b�v�f�[�g�֐�


		private:


		};
	}
}


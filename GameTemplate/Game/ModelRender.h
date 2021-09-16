#pragma once

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace Graphic
	{
		/**
		 * @brief ���f���֘A�̃l�[���X�y�[�X
		*/
		namespace Model
		{
			/**
			 * @brief ���f���`��N���X
			*/
			class CModelRender : public IGameObject
			{
			public:		//�R���X�g���N�^�ƃf�X�g���N�^
				CModelRender() = default;	//�R���X�g���N�^
				~CModelRender() = default;	//�f�X�g���N�^

			public:		//�I�[�o�[���C�h���������o�֐�
				bool Start() override final;			//�X�^�[�g�֐�
				void OnDestroy() override final;		//�j�����ꂽ�Ƃ��ɌĂ΂��֐�
				void AlwaysUpdate() override final;		//��ɌĂ΂��A�b�v�f�[�g�֐�


			};

		}
	}
}
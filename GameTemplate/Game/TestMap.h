#pragma once

namespace nsMyGame
{
	//�O���錾
	namespace nsGraphic { 
		namespace nsModel { class CModelRender; }	//���f�������_���[�N���X
	}

	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		/**
		 * @brief �e�X�g�p�̃}�b�v
		*/
		class CTestMap : public IGameObject
		{
		public:		//�R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CTestMap() = default;

			/**
			 * @brief �f�X�g���N�^
			*/
			~CTestMap() = default;

		public:		//�I�[�o�[���C�h���������o�֐�
			/**
			 * @brief �X�^�[�g�֐�
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool Start() override final;

			/**
			 * @brief �j���������ɌĂ΂��
			*/
			void OnDestroy() override final;

			/**
			 * @brief �A�b�v�f�[�g�֐�
			*/
			void Update() override final;

		private:	//�񋓌^
			enum EnAnimationClips
			{
				enAnim_idle,
				enAnim_walk,
				enAnim_num
			};

		private:	//�f�[�^�����o
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< ���f�������_���[
			AnimationClip m_animationClip[enAnim_num];
			EnAnimationClips m_animState = enAnim_idle;
		};

	}
}
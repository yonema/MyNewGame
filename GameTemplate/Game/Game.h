#pragma once

/**
 * @brief ���̃Q�[���̃l�[���X�y�[�X
*/
namespace nsNinjaAttract
{
	namespace nsLight { class CDirectionalLight; }
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

			/**
			 * @brief �j�������Ƃ��ɌĂ΂��֐�
			*/
			void OnDestroy() override final;


		private:	// �萔
			static const char* const m_kMapObjName;				//!< �}�b�v�̃I�u�W�F�N�g�̖��O
			static const Vector3 m_kGameDirectionalLightDir;	//!< �Q�[���S�̂̃f�B���N�V�������C�g�̕���
			static const Vector4 m_kGameDirectionalLightColor;	//!< �Q�[���S�̂̃f�B���N�V�������C�g�̃J���[
			static const int m_kCurrentMapNum = 7;				//!< ���݂̃}�b�v�ԍ�

		private:	// �f�[�^�����o
			nsLight::CDirectionalLight* m_gameDirectionalLight = nullptr;	//!< �Q�[���S�̂̃f�B���N�V�������C�g
			int m_mapNum = 0;												//!< �}�b�v�ԍ�
		};
	}
}


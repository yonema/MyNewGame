#pragma once

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsTimer { class CStopWatch; }		// �^�C�}�[

	/**
	 * @brief ���̃Q�[���p�̃Q�[���G���W���l�[���X�y�[�X
	*/
	namespace nsMyEngine
	{
		/**
		 * @brief ���̃Q�[���p�̃Q�[���G���W���N���X�BTkEngine�����b�v����
		* @note �V���O���g���p�^�[�����g�p
		*/
		class CMyEngine : private nsUtil::Noncopyable
		{
		private:	// �G�C���A�X�錾
			using CTkEnginePtr = std::unique_ptr<TkEngine>;	//!< tkEngine�̃��j�[�N�|�C���^
			using CStopWatchPtr = std::unique_ptr<nsTimer::CStopWatch>;	//!< �X�g�b�v�E�H�b�`�̃��j�[�N�|�C���^

		private:	// �R���X�g���N�^�ƃf�X�g���N�^�B
					// �V���O���g���p�^�[���̂���private�ɉB��
			/**
			 * @brief �R���X�g���N�^
			*/
			CMyEngine();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CMyEngine();
		public:		// �����o�֐�

			/**
			 * @brief �Q�[�����[�v�����s����
			*/
			void ExecuteGameLoop();

			/**
			 * @brief ���O�̔j������
			*/
			void PreDelete();

		public:		// static�ȃ����o�֐�
			/**
			 * @brief �B��̃C���X�^���X�𐶐�����
			 * @note �V���O���g���p�^�[��
			*/
			static void CreateInstance()
			{
				m_instance = new CMyEngine;
			}

			/**
			 * @brief �B��̃C���X�^���X��j������
			 * @note �V���O���g���p�^�[��
			*/
			static void DeleteInstance()
			{
				if (m_instance)
					delete m_instance;
			}

			/**
			 * @brief �B��̃C���X�^���X�̎Q�Ƃ𓾂�
			 * @return �B��̃C���X�^���X�̎Q��
			 * @note �V���O���g���p�^�[��
			*/
			static CMyEngine* GetInstance()
			{
				return m_instance;
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �e��C���X�^���X�𐶐�
			*/
			void CreateInstances();

			/**
			 * @brief �e��C���X�^���X��j��
			*/
			void DeleteInstances();

			/**
			 * @brief �e��A�b�v�f�[�g�����s������
			*/
			void ExecuteUpdate();

			/**
			 * @brief �X�s�����b�N�ɂ��FPS�Œ菈��
			*/
			void Spinlock();

		private:	// �f�[�^�����o
			CTkEnginePtr m_tkEngine;	//!< tkEngine
			CStopWatchPtr m_stopWatch;	//!< �X�g�b�v�E�H�b�`
		private:	// static�ȃf�[�^�����o
			static CMyEngine* m_instance;		//!< �B��̃C���X�^���X
		};
	}
}



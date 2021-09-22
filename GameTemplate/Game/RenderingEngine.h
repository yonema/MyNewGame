#pragma once

// �O���錾
//class GraphicEngine;

namespace nsMyGame
{

	/**
	 * @brief ���̃Q�[���p�̃Q�[���G���W���l�[���X�y�[�X
	*/
	namespace nsMyEngine
	{
		/**
		 * @brief �����_�����O�G���W���N���X�BGraphicsEngine�̃��b�v�N���X�B
		 * @note �V���O���g���p�^�[�����g�p
		*/
		class CRenderingEngine : private nsUtil::Noncopyable
		{
		private:	// �R���X�g���N�^�ƃf�X�g���N�^�B
					// �V���O���g���p�^�[���̂���private�ɉB��
			/**
			 * @brief �R���X�g���N�^
			*/
			CRenderingEngine();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CRenderingEngine();

		public:		// �����o�֐�

			/**
			 * @brief �����_�����O�G���W�������s
			*/
			void Execute();

		public:		// static�ȃ����o�֐�
			/**
			 * @brief �B��̃C���X�^���X�𐶐�����
			 * @note �V���O���g���p�^�[��
			*/
			static void CreateInstance()
			{
				m_instance = new CRenderingEngine;
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
			static CRenderingEngine* GetInstance()
			{
				return m_instance;
			}
		private:	// private�ȃ����o�֐�

		private:	// �f�[�^�����o

		private:	// static�ȃf�[�^�����o
			static CRenderingEngine* m_instance;		//!< �B��̃C���X�^���X
		};
	}
}


#pragma once



namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic { class CRender; };

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

			void AddRenderObject(nsGraphic::CRender* renderObject)
			{
#ifdef MY_DEBUG
				if (m_renderObjects.size() >= m_kRenderObjectsCapacityNum)
					MessageBoxA(nullptr, "�����_�����O�I�u�W�F�g���\�z��葽������Ă�", "�x��", MB_OK);
#endif
				m_renderObjects.emplace_back(renderObject);
			}

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

			/**
			 * @brief GBuffer�ɕ`�悷��
			 * @param rc �����_�����O�R���e�L�X�g
			*/
			void RenderToGBuffer(RenderContext& rc);

			/**
			 * @brief 2D��`�悷��
			 * @param rc �����_�����O�R���e�L�X�g
			*/
			void Render2D(RenderContext& rc);

		private:	// �f�[�^�����o
			std::vector<nsGraphic::CRender*> m_renderObjects;	//!< �`�悷��I�u�W�F�N�g

		private:	// static�ȃf�[�^�����o
			static CRenderingEngine* m_instance;		//!< �B��̃C���X�^���X
			static const int m_kRenderObjectsCapacityNum = 255;
		};
	}
}


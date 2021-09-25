#pragma once
#include "RenderingEngineConstData.h"
#include "PostEffect.h"

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
				if (m_renderObjects.size() >= nsRenderingEngineConstData::m_kRenderObjectsCapacityNum)
					MessageBoxA(nullptr, "�����_�����O�I�u�W�F�g���\�z��葽������Ă�", "�x��", MB_OK);
#endif
				m_renderObjects.emplace_back(renderObject);
			}

			/**
			 * @brief ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[�t�H�[�}�b�g���擾
			 * @return ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[�t�H�[�}�b�g
			*/
			DXGI_FORMAT GetMainRenderTargetFormat() const
			{
				return m_mainRenderTarget.GetColorBufferFormat();
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
			 * @brief ���C�������_�����O�^�[�Q�b�g�̏�����
			*/
			void InitMainRenderTarget();

			/**
			 * @brief GBuffer��������
			*/
			void InitGBuffer();

			/**
			 * @brief ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�̏�����
			*/
			void InitCopyMainRenderTargetToFrameBufferSprite();

			/**
			 * @brief �f�B�t�@�\�h���C�e�B���O���s�����߂̃X�v���C�g�̏�����
			*/
			void InitDefferdLightingSprite();

			/**
			 * @brief GBuffer�ɕ`�悷��
			 * @param rc �����_�����O�R���e�L�X�g
			*/
			void RenderToGBuffer(RenderContext& rc);

			/**
			 * @brief �f�B�t�@�\�h���C�e�B���O
			 * @param rc �����_�����O�R���e�L�X�g
			*/
			void DefferdLighting(RenderContext& rc);

			/**
			 * @brief �t�H���[�h�����_�����O
			 * @param[in] rc �����_�����O�R���e�L�X�g
			*/
			void ForwardRendering(RenderContext& rc);

			/**
			 * @brief 2D��`�悷��
			 * @param rc �����_�����O�R���e�L�X�g
			*/
			void Render2D(RenderContext& rc);

			/**
			 * @brief ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[����
			 * @param rc �����_�����O�R���e�L�X�g
			*/
			void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

		private:	// �f�[�^�����o
			std::vector<nsGraphic::CRender*> m_renderObjects;	//!< �`�悷��I�u�W�F�N�g

			RenderTarget m_mainRenderTarget;	//!< ���C�������_�����O�^�[�Q�b�g
			RenderTarget m_GBuffer[nsRenderingEngineConstData::enGBufferNum];	//!< GBuffer

			Sprite m_copyMainRtToFrameBufferSprite;	//!< ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
			Sprite m_diferredLightingSprite;	//!< �f�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g

			nsGraphic::nsPostEffect::CPostEffect m_postEffect;	//!< �|�X�g�G�t�F�N�g�N���X

			//!< �f�B�t�@�[�h���C�e�B���O�p�̒萔�o�b�t�@
			nsRenderingEngineConstData::SDefferdLightingCB m_defferdLightingCB;

		private:	// static�ȃf�[�^�����o
			static CRenderingEngine* m_instance;		//!< �B��̃C���X�^���X
		};
	}
}


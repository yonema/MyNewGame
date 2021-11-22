#pragma once
#include "RenderingEngineConstData.h"
#include "TResourceBank.h"
#include "PostEffect.h"
#include "ShadowMapRender.h"
#include "LightConstData.h"

namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic { class CRender; };		// �����_���[�N���X
	namespace nsTimer { class CStopWatch; }		// �^�C�}�[

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
		private:	// �G�C���A�X�錾
			using SpritePtr = std::unique_ptr<Sprite>;	//!< �X�v���C�g�Ƀ��j�[�N�|�C���^�̌^

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
			 * @brief IBL���s�����H��ݒ肷��
			 * @param[in] isIBL IBL���s�����H
			*/
			void SetIsIBL(const bool isIBL)
			{
				m_IBLCB.isIBL = isIBL;
			}

			/**
			 * @brief IBL�p�̒萔�o�b�t�@�̎Q�Ƃ𓾂�
			 * @return IBL�p�̒萔�o�b�t�@�̎Q��
			*/
			nsRenderingEngineConstData::SIBLCB& GetIBLCB()
			{
				return m_IBLCB;
			}

			/**
			 * @brief tkm�t�@�C���̃��\�[�X�����\�[�X�o���N����擾�B���o�^�̏ꍇ��nullptr��߂��B
			 * @param[in] filePath tkm�t�@�C���p�X
			 * @return ���\�[�X
			*/
			TkmFile* GetTkmFileFromBank(const char* filePath)
			{
				return m_tkmFileBank.Get(filePath);
			}

			/**
			 * @brief tkm�t�@�C���̃��\�[�X�����\�[�X�o���N�ɓo�^
			 * @param[in] filePath tkm�t�@�C���p�X
			 * @param[in] tkmFile tkm�t�@�C�����\�[�X
			*/
			void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
			{
				m_tkmFileBank.Regist(filePath, tkmFile);
			}

			/**
			 * @brief �V�F�[�_�[�̃��\�[�X�����\�[�X�o���N����擾�B���o�^�̏ꍇ��nullptr��߂��B
			 * @param[in] filePath �V�F�[�_�[�t�@�C���p�X
			 * @param[in] entryPointFuncName �G���g���[�|�C���g
			 * @return ���\�[�X
			*/
			Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
			{
				std::string programName = filePath;
				programName += entryPointFuncName;
				return m_shaderBank.Get(programName.c_str());
			}

			/**
			 * @brief �V�F�[�_�[�̃��\�[�X�����\�[�X�o���N�ɓo�^
			 * @param filePath �V�F�[�_�[�t�@�C���p�X
			 * @param entryPointFuncName �G���g���[�|�C���g
			 * @param shader �V�F�[�_�[���\�[�X
			*/
			void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
			{
				std::string programName = filePath;
				programName += entryPointFuncName;
				m_shaderBank.Regist(programName.c_str(), shader);
			}

			/**
			 * @brief dds�t�@�C���̃��\�[�X�����\�[�X�o���N����擾�B���o�^�̏ꍇ��nullptr��߂��B
			 * @param[in] filePath �t�@�C���p�X
			 * @return dds�t�@�C���̃��\�[�X
			*/
			char* GetDdsFileFromBank(const char* filePath)
			{
				return m_ddsFileBank.Get(filePath);
			}

			/**
			 * @brief dds�t�@�C���̃��\�[�X�����\�[�X�o���N�ɓo�^
			 * @param[in] filePath �t�@�C���p�X
			 * @param[in] ddsFile dds�t�@�C��
			*/
			void RegistDdsFileToBank(const char* filePath, char* ddsFile)
			{
				m_ddsFileBank.Regist(filePath, ddsFile);
			}

			/**
			 * @brief �e�N�X�`���̃��\�[�X�����\�[�X�o���N����擾�B���o�^�̏ꍇ��nullptr��߂��B
			 * @param[in] filePath �t�@�C���p�X
			 * @return �e�N�X�`���̃��\�[�X
			*/
			Texture* GetTextureFileFromBank(const char* filePath)
			{
				return m_textureFileBank.Get(filePath);
			}

			/**
			 * @brief �e�N�X�`���̃��\�[�X�����\�[�X�o���N�ɓo�^
			 * @param[in] filePath �t�@�C���p�X
			 * @param[in] texture �e�N�X�`���̃��\�[�X
			*/
			void RegistTextureFileToBank(const char* filePath, Texture* texture)
			{
				m_textureFileBank.Regist(filePath, texture);
			}

			/**
			 * @brief �w�肵��GBuffer�̃J���[�t�H�[�}�b�g���擾
			 * @param[in] enGBufferNo GBuffer�̔ԍ�
			 * @return �J���[�t�H�[�}�b�g
			*/
			DXGI_FORMAT GetGBufferColorFormat(const nsRenderingEngineConstData::EnGBuffer enGBufferNo) const
			{
				return m_GBuffer[enGBufferNo].GetColorBufferFormat();
			}

			/**
			 * @brief IBL�Ɏg�p����e�N�X�`���̎Q�Ƃ𓾂�
			 * @return IBL�Ɏg�p����e�N�X�`���̎Q��
			*/
			Texture& GetIBLTexture()
			{
				return m_IBLTexture;
			}

			/**
			 * @brief �V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g�̃e�N�X�`���̎Q�Ƃ𓾂�
			 * @param ligNo ���C�g�̔ԍ�
			 * @param shadowMapNo �V���h�E�}�b�v�̔ԍ�
			 * @return �e�N�X�`���̎Q��
			*/
			Texture& GetShadowMapRenderTargetTexture(const int ligNo, const int shadowMapNo)
			{
				return m_shadowMapRenders[ligNo].GetShadowMap(shadowMapNo);
			};

			/**
			 * @brief IBL���ď���������
			 * @param[in] ddsFilePath IBL�̃e�N�X�`����dds�t�@�C���p�X
			 * @param[in] luminance IBL�̖��邳
			 * @param[in] isIBL IBL���s�����H
			*/
			void ReInitIBL(const wchar_t* ddsFilePath, const float luminance,const bool isIBL);

			/**
			 * @brief �����_�����O�G���W�������s
			 * @param[in] stopWatch �X�g�b�v�E�H�b�`
			*/
			void Execute(const nsTimer::CStopWatch& stopWatch);

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
			 * @brief �V���h�E�}�b�v�����_���[�̏�����
			*/
			void InitShadowMapRender();

			/**
			 * @brief �f�B�t�@�\�h���C�e�B���O���s�����߂̃X�v���C�g�̏�����
			*/
			void InitDefferdLightingSprite();

			/**
			 * @brief �V���h�E�}�b�v�ɕ`�悷��
			 * @param[in] rc �����_�����O�R���e�L�X�g
			*/
			void RenderToShadowMap(RenderContext& rc);

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

			/**
			 * @brief IBL�̃f�[�^������������
			 * @param[in] ddsFilePath IBL�̃e�N�X�`����dds�t�@�C���p�X
			 * @param[in] luminance IBL�̖��邳
			*/
			void InitIBLData(const wchar_t* ddsFilePath, const float luminance);

		private:	// �f�[�^�����o
			std::vector<nsGraphic::CRender*> m_renderObjects;	//!< �`�悷��I�u�W�F�N�g

			RenderTarget m_mainRenderTarget;	//!< ���C�������_�����O�^�[�Q�b�g
			RenderTarget m_GBuffer[nsRenderingEngineConstData::enGBufferNum];	//!< GBuffer

			Sprite m_copyMainRtToFrameBufferSprite;	//!< ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
			SpritePtr m_diferredLightingSprite;	//!< �f�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g
												//!< ReInit����Ƃ��Ƀ��������[�N��h�����߃��j�[�N�|�C���^�ɂ���
			//!< �V���h�E�}�b�v�����_���[
			nsGraphic::nsShadow::CShadowMapRender m_shadowMapRenders[nsLight::nsLightConstData::kMaxDirectionalLightNum];
			nsGraphic::nsPostEffect::CPostEffect m_postEffect;	//!< �|�X�g�G�t�F�N�g�N���X

			//!< �f�B�t�@�[�h���C�e�B���O�p�̒萔�o�b�t�@
			nsRenderingEngineConstData::SDefferdLightingCB m_defferdLightingCB;
			nsRenderingEngineConstData::SIBLCB m_IBLCB;	//!< IBL�p�̒萔�o�b�t�@
			Texture m_IBLTexture;	//!< IBL�Ɏg�p����e�N�X�`��

			nsUtil::TResourceBank<TkmFile> m_tkmFileBank;	//!< tkm�t�@�C���o���N
			nsUtil::TResourceBank<Shader> m_shaderBank;		//!< �V�F�[�_�[�o���N
			nsUtil::TResourceBank<char> m_ddsFileBank;	//!< dds�t�@�C���o���N
			nsUtil::TResourceBank<Texture> m_textureFileBank;	//!< �e�N�X�`���o���N

		private:	// static�ȃf�[�^�����o
			static CRenderingEngine* m_instance;		//!< �B��̃C���X�^���X
		};
	}
}


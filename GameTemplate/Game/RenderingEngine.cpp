#include "stdafx.h"
#include "RenderingEngine.h"
#include "system/system.h"
#include "Render.h"
#include "LightManager.h"

namespace nsMyGame
{

	/**
	 * @brief ���̃Q�[���p�̃Q�[���G���W���l�[���X�y�[�X
	*/
	namespace nsMyEngine
	{
		CRenderingEngine* CRenderingEngine::m_instance = nullptr;	// �B��̃C���X�^���X

		// �����_�����O�G���W���̒萔�f�[�^���g�p�\�ɂ���
		using namespace nsRenderingEngineConstData;

		/**
		 * @brief �R���X�g���N�^
		*/
		CRenderingEngine::CRenderingEngine()
		{
			// �`��ł���I�u�W�F�N�g�̃L���p�V�e�B���m�ۂ���
			m_renderObjects.reserve(m_kRenderObjectsCapacityNum);

			InitMainRenderTarget();	// ���C�������_�����O�^�[�Q�b�g�̏�����
			InitGBuffer();	// GBuffer��������
			// ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�̏�����
			InitCopyMainRenderTargetToFrameBufferSprite();
			InitDefferdLightingSprite();	// �f�B�t�@�\�h���C�e�B���O���s�����߂̃X�v���C�g�̏�����

			m_postEffect.Init(m_mainRenderTarget);	// �|�X�g�G�t�F�N�g�̏�����

			return;
		}
		/**
		 * @brief �f�X�g���N�^
		*/
		CRenderingEngine::~CRenderingEngine()
		{
			return;
		}

		/**
		 * @brief ���C�������_�����O�^�[�Q�b�g�̏�����
		*/
		void CRenderingEngine::InitMainRenderTarget()
		{
			m_mainRenderTarget.Create(
				g_graphicsEngine->GetFrameBufferWidth(),
				g_graphicsEngine->GetFrameBufferHeight(),
				1,
				1,
				DXGI_FORMAT_R16G16B16A16_FLOAT,	// 16bit���������_�^
				DXGI_FORMAT_UNKNOWN
			);
			return;
		}

		/**
		 * @brief GBuffer��������
		*/
		void CRenderingEngine::InitGBuffer()
		{
			int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
			int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

			// �A���x�h�J���[���o�͗p�̃����_�����O�^�[�Q�b�g������������
			m_GBuffer[enGBufferAlbedoDepth].Create(
				frameBuffer_w,
				frameBuffer_h,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,	// 32bit���������_�^
				DXGI_FORMAT_D32_FLOAT
			);

			// �@���o�͗p�̃����_�����O�^�[�Q�b�g������������
			m_GBuffer[enGBufferNormal].Create(
				frameBuffer_w,
				frameBuffer_h,
				1,
				1,
				DXGI_FORMAT_R8G8B8A8_SNORM,	// 8bit�����t�����K������
				DXGI_FORMAT_UNKNOWN
			);


			// ���^���b�N�A�e�p�����[�^�A�X���[�X�o�͗p�̃����_�����O�^�[�Q�b�g������������    
			m_GBuffer[enGBufferMetaricShadowSmooth].Create(
				frameBuffer_w,
				frameBuffer_h,
				1,
				1,
				DXGI_FORMAT_R8G8B8A8_UNORM,	// 8bit�����Ȃ����K������
				DXGI_FORMAT_UNKNOWN
			);

			return;
		}

		/**
		 * @brief ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�̏�����
		*/
		void CRenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
		{
			SpriteInitData spriteInitData;

			// �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g
			spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

			// �����_�����O�悪�t���[���o�b�t�@�[�Ȃ̂ŁA�𑜓x�̓t���[���o�b�t�@�[�Ɠ���
			spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
			spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
			spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

			// �K���}�␳�����2D�`��̃V�F�[�_�[���w�肷��
			spriteInitData.m_fxFilePath = m_kSpriteFxFilePath;

			// �������I�u�W�F�N�g���g���āA�X�v���C�g������������
			m_copyMainRtToFrameBufferSprite.Init(spriteInitData);

			return;
		}

		/**
		 * @brief �f�B�t�@�\�h���C�e�B���O���s�����߂̃X�v���C�g�̏�����
		*/
		void CRenderingEngine::InitDefferdLightingSprite()
		{
			// �X�v���C�g���������f�[�^
			SpriteInitData spriteInitData;

			// ��ʑS�̂Ƀ����_�����O����̂ŕ��ƍ����̓t���[���o�b�t�@�[�̕��ƍ����Ɠ���
			spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
			spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

			// �f�B�t�@�[�h���C�e�B���O�Ŏg�p����e�N�X�`����ݒ�
			int texNo = 0;
			for (auto& GBuffer : m_GBuffer)
			{
				spriteInitData.m_textures[texNo++] = &GBuffer.GetRenderTargetTexture();
			}

			// fx�t�@�C���p�X��ݒ�
			spriteInitData.m_fxFilePath = m_kDefferdLightingSpriteFxFilePath;

			// �萔�o�b�t�@�̓o�^
			// ���C�g����o�^
			spriteInitData.m_expandConstantBuffer[0] = 
				&nsLight::CLightManager::GetInstance()->GetLightData();
			spriteInitData.m_expandConstantBufferSize[0] =
				sizeof(nsLight::CLightManager::GetInstance()->GetLightData());
			// �f�B�t�@�[�h�����_�����O�p�̒萔�o�b�t�@��o�^
			spriteInitData.m_expandConstantBuffer[1] = &m_defferdLightingCB;
			spriteInitData.m_expandConstantBufferSize[1] = sizeof(spriteInitData);

			// ���C�������_�����O�^�[�Q�b�g�ɕ`�悷�邽��
			// ���C�������_�����O�^�[�Q�b�g�ƃJ���[�t�H�[�}�b�g�����킹��
			spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();

			// IBL���s�����H�A����
			// IBL�Ɏg�p����e�N�X�`�����L�����H
			if (m_defferdLightingCB.isIBL == true && m_IBLTexture.IsValid())
			{
				// IBL�Ɏg�p����e�N�X�`����ݒ�ݒ肷��
				spriteInitData.m_textures[texNo++] = &m_IBLTexture;
			}

			// �������f�[�^���g���ăX�v���C�g��������
			m_diferredLightingSprite.Init(spriteInitData);

			return;
		}

		/**
		 * @brief IBL���ď���������
		 * @param[in] ddsFilePath IBL�̃e�N�X�`����dds�t�@�C���p�X
		 * @param[in] luminance IBL�̖��邳
		 * @param[in] isIBL IBL���s�����H
		*/
		void CRenderingEngine::ReInitIBL(const wchar_t* ddsFilePath, const float luminance, const bool isIBL)
		{
			// IBL�̃f�[�^������������
			InitIBLData(ddsFilePath, luminance);

			// IBL���s�����H��ݒ�
			m_defferdLightingCB.isIBL = isIBL;

			// �f�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g������������
			InitDefferdLightingSprite();

			return;
		}


		/**
		 * @brief �����_�����O�G���W�������s
		*/
		void CRenderingEngine::Execute()
		{
			// �����_�����O�R���e�L�X�g
			RenderContext& rc = g_graphicsEngine->GetRenderContext();

			// �`��I�u�W�F�N�g�̓o�^
			GameObjectManager::GetInstance()->ExecuteAddRender();

			// GBuffer�ɕ`�悷��
			RenderToGBuffer(rc);

			// �f�B�t�@�\�h���C�f�B���O
			DefferdLighting(rc);

			// �t�H���[�h�����_�����O
			ForwardRendering(rc);

			// �G�t�F�N�g�̕`��
			EffectEngine::GetInstance()->Draw();

			// �|�X�g�G�t�F�N�g�����s
			m_postEffect.Render(rc, m_mainRenderTarget);

			// 2D��`�悷��
			Render2D(rc);

			// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[����
			CopyMainRenderTargetToFrameBuffer(rc);

			m_renderObjects.clear();

			return;
		}

		/**
		 * @brief GBuffer�ɕ`�悷��
		 * @param rc �����_�����O�R���e�L�X�g
		*/
		void CRenderingEngine::RenderToGBuffer(RenderContext& rc)
		{
			// �����_�����O�^�[�Q�b�g��G-Buffer�ɕύX
			RenderTarget* rts[enGBufferNum] = {
				&m_GBuffer[enGBufferAlbedoDepth],         // 0�Ԗڂ̃����_�����O�^�[�Q�b�g
				&m_GBuffer[enGBufferNormal],              // 1�Ԗڂ̃����_�����O�^�[�Q�b�g
				&m_GBuffer[enGBufferMetaricShadowSmooth], // 2�Ԗڂ̃����_�����O�^�[�Q�b�g
			};

			// �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
			rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

			// �����_�����O�^�[�Q�b�g��ݒ�
			rc.SetRenderTargets(ARRAYSIZE(rts), rts);

			// �����_�����O�^�[�Q�b�g���N���A
			rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

			// �`��
			for (nsGraphic::CRender* renderObject : m_renderObjects)
			{
				renderObject->OnRenderToGBuffer(rc);
			}

			// �����_�����O�^�[�Q�b�g�ւ̏������ݑ҂�
			rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

			return;
		}

		/**
		 * @brief �f�B�t�@�\�h���C�e�B���O
		 * @param rc �����_�����O�R���e�L�X�g
		*/
		void CRenderingEngine::DefferdLighting(RenderContext& rc)
		{
			// �f�B�t�@�[�h���C�e�B���O�ɕK�v�ȃf�[�^���X�V����
			// �r���[�v���W�F�N�V�����s��̋t�s��X�V
			m_defferdLightingCB.mViewProjInv.Inverse(g_camera3D->GetViewProjectionMatrix());

			// �����_�����O������C�������_�����O�^�[�Q�b�g�ɂ���
			// ���C�������_�����O�^�[�Q�b�g��ݒ�
			rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
			rc.SetRenderTargetAndViewport(m_mainRenderTarget);

			// G-Buffer�̓��e�����ɂ��ăf�B�t�@�[�h���C�e�B���O
			m_diferredLightingSprite.Draw(rc);

			// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
			rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

			return;
		}

		/**
		 * @brief �t�H���[�h�����_�����O
		 * @param[in] rc �����_�����O�R���e�L�X�g
		*/
		void CRenderingEngine::ForwardRendering(RenderContext& rc)
		{
			// �����_�����O������C�������_�����O�^�[�Q�b�g�ɂ���
			// �[�x�o�b�t�@��GBuffer�̂��̂��g�p����
			rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
			rc.SetRenderTarget(
				m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
				m_GBuffer[enGBufferAlbedoDepth].GetDSVCpuDescriptorHandle()
			);

			// �`��
			for (nsGraphic::CRender* renderObject : m_renderObjects)
			{
				renderObject->OnForwardRender(rc);
			}

			// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
			rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

			return;
		}

		/**
		 * @brief 2D��`�悷��
		 * @param rc �����_�����O�R���e�L�X�g
		*/
		void CRenderingEngine::Render2D(RenderContext& rc)
		{
			for (nsGraphic::CRender* renderObject : m_renderObjects)
			{
				renderObject->OnRender2D(rc);
			}
			return;
		}

		/**
		 * @brief ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[����
		 * @param rc �����_�����O�R���e�L�X�g
		*/
		void CRenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
		{
			// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
			rc.SetRenderTarget(
				g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
				g_graphicsEngine->GetCurrentFrameBuffuerDSV()
			);

			// �r���[�|�[�g���w�肷��
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferWidth());
			viewport.Height = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferHeight());
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			rc.SetViewportAndScissor(viewport);
			m_copyMainRtToFrameBufferSprite.Draw(rc);

			return;
		}

		/**
		 * @brief IBL�̃f�[�^������������
		 * @param[in] ddsFilePath IBL�̃e�N�X�`����dds�t�@�C���p�X
		 * @param[in] luminance IBL�̖��邳
		*/
		void CRenderingEngine::InitIBLData(const wchar_t* ddsFilePath, const float luminance)
		{
			// IBL�Ɏg�p����e�N�X�`���̏�����
			m_IBLTexture.InitFromDDSFile(ddsFilePath);

			// IBL�̖��邳��ݒ�
			m_defferdLightingCB.IBLLuminance = luminance;

			return;
		}
	}
}
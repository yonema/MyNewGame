#include "stdafx.h"
#include "Sprite.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "../GameTemplate/Game/MyEngine.h"
#include "../GameTemplate/Game/RenderingEngine.h"

/**
 * @brief �R���X�g���N�^
*/
SpriteInitData::SpriteInitData()
{
	// ���[�U�[�g���p�̒萔�o�b�t�@��������
	for (int i = 0; i < m_kMaxExCBNum; i++)
	{
		m_expandConstantBuffer[i] = nullptr;
		m_expandConstantBufferSize[i] = 0;
	}
	return;
}

	namespace {
		struct SSimpleVertex {
			Vector4 pos;
			Vector2 tex;
		};
	}
	const Vector2	Sprite::DEFAULT_PIVOT = { 0.5f, 0.5f };
	Sprite::~Sprite()
	{
	}
	void Sprite::InitTextures(const SpriteInitData& initData)
	{
		//�X�v���C�g�Ŏg�p����e�N�X�`������������B
		if (initData.m_ddsFilePath[0] != nullptr) {
			//dds�t�@�C���̃p�X���w�肳��Ă�̂Ȃ�Adds�t�@�C������e�N�X�`�����쐬����B
			int texNo = 0;
			while (initData.m_ddsFilePath[texNo] && texNo < MAX_TEXTURE) {

				// �ύX�B

				// �e�N�X�`���o���N���烊�\�[�X��T���Ď���Ă���
				m_textures[texNo] = nsNinjaAttract::nsMyEngine::
					CRenderingEngine::GetInstance()->GetTextureFileFromBank(initData.m_ddsFilePath[texNo]);

				if (m_textures[texNo] == nullptr)
				{
					// ���\�[�X���Ȃ�������V������������
					wchar_t wddsFilePath[1024];
					mbstowcs(wddsFilePath, initData.m_ddsFilePath[texNo], 1023);
					m_textures[texNo] = new Texture();
					m_textures[texNo]->InitFromDDSFile(wddsFilePath);
					// ���\�[�X��o�^����
					nsNinjaAttract::nsMyEngine::
						CRenderingEngine::GetInstance()->RegistTextureFileToBank(initData.m_ddsFilePath[texNo], m_textures[texNo]);
				}

				texNo++;
			}
			m_numTexture = texNo;
		}
		else if (initData.m_textures[0] != nullptr) {
			//�O���e�N�X�`�����w�肳��Ă���B
			int texNo = 0;
			while (initData.m_textures[texNo] != nullptr) {
				m_textureExternal[texNo] = initData.m_textures[texNo];
				texNo++;
			}
			m_numTexture = texNo;
		}
		else {
			//�e�N�X�`�����w�肳��ĂȂ��B
			MessageBoxA(nullptr, "initData.m_ddsFilePath��m_textures�̂ǂ��炩�Ɏg�p����e�N�X�`���̏���ݒ肵�Ă��������B", "�G���[", MB_OK);
			std::abort();
		}
	}
	void Sprite::InitShader(const SpriteInitData& initData)
	{
		if (initData.m_fxFilePath == nullptr) {
			MessageBoxA(nullptr, "fx�t�@�C�����w�肳��Ă��܂���B", "�G���[", MB_OK);
			std::abort();
		}
		wchar_t fxFilePath[1024];
		mbstowcs(fxFilePath, initData.m_fxFilePath, 1023);
		//�V�F�[�_�[�����[�h����B
		m_vs.LoadVS(fxFilePath, initData.m_vsEntryPointFunc);
		m_ps.LoadPS(fxFilePath, initData.m_psEntryPoinFunc);
	}
	void Sprite::InitDescriptorHeap(const SpriteInitData& initData)
	{
		if (m_textureExternal[0] != nullptr) {
			//�O���̃e�N�X�`�����w�肳��Ă���B
			for (int texNo = 0; texNo < m_numTexture; texNo++) {
				m_descriptorHeap.RegistShaderResource(texNo, *m_textureExternal[texNo]);
			}
		}
		else {
			for (int texNo = 0; texNo < m_numTexture; texNo++) {
				m_descriptorHeap.RegistShaderResource(texNo, *m_textures[texNo]);
			}
		}
		if (initData.m_expandShaderResoruceView != nullptr) {
			//�g���V�F�[�_�[���\�[�X�r���[�B
			m_descriptorHeap.RegistShaderResource(
				EXPAND_SRV_REG__START_NO,
				*initData.m_expandShaderResoruceView
			);
		}
		m_descriptorHeap.RegistConstantBuffer(0, m_constantBufferGPU);

		// �ύX�B�ǉ��B
		for (int i = 0; i < SpriteInitData::m_kMaxExCBNum; i++)
		{
			if (m_userExpandConstantBufferCPU[i] != nullptr) {
				//���[�U�[�g���̒萔�o�b�t�@��b1�Ɋ֘A�t������B
				m_descriptorHeap.RegistConstantBuffer(1 + i, m_userExpandConstantBufferGPU[i]);
			}
		}
		m_descriptorHeap.Commit();
	}
	void Sprite::InitVertexBufferAndIndexBuffer(const SpriteInitData& initData)
	{
		float halfW = m_size.x * 0.5f;
		float halfH = m_size.y * 0.5f;
		//���_�o�b�t�@�̃\�[�X�f�[�^�B
		SSimpleVertex vertices[] =
		{
			{
				Vector4(-halfW, -halfH, 0.0f, 1.0f),
				Vector2(0.0f, 1.0f),
			},
			{
				Vector4(halfW, -halfH, 0.0f, 1.0f),
				Vector2(1.0f, 1.0f),
			},
			{
				Vector4(-halfW, halfH, 0.0f, 1.0f),
				Vector2(0.0f, 0.0f)
			},
			{
				Vector4(halfW, halfH, 0.0f, 1.0f),
				Vector2(1.0f, 0.0f)
			}

		};
		unsigned short indices[] = { 0,1,2,3 };

		m_vertexBuffer.Init(sizeof(vertices), sizeof(vertices[0]));
		m_vertexBuffer.Copy(vertices);

		m_indexBuffer.Init(sizeof(indices), sizeof(indices[0]));
		m_indexBuffer.Copy(indices);
	}
	void Sprite::InitPipelineState(const SpriteInitData& initData)
	{
		// ���_���C�A�E�g���`����B
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		//�p�C�v���C���X�e�[�g���쐬�B
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		if (initData.m_alphaBlendMode == AlphaBlendMode_Trans) {
			//�����������̃u�����h�X�e�[�g���쐬����B
			psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
			psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		}
		else if (initData.m_alphaBlendMode == AlphaBlendMode_Add) {
			//���Z�����B
			psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
			psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
			psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		}

		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// �ύX
		for (auto& format : initData.m_colorBufferFormat) {
			if (format == DXGI_FORMAT_UNKNOWN) {
				break;
			}
			psoDesc.RTVFormats[psoDesc.NumRenderTargets] = format;
			psoDesc.NumRenderTargets++;
		}

		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;

		m_pipelineState.Init(psoDesc);
	}
	void Sprite::InitConstantBuffer(const SpriteInitData& initData)
	{
		//�萔�o�b�t�@�̏������B
		m_constantBufferGPU.Init(sizeof(m_constantBufferCPU), nullptr);

		// �ύX�B�ǉ��B
		for (int i = 0; i < SpriteInitData::m_kMaxExCBNum; i++)
		{
			//���[�U�[�g���̒萔�o�b�t�@���w�肳��Ă���B
			if (initData.m_expandConstantBuffer[i] != nullptr)
			{
				m_userExpandConstantBufferCPU[i] = initData.m_expandConstantBuffer[i];
				m_userExpandConstantBufferGPU[i].Init(
					initData.m_expandConstantBufferSize[i],
					initData.m_expandConstantBuffer[i]
				);
			}
		}
	}
	void Sprite::Init(const SpriteInitData& initData)
	{
		m_size.x = static_cast<float>(initData.m_width);
		m_size.y = static_cast<float>(initData.m_height);

		//�e�N�X�`�����������B
		InitTextures(initData);
		//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���������B
		InitVertexBufferAndIndexBuffer(initData);
		//�萔�o�b�t�@���������B
		InitConstantBuffer(initData);
		
		//���[�g�V�O�l�`���̏������B
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			initData.textureAddressMode,	// �ύX
			initData.textureAddressMode,	// �ύX
			initData.textureAddressMode		// �ύX
		);

		//�V�F�[�_�[���������B
		InitShader(initData);
		//�p�C�v���C���X�e�[�g�̏������B
		InitPipelineState(initData);
		//�f�B�X�N���v�^�q�[�v���������B
		InitDescriptorHeap(initData);

		m_isInited = true;
	}
	void Sprite::Update(const Vector3& pos, const Quaternion& rot, const Vector3& scale, const Vector2& pivot)
	{
		if (m_isInited == false) {
			//��������
			return;
		}
		//�s�{�b�g���l���ɓ��ꂽ���s�ړ��s����쐬�B
		//�s�{�b�g�͐^�񒆂�0.0, 0.0�A���オ-1.0f, -1.0�A�E����1.0�A1.0�ɂȂ�悤�ɂ���B
		Vector2 localPivot = pivot;
		localPivot.x -= 0.5f;
		localPivot.y -= 0.5f;
		localPivot.x *= -2.0f;
		localPivot.y *= -2.0f;
		//�摜�̃n�[�t�T�C�Y�����߂�B
		Vector2 halfSize = m_size;
		halfSize.x *= 0.5f;
		halfSize.y *= 0.5f;
		Matrix mPivotTrans;

		mPivotTrans.MakeTranslation(
			{ halfSize.x * localPivot.x, halfSize.y * localPivot.y, 0.0f }
		);
		Matrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		m_world = mPivotTrans * mScale;
		m_world = m_world * mRot;
		m_world = m_world * mTrans;
	}
	void Sprite::Draw(RenderContext& renderContext)
	{
		if (m_isInited == false) {
			//���������B
			return;
		}
		//���݂̃r���[�|�[�g���畽�s���e�s����v�Z����B
		D3D12_VIEWPORT viewport = renderContext.GetViewport();

		Matrix viewMatrix = g_camera2D->GetViewMatrix();
		//Matrix projMatrix = g_camera2D->GetProjectionMatrix();
		Matrix projMatrix;
		projMatrix.MakeOrthoProjectionMatrix(viewport.Width, viewport.Height, 0.1f, 1.0f);

		m_constantBufferCPU.mvp = m_world * viewMatrix * projMatrix;
		//m_constantBufferCPU.mulColor.x = 1.0f;
		//m_constantBufferCPU.mulColor.y = 1.0f;
		//m_constantBufferCPU.mulColor.z = 1.0f;
		//m_constantBufferCPU.mulColor.w = 1.0f;
		m_constantBufferCPU.screenParam.x = g_camera3D->GetNear();
		m_constantBufferCPU.screenParam.y = g_camera3D->GetFar();
		m_constantBufferCPU.screenParam.z = FRAME_BUFFER_W;
		m_constantBufferCPU.screenParam.w = FRAME_BUFFER_H;

		//�萔�o�b�t�@���X�V�B
		m_constantBufferGPU.CopyToVRAM(&m_constantBufferCPU);
		//�ύX�B�ǉ��B
		for (int i = 0; i < SpriteInitData::m_kMaxExCBNum; i++)
		{
			if (m_userExpandConstantBufferCPU[i] != nullptr)
			{
				m_userExpandConstantBufferGPU[i].CopyToVRAM(m_userExpandConstantBufferCPU[i]);
			}
		}
		//���[�g�V�O�l�`����ݒ�B
		renderContext.SetRootSignature(m_rootSignature);
		//�p�C�v���C���X�e�[�g��ݒ�B
		renderContext.SetPipelineState(m_pipelineState);
		//���_�o�b�t�@��ݒ�B
		renderContext.SetVertexBuffer(m_vertexBuffer);
		//�C���f�b�N�X�o�b�t�@��ݒ�B
		renderContext.SetIndexBuffer(m_indexBuffer);
		//�v���~�e�B�u�g�|���W�[��ݒ肷��B
		renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//�f�B�X�N���v�^�q�[�v��ݒ肷��B
		renderContext.SetDescriptorHeap(m_descriptorHeap);
		//�`��
		renderContext.DrawIndexed(m_indexBuffer.GetCount());
	}


	// �ǉ�
	Sprite::Sprite()
	{
		m_constantBufferCPU.mulColor.x = 1.0f;
		m_constantBufferCPU.mulColor.y = 1.0f;
		m_constantBufferCPU.mulColor.z = 1.0f;
		m_constantBufferCPU.mulColor.w = 1.0f;

		return;
	}

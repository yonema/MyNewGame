#include "stdafx.h"
#include "Material.h"
#include "../GameTemplate/Game/RenderingEngine.h"

//���[�g�V�O�l�`���ƃp�C�v���C���X�e�[�g����̓J���J���J��
enum {
	enDescriptorHeap_CB,
	enDescriptorHeap_SRV,
	enNumDescriptorHeap
};
	
void Material::InitTexture(const TkmFile::SMaterial& tkmMat)
{
	// �e�N�X�`�����\���ĂȂ��Ƃ��p�̃k���e�N�X�`��
	const auto& nullTextureMaps = g_graphicsEngine->GetNullTextureMaps();

	// �e�N�X�`���������p�̃f�[�^
	const char* filePath = nullptr;
	char* map = nullptr;
	unsigned int mapSize;
	Texture* textureMap = nullptr;

	//////// �A���x�h�}�b�v ////////

	// �������f�[�^��ݒ肷��
	if (tkmMat.albedoMap != nullptr)
	{
		// �A���x�h�}�b�v���\���Ă���

		// �e�N�X�`���������p�̃f�[�^���擾
		filePath = tkmMat.albedoMapFilePath.c_str();
		map = tkmMat.albedoMap;
		mapSize = tkmMat.albedoMapSize;
	}
	else
	{
		// �A���x�h�}�b�v���\���Ă��Ȃ�

		// �k���e�N�X�`���̃f�[�^���擾
		filePath = nullTextureMaps.GetAlbedoMapFilePath();
		map = nullTextureMaps.GetAlbedoMap().get();
		mapSize = nullTextureMaps.GetAlbedoMapSize();
	}

	// �e�N�X�`���o���N���烊�\�[�X��T���Ď���Ă���
	textureMap = nsNinjaAttract::nsMyEngine::
		CRenderingEngine::GetInstance()->GetTextureFileFromBank(filePath);

	if (textureMap == nullptr)
	{
		// ���\�[�X���Ȃ�������V������������
		textureMap = new Texture();
		textureMap->InitFromMemory(map, mapSize);
		// ���\�[�X��o�^����
		nsNinjaAttract::nsMyEngine::
			CRenderingEngine::GetInstance()->RegistTextureFileToBank(filePath, textureMap);
	}

	// �A���x�h�}�b�v�Ƀe�N�X�`�����\�[�X��ݒ肷��
	m_albedoMap = textureMap;


	//////// �@���}�b�v ////////

	// �������f�[�^��ݒ肷��
	if (tkmMat.normalMap != nullptr)
	{
		// �@���}�b�v���\���Ă���

		// �e�N�X�`���������p�̃f�[�^���擾
		filePath = tkmMat.normalMapFilePath.c_str();
		map = tkmMat.normalMap;
		mapSize = tkmMat.normalMapSize;
	}
	else
	{
		// �@���}�b�v���\���Ă��Ȃ�

		// �k���e�N�X�`���̃f�[�^���擾
		filePath = nullTextureMaps.GetNormalMapFilePath();
		map = nullTextureMaps.GetNormalMap().get();
		mapSize = nullTextureMaps.GetNormalMapSize();
	}

	// �e�N�X�`���o���N���烊�\�[�X��T���Ď���Ă���
	textureMap = nsNinjaAttract::nsMyEngine::
		CRenderingEngine::GetInstance()->GetTextureFileFromBank(filePath);

	if (textureMap == nullptr)
	{
		// ���\�[�X���Ȃ�������V������������
		textureMap = new Texture();
		textureMap->InitFromMemory(map, mapSize);
		// ���\�[�X��o�^����
		nsNinjaAttract::nsMyEngine::
			CRenderingEngine::GetInstance()->RegistTextureFileToBank(filePath, textureMap);
	}

	// �@���}�b�v�Ƀe�N�X�`�����\�[�X��ݒ肷��
	m_normalMap = textureMap;

	//////// �X�y�L�����}�b�v ////////

	// �������f�[�^��ݒ肷��
	if (tkmMat.specularMap != nullptr)
	{
		// �X�y�L�����}�b�v���\���Ă���

		// �e�N�X�`���������p�̃f�[�^���擾
		filePath = tkmMat.specularMapFilePath.c_str();
		map = tkmMat.specularMap;
		mapSize = tkmMat.specularMapSize;
	}
	else
	{
		// �X�y�L�����}�b�v���\���Ă��Ȃ�

		// �k���e�N�X�`���̃f�[�^���擾
		filePath = nullTextureMaps.GetSpecularMapFilePath();
		map = nullTextureMaps.GetSpecularMap().get();
		mapSize = nullTextureMaps.GetSpecularMapSize();
	}

	// �e�N�X�`���o���N���烊�\�[�X��T���Ď���Ă���
	textureMap = nsNinjaAttract::nsMyEngine::
		CRenderingEngine::GetInstance()->GetTextureFileFromBank(filePath);

	if (textureMap == nullptr)
	{
		// ���\�[�X���Ȃ�������V������������
		textureMap = new Texture();
		textureMap->InitFromMemory(map, mapSize);
		// ���\�[�X��o�^����
		nsNinjaAttract::nsMyEngine::
			CRenderingEngine::GetInstance()->RegistTextureFileToBank(filePath, textureMap);
	}

	// �X�y�L�����}�b�v�Ƀe�N�X�`�����\�[�X��ݒ肷��
	m_specularMap = textureMap;


	//////// ���˃}�b�v ////////

	// �������f�[�^��ݒ肷��
	if (tkmMat.reflectionMap != nullptr)
	{
		// ���˃}�b�v���\���Ă���

		// �e�N�X�`���������p�̃f�[�^���擾
		filePath = tkmMat.reflectionMapFilePath.c_str();
		map = tkmMat.reflectionMap;
		mapSize = tkmMat.reflectionMapSize;
	}
	else
	{
		// ���˃}�b�v���\���Ă��Ȃ�

		// �k���e�N�X�`���̃f�[�^���擾
		filePath = nullTextureMaps.GetReflectionMapFilePath();
		map = nullTextureMaps.GetReflectionMap().get();
		mapSize = nullTextureMaps.GetReflectionMapSize();
	}

	// �e�N�X�`���o���N���烊�\�[�X��T���Ď���Ă���
	textureMap = nsNinjaAttract::nsMyEngine::
		CRenderingEngine::GetInstance()->GetTextureFileFromBank(filePath);

	if (textureMap == nullptr)
	{
		// ���\�[�X���Ȃ�������V������������
		textureMap = new Texture();
		textureMap->InitFromMemory(map, mapSize);
		// ���\�[�X��o�^����
		nsNinjaAttract::nsMyEngine::
			CRenderingEngine::GetInstance()->RegistTextureFileToBank(filePath, textureMap);
	}

	// ���˂Ƀe�N�X�`�����\�[�X��ݒ肷��
	m_reflectionMap = textureMap;


	//////// ���܃}�b�v ////////

	// �������f�[�^��ݒ肷��
	if (tkmMat.refractionMap != nullptr)
	{
		// ���܃}�b�v���\���Ă���

		// �e�N�X�`���������p�̃f�[�^���擾
		filePath = tkmMat.refractionMapFilePath.c_str();
		map = tkmMat.refractionMap;
		mapSize = tkmMat.refractionMapSize;
	}
	else
	{
		// ���܃}�b�v���\���Ă��Ȃ�

		// �k���e�N�X�`���̃f�[�^���擾
		filePath = nullTextureMaps.GetRefractionMapFilePath();
		map = nullTextureMaps.GetRefractionMap().get();
		mapSize = nullTextureMaps.GetRefractionMapSize();
	}

	// �e�N�X�`���o���N���烊�\�[�X��T���Ď���Ă���
	textureMap = nsNinjaAttract::nsMyEngine::
		CRenderingEngine::GetInstance()->GetTextureFileFromBank(filePath);

	if (textureMap == nullptr)
	{
		// ���\�[�X���Ȃ�������V������������
		textureMap = new Texture();
		textureMap->InitFromMemory(map, mapSize);
		// ���\�[�X��o�^����
		nsNinjaAttract::nsMyEngine::
			CRenderingEngine::GetInstance()->RegistTextureFileToBank(filePath, textureMap);
	}

	// ���܂Ƀe�N�X�`�����\�[�X��ݒ肷��
	m_refractionMap = textureMap;

}
void Material::InitFromTkmMaterila(
	const TkmFile::SMaterial& tkmMat,
	const wchar_t* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntryPointFunc,
	const char* psEntryPointFunc,
	const std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>& colorBufferFormat,
	const D3D12_CULL_MODE cullMode
)
{
	//�e�N�X�`�������[�h�B
	InitTexture(tkmMat);
	
	//�萔�o�b�t�@���쐬�B
	SMaterialParam matParam;
	matParam.hasNormalMap = m_normalMap->IsValid() ? 1 : 0;
	matParam.hasSpecMap = m_specularMap->IsValid() ? 1 : 0;
	m_constantBuffer.Init(sizeof(SMaterialParam), &matParam);

	//���[�g�V�O�l�`�����������B
	m_rootSignature.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);

	if (wcslen(fxFilePath) > 0) {
		//�V�F�[�_�[���������B
		InitShaders(fxFilePath, vsEntryPointFunc, vsSkinEntryPointFunc, psEntryPointFunc);
		//�p�C�v���C���X�e�[�g���������B
		InitPipelineState(colorBufferFormat, cullMode);
	}
}
void Material::InitPipelineState(
	const std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>& colorBufferFormat,
	const D3D12_CULL_MODE cullMode
)
{
	// ���_���C�A�E�g���`����B
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//�p�C�v���C���X�e�[�g���쐬�B
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel->GetCompiledBlob());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel->GetCompiledBlob());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = TRUE;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// �ǉ��B�ύX�B
	psoDesc.RasterizerState.CullMode = cullMode;	//�J�����O�ݒ�


	int numRenderTarget = 0;
	for (auto& format : colorBufferFormat) {
		if (format == DXGI_FORMAT_UNKNOWN) {
			//�t�H�[�}�b�g���w�肳��Ă��Ȃ��ꏊ��������I���B
			break;
}
		psoDesc.RTVFormats[numRenderTarget] = colorBufferFormat[numRenderTarget];
		numRenderTarget++;
		}
	psoDesc.NumRenderTargets = numRenderTarget;
#if 0 //�Â������B
	psoDesc.NumRenderTargets = 3;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//�A���x�h�J���[�o�͗p�B
#ifdef SAMPE_16_02
	psoDesc.RTVFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;	//�@���o�͗p�B	
	psoDesc.RTVFormats[2] = DXGI_FORMAT_R32_FLOAT;						//Z�l�B
#else
	psoDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;			//�@���o�͗p�B	
	psoDesc.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;	//Z�l�B
#endif
#endif
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	m_skinModelPipelineState.Init(psoDesc);

	//�����ăX�L���Ȃ����f���p���쐬�B
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel->GetCompiledBlob());
	m_nonSkinModelPipelineState.Init(psoDesc);

	//�����Ĕ������}�e���A���p�B
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel->GetCompiledBlob());
	psoDesc.BlendState.IndependentBlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

	
	m_transSkinModelPipelineState.Init(psoDesc);

	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel->GetCompiledBlob());
	m_transNonSkinModelPipelineState.Init(psoDesc);

}
void Material::InitShaders(
	const wchar_t* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntriyPointFunc,
	const char* psEntryPointFunc
)
{
	// �ύX�B�ǉ��B

	// �G�C���A�X�錾
	// �����_�����O�G���W��
	using CRenderingEngine = nsNinjaAttract::nsMyEngine::CRenderingEngine;

	//fxFilePath��wchar����char�ɕϊ�
	char filePath[256];
	wcstombs(filePath, fxFilePath, 256);

	//////// 1.�X�L���Ȃ����f���p�̃V�F�[�_�[�����[�h����B ////////
	// �V�F�[�_�[�t�@�C���o���N���烊�\�[�X��T���ĂƂ��Ă���
	m_vsNonSkinModel = CRenderingEngine::GetInstance()->GetShaderFromBank(filePath, vsEntryPointFunc);
	// ���o�^���H
	if (m_vsNonSkinModel == nullptr)
	{
		// ���o�^�Ȃ�
		// �V�������\�[�X�𐶐�
		m_vsNonSkinModel = new Shader;
		m_vsNonSkinModel->LoadVS(fxFilePath, vsEntryPointFunc);
		// ���\�[�X�����\�[�X�o���N�ɓo�^����
		CRenderingEngine::GetInstance()->RegistShaderToBank(filePath, vsEntryPointFunc, m_vsNonSkinModel);
	}
	
	//////// 2.�X�L�����胂�f���p�̃V�F�[�_�[�����[�h����B ////////
	// �V�F�[�_�[�t�@�C���o���N���烊�\�[�X��T���ĂƂ��Ă���
	m_vsSkinModel = CRenderingEngine::GetInstance()->GetShaderFromBank(filePath, vsSkinEntriyPointFunc);
	// ���o�^���H
	if (m_vsSkinModel == nullptr)
	{
		// ���o�^�Ȃ�
		// �V�������\�[�X�𐶐�
		m_vsSkinModel = new Shader;
		m_vsSkinModel->LoadVS(fxFilePath, vsSkinEntriyPointFunc);
		// ���\�[�X�����\�[�X�o���N�ɓo�^����
		CRenderingEngine::GetInstance()->RegistShaderToBank(filePath, vsSkinEntriyPointFunc, m_vsSkinModel);
	}
	
	//////// 3.���f���p�̃s�N�Z���V�F�[�_�[����[�ǂ���B ////////
		// �V�F�[�_�[�t�@�C���o���N���烊�\�[�X��T���ĂƂ��Ă���
	m_psModel = CRenderingEngine::GetInstance()->GetShaderFromBank(filePath, psEntryPointFunc);
	// ���o�^���H
	if (m_psModel == nullptr)
	{
		// ���o�^�Ȃ�
		// �V�������\�[�X�𐶐�
		m_psModel = new Shader;
		m_psModel->LoadPS(fxFilePath, psEntryPointFunc);
		// ���\�[�X�����\�[�X�o���N�ɓo�^����
		CRenderingEngine::GetInstance()->RegistShaderToBank(filePath, psEntryPointFunc, m_psModel);
	}

	return;
}
void Material::BeginRender(RenderContext& rc, int hasSkin)
{
	rc.SetRootSignature(m_rootSignature);
	
	if (hasSkin) {
	//	rc.SetPipelineState(m_skinModelPipelineState);
		rc.SetPipelineState(m_transSkinModelPipelineState);
	}
	else {
	//	rc.SetPipelineState(m_nonSkinModelPipelineState);
		rc.SetPipelineState(m_transNonSkinModelPipelineState);
	}
}

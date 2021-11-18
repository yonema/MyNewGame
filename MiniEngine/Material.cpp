#include "stdafx.h"
#include "Material.h"
#include "../GameTemplate/Game/RenderingEngine.h"

//ルートシグネチャとパイプラインステート周りはカリカリカリ
enum {
	enDescriptorHeap_CB,
	enDescriptorHeap_SRV,
	enNumDescriptorHeap
};
	
void Material::InitTexture(const TkmFile::SMaterial& tkmMat)
{
	const auto& nullTextureMaps = g_graphicsEngine->GetNullTextureMaps();
	if (tkmMat.albedoMap != nullptr) {
		m_albedoMap.InitFromMemory(tkmMat.albedoMap, tkmMat.albedoMapSize);
	}
	else {
		m_albedoMap.InitFromMemory(
			nullTextureMaps.GetAlbedoMap().get(), 
			nullTextureMaps.GetAlbedoMapSize());
	}
	if (tkmMat.normalMap != nullptr) {
		m_normalMap.InitFromMemory(tkmMat.normalMap, tkmMat.normalMapSize);
	}
	else {
		m_normalMap.InitFromMemory(
			nullTextureMaps.GetNormalMap().get(), 
			nullTextureMaps.GetNormalMapSize());
	}
	if (tkmMat.specularMap != nullptr) {
		m_specularMap.InitFromMemory(tkmMat.specularMap, tkmMat.specularMapSize);
	}
	else {
		m_specularMap.InitFromMemory(
			nullTextureMaps.GetSpecularMap().get(),
			nullTextureMaps.GetSpecularMapSize());
	}

	if (tkmMat.reflectionMap != nullptr) {
		m_reflectionMap.InitFromMemory(tkmMat.reflectionMap, tkmMat.reflectionMapSize);
	}
	else {
		m_reflectionMap.InitFromMemory(
			nullTextureMaps.GetReflectionMap().get(),
			nullTextureMaps.GetReflectionMapSize());
	}

	if (tkmMat.refractionMap != nullptr) {
		m_refractionMap.InitFromMemory(tkmMat.refractionMap, tkmMat.refractionMapSize);
	}
	else {
		m_refractionMap.InitFromMemory(
			nullTextureMaps.GetRefractionMap().get(),
			nullTextureMaps.GetRefractionMapSize());
	}
}
void Material::InitFromTkmMaterila(
	const TkmFile::SMaterial& tkmMat,
	const wchar_t* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntryPointFunc,
	const char* psEntryPointFunc,
	const std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>& colorBufferFormat
)
{
	//テクスチャをロード。
	InitTexture(tkmMat);
	
	//定数バッファを作成。
	SMaterialParam matParam;
	matParam.hasNormalMap = m_normalMap.IsValid() ? 1 : 0;
	matParam.hasSpecMap = m_specularMap.IsValid() ? 1 : 0;
	m_constantBuffer.Init(sizeof(SMaterialParam), &matParam);

	//ルートシグネチャを初期化。
	m_rootSignature.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);

	if (wcslen(fxFilePath) > 0) {
		//シェーダーを初期化。
		InitShaders(fxFilePath, vsEntryPointFunc, vsSkinEntryPointFunc, psEntryPointFunc);
		//パイプラインステートを初期化。
		InitPipelineState(colorBufferFormat);
	}
}
void Material::InitPipelineState(
	const std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>& colorBufferFormat
)
{
	// 頂点レイアウトを定義する。
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

	//パイプラインステートを作成。
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

	int numRenderTarget = 0;
	for (auto& format : colorBufferFormat) {
		if (format == DXGI_FORMAT_UNKNOWN) {
			//フォーマットが指定されていない場所が来たら終わり。
			break;
}
		psoDesc.RTVFormats[numRenderTarget] = colorBufferFormat[numRenderTarget];
		numRenderTarget++;
		}
	psoDesc.NumRenderTargets = numRenderTarget;
#if 0 //古い実装。
	psoDesc.NumRenderTargets = 3;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//アルベドカラー出力用。
#ifdef SAMPE_16_02
	psoDesc.RTVFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;	//法線出力用。	
	psoDesc.RTVFormats[2] = DXGI_FORMAT_R32_FLOAT;						//Z値。
#else
	psoDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;			//法線出力用。	
	psoDesc.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;	//Z値。
#endif
#endif
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	m_skinModelPipelineState.Init(psoDesc);

	//続いてスキンなしモデル用を作成。
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel->GetCompiledBlob());
	m_nonSkinModelPipelineState.Init(psoDesc);

	//続いて半透明マテリアル用。
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
	// 変更。追加。

	// エイリアス宣言
	// レンダリングエンジン
	using CRenderingEngine = nsMyGame::nsMyEngine::CRenderingEngine;

	//fxFilePathをwcharからcharに変換
	char filePath[256];
	wcstombs(filePath, fxFilePath, 256);

	//////// 1.スキンなしモデル用のシェーダーをロードする。 ////////
	// シェーダーファイルバンクからリソースを探してとってくる
	m_vsNonSkinModel = CRenderingEngine::GetInstance()->GetShaderFromBank(filePath, vsEntryPointFunc);
	// 未登録か？
	if (m_vsNonSkinModel == nullptr)
	{
		// 未登録なら
		// 新しくリソースを生成
		m_vsNonSkinModel = new Shader;
		m_vsNonSkinModel->LoadVS(fxFilePath, vsEntryPointFunc);
		// リソースをリソースバンクに登録する
		CRenderingEngine::GetInstance()->RegistShaderToBank(filePath, vsEntryPointFunc, m_vsNonSkinModel);
	}
	
	//////// 2.スキンありモデル用のシェーダーをロードする。 ////////
	// シェーダーファイルバンクからリソースを探してとってくる
	m_vsSkinModel = CRenderingEngine::GetInstance()->GetShaderFromBank(filePath, vsSkinEntriyPointFunc);
	// 未登録か？
	if (m_vsSkinModel == nullptr)
	{
		// 未登録なら
		// 新しくリソースを生成
		m_vsSkinModel = new Shader;
		m_vsSkinModel->LoadVS(fxFilePath, vsSkinEntriyPointFunc);
		// リソースをリソースバンクに登録する
		CRenderingEngine::GetInstance()->RegistShaderToBank(filePath, vsSkinEntriyPointFunc, m_vsSkinModel);
	}
	
	//////// 3.モデル用のピクセルシェーダーをろーどする。 ////////
		// シェーダーファイルバンクからリソースを探してとってくる
	m_psModel = CRenderingEngine::GetInstance()->GetShaderFromBank(filePath, psEntryPointFunc);
	// 未登録か？
	if (m_psModel == nullptr)
	{
		// 未登録なら
		// 新しくリソースを生成
		m_psModel = new Shader;
		m_psModel->LoadPS(fxFilePath, psEntryPointFunc);
		// リソースをリソースバンクに登録する
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

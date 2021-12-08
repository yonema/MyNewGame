#include "stdafx.h"
#include "Model.h"
#include "Material.h"
#include "../GameTemplate/Game/RenderingEngine.h"

/**
 * @brief コンストラクタ
*/
ModelInitData::ModelInitData()
{
	// ユーザー拡張用の定数バッファの初期化
	for (int i = 0; i < MeshParts::m_kMaxExCBNum; i++)
	{
		m_expandConstantBuffer[i] = nullptr;
		m_expandConstantBufferSize[i] = 0;
	}
	// ユーザー拡張用のシェーダーリソースビューを初期化
	for (int i = 0; i < MeshParts::m_kMaxExSRVNum; i++)
	{
		m_expandShaderResoruceView[i] = nullptr;
	}

	return;
}


void Model::Init(const ModelInitData& initData)
{
	MY_ASSERT(
		initData.m_fxFilePath, 
		"error : initData.m_fxFilePathが指定されていません。"
	);
	MY_ASSERT(
		initData.m_tkmFilePath,
		"error : initData.m_tkmFilePathが指定されていません。"
	);
	//内部のシェーダーをロードする処理が求めているのが
	//wchar_t型の文字列なので、ここで変換しておく。
	wchar_t wfxFilePath[256] = {L""};
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fxファイルパスが指定されていません。", "エラー", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}
	
	if (initData.m_skeleton != nullptr) {
		//スケルトンが指定されている。
		m_meshParts.BindSkeleton(*initData.m_skeleton);
	}
	
	m_modelUpAxis = initData.m_modelUpAxis;

	// 変更。追加。

	// エイリアス宣言
	// レンダリングエンジンクラス
	using CRenderingEngine = nsMyGame::nsMyEngine::CRenderingEngine;

	const char* tkmFilePath = initData.m_tkmFilePath;
	if (initData.m_lodNum != 0)
	{
		tkmFilePath = initData.m_lodTkmFilePath;
	}

	// tkmファイルバンクからリソースを探して取ってくる
	auto tkmFile = CRenderingEngine::GetInstance()->GetTkmFileFromBank(tkmFilePath);

	// 未登録か？
	if (tkmFile == nullptr)
	{
		// 未登録なら
		// 新しくリソースを生成
		tkmFile = new TkmFile;
		tkmFile->Load(tkmFilePath);
		// リソースをリソースバンクに登録する
		CRenderingEngine::GetInstance()->RegistTkmFileToBank(tkmFilePath, tkmFile);
	}

	// tkmファイルのリソースを保持して
	m_tkmFile = tkmFile;

	// メッシュパーツを初期化する
	m_meshParts.InitFromTkmFile(
		*m_tkmFile, 
		wfxFilePath, 
		initData.m_vsEntryPointFunc,
		initData.m_vsSkinEntryPointFunc,
		initData.m_psEntryPointFunc,
		initData.m_expandConstantBuffer,
		initData.m_expandConstantBufferSize,
		initData.m_expandShaderResoruceView,
		initData.m_colorBufferFormat,
		initData.m_cullMode
	);

	UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	
}

void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
{
	Matrix mBias;
	if (m_modelUpAxis == enModelUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(Math::PI * -0.5f);
	}
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(pos);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	m_world = mBias * mScale * mRot * mTrans;
}

void Model::ChangeAlbedoMap(const char* materialName, Texture& albedoMap)
{
	m_meshParts.QueryMeshs([&](const SMesh& mesh) {
		//todo マテリアル名をtkmファイルに出力したなかった・・・。
		//todo 今は全マテリアル差し替えます
		for (Material* material : mesh.m_materials) {
			material->GetAlbedoMap().InitFromD3DResource(albedoMap.Get());
		}
	});
	//ディスクリプタヒープの再作成。
	m_meshParts.CreateDescriptorHeaps();
	
}
void Model::Draw(RenderContext& rc, const int numInstance)
{
	m_meshParts.Draw(
		rc, 
		m_world, 
		g_camera3D->GetViewMatrix(), 
		g_camera3D->GetProjectionMatrix(),
		m_emmisonColor,
		m_mulColor,
		numInstance
	);
}

void Model::Draw(
	RenderContext& rc,
	const Matrix& viewMatrix,
	const Matrix& projMatrix,
	const int numInstance
)
{

	m_meshParts.Draw(
		rc,
		m_world,
		viewMatrix,
		projMatrix,
		m_emmisonColor,
		m_mulColor,
		numInstance
	);

	return;
}


/**
 * @brief Modelクラスの設定に基づいたワールド行列を計算し、計算されたワールド行列が戻り値として返す。
 * @param[in] pos 座標
 * @param[in] rot 回転
 * @param[in] scale 拡大率
 * @return ワールド行列
*/
Matrix Model::CalcWorldMatrix(const Vector3& pos, const Quaternion& rot, const Vector3& scale) const
{
	Matrix mBias, mWorld;
	if (m_modelUpAxis == enModelUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(Math::PI * -0.5f);
	}
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(pos);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	mWorld = mBias * mScale * mRot * mTrans;
	return mWorld;
}

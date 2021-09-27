#include "stdafx.h"
#include "Model.h"
#include "Material.h"
#include "../GameTemplate/Game/RenderingEngine.h"

/**
 * @brief �R���X�g���N�^
*/
ModelInitData::ModelInitData()
{
	// ���[�U�[�g���p�̒萔�o�b�t�@�̏�����
	for (int i = 0; i < MeshParts::m_kMaxExCBNum; i++)
	{
		m_expandConstantBuffer[i] = nullptr;
		m_expandConstantBufferSize[i] = 0;
	}
	// ���[�U�[�g���p�̃V�F�[�_�[���\�[�X�r���[��������
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
		"error : initData.m_fxFilePath���w�肳��Ă��܂���B"
	);
	MY_ASSERT(
		initData.m_tkmFilePath,
		"error : initData.m_tkmFilePath���w�肳��Ă��܂���B"
	);
	//�����̃V�F�[�_�[�����[�h���鏈�������߂Ă���̂�
	//wchar_t�^�̕�����Ȃ̂ŁA�����ŕϊ����Ă����B
	wchar_t wfxFilePath[256] = {L""};
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fx�t�@�C���p�X���w�肳��Ă��܂���B", "�G���[", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}
	
	if (initData.m_skeleton != nullptr) {
		//�X�P���g�����w�肳��Ă���B
		m_meshParts.BindSkeleton(*initData.m_skeleton);
	}
	
	m_modelUpAxis = initData.m_modelUpAxis;

	// �ύX�B�ǉ��B

	// �G�C���A�X�錾
	// �����_�����O�G���W���N���X
	using CRenderingEngine = nsMyGame::nsMyEngine::CRenderingEngine;

	// tkm�t�@�C���o���N���烊�\�[�X��T���Ď���Ă���
	auto tkmFile = CRenderingEngine::GetInstance()->GetTkmFileFromBank(initData.m_tkmFilePath);

	// ���o�^���H
	if (tkmFile == nullptr)
	{
		// ���o�^�Ȃ�
		// �V�������\�[�X�𐶐�
		tkmFile = new TkmFile;
		tkmFile->Load(initData.m_tkmFilePath);
		// ���\�[�X�����\�[�X�o���N�ɓo�^����
		CRenderingEngine::GetInstance()->RegistTkmFileToBank(initData.m_tkmFilePath, tkmFile);
	}

	// tkm�t�@�C���̃��\�[�X��ێ�����
	m_tkmFile = tkmFile;

	// ���b�V���p�[�c������������
	m_meshParts.InitFromTkmFile(
		*m_tkmFile, 
		wfxFilePath, 
		initData.m_vsEntryPointFunc,
		initData.m_vsSkinEntryPointFunc,
		initData.m_psEntryPointFunc,
		initData.m_expandConstantBuffer,
		initData.m_expandConstantBufferSize,
		initData.m_expandShaderResoruceView
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
		//todo �}�e���A������tkm�t�@�C���ɏo�͂����Ȃ������E�E�E�B
		//todo ���͑S�}�e���A�������ւ��܂�
		for (Material* material : mesh.m_materials) {
			material->GetAlbedoMap().InitFromD3DResource(albedoMap.Get());
		}
	});
	//�f�B�X�N���v�^�q�[�v�̍č쐬�B
	m_meshParts.CreateDescriptorHeaps();
	
}
void Model::Draw(RenderContext& rc)
{
	m_meshParts.Draw(
		rc, 
		m_world, 
		g_camera3D->GetViewMatrix(), 
		g_camera3D->GetProjectionMatrix(),
		m_emmisonColor,
		m_mulColor
	);
}

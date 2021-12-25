#pragma once

#include "Indexbuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"

class Texture;

// �ύX�B�ǉ��B
//�X�v���C�g�ɐݒ�ł���ő�e�N�X�`�����B
const int MAX_TEXTURE = 32;	
//�g��SRV���ݒ肳��郌�W�X�^�̊J�n�ԍ��B
const int EXPAND_SRV_REG__START_NO = 20;

class IShaderResource;

/// <summary>
/// �A���t�@�u�����f�B���O���[�h
/// </summary>
enum AlphaBlendMode {
	AlphaBlendMode_None,	//�A���t�@�u�����f�B���O�Ȃ�(�㏑��)�B
	AlphaBlendMode_Trans,	//����������
	AlphaBlendMode_Add,		//���Z����
};
/// <summary>
/// �X�v���C�g�̏������f�[�^�B
/// </summary>
struct SpriteInitData {
	const char* m_ddsFilePath[MAX_TEXTURE]= {nullptr};		//DDS�t�@�C���̃t�@�C���p�X�B
	Texture* m_textures[MAX_TEXTURE] = { nullptr };			//�g�p����e�N�X�`���BDDS�t�@�C���̃p�X���w�肳��Ă���ꍇ�́A���̃p�����[�^�͖�������܂��B
	const char* m_vsEntryPointFunc = "VSMain";				//���_�V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_psEntryPoinFunc = "PSMain";				//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_fxFilePath = nullptr;						//.fx�t�@�C���̃t�@�C���p�X�B
	UINT m_width = 0;										//�X�v���C�g�̕��B
	UINT m_height = 0;										//�X�v���C�g�̍����B
	// �ύX�B�ǉ��B
	static const int m_kMaxExCBNum = 4;
	void* m_expandConstantBuffer[m_kMaxExCBNum] = {};	//���[�U�[�g���̒萔�o�b�t�@
	int m_expandConstantBufferSize[m_kMaxExCBNum] = {};	//���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B
	IShaderResource* m_expandShaderResoruceView = nullptr;	//���[�U�[�g���̃V�F�[�_�[���\�[�X�B
	AlphaBlendMode m_alphaBlendMode = AlphaBlendMode_None;	//�A���t�@�u�����f�B���O���[�h�B
	// �ύX�B�ǉ��B
	// <�J���[�o�b�t�@�[�t�H�[�}�b�g, �����_�����O�^�[�Q�b�g�̍ő吔>
	std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT> m_colorBufferFormat = {
	DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	};	//�����_�����O����J���[�o�b�t�@�̃t�H�[�}�b�g�B
	D3D12_TEXTURE_ADDRESS_MODE textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;	// �e�N�X�`���A�h���b�V���O���[�h�B

	/**
	 * @brief �R���X�g���N�^
	*/
	SpriteInitData();
};

/// <summary>
/// �X�v���C�g�N���X�B
/// </summary>
class Sprite  {
public:
	static const Vector2	DEFAULT_PIVOT;					//!<�s�{�b�g�B
	virtual ~Sprite();
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="initData">�������f�[�^</param>
	void Init(const SpriteInitData& initData);
	/// <summary>
	/// �X�V�B
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="pivot">
	/// �s�{�b�g
	/// 0.5, 0.5�ŉ摜�̒��S����_�B
	/// 0.0, 0.0�ŉ摜�̍����B
	/// 1.0, 1.0�ŉ摜�̉E��B
	/// Unity��uGUI�ɏ����B
	/// </param>
	void Update(const Vector3& pos, const Quaternion& rot, const Vector3& scale, const Vector2& pivot = DEFAULT_PIVOT);
	/// <summary>
	/// �`��B
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g/param>
	void Draw(RenderContext& renderContext);
	/// <summary>
	/// ����������Ă��邩����B
	/// </summary>
	/// <returns></returns>
	bool IsInited() const
	{
		return m_isInited;
	}
private:
	/// <summary>
	/// �e�N�X�`�����������B
	/// </summary>
	/// <param name="initData"></param>
	void InitTextures(const SpriteInitData& initData);
	/// <summary>
	/// �V�F�[�_�[���������B
	/// </summary>
	/// <param name="initData"></param>
	void InitShader( const SpriteInitData& initData );
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v���������B
	/// </summary>
	/// <param name="initData"></param>
	void InitDescriptorHeap(const SpriteInitData& initData);
	/// <summary>
	/// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���������B
	/// </summary>
	/// <param name="initData"></param>
	void InitVertexBufferAndIndexBuffer(const SpriteInitData& initData);
	/// <summary>
	/// �p�C�v���C���X�e�[�g������������B
	/// </summary>
	void InitPipelineState(const SpriteInitData& initData);
	/// <summary>
	/// �萔�o�b�t�@���������B
	/// </summary>
	/// <param name="initData"></param>
	void InitConstantBuffer(const SpriteInitData& initData);
private:
	IndexBuffer m_indexBuffer;			//�C���f�b�N�X�o�b�t�@�B
	VertexBuffer m_vertexBuffer;		//���_�o�b�t�@�B
	int m_numTexture = 0;				//�e�N�X�`���̖����B
	Texture m_textures[MAX_TEXTURE];	//�e�N�X�`���B
	Texture* m_textureExternal[MAX_TEXTURE] = {nullptr};	//�O������w�肳�ꂽ�e�N�X�`��
	Vector3 m_position ;				//���W�B
	Vector2 m_size;						//�T�C�Y�B
	Quaternion m_rotation ;			//��]�B
	Matrix m_world;					//���[���h�s��B

	struct LocalConstantBuffer {
		Matrix mvp;
		Vector4 mulColor;
		Vector4 albedoColor;
		int isControlAlbedo = 0;
		Vector4 screenParam;
	};
	LocalConstantBuffer m_constantBufferCPU;	//CPU���̒萔�o�b�t�@�B
	ConstantBuffer		m_constantBufferGPU;	//GPU���̒萔�o�b�t�@�B
	// �ύX�B�ǉ��B
	//ConstantBuffer		m_userExpandConstantBufferGPU;	//���[�U�[�g���̒萔�o�b�t�@(GPU��)
	//void* m_userExpandConstantBufferCPU = nullptr;		//���[�U�[�g���̒萔�o�b�t�@(CPU��)
	DescriptorHeap		m_descriptorHeap;		//�f�B�X�N���v�^�q�[�v�B
	RootSignature		m_rootSignature;		//���[�g�V�O�l�`���B
	PipelineState		m_pipelineState;		//�p�C�v���C���X�e�[�g�B
	Shader				m_vs;					//���_�V�F�[�_�[�B
	Shader				m_ps;					//�s�N�Z���V�F�[�_�[�B
	bool				m_isInited = false;		//�������ς݁H

	// �ǉ�
public:
	Sprite();

	/**
	 * @brief ��Z�J���[��ݒ肷��
	 * @param [in] mulColor ��Z�J���[
	*/
	void SetMulColor(const Vector4& mulColor)
	{
		m_constantBufferCPU.mulColor = mulColor;
	}

	/**
	 * @brief ��Z�J���[�𓾂�
	 * @return ��Z�J���[
	*/
	const Vector4& GetMulColor() const
	{
		return m_constantBufferCPU.mulColor;
	}

	/**
	 * @brief �X�v���C�g�̃A���t�@�l��ݒ肷��
	 * @param [in] alphaValue �A���t�@�l
	*/
	void SetAlphaValue(const float alphaValue)
	{
		m_constantBufferCPU.mulColor.w = alphaValue;
	}

	/**
	 * @brief �A���x�h�J���[�𐧌䂷�邩�H��ݒ�
	 * @param[in] isContolAlbedo �A���x�h�J���[�𐧌䂷�邩�H
	*/
	void SetIsControlAlbedo(const bool isContolAlbedo)
	{
		m_constantBufferCPU.isControlAlbedo = isContolAlbedo;
	}

	/**
	 * @brief �A���x�h�J���[��ݒ�BisContolAlbedo��true�̂Ƃ��̂ݗL���B
	 * @param[in] albedoColor �A���x�h�J���[
	*/
	void SetAlbedoColor(const Vector4& albedoColor)
	{
		m_constantBufferCPU.albedoColor = albedoColor;
	}

private:	// �f�[�^�����o
	//���[�U�[�g���̒萔�o�b�t�@(GPU��)
	ConstantBuffer m_userExpandConstantBufferGPU[SpriteInitData::m_kMaxExCBNum];
	//���[�U�[�g���̒萔�o�b�t�@(CPU��)
	void* m_userExpandConstantBufferCPU[SpriteInitData::m_kMaxExCBNum] = {};
};
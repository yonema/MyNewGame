#pragma once

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief ���f���֘A�̃l�[���X�y�[�X
		*/
		namespace nsModel
		{
			/**
			 * @brief ���f���̒萔�f�[�^
			*/
			namespace nsModelConstData
			{
				constexpr const char* const kTkmFileExtensinon = ".tkm";	//!< tkm�t�@�C���̊g���q
				constexpr const char* const kTksFileExtensinon = ".tks";	//!< tks�t�@�C���̊g���q
				constexpr size_t kFileExtensionLength = 4;					//!< �g���q�̒���

				//!< ���_�V�F�[�_�̃G���g���[�|�C���g
				constexpr const char* const kVsEntryPointFunc = "VSMain";
				//!< �X�L������̒��_�V�F�[�_�[�̃G���g���[�|�C���g
				constexpr const char* const kVsSkinEntryPointFunc = "VSSkinMain";
				//!< �s�N�Z���V�F�[�_�̃G���g���[�|�C���g
				constexpr const char* const kPsEntryPointFunc = "PSMain";
				//!< GBuffer�ɕ`�悷�郂�f���̃V�F�[�_�[�t�@�C���p�X
				constexpr const char* const kRenderToGBufferFxFilePath = "Assets/shader/RenderToGBufferFor3DModel.fx";
				//!< �������`�悷�郂�f���̃V�F�[�_�[�t�@�C���p�X
				constexpr const char* const kTranslucentModelFxFilePath = "Assets/shader/TranslucentModel.fx";
				//!< �f�t�H���g�̃V�F�[�_�[�t�@�C���p�X
				constexpr const char* const kDefaultFxFilePath = kRenderToGBufferFxFilePath;

				constexpr float kAnimationInterpolateTime = 0.2f;	//!< �A�j���[�V�����ۊǎ���

			}
		}
	}
}
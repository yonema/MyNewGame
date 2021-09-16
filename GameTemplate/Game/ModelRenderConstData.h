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

				constexpr const char* const kVsEntryPointFunc = "VSMain";					//!< ���_�V�F�[�_�̃G���g���[�|�C���g
				constexpr const char* const kVsSkinEntryPointFunc = "VSSkinMain";			//!< �X�L������̒��_�V�F�[�_�[�̃G���g���[�|�C���g
				constexpr const char* const kPsEntryPointFunc = "PSMain";					//!< �s�N�Z���V�F�[�_�̃G���g���[�|�C���g
				constexpr const char* const kDefaultFxFilePath = "Assets/shader/model.fx";	//!< �f�t�H���g�̃V�F�[�_�[�t�@�C���p�X

			}
		}
	}
}
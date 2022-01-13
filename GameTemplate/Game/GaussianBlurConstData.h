#pragma once
namespace nsNinjaAttract
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �K�E�V�A���u���[�̒萔�f�[�^
		*/
		namespace nsGaussianBlurConstData
		{
			constexpr int kWeightsNum = 8;		//!< �d�݂̐�

			//!< fx�t�@�C���p�X
			constexpr const char* const kfxFilePath = "Assets/shader/gaussianBlur.fx";

			//!< �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
			constexpr const char* const kPsEntryFunc = "PSBlur";
			//!< �A���t�@�`�����l���ɂ��u���[��������Ƃ��̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
			constexpr const char* const kPsEntryFunc_alpha = "PSBlur_Alpha";

			//!< ���u���[��������Ƃ��̒��_�V�F�[�_�[�̃G���g���[�|�C���g
			constexpr const char* const kVsEntryFunc_xBlur = "VSXBlur";
			//!< �c�u���[��������Ƃ��̒��_�V�F�[�_�[�̃G���g���[�|�C���g
			constexpr const char* const kVsEntryFunc_yBlur = "VSYBlur";

		}
	}
}
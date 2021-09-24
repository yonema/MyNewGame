#pragma once

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �|�X�g�G�t�F�N�g�֘A�̃l�[���X�y�[�X
		*/
		namespace nsPostEffect
		{
			/**
			 * @brief �|�X�g�G�t�F�N�g�̒萔�f�[�^
			*/
			namespace nsPostEffectConstDat
			{

			}

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

			/**
			 * @brief �u���[���̒萔�f�[�^
			*/
			namespace nsBloomConstData
			{
				constexpr int kGaussianBlurNum = 4;		//!< �K�E�V�A���u���[�̐�
				constexpr int kGaussianBulrPower = 10;	//!< �K�E�V�A���u���[�̃u���[�̋���

				//!< fx�t�@�C���p�X
				constexpr const char* const kfxFilePath = "Assets/shader/bloom.fx";

				//!< �P�x���o���̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
				constexpr const char* const kPsEntryFunc_samplingLuminance = "PSSamplingLuminance";
				//!< �ŏI�������̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
				constexpr const char* const kPsEntryFunc_bloomFinal = "PSBloomFinal";
			}
		}
	}
}
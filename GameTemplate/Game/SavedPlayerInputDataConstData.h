#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief �O���f�[�^�p�l�[���X�y�[�X
	*/
	namespace nsExternalData
	{

		/**
		 * @brief �v���C���[�̓��͏��̕ۑ��f�[�^�N���X�̒萔�f�[�^
		*/
		namespace nsSavedPlayerInputDataConstData
		{
			/**
			 * @brief �ۑ��t�@�C���̎��
			*/
			enum EnSavedFileType
			{
				enST_titleDirecting,
				enSavedFileTypeNum
			};


			//!< �����o���t�@�C���p�X
			static const char* const kOutputFilePath[enSavedFileTypeNum]
			{
				"Assets/outputData/playerInputData/titleDirecting.dat"
			};

		}
	}
}
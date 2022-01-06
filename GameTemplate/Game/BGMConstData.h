#pragma once

namespace nsMyGame
{
	namespace nsBGM
	{
		namespace nsBGMConstData
		{
			/**
			 * @brief BGM�̎��
			*/
			enum EnBGMType
			{
				enBT_Normal,
				enBT_Swing,
				enBGMTypeNum
			};

			//!< BGM�̃T�E���h�̃t�@�C���p�X
			constexpr const wchar_t* const kBGMSoundFilePaht[enBGMTypeNum] =
			{
				L"Assets/sound/bgm/normalBGM.wav",
				L"Assets/sound/bgm/swingBGM.wav"
			};

			constexpr float kBGMSoundVolume[enBGMTypeNum] =
			{
				0.5f,0.5f
			};

			//!< �N���X�t�F�[�h�̃^�C��
			constexpr float kCrossFadeTime = 3.0f;
		}
	}
}
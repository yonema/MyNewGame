#pragma once

namespace nsNinjaAttract
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
				enBT_qte,
				enBGMTypeNum
			};

			//!< BGM�̃T�E���h�̃t�@�C���p�X
			constexpr const wchar_t* const kBGMSoundFilePath[enBGMTypeNum] =
			{
				L"Assets/sound/bgm/normalBGM.wav",
				L"Assets/sound/bgm/swingBGM.wav",
				L"Assets/sound/bgm/qteBGM.wav"
			};
			//!< ���U���g�̃T�E���h�̃t�@�C���p�X
			constexpr const wchar_t* const kResultSoundFilePath = L"Assets/sound/mission/result.wav";

			constexpr float kBGMSoundVolume[enBGMTypeNum] =
			{
				0.5f,0.75f,0.75f
			};

			//!< �N���X�t�F�[�h�̃^�C��
			constexpr float kCrossFadeTime[enBGMTypeNum] =
			{
				5.0f,2.0f,1.0f
			};

			constexpr float kResultSoundVolume = 0.5f;	//!< ���U���g�T�E���h�̉���
			constexpr float kResultBGMVolume = 0.22f;	//!< ���U���g����BGM�̉���
		}
	}
}
#pragma once

namespace nsNinjaAttract
{
	namespace nsBGM
	{
		namespace nsBGMConstData
		{
			/**
			 * @brief BGMの種類
			*/
			enum EnBGMType
			{
				enBT_Normal,
				enBT_Swing,
				enBT_qte,
				enBGMTypeNum
			};

			//!< BGMのサウンドのファイルパス
			constexpr const wchar_t* const kBGMSoundFilePath[enBGMTypeNum] =
			{
				L"Assets/sound/bgm/normalBGM.wav",
				L"Assets/sound/bgm/swingBGM.wav",
				L"Assets/sound/bgm/qteBGM.wav"
			};
			//!< リザルトのサウンドのファイルパス
			constexpr const wchar_t* const kResultSoundFilePath = L"Assets/sound/mission/result.wav";

			constexpr float kBGMSoundVolume[enBGMTypeNum] =
			{
				0.5f,0.75f,0.75f
			};

			//!< クロスフェードのタイム
			constexpr float kCrossFadeTime[enBGMTypeNum] =
			{
				5.0f,2.0f,1.0f
			};

			constexpr float kResultSoundVolume = 0.5f;	//!< リザルトサウンドの音量
			constexpr float kResultBGMVolume = 0.22f;	//!< リザルト中のBGMの音量
		}
	}
}
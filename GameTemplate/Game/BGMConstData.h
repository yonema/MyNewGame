#pragma once

namespace nsMyGame
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
				enBGMTypeNum
			};

			//!< BGMのサウンドのファイルパス
			constexpr const wchar_t* const kBGMSoundFilePaht[enBGMTypeNum] =
			{
				L"Assets/sound/bgm/normalBGM.wav",
				L"Assets/sound/bgm/swingBGM.wav"
			};

			constexpr float kBGMSoundVolume[enBGMTypeNum] =
			{
				0.5f,0.5f
			};

			//!< クロスフェードのタイム
			constexpr float kCrossFadeTime = 3.0f;
		}
	}
}
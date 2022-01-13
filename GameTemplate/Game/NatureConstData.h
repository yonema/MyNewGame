#pragma once

namespace nsNinjaAttract
{

	/**
	 * @brief 自然関連のネームスペース
	*/
	namespace nsNature
	{
		/**
		 * @brief スカイキューブの定数データ
		*/
		namespace nsSkyCubeConstData
		{
			//!< スカイのtkmファイルパス
			constexpr const char* const kTkmFilePath = "Assets/modelData/sky/sky.tkm";
			//!< スカイのfxファイルパス
			constexpr const char* const kFxFilePath = "Assets/shader/SkyCubeMap.fx";

			//!< スカイのテクスチャのデフォルトのDDSファイルパス
			constexpr const wchar_t* const kDefaultDDSPath = L"Assets/modelData/sky/skyCubeMap.dds";
			constexpr float kDefaultIBLLuminance = 1.0f;	//!< IBLのデフォルトの明るさ

			//!< デフォルトのスカイの拡大率
			const Vector3 kDefaultSkySclae = Vector3::One * 10000.0f;

			/**
			 * @brief スカイキューブのタイプ
			*/
			enum EnSkyCubeType
			{
				enSkyCubeType_day,		// 昼間
				enSkyCubeType_night,	// 夜間
				enSkyCubeType_snow,		// 雪山
				enSkyCubeType_default	// デフォルト
			};

			/**
			 * @brief 指定したタイプの、スカイキューブのテクスチャのDDSファイルパスを得る
			 * @param[in] skyCubeType スカイキューブのタイプ
			 * @return DDSファイルパス
			*/
			constexpr const wchar_t* const GetSkyCubeTextureDDSFilePath(const EnSkyCubeType skyCubeType)
			{
				// タイプごとで処理を振り分ける
				switch (skyCubeType)
				{
				case enSkyCubeType_day:		// 昼間
					return L"Assets/modelData/sky/skyCubeMap.dds";
					break;
				case enSkyCubeType_night:	// 夜間
					return L"Assets/modelData/sky/skyCubeMapNight.dds";
					break;
				case enSkyCubeType_snow:	// 雪山
					return L"Assets/modelData/sky/skyCubeMapSnow.dds";
					break;
				case enSkyCubeType_default:	// デフォルト
					return kDefaultDDSPath;
					break;
				default:					// デフォルト
					return kDefaultDDSPath;
					break;
				}
			}

			constexpr float GetIBLLuminance(const EnSkyCubeType skyCubeType)
			{
				// タイプごとで処理を振り分ける
				switch (skyCubeType)
				{
				case enSkyCubeType_day:		// 昼間
					return 1.0f;
					break;
				case enSkyCubeType_night:	// 夜間
					return 1.0f;
					break;
				case enSkyCubeType_snow:	// 雪山
					return 1.0f;
					break;
				case enSkyCubeType_default:	// デフォルト
					return kDefaultIBLLuminance;
					break;
				default:					// デフォルト
					return kDefaultIBLLuminance;
					break;
				}
			}

		}
	}
}
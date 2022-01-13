#pragma once

namespace nsNinjaAttract
{

	/**
	 * @brief ���R�֘A�̃l�[���X�y�[�X
	*/
	namespace nsNature
	{
		/**
		 * @brief �X�J�C�L���[�u�̒萔�f�[�^
		*/
		namespace nsSkyCubeConstData
		{
			//!< �X�J�C��tkm�t�@�C���p�X
			constexpr const char* const kTkmFilePath = "Assets/modelData/sky/sky.tkm";
			//!< �X�J�C��fx�t�@�C���p�X
			constexpr const char* const kFxFilePath = "Assets/shader/SkyCubeMap.fx";

			//!< �X�J�C�̃e�N�X�`���̃f�t�H���g��DDS�t�@�C���p�X
			constexpr const wchar_t* const kDefaultDDSPath = L"Assets/modelData/sky/skyCubeMap.dds";
			constexpr float kDefaultIBLLuminance = 1.0f;	//!< IBL�̃f�t�H���g�̖��邳

			//!< �f�t�H���g�̃X�J�C�̊g�嗦
			const Vector3 kDefaultSkySclae = Vector3::One * 10000.0f;

			/**
			 * @brief �X�J�C�L���[�u�̃^�C�v
			*/
			enum EnSkyCubeType
			{
				enSkyCubeType_day,		// ����
				enSkyCubeType_night,	// ���
				enSkyCubeType_snow,		// ��R
				enSkyCubeType_default	// �f�t�H���g
			};

			/**
			 * @brief �w�肵���^�C�v�́A�X�J�C�L���[�u�̃e�N�X�`����DDS�t�@�C���p�X�𓾂�
			 * @param[in] skyCubeType �X�J�C�L���[�u�̃^�C�v
			 * @return DDS�t�@�C���p�X
			*/
			constexpr const wchar_t* const GetSkyCubeTextureDDSFilePath(const EnSkyCubeType skyCubeType)
			{
				// �^�C�v���Ƃŏ�����U�蕪����
				switch (skyCubeType)
				{
				case enSkyCubeType_day:		// ����
					return L"Assets/modelData/sky/skyCubeMap.dds";
					break;
				case enSkyCubeType_night:	// ���
					return L"Assets/modelData/sky/skyCubeMapNight.dds";
					break;
				case enSkyCubeType_snow:	// ��R
					return L"Assets/modelData/sky/skyCubeMapSnow.dds";
					break;
				case enSkyCubeType_default:	// �f�t�H���g
					return kDefaultDDSPath;
					break;
				default:					// �f�t�H���g
					return kDefaultDDSPath;
					break;
				}
			}

			constexpr float GetIBLLuminance(const EnSkyCubeType skyCubeType)
			{
				// �^�C�v���Ƃŏ�����U�蕪����
				switch (skyCubeType)
				{
				case enSkyCubeType_day:		// ����
					return 1.0f;
					break;
				case enSkyCubeType_night:	// ���
					return 1.0f;
					break;
				case enSkyCubeType_snow:	// ��R
					return 1.0f;
					break;
				case enSkyCubeType_default:	// �f�t�H���g
					return kDefaultIBLLuminance;
					break;
				default:					// �f�t�H���g
					return kDefaultIBLLuminance;
					break;
				}
			}

		}
	}
}
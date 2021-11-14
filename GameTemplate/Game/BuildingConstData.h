#pragma once

namespace nsMyGame
{

	/**
	 * @brief �����֘A�̃l�[���X�y�[�X
	*/
	namespace nsBuilding
	{
		/**
		 * @brief �����֘A�̒萔�f�[�^
		*/
		namespace nsBuildingConstData 
		{
			constexpr UINT kDefaultWidthSegments = 10;				//!< �f�t�H���g�̕��Z�O�����g
			constexpr UINT kDefaultHeightSegments = 20;				//!< �f�t�H���g�̍����Z�O�����g
			constexpr UINT kDefaultLengthSegments = 10;				//!< �f�t�H���g�̒����Z�O�����g
			constexpr float kDefaultHeightLowerLimit = 1000.0f;		//!< �f�t�H���g�̍����̉���

			
			/**
			 * @brief �����̃^�C�v
			*/
			enum EnBuildingType
			{
				enBuilding_L_01a,
				enBuilding_L_06b,
				enBuilding_M_09,
				enBuilding_M_11,
				enBuilding_S_10,
				enBuilding_S_04b,
				enBuildingTypeNum	//!< �����̃^�C�v�̐�
			};

			constexpr const wchar_t* const kBuildingForwardName = L"Building";	//!< �����̐擪�̖��O

			/**
			 * @brief �����̖��O
			*/
			constexpr const char* const kBuildingNames[enBuildingTypeNum]
			{
				"Building_L_01a",
				"Building_L_06b",
				"Building_M_09",
				"Building_M_11",
				"Building_S_10",
				"Building_S_04b"
			};

			/**
			 * @brief �����̃��f���̃t�@�C���p�X
			*/
			constexpr const char* const kBuildingModelFilePath[enBuildingTypeNum]
			{
				"Assets/modelData/building/Building_L_01a.tkm",
				"Assets/modelData/building/Building_L_06b.tkm",
				"Assets/modelData/building/Building_M_09.tkm",
				"Assets/modelData/building/Building_M_11.tkm",
				"Assets/modelData/building/Building_S_10.tkm",
				"Assets/modelData/building/Building_S_04b.tkm"
			};

		}
	}
}
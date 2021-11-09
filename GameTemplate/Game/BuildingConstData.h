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
			//constexpr float kDefaultHeightLowerLimit = -10.0f;
			//constexpr float kDefaultHeightLowerLimit = 0.0f;
			
			/**
			 * @brief �����̃^�C�v
			*/
			enum EnBuildingType
			{
				enTestBuilding,		//!< �e�X�g�̃r��
				enBuildingTypeNum,	//!< �����̃^�C�v�̎��
			};

			/**
			 * @brief �����̃��f���̃t�@�C���p�X
			*/
			constexpr const char* const kBuildingModelFilePath[enBuildingTypeNum]
			{
				//"Assets/modelData/levelSource/testBuilding.tkm"
				"Assets/modelData/building/Bulding_L_01a.tkm"
			};

		}
	}
}
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
			constexpr UINT kDefaultWidthSegments = 5;				//!< �f�t�H���g�̕��Z�O�����g
			constexpr UINT kDefaultHeightSegments = 5;				//!< �f�t�H���g�̍����Z�O�����g
			constexpr UINT kDefaultLengthSegments = 5;				//!< �f�t�H���g�̒����Z�O�����g
			//constexpr float kDefaultHeightLowerLimit = 1000.0f;		//!< �f�t�H���g�̍����̉���
			//constexpr float kDefaultHeightLowerLimit = -10.0f;
			constexpr float kDefaultHeightLowerLimit = 0.0f;
		}
	}
}
#pragma once


namespace nsMyGame
{
	/**
	 * @brief �S�[���֘A�̃l�[���X�y�[�X
	*/
	namespace nsGoal
	{
		/**
		 * @brief �S�[���N���X�̒萔�f�[�^
		*/
		namespace nsGoalConstData
		{
			//!< �S�[���̃��f���t�@�C���p�X
			constexpr const char* const kGoalModelFilePath = "Assets/modelData/props/Scroll.tkm";

			constexpr float kRotateSpeed = 1.0f;	//!< ��]�X�s�[�h

			constexpr float kTiltAngle = 45.0f;	//!< �X���̊p�x

			constexpr float kGoalThreshold = 75.0f;	//!< �S�[���̂������l

		}
	}
}
#pragma once

namespace nsMyGame
{

	/**
	 * @brief AI�L�����N�^�[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAICharacter
	{
		/**
		 * @brief AI�L�����N�^�[�̒萔�f�[�^
		*/
		namespace nsAICharacterConstData
		{
			// �Ԃ̃��f���f�[�^�̃t�@�C���p�X
			constexpr const char* const kCarModelFilePath = "Assets/modelData/car/sedan_car.tkm";

			// ���ۂ̃^�[�Q�b�g�|�C���g�̊Ԋu
			constexpr float kRealBetweenTargetPoint = 11200.0f;
			// �^�[�Q�b�g�|�C���g�̊��o�̂�Ƃ�
			constexpr float kBetweenTargetPointBuff = 1000.0f;
			// �g�p����^�[�Q�b�g�|�C���g�̊Ԋu
			constexpr float kBetweenTargetPoint = kRealBetweenTargetPoint + kBetweenTargetPointBuff;
			// �g�p����^�[�Q�b�g�|�C���g�̍Œ�Ԋu
			constexpr float kMinBetweenTargetPoint = 100.0f;
			// �p�x�̂������l
			// 1.22rad��70.0deg
			constexpr float kAngleThreshold = 1.22f;
			// ���i�p�̊p�x�̂������l
			// 0.52rad��30.0deg
			constexpr float kStraightAngleThreshold = 0.52f;

			/**
			 * @brief �^�[�Q�b�g�|�C���g�̌��̎��
			*/
			enum EnCandidateTargetPointType
			{
				enTurnRight,					//!< �E��
				enTurnLeft,						//!< ����
				enGoStraight,					//!< ���i
				enCandidateTargetPointTypeNum	//!< ���̎�ނ̐�
			};
		}
	}
}
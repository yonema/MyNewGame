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

			constexpr float kCarSpeed = 2000.0f;	//!< �Ԃ̑��x
			constexpr float kCarDriftMaxAngle = 90.0f;	//!< �Ԃ̃h���t�g�̍ő�p�x
			constexpr float kCarModelRotRate = 0.2f;	//!< �Ԃ̃��f���̋��ʐ��`�⊮��
			constexpr float kCarDriftTurnBackRate = 0.6f;	//!< �Ԃ̃h���t�g�̉�]��߂��n�߂郌�[�g
			constexpr float kCarDriftSpeedDownScale = 0.7f;	//!< �Ԃ̃h���t�g�̃X�s�[�h�_�E����
			constexpr float kCarDriftSpeedDownRate = 0.5f;	//!< �Ԃ̃h���t�g�̃X�s�[�h�������n�߂郌�[�g

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

			constexpr int kCurveRelayPointNum = 10;	//!< �J�[�u�̒��p�n�_�̐�

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
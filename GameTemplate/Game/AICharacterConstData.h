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

			//!< �����̃G�t�F�N�g�̃t�@�C���p�X
			constexpr const char16_t* const kExplotionEffectFilePath = u"Assets/effect/explosion.efk";
			// �����̃G�t�F�N�g���o����W�́A�����̃o�b�t�@
			constexpr float kExplotionPosBufHeight = 50.0f;
			//!< �����̃G�t�F�N�g�̊g�嗦
			static const Vector3 kExplotionScale = { 2.0f,1.0f,2.0f };

			//!< ���̃G�t�F�N�g�̃t�@�C���p�X
			constexpr const char16_t* const kSmokeEffectFilePath = u"Assets/effect/smoke.efk";
			constexpr int kSmokeNum = 20;	//!< ���̐�
			constexpr float kSmokeTime = 2.0f;	//!< ���̎���
			// ���̃G�t�F�N�g���o����W�́A�O�����̃o�b�t�@
			constexpr float kSmokePosBufForward = 100.0f;
			// ���̃G�t�F�N�g���o����W�́A�����̃o�b�t�@
			constexpr float kSmokePosBufHeight = 100.0f;

			//!< �������̉��̃G�t�F�N�g�̃t�@�C���p�X
			constexpr const char16_t* const kSmokeExplosionEffectFilePath = u"Assets/effect/smokeExplosion.efk";

			constexpr float kCarSpeed = 2500.0f;	//!< �Ԃ̑��x
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

			//!< �Ԃ̉^�]���̃T�E���h�̃t�@�C���p�X
			constexpr const wchar_t* const kCarDriveSoundFilePath = L"Assets/sound/car/car_idle.wav";
			//!< �Ԃ̃h���t�g�̃T�E���h�̃t�@�C���p�X
			constexpr const wchar_t* const kCarDriftSoundFilePath = L"Assets/sound/car/car_break.wav";
			//!< �Γق̏p�̔����̃T�E���h�̃t�@�C���p�X
			constexpr const wchar_t* const kFireExplosionSoundFilePath = L"Assets/sound/car/fire_explosion.wav";
			//!< �Ԃ̔����̃T�E���h�̃t�@�C���p�X
			constexpr const wchar_t* const kCarExplosionSoundFilePath = L"Assets/sound/car/car_explositon.wav";

			//!< �T�E���h����������ő勗��
			constexpr float kMaxSoundDistance = 10000.0f;
			//!< �^�]���̃T�E���h�̃^�C��
			constexpr float kDriveSoundTime = 9.5f;
			constexpr float kDriveSoundVolume = 1.0f;	//!< �^�]���̃T�E���h�̉���
			constexpr float kDriftSoundVolume = 1.0f;	//!< �h���t�g�̃T�E���h�̉���
			constexpr float kFireExplosionSoundVolume = 3.0f;	//!< �Γق̔����̃T�E���h�̉���
			constexpr float kCarExplosionSoundVolume = 3.0f;	//!< �Ԃ̔����̃T�E���h�̉���
		}
	}
}
#pragma once

namespace nsMyGame
{
	/**
	 * @brief ���C�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsLight
	{
		/**
		 * @brief ���C�g�̒萔�f�[�^�̃l�[���X�y�[�X
		*/
		namespace nsLightConstData
		{
			constexpr int kMaxDirectionalLightNum = 4;	//!< �f�B���N�V�������C�g�̍ő吔
			constexpr int kMaxPointLightNum = 16;		//!< �|�C���g���C�g�̍ő吔
			constexpr int kMaxSpotLightNum = 16;		//!< �X�|�b�g���C�g�̍ő吔

			/**
			 * @brief �f�B���N�V�������C�g�̃f�t�H���g�̕����𓾂�
			 * @return �f�B���N�V�������C�g�̃f�t�H���g�̕���
			*/
			static inline Vector3& GetDefaultDirection()
			{
				static Vector3 defaultDirection = { -1.0f,-1.0f,1.0f };
				defaultDirection.Normalize();
				return defaultDirection;
			}

			const Vector4 kDefaultLightColor = { 1.0f,1.0f,1.0f,1.0f };	//!< �f�t�H���g�̃��C�g�J���[

			constexpr float kDefaultPointLightRange = 10.0f;	//!< �f�t�H���g�̃|�C���g���C�g�̉e���͈�

			constexpr float kDefaultSpotLightAngle = 45.0f;		//!< �f�t�H���g�̃X�|�b�g���C�g�̎ˏo�p�x

			/**
			 * @brief �A���r�G���g���C�g�̃f�t�H���g�̒l�𓾂�
			 * @return �A���r�G���g���C�g�̃f�t�H���g�̒l
			*/
			static inline Vector3& GetDefaultAmbientLight()
			{
				static Vector3 ambientLight = { 1.0f,1.0f,1.0f };
				ambientLight.Normalize();
				float power = 1.0f;
				ambientLight *= power;
				return ambientLight;
			}
		}
	}
}

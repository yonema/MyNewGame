#pragma once
#include "LightConstData.h"
#include "ShadowConstDatah.h"

namespace nsMyGame
{
	/**
	 * @brief ���C�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsLight
	{
		/**
		 * @brief ���C�g�̃f�[�^�̃l�[���X�y�[�X
		*/
		namespace nsLightData
		{
			//���C�g�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsLightConstData;

			/**
			 * @brief �f�B���N�V�������C�g�̃f�[�^
			*/
			struct SDirectionalLightData
			{
				Vector3 direction = GetDefaultDirection();		//!< ���C�g�̕���
				int castShadow = 1;								//!< �e�𐶐����邩�H0:�������Ȃ��A1:��������
				Vector4 color = kDefaultDirectionalLightColor;	//!< ���C�g�̃J���[
			};

			/**
			 * @brief �|�C���g���C�g�̃f�[�^
			*/
			struct SPointLightData
			{
				Vector3 position = Vector3::Zero;			//!< ���C�g�̍��W
				float range = kDefaultPointLightRange;		//!< ���C�g�̉e���͈�
				Vector4 color = kDefaultPointLightColor;	//!< ���C�g�̃J���[
			};

			/**
			 * @brief �X�|�b�g���C�g�̃f�[�^
			*/
			struct SSpotLightData
			{
				SPointLightData pointLightData;				//!< �|�C���g���C�g�̃f�[�^
				Vector3 direction = GetDefaultDirection();	//!< ���C�g�̎ˏo����
				float angle = kDefaultSpotLightAngle;		//!< ���C�g�̎ˏo�p�x�iradian�P�ʁj
			};

			/**
			 * @brief ���C�g�̃f�[�^
			*/
			struct SLightData
			{
				SDirectionalLightData directionalLightData[kMaxDirectionalLightNum];	//!< �f�B���N�V�������C�g�̃f�[�^
				SPointLightData pointLightData[kMaxPointLightNum];	//!< �|�C���g���C�g�̃f�[�^
				SSpotLightData spotLightData[kMaxSpotLightNum];		//!< �X�|�b�g���C�g�̃f�[�^
				Vector3 eyePos = Vector3::Zero;						//!< ���_
				int directionalLightNum = 0;						//!< �f�B���N�V�������C�g�̐�
				Vector3 ambientLight = GetDefaultAmbientLight();	//!< �A���r�G���g���C�g
				int pointLightNum = 0;								//!< �|�C���g���C�g�̐�
				//!< ���C�g�r���[�v���W�F�N�V�����s��
				Matrix mlvp[kMaxDirectionalLightNum][nsGraphic::nsShadow::nsShadowConstData::enShadowMapArea_num];
				int spotLightNum = 0;								//!< �X�|�b�g���C�g�̐�
			};
		}
	}
}

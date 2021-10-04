#pragma once

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �V���h�E�֘A�̃l�[���X�y�[�X
		*/
		namespace nsShadow
		{
			/**
			 * @brief �V���h�E�̒萔�f�[�^
			*/
			namespace nsShadowConstData
			{
				/**
				 * @brief �V���h�E�}�b�v�̃G���A�̗񋓌^
				*/
				enum EnShadowMapArea
				{
					enShadowMapArea_near,	//!< �߉e�͈͂̃V���h�E�}�b�v
					enShadowMapArea_middle,	//!< ���e�͈͂̃V���h�E�}�b�v
					enShadowMapArea_far,	//!< ���e�͈͂̃V���h�E�}�b�v
					enShadowMapArea_num		//!< �V���h�E�}�b�v�̃G���A�̐�
				};

				//!< �f�t�H���g�̃J�X�P�[�h�V���h�E�̃G���A��
				constexpr float kDefaultCascadeAreaRateArray[enShadowMapArea_num] = { 0.05f,0.3f, 1.0f };

				//!< �f�t�H���g�̃N���A�J���[
				constexpr float kDefaultClearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

				constexpr float kBlurPower = 1.0f;	//!< �u���[�̋���

				const Vector3 kLightTargetPos = Vector3::Zero;	//!< ���C�g�̃^�[�Q�b�g�̍��W
				constexpr float kLightMaxHeight = 5000.0f;		//!< ���C�g�̍ő�̍���
				constexpr float kLightMinFar = 1.0f;	//!< ���C�g�̍ŏ��t�@�[
				constexpr float kLightMaxFar = 15000.0f;	//!< ���C�g�̍ő�t�@�[

				constexpr float kNormalScreenWidth = 2.0f;	//!< ���K���X�N���[�����W�̉���
				constexpr float kNormalScreenHeight = 2.0f;	//!< ���K���X�N���[�����W�̏c��
			}
		}
	}
}
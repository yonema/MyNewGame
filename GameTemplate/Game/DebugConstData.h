#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief �f�o�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsDebug
	{
		/**
		 * @brief �f�o�b�N�̒萔�f�[�^
		*/
		namespace nsDebugConstData
		{
			//////// �x�N�g���`��N���X�̒萔 ////////

			constexpr int kVectorRenderDatasNum = 16;	//!< �x�N�g���`��f�[�^�W�̐�

			//!< �x�N�g���̃f�t�H���g�̖��O
			constexpr const char* const kDefaultVectorName = "vector";
		
			//!< �x�N�g���̃��f���̕\��
			constexpr const char* const kVectorModel = "Assets/modelData/preset/vector.tkm";

			constexpr float kVectorFontShadowOffset = 1.0f;	//!< �x�N�g���̃t�H���g�̃V���h�E�I�t�Z�b�g
			constexpr float kVectorFontScale = 0.4f;	//!< �x�N�g���̃t�H���g�̊g�嗦
			constexpr float kVectorModelLength = 100.0f;	//!< �x�N�g���̃��f���̒���
			constexpr float kVectorModelWidthAndHeightScale = 0.2f;	//!< �x�N�g���̃��f���̕��ƍ����{��


			//////// �e�L�X�g�p�l���`��N���X�̒萔 ////////

			constexpr int kTextPanelDatasNum = 64;		//!< �e�L�X�g�p�l���̃f�[�^�W�̐�

			constexpr int kTextSize = 128;	//!< �e�L�X�g�̃T�C�Y
			//!<�@�f�t�H���g�̃e�L�X�g�̍��W
			static const Vector2 kDefaultTextPosition = { 300.0f,350.0f };
			constexpr float kDefaultTextScale = 0.5f;	//!< �f�t�H���g�̃e�L�X�g�̊g�嗦
			constexpr float kDefaultTextShadowOffset = 1.0f;	//!< �f�t�H���g�̃e�L�X�g�̃V���h�E�I�t�Z�b�g
		}
	}
}
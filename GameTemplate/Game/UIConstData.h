#pragma once

namespace nsMyGame
{
	/**
	 * @brief UI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUI
	{
		/**
		 * @brief �Q�[�����C��UI�N���X�̒萔�f�[�^
		*/
		namespace nsGameMainUIConstData
		{
			//!< �^�C�}�[�̃t�H���g�̃e�L�X�g�̐擪
			constexpr const wchar_t* const kTimerFontTextHead = L"Time:";
			//!< �^�C�}�[�̃t�H���g�̃e�L�X�g�̖���
			constexpr const wchar_t* const kTimerFontTextEnd = L"%02d:%02d";
			//!< �^�C�}�[�̃t�H���g�̍��W
			static const Vector2 kTimerFontPosition = { -600.0f,300.0f };
			//!< �^�C�}�[�̃t�H���g�̃J���[
			static const Vector4 kTimerFontColor = { 1.0f,1.0f,1.0f,1.0f };
			//!< �^�C�}�[�̃t�H���g�̃V���h�E�I�t�Z�b�g
			constexpr float kTimerFontShadowOffset = 2.0f;
			//!< �^�C�}�[�̃t�H���g�̃V���h�E�J���[
			static const Vector4 kTimerFontShadowColor = { 0.0f,0.0f,0.0f,1.0f };

			//!< �S�[���̃X�v���C�g�t�@�C���p�X
			constexpr const char* const kGoalSpriteFilePath = "Assets/Image/sample.dds";
			constexpr int kGoalSpriteWidth = 256;	//!< �S�[���̃X�v���C�g�̕�
			constexpr int kGoalSpriteHeight = 256;	//!< �S�[���̃X�v���C�g�̍���
			//!< �S�[���̃X�v���C�g�̍��W
			static const Vector3 kGoalSpritePosition = { 0.0f,200.0f,0.0f };

		}
	}
}
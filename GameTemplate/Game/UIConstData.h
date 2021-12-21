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

			//!< �~�j�}�b�v�̔w�i�p�X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kMiniMapBackSpriteFilePath = "Assets/Image/miniMap/map_back.DDS";
			//!< �~�j�}�b�v�̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kMiniMapSpriteFilePath = "Assets/Image/miniMap/map.DDS";
			//!< �~�j�}�b�v��FX�t�@�C���p�X
			constexpr const char* const kMiniMapFxFilePath = "Assets/shader/miniMap.fx";
			// ���̉摜�T�C�Y * �傫���{��
			constexpr int kMiniMapSpriteWidth = static_cast<int>(720 * 0.8f);	//!< �~�j�}�b�v�̕�
			constexpr int kMiniMapSpriteHeight = static_cast<int>(720 * 0.8f);	//!< �~�j�}�b�v�̍���
			// ���̉摜�T�C�Y * �A�X�y�N�g�� * �傫���{��
			constexpr int kMiniMapDrawWidth = static_cast<int>(720 * 0.4f * 0.7f);	//!< �~�j�}�b�v�̕�
			constexpr int kMiniMapDrawHeight = static_cast<int>(720 * 0.3f * 0.7f);	//!< �~�j�}�b�v�̍���
			//!< �~�j�}�b�v�̃X�v���C�g�̍��W
			static const Vector3 kMiniMapSpritePosition = { 500.0f,-250.0f,0.0f };

			//!< �v���C���[�̃A�C�R���̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kPlayerIconSpriteFilePath = "Assets/Image/miniMap/playerArrow.DDS";
			constexpr int kPlayerIconSpriteWidth = static_cast<int>(64 * 0.5f);	//! �v���C���[�A�C�R���̕�
			constexpr int kPlayerIconSpriteHeight = static_cast<int>(64 * 0.5f);	//! �v���C���[�A�C�R���̍���
			// �v���C���[�̃A�C�R���̃X�v���C�g�̍��W
			static const Vector3 kPlayerIconSpritePosition = kMiniMapSpritePosition;
			
		}
	}
}
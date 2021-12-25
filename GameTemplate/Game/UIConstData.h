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

			constexpr float kStageWidth = 70000.0f;				//!< �X�e�[�W�̕�
			constexpr float kStageHeight = 70000.0f;			//!< �X�e�[�W�̍���
			constexpr float kPlayerCenterBuffWidth = 35000.0f;	//!< �v���C���[���Z���^�[�Ɋ񂹂邽�߂̕�
			constexpr float kPlayerCenterBuffHeight = 2800.0f;	//!< �v���C���[���Z���^�[�Ɋ񂹂邽�߂̍���

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
			static const Vector2 kMiniMapSpritePosition = { 500.0f,-250.0f};

			//!< �v���C���[�̃A�C�R���̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kPlayerIconSpriteFilePath = "Assets/Image/miniMap/playerArrow.DDS";
			constexpr int kPlayerIconSpriteWidth = static_cast<int>(64 * 0.5f);	//! �v���C���[�A�C�R���̕�
			constexpr int kPlayerIconSpriteHeight = static_cast<int>(64 * 0.5f);	//! �v���C���[�A�C�R���̍���
			// �v���C���[�̃A�C�R���̃X�v���C�g�̍��W
			static const Vector2 kPlayerIconSpritePosition = kMiniMapSpritePosition;

			//!<  �Ԃ̃A�C�R���̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kCarIconSpriteFilePath = "Assets/Image/miniMap/carIcon.DDS";
			constexpr int kCarIconSpriteWidth = static_cast<int>(512 * 0.1f);	//! �ԃA�C�R���̕�
			constexpr int kCarIconSpriteHeight = static_cast<int>(512 * 0.1f);	//! �ԃA�C�R���̍���

			//!<  �Ԃ̃~�j�A�C�R���̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kCarMiniIconSpriteFilePath = "Assets/Image/miniMap/carMiniIcon.DDS";
			constexpr int kCarMiniIconSpriteWidth = static_cast<int>(64 * 0.5f);	//! �ԃ~�j�A�C�R���̕�
			constexpr int kCarMiniIconSpriteHeight = static_cast<int>(64 * 0.5f);	//! �ԃ~�j�A�C�R���̍���

			//!<  �Ԃ̃~�j�A�C�R���̉�ʂ݂͂����p�̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const kCarMiniIconOutSpriteFilePath = "Assets/Image/miniMap/outOfView.DDS";
			constexpr int kCarMiniIconOutSpriteWidth = static_cast<int>(64 * 0.25f);	//! �ԃ~�j�A�C�R���̕�
			constexpr int kCarMiniIconOutSpriteHeight = static_cast<int>(64 * 0.25f);	//! �ԃ~�j�A�C�R���̍���
			constexpr float kCarMiniIconOutBuff = 10.0f;	//!< ���ʂ̎ԃA�C�R���Ƃ��炷��

			// �~�j�}�b�v��4���_
			static const Vector2 kMiniMapVert[4] =
			{
				// ����
				{
					kPlayerIconSpritePosition.x - kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y + kMiniMapDrawHeight * 0.5f
				},
				// �E��
				{
					kPlayerIconSpritePosition.x + kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y + kMiniMapDrawHeight * 0.5f
				},
				// ����
				{
					kPlayerIconSpritePosition.x - kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y - kMiniMapDrawHeight * 0.5f
				},
				// �E��
				{
					kPlayerIconSpritePosition.x + kMiniMapDrawWidth * 0.5f,
					kPlayerIconSpritePosition.y - kMiniMapDrawHeight * 0.5f
				},

			};

			// �~�j�}�b�v�̕ӂ̃x�N�g��
			static Vector2 kMiniMapEdge[4]
			{
				// ���ォ��E��B�n�_vert0�B
				{
					kMiniMapVert[1].x - kMiniMapVert[0].x,
					kMiniMapVert[1].y - kMiniMapVert[0].y
				},
				// �E�ォ��E���B�n�_vert1�B
				{
					kMiniMapVert[3].x - kMiniMapVert[1].x,
					kMiniMapVert[3].y - kMiniMapVert[1].y
				},
				// �������獶��B�n�_vert2�B
				{
					kMiniMapVert[0].x - kMiniMapVert[2].x,
					kMiniMapVert[0].y - kMiniMapVert[2].y
				},
				// �E�����獶���B�n�_vert3�B
				{
					kMiniMapVert[2].x - kMiniMapVert[3].x,
					kMiniMapVert[2].y - kMiniMapVert[3].y
				},
			};
			
		}
	}
}
#pragma once

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �X�v���C�g�֘A�̃l�[���X�y�[�X
		*/
		namespace nsSprite
		{
			/**
			 * @brief �X�v���C�g�̒萔�f�[�^
			*/
			namespace nsSpriteConstData
			{
				const Vector2 kDefaultPivot = { 0.5f,0.5f };							//!< �f�t�H���g�̃s�{�b�g
				constexpr const char* kDefaultFxFilePath = "Assets/shader/sprite.fx";	//!< �f�t�H���g�̃V�F�[�_�[�̃t�@�C���p�X
				constexpr const char* const kVsEntryPointFunc = "VSMain";				//1< ���_�V�F�[�_�[�̃G���g���[�|�C���g
				constexpr const char* const kPsEntryPointFunc = "PSMain";				//1< �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
			}
		}
	}
}
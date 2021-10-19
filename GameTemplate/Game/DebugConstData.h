#pragma once

namespace nsMyGame
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
			//!< �x�N�g���̃f�t�H���g�̖��O
			constexpr const char* const kDefaultVectorName = "vector";
		
			//!< �x�N�g���̃��f���̕\��
			constexpr const char* const kVectorModel = "Assets/modelData/preset/vector.tkm";

			constexpr float kVectorFontShadowOffset = 1.0f;	//!< �x�N�g���̃t�H���g�̃V���h�E�I�t�Z�b�g
			static const Vector4 kVectorFontColor = Vector4::Black;	//!< �x�N�g���̃t�H���g�̃J���[
			constexpr float kVectorFontScale = 0.4f;	//!< �x�N�g���̃t�H���g�̊g�嗦
			constexpr float kVectorModelLength = 100.0f;	//!< �x�N�g���̃��f���̒���
			constexpr float kVectorModelWidthAndHeightScale = 0.2f;	//!< �x�N�g���̃��f���̕��ƍ����{��
		}
	}
}
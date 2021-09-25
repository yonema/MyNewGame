#pragma once

namespace nsMyGame
{

	/**
	 * @brief ���̃Q�[���p�̃Q�[���G���W���l�[���X�y�[�X
	*/
	namespace nsMyEngine
	{
		/**
		 * @brief �����_�����O�G���W���̒萔�f�[�^
		*/
		namespace nsRenderingEngineConstData
		{
			constexpr int m_kRenderObjectsCapacityNum = 255;	//!< �`��I�u�W�F�N�g�̃L���p�V�e�B

			//!< �X�v���C�g��fx�t�@�C���p�X
			constexpr const char* const m_kSpriteFxFilePath = "Assets/shader/sprite.fx";
			//!< �f�B�t�@�\�h���C�e�B���O���s�����߂̃X�v���C�g��fx�t�@�C�X�p�X
			constexpr const char* const m_kDefferdLightingSpriteFxFilePath = "Assets/shader/DeferredLighting.fx";

			/**
			 * @brief GBuffer�̗񋓌^
			 * @warning RenderToGBuffer3DModel.fx��SPSOut�Ɠ������e�ɂ���B�inum�ȊO�j
			*/
			enum EnGBuffer
			{
				enGBufferAlbedoDepth,           // �A���x�h�Ɛ[�x�l�B���ɐ[�x�l���L������Ă��܂��B
				enGBufferNormal,                // �@��
				enGBufferMetaricShadowSmooth,   // ���^���b�N�A�e�p�����[�^�A�X���[�X�B
												// ���^���b�N��r�A�e�p�����[�^��g�A�X���[�X��a�Bg�͖��g�p�B
				enGBufferNum,                   // G-Buffer�̐�
			};

			/**
			 * @brief �f�B�t�@�[�h���C�e�B���O�p�̒萔�o�b�t�@�̍\����
			*/
			struct SDefferdLightingCB
			{
				Matrix mViewProjInv;	//!< �r���[�v���W�F�N�V�����s��̋t�s��
			};
		}
	}
}
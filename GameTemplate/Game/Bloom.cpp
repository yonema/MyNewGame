#include "stdafx.h"
#include "Bloom.h"

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �|�X�g�G�t�F�N�g�֘A�̃l�[���X�y�[�X
		*/
		namespace nsPostEffect
		{
			// �u���[���̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsBloomConstData;

			/**
			 * @brief �������֐�
			 * @param[in] mainRenderTarget ���C�������_�����O�^�[�Q�b�g
			*/
			void CBloom::OnInit(RenderTarget& mainRenderTarget)
			{
				// �����_�����O�^�[�Q�b�g��������
				InitRenderTarget(mainRenderTarget);

				// �X�v���C�g�ƃK�E�V�A���u���[��������
				InitSpriteAndGaussianBlur(mainRenderTarget);

				return;
			}

			/**
			 * @brief �`��֐�
			 * @param[in] rc �����_�����O�R���e�L�X�g
			 * @param[in] mainRenderTarget ���C�������_�����O�^�[�Q�b�g
			*/
			void CBloom::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
			{
				//////// 1.�P�x���o ////////
				// �P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
				rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
				// �����_�����O�^�[�Q�b�g��ݒ�
				rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
				// �����_�����O�^�[�Q�b�g���N���A
				rc.ClearRenderTargetView(m_luminanceRenderTarget);
				// �P�x���o���s��
				m_luminanceSprite.Draw(rc);
				// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
				rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

				//////// 2.�K�E�V�A���u���[��4����s���� ////////
				for (int i = 0; i < kGaussianBlurNum; i++)
				{
					m_gaussianBlur[i].ExecuteOnGPU(rc, kGaussianBulrPower);
				}

				//////// 3.4���̃{�P�摜���������ă��C�������_�����O�^�[�Q�b�g�ɉ��Z����////////
				// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
				rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
				// �����_�����O�^�[�Q�b�g��ݒ�
				rc.SetRenderTargetAndViewport(mainRenderTarget);
				// �ŏI����
				m_finalSprite.Draw(rc);
				// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
				rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

				return;
			}

			/**
			 * @brief �����_�����O�^�[�Q�b�g��������
			 * @param[in] mainRenderTarget ���C�������_�����O�^�[�Q�b�g
			*/
			void CBloom::InitRenderTarget(const RenderTarget& mainRenderTarget)
			{
				m_luminanceRenderTarget.Create(
					mainRenderTarget.GetWidth(),   // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
					mainRenderTarget.GetHeight(),  // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
					1,
					1,
					mainRenderTarget.GetColorBufferFormat(),
					DXGI_FORMAT_UNKNOWN
				);

				return;
			}

			/**
			 * @brief �X�v���C�g�ƃK�E�V�A���u���[�̏�����
			 * @param[in] mainRenderTarget ���C�������_�����O�^�[�Q�b�g
			*/
			void CBloom::InitSpriteAndGaussianBlur(RenderTarget& mainRenderTarget)
			{
				//////// 1.�P�x���o�p�̃X�v���C�g�������� ////////
				{
					// �X�v���C�g�̏������f�[�^
					SpriteInitData spriteInitData;
					// �u���[���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
					spriteInitData.m_fxFilePath = kfxFilePath;
					// �P�x���o�p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
					spriteInitData.m_psEntryPoinFunc = kPsEntryFunc_samplingLuminance;
					// �X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ���
					spriteInitData.m_width = mainRenderTarget.GetWidth();
					spriteInitData.m_height = mainRenderTarget.GetHeight();
					// �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[
					spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
					// �`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
					spriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

					// �P�x���o�p�̃X�v���C�g��������
					m_luminanceSprite.Init(spriteInitData);
				}

				//////// 2.�K�E�V�A���u���[�������� ////////
				// gaussianBlur[0]�͋P�x�e�N�X�`���ɃK�E�V�A���u���[��������
				m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
				// gaussianBlur[1]��gaussianBlur[0]�̃e�N�X�`���ɃK�E�V�A���u���[��������
				m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
				// gaussianBlur[2]��gaussianBlur[1]�̃e�N�X�`���ɃK�E�V�A���u���[��������
				m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
				// gaussianBlur[3]��gaussianBlur[2]�̃e�N�X�`���ɃK�E�V�A���u���[��������
				m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

				//////// 3.�ŏI�����p�̃X�v���C�g������������ ////////
				{
					// �X�v���C�g�̏������f�[�^
					SpriteInitData spriteInitData;
					// �u���[���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
					spriteInitData.m_fxFilePath = kfxFilePath;
					// �ŏI�����p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
					spriteInitData.m_psEntryPoinFunc = kPsEntryFunc_bloomFinal;
					// �{�P�e�N�X�`����4���w�肷��
					for (int i = 0; i < kGaussianBlurNum; i++)
					{
						spriteInitData.m_textures[i] = &m_gaussianBlur[i].GetBokeTexture();
					}
					// �𑜓x��mainRenderTarget�̕��ƍ���
					spriteInitData.m_width = mainRenderTarget.GetWidth();
					spriteInitData.m_height = mainRenderTarget.GetHeight();
					// �`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
					spriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();
					// �������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z�ɂ���
					spriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;

					// �ŏI�����p�X�v���C�g�̏�����
					m_finalSprite.Init(spriteInitData);
				}

				return;
			}
		}
	}
}
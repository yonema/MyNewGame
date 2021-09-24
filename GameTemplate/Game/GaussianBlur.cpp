#include "stdafx.h"
#include "GaussianBlur.h"

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
			// �K�E�V�A���u���[�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsGaussianBlurConstData;

			/**
			 * @brief �������֐�
			 * @param[in,out] originalTexture �K�E�V�A���u���[��������I���W�i���e�N�X�`��
			 * @param[in] isBlurAlpha �A���t�@�l�`�����l���ɂ��u���[�������邩�H
			*/
			void CGaussianBlur::Init(Texture* originalTexture, const bool isBlurAlpha)
			{
				m_originalTexture = originalTexture;

				//�����_�����O�^�[�Q�b�g���������B
				InitRenderTargets();
				//�X�v���C�g���������B
				InitSprites(isBlurAlpha);

				return;
			}

			/**
			 * @brief �K�E�V�A���u���[��GPU��Ŏ��s
			 * @warning �{�֐��̌Ăяo���́ADirectX12�𗘗p�����`��R�}���h�������ɌĂяo���K�v������܂��B
			 * @param[in] rc �����_�����O�R���e�L�X�g
			 * @param[in] blurPower �u���[�̋����B�l���傫���قǃ{�P�������Ȃ�B
			*/
			void CGaussianBlur::ExecuteOnGPU(RenderContext& rc, const float blurPower)
			{
				//////// 1.�d�݃e�[�u�����X�V����B ////////
				UpdateWeightsTable(blurPower);

				////// 2.���u���[�����s�B ////////
				// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���悤�ɂȂ�܂�wait������B
				rc.WaitUntilToPossibleSetRenderTarget(m_xBlurRenderTarget);
				// �����_�����O�^�[�Q�b�g��ݒ�B
				rc.SetRenderTargetAndViewport(m_xBlurRenderTarget);

				// �h���[�B
				m_xBlurSprite.Draw(rc);
				//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
				rc.WaitUntilFinishDrawingToRenderTarget(m_xBlurRenderTarget);

				//////// 3.�c�u���[�����s�B ////////
				// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���悤�ɂȂ�܂�wait������B
				rc.WaitUntilToPossibleSetRenderTarget(m_yBlurRenderTarget);
				// �����_�����O�^�[�Q�b�g��ݒ�B
				rc.SetRenderTargetAndViewport(m_yBlurRenderTarget);

				// �h���[�B
				m_yBlurSprite.Draw(rc);
				// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
				rc.WaitUntilFinishDrawingToRenderTarget(m_yBlurRenderTarget);

				return;
			}

			/**
			 * @brief �����_�����O�^�[�Q�b�g��������
			*/
			void CGaussianBlur::InitRenderTargets()
			{
				int w = m_originalTexture->GetWidth();
				int h = m_originalTexture->GetHeight();
				//X�u���[�p�̃����_�����O�^�[�Q�b�g���쐬����B
				m_xBlurRenderTarget.Create(
					w / 2,	// x�������Ƀ_�E���T���v�����O
					h,
					1,
					1,
					m_originalTexture->GetFormat(),
					DXGI_FORMAT_UNKNOWN
				);

				//Y�u���[�p�̃����_�����O�^�[�Q�b�g���쐬����B
				m_yBlurRenderTarget.Create(
					w / 2,	// x��������
					h / 2,	// y�����������Ƀ_�E���T���v�����O
					1,
					1,
					m_originalTexture->GetFormat(),
					DXGI_FORMAT_UNKNOWN
				);

				return;
			}

			/**
			 * @brief �X�v���C�g��������
			 * @param isBlurAlpha �A���t�@�`�����l���ɂ��u���[�������邩�H
			*/
			void CGaussianBlur::InitSprites(const bool isBlurAlpha)
			{
				// �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
				const char* psEntryFunc = nullptr;
				// �A���t�@�`�����l���ɂ��u���[�������邩�H
				if (isBlurAlpha)
				{
					psEntryFunc = kPsEntryFunc_alpha;	// ������
				}
				else 
				{
					psEntryFunc = kPsEntryFunc;			// �����Ȃ�
				}

				//���u���[�p�̃X�v���C�g������������B
				{
					// �X�v���C�g�̏������f�[�^
					SpriteInitData xBlurSpriteInitData;
					xBlurSpriteInitData.m_fxFilePath = kfxFilePath;
					xBlurSpriteInitData.m_vsEntryPointFunc = kVsEntryFunc_xBlur;
					xBlurSpriteInitData.m_psEntryPoinFunc = psEntryFunc;
					//�X�v���C�g�̉𑜓x��m_xBlurRenderTarget�Ɠ����B
					xBlurSpriteInitData.m_width = m_xBlurRenderTarget.GetWidth();
					xBlurSpriteInitData.m_height = m_xBlurRenderTarget.GetHeight();
					//�e�N�X�`���͋P�x�e�N�X�`���B
					xBlurSpriteInitData.m_textures[0] = m_originalTexture;
					//�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��B
					xBlurSpriteInitData.m_colorBufferFormat[0] = m_xBlurRenderTarget.GetColorBufferFormat();
					//���[�U�[�g���̒萔�o�b�t�@�Ƀu���[�p�̃p�����[�^�[��ݒ肷��B
					xBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
					xBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

					//�������������Ƃɉ��u���[�p�̃X�v���C�g������������B
					m_xBlurSprite.Init(xBlurSpriteInitData);
				}
				//�c�u���[�p�̃X�v���C�g������������B
				{
					// �X�v���C�g�̏������f�[�^
					SpriteInitData yBlurSpriteInitData;
					yBlurSpriteInitData.m_fxFilePath = kfxFilePath;
					yBlurSpriteInitData.m_vsEntryPointFunc = kVsEntryFunc_yBlur;
					yBlurSpriteInitData.m_psEntryPoinFunc = psEntryFunc;
					//�X�v���C�g�̉𑜓x��m_yBlurRenderTarget�Ɠ����B
					yBlurSpriteInitData.m_width = m_yBlurRenderTarget.GetWidth();
					yBlurSpriteInitData.m_height = m_yBlurRenderTarget.GetHeight();
					//�e�N�X�`���͉��u���[�����������́B
					yBlurSpriteInitData.m_textures[0] = &m_xBlurRenderTarget.GetRenderTargetTexture();
					//�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��B
					yBlurSpriteInitData.m_colorBufferFormat[0] = m_yBlurRenderTarget.GetColorBufferFormat();
					//���[�U�[�g���̒萔�o�b�t�@�Ƀu���[�p�̃p�����[�^�[��ݒ肷��B
					yBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
					yBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

					//�������������Ƃɏc�u���[�p�̃X�v���C�g������������B
					m_yBlurSprite.Init(yBlurSpriteInitData);
				}

				return;
			}

			/**
			 * @brief �d�݃e�[�u�����X�V����
			 * @param blurPower �u���[�̋����B�l���傫���قǃ{�P�������Ȃ�B
			*/
			void CGaussianBlur::UpdateWeightsTable(const float blurPower)
			{
				float total = 0;
				for (int i = 0; i < kWeightsNum; i++) {
					m_weights[i] = expf(-0.5f * (float)(i * i) / blurPower);
					total += 2.0f * m_weights[i];

				}
				// �K�i��
				for (int i = 0; i < kWeightsNum; i++) {
					m_weights[i] /= total;
				}

				return;
			}
		}
	}
}
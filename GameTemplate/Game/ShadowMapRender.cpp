#include "stdafx.h"
#include "ShadowMapRender.h"
#include "Render.h"
#include "LightManager.h"

namespace nsMyGame
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		// �O���錾
		class CRender;

		/**
		 * @brief �V���h�E�֘A�̃l�[���X�y�[�X
		*/
		namespace nsShadow
		{
			using namespace nsShadowConstData;
			/**
			 * @brief �������֐�
			*/
			void CShadowMapRender::Init()
			{
				// �J�X�P�[�h�V���h�E�̃G���A���̏�����
				for (int i = 0; i < enShadowMapArea_num; i++)
				{
					m_cascadeAreaRateArray[i] = kDefaultCascadeAreaRateArray[i];
				}

				// �J���[�t�H�[�}�b�g
				DXGI_FORMAT colorFormat = DXGI_FORMAT_R32G32_FLOAT;
				// �f�v�X�X�e���V���t�H�[�}�b�g
				DXGI_FORMAT depthFormat = DXGI_FORMAT_D32_FLOAT;
				// �N���A�J���[
				float clearColor[4];
				for (int i = 0; i < 4; i++)
				{
					clearColor[i] = kDefaultClearColor[i];
				}

				// �ߌi�p�̃V���h�E�}�b�v�̐���
				m_shadowMaps[enShadowMapArea_near].Create(
					2048,
					2048,
					1,
					1,
					colorFormat,
					depthFormat,
					clearColor
				);
				// ���i�p�̃V���h�E�}�b�v�̐���
				m_shadowMaps[enShadowMapArea_middle].Create(
					1024,
					1024,
					1,
					1,
					colorFormat,
					depthFormat,
					clearColor
				);
				// ���i�p�̃V���h�E�}�b�v�̐���
				m_shadowMaps[enShadowMapArea_far].Create(
					512,
					512,
					1,
					1,
					colorFormat,
					depthFormat,
					clearColor
				);

				// �K�E�V�A���u���[�̏�����
				m_blur[0].Init(&m_shadowMaps[enShadowMapArea_near].GetRenderTargetTexture());
				m_blur[1].Init(&m_shadowMaps[enShadowMapArea_middle].GetRenderTargetTexture());
				m_blur[2].Init(&m_shadowMaps[enShadowMapArea_far].GetRenderTargetTexture());

				return;
			}


			/**
			 * @brief �`��֐�
			 * @param[in] rc �����_�����O�R���e�L�X�g
			 * @param[in] ligNo ���C�g�̔ԍ�
			 * @param[in] lightDirection ���C�g�̕���
			 * @param[in] renderObjects �`�悷��I�u�W�F�N�g
			*/
			void CShadowMapRender::Render(
				RenderContext& rc,
				const int ligNo,
				const Vector3& lightDirection,
				std::vector<CRender*>& renderObjects
			)
			{

				// ���C�g�̍ő�̍����������_���[��AABB����v�Z����B
				m_cascadeShadowMapMatrix.CalcLightViewProjectionCropMatrix(
					lightDirection,
					m_cascadeAreaRateArray
				);

				// �V���h�E�}�b�v�̔ԍ�
				int shadowMapNo = 0;
				// ���ꂼ��̃V���h�E�}�b�v�ɃA�N�Z�X
				for (auto& shadowMap : m_shadowMaps)
				{
					// �V���h�E�}�b�v���g�p�\�ɂ���
					rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
					rc.SetRenderTargetAndViewport(shadowMap);
					rc.ClearRenderTargetView(shadowMap);

					// �`�悷��I�u�W�F�N�g�����o���ĕ`�悷��
					for (auto& renderer : renderObjects)
					{
						renderer->OnRenderShadowMap(
							rc,
							ligNo,
							shadowMapNo,
							m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(shadowMapNo)
						);
					}

					// �������݊����҂�
					rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
					shadowMapNo++;
				}

				// �u���[�����s����B
				for (auto& blur : m_blur) {
					blur.ExecuteOnGPU(rc, kBlurPower);
				}


				return;
			}


		}
	}
}
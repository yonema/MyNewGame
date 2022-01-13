#include "stdafx.h"
#include "PlayerShadowMap.h"
#include "ShadowConstDatah.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �V���h�E�֘A�̃l�[���X�y�[�X
		*/
		namespace nsShadow
		{
			using namespace nsShadowConstData;

			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerShadowMap::CPlayerShadowMap()
			{

				// ����������
				Init();

				return;
			}

			/**
			 * @brief ����������
			*/
			void CPlayerShadowMap::Init()
			{
				// �V���h�E�}�b�v�̃N���A�J���[
				float clearColor[4];
				for (int i = 0; i < 4; i++)
				{
					clearColor[i] = kDefaultClearColor[i];
				}

				// �V���h�E�}�b�v�̍쐻
				m_shadowMap.Create
				(
					kShadowMapSizes[0],
					kShadowMapSizes[0],
					1,
					1,
					DXGI_FORMAT_R32G32_FLOAT,
					DXGI_FORMAT_D32_FLOAT,
					clearColor
				);

				// �K�E�V�A���u���[�̏�����
				m_blur.Init(&m_shadowMap.GetRenderTargetTexture());

				return;
			}

			/**
			 * @brief �`��֐�
			 * @param[in] rc �����_�[�R���e�L�X�g
			 * @param[in] ligNo ���C�g�̔ԍ�
			 * @param[in] lightDirection ���C�g�̕���
			*/
			void CPlayerShadowMap::Render(RenderContext& rc,const int ligNo, const Vector3& lightDirection)
			{
				if (m_playerRender == nullptr)
				{
					// �v���C���[�����_���[���Z�b�g����Ă��Ȃ�������A�������^�[���B
					return;
				}

				// ���C�g�r���[�v���W�F�N�V�����s����v�Z
				CalcLightViewProjectionMatrix(&m_lvpMatrix, lightDirection);

				// �V���h�E�}�b�v���g�p�\�ɂ���
				rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
				rc.SetRenderTargetAndViewport(m_shadowMap);
				rc.ClearRenderTargetView(m_shadowMap);

				// �v���C���[��`��
				m_playerRender->OnRenderPlayerShadowMap(
					rc, ligNo, m_lvpMatrix
				);

				// �������݊����҂�
				rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

				// �u���[�����s����
				m_blur.ExecuteOnGPU(rc, kBlurPower);


				return;
			}

			/**
			 * @brief ���C�g�r���[�v���W�F�N�V�����s����v�Z����
			 * @param[out] lvpMatrixOut ���C�g�r���[�v���W�F�N�V�����̏o��
			 * @param[in] lightDirection ���C�g�̕���
			*/
			void CPlayerShadowMap::CalcLightViewProjectionMatrix(Matrix* lvpMatrixOut, const Vector3& lightDirection)
			{
				// �r���[�s����v�Z����
				Matrix viewMatrix;
				// ���C�g�̃^�[�Q�b�g
				Vector3 lightTarget = g_camera3D->GetTarget();
				// ���C�g�̃|�W�V����
				Vector3 lightPos = lightTarget;
				// ���C�g�^�[�Q�b�g���烉�C�g�̕����ɉ������������Ƃ�
				Vector3 targetToLightPos = lightDirection;
				targetToLightPos.Scale(-100.0f);
				lightPos += targetToLightPos;
				//if (lightDirection.y != 0.0f)
				//	lightPos += (lightDirection) * (kLightMaxHeight / lightDirection.y);
				//else
				//	lightPos += lightDirection * kLightMaxHeight;
				// �������ݒ�
				if (fabsf(lightDirection.y) > 0.9999f) {
					// �قڐ^��A�^���������Ă���
					// X�A�b�v
					viewMatrix.MakeLookAt(lightPos, lightTarget, Vector3::AxisX);
				}
				else
				{
					// Y�A�b�v
					viewMatrix.MakeLookAt(lightPos, lightTarget, Vector3::AxisY);
				}

				// �v���W�F�N�V�����s����v�Z����
				Matrix projMatrix;
				// ���s���e�s��Ōv�Z����
				projMatrix.MakeOrthoProjectionMatrix(
					500.0f,
					//5000.0f,
					500.0f,
					//5000.0f,
					kLightMinFar,
					1000.0f
				);

				// ���C�g�r���[�v���W�F�N�V�����s����v�Z����
				*lvpMatrixOut = viewMatrix * projMatrix;


				return;
			}


		}
	}
}
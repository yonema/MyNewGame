#include "stdafx.h"
#include "CascadeShadowMapMatrix.h"


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
			// �V���h�E�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nsShadowConstData;

			/**
			 * @brief ���C�g�r���[�v���W�F�N�V�����N���b�v�s����v�Z����
			 * @param[in] lightDirection ���C�g�̕���
			 * @param[in] cascadeAreaRateTbl �J�X�P�[�h�V���h�E�̃G���A���e�[�u��
			 * @param[in] sceneMaxPosition �Q�[���V�[���̍ő���W
			 * @param[in] sceneMinPosition �Q�[���V�[���̍ŏ����W
			*/
			void CCascadeShadowMapMatrix::CalcLightViewProjectionCropMatrix(
				const Vector3& lightDirection,
				const float cascadeAreaRateTbl[enShadowMapArea_num],
				const Vector3& sceneMaxPosition,
				const Vector3& sceneMinPosition
			)
			{
				// ���C�g�r���[�v���W�F�N�V����
				Matrix lvpMatrix;
				// ���C�g�r���[�v���W�F�N�V�������v�Z����
				CalcAndGetLightViewProjectionMatrix(&lvpMatrix,
					lightDirection,
					sceneMaxPosition,
					sceneMinPosition
				);

				// �J�����̑O����
				const Vector3& cameraForward = g_camera3D->GetForward();
				// �J�����̉E����
				const Vector3& cameraRight = g_camera3D->GetRight();
				// �J�����̏����
				const Vector3 cameraUp = Cross(cameraForward, cameraRight);

				// nearDepth�̓G���A�̍ŏ��[�x�l��\��
				// ��ԋ߂��G���A�̍ŏ��[�x�l�̓J�����̃j�A�N���b�v
				float nearDepth = g_camera3D->GetNear();

				// �J�X�P�[�h�V���h�E�̍ő�[�x
				float maxDepth = kCascadeMaxDepth;

				// �����G���A�̍ő�[�x�l���`����
				float cascadeAreaTbl[enShadowMapArea_num] = {
					maxDepth* cascadeAreaRateTbl[enShadowMapArea_near],     // �߉e���f���ő�[�x�l
					maxDepth* cascadeAreaRateTbl[enShadowMapArea_middle],   // ���e���f���ő�[�x�l
					maxDepth* cascadeAreaRateTbl[enShadowMapArea_far] ,     // ���e���f���ő�[�x�l�B
				};

				// ���ꂼ��̃V���h�E�}�b�v�̃G���A���v�Z����
				for (int areaNo = 0; areaNo < enShadowMapArea_num; areaNo++)
				{
					//////// 1.�G���A�����鎋����̂W���_�����߂� ////////

					// 8���_�̍��W
					Vector3 vertex[8];

					// �G���A��8���_�̍��W���v�Z���Ă��瓾��
					CalcAndGetAreaVertex(
						vertex,
						nearDepth,
						cascadeAreaTbl[areaNo],
						cameraForward,
						cameraRight,
						cameraUp
					);

					//////// 2.���߂�8���_��LVP��Ԃɕϊ����āA�ő�l�A�ŏ��l�����߂� ////////

					// �ő�l�ƍŏ��l
					Vector3 vMax, vMin;
					vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };// �ő�l�̏����l�͈�ԏ��������l������
					vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };	// �ŏ��l�̏����l�͈�ԑ傫�����l������
					// �S�Ă̒��_�ɃA�N�Z�X
					for (auto& v : vertex)
					{
						lvpMatrix.Apply(v);	// LVP��Ԃɕϊ�
						vMax.Max(v);		// �ő�l��ݒ�
						vMin.Min(v);		// �ŏ��l��ݒ�
					}

					//////// 3.�N���b�v�s������߂� ////////

					// �N���b�v�s��
					Matrix clopMatrix;
					// �N���b�v�s����v�Z���Ă��瓾��
					CalcAndGetClopMatrix(&clopMatrix, vMax, vMin);

					//////// 4.LVP�s��ɃN���b�v�s�����Z���čŏI�I�ȍs������߂� ////////

					// ���C�g�r���[�v���W�F�N�V�����s��ɃN���b�v�s�����Z����
					m_lvpcMatrix[areaNo] = lvpMatrix * clopMatrix;

					// ���̃G���A�̋ߕ��ʂ܂ł̋�����������
					nearDepth = cascadeAreaTbl[areaNo];
				}


				return;
			}

			/**
			 * @brief ���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă��瓾��
			 * @param[out] lvpMatrixOut ���C�g�r���[�v���W�F�N�V�����s��
			 * @param[in] lightDirection ���C�g�̕���
			 * @param[in] sceneMaxPosition �Q�[���V�[���̍ő���W
			 * @param[in] sceneMinPosition �Q�[���V�[���̍ŏ����W
			*/
			void CCascadeShadowMapMatrix::CalcAndGetLightViewProjectionMatrix(
				Matrix* lvpMatrixOut,
				const Vector3& lightDirection,
				const Vector3& sceneMaxPosition,
				const Vector3& sceneMinPosition
			)
			{
				// �r���[�s����v�Z����
				Matrix viewMatrix;
				// ���C�g�̃^�[�Q�b�g
				//const Vector3 lightTarget = kLightTargetPos;
				//Vector3 lightTarget = g_camera3D->GetTarget();
				Vector3 lightTarget = (sceneMaxPosition + sceneMinPosition) * 0.5f;
				//// �����͌Œ�
				lightTarget.y = 0.0f;
				// ���C�g�̃|�W�V����
				Vector3 lightPos = lightTarget;
				// ���C�g�^�[�Q�b�g���烉�C�g�̕����ɉ������������Ƃ�
				Vector3 targetToLightPos = lightDirection;
				targetToLightPos.Scale(-kLightMaxHeight);
				lightPos += targetToLightPos;

				//lightPos += (lightDirection) * (kLightMaxHeight / lightDirection.y);
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
					//1280,
					5000.0f,
					//720,
					5000.0f,
					kLightMinFar,
					kLightMaxFar
				);

				// ���C�g�r���[�v���W�F�N�V�����s����v�Z����
				*lvpMatrixOut = viewMatrix * projMatrix;

				return;
			}


			/**
			 * @brief �G���A��8���_�̍��W���v�Z���Ă��瓾��
			 * @param[out] vertexOut ���_�̍��W������z��
			 * @param[in] nearDepth �G���A�̍ŏ��[�x�l
			 * @param[in] farDepth �G���A�̍ő�[�x�l
			 * @param[in] cameraForward �J�����̑O����
			 * @param[in] cameraRight �J�����̉E����
			 * @param[in] cameraUp �J�����̏����
			*/
			void CCascadeShadowMapMatrix::CalcAndGetAreaVertex(
				Vector3 vertexOut[8],
				const float nearDepth,
				const float farDepth,
				const Vector3& cameraForward,
				const Vector3& cameraRight,
				const Vector3& cameraUp
			)
			{
				// �G���A�̋ߕ��ʁA�����ʂ̏㉺���E�̖ʂւ̋��������ꂼ�ꋁ�߂�

				// �G���A�̋ߕ��ʂ̒��S����̏�ʁA���ʂ܂ł̋��������߂�
				float nearY = tanf(g_camera3D->GetViewAngle() * 0.5f) * nearDepth;
				// �G���A�̋ߕ��ʂ̒��S����̉E�ʁA���ʂ܂ł̋��������߂�
				float nearX = nearY * g_camera3D->GetAspect();
				// �G���A�̉����ʂ̒��S����̏�ʁA���ʂ܂ł̋��������߂�
				float farY = tanf(g_camera3D->GetViewAngle() * 0.5f) * farDepth;
				// �G���A�̉����ʂ̒��S����̉E�ʁA���ʂ܂ł̋��������߂�
				float farX = farY * g_camera3D->GetAspect();
				// �G���A�̋ߕ��ʂ̒��S���W�����߂�
				Vector3 nearPos = g_camera3D->GetPosition() + cameraForward * nearDepth;
				// �G���A�̉����ʂ̒��S���W�����߂�
				Vector3 farPos = g_camera3D->GetPosition() + cameraForward * farDepth;

				// 8���_�̍��W�����߂�

				// �ߕ��ʂ̉E��̒��_
				vertexOut[0] += nearPos + cameraUp * nearY + cameraRight * nearX;
				// �ߕ��ʂ̍���̒��_
				vertexOut[1] += nearPos + cameraUp * nearY + cameraRight * -nearX;
				// �ߕ��ʂ̉E���̒��_
				vertexOut[2] += nearPos + cameraUp * -nearY + cameraRight * nearX;
				// �ߕ��ʂ̍����̒��_
				vertexOut[3] += nearPos + cameraUp * -nearY + cameraRight * -nearX;
				// �����ʂ̉E��̒��_
				vertexOut[4] += farPos + cameraUp * farY + cameraRight * farX;
				// �����ʂ̍���̒��_
				vertexOut[5] += farPos + cameraUp * farY + cameraRight * -farX;
				// �����ʂ̉E���̒��_
				vertexOut[6] += farPos + cameraUp * -farY + cameraRight * farX;
				// �����ʂ̍����̒��_
				vertexOut[7] += farPos + cameraUp * -farY + cameraRight * -farX;

				return;
			}

			/**
			 * @brief �N���b�v�s����v�Z���Ă��瓾��
			 * @param[out] clopMatrixOut �N���b�v�s��
			 * @param[in] vMax �ő�l
			 * @param[in] vMin �ŏ��l
			*/
			void CCascadeShadowMapMatrix::CalcAndGetClopMatrix(
				Matrix* clopMatrixOut,
				const Vector3& vMax,
				const Vector3& vMin
			)
			{
				// ���K���X�N���[�����W�n�Ƀs�b�^�����܂�g�嗦�ƕ��s�ړ��ʂ����߂�

				// �g�嗦�����߂�
				float xScale = kNormalScreenWidth / (vMax.x - vMin.x);
				float yScale = kNormalScreenHeight / (vMax.y - vMin.y);

				// ���s�ړ��ʂ����߂�
				// ���S���W�̕��������Ό����ɕ��s�ړ�������
				float xOffset = (vMax.x + vMin.x) * -0.5f * xScale;
				float yOffset = (vMax.y + vMin.y) * -0.5f * yScale;

				// �N���b�v�s������߂�
				clopMatrixOut->m[0][0] = xScale;
				clopMatrixOut->m[1][1] = yScale;
				clopMatrixOut->m[3][0] = xOffset;
				clopMatrixOut->m[3][1] = yOffset;

				return;
			}

		}
	}
}
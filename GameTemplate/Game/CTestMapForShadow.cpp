#include "stdafx.h"
#include "CTestMapForShadow.h"
#include "ModelRender.h"

namespace nsMyGame
{
	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		/**
		 * @brief �e�X�g�}�b�v�i���x���j�p�̃l�[���X�y�[�X
		*/
		namespace nsTestMaps
		{
			/**
			 * @brief �X�^�[�g�֐�
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool CTestMapForShadow::Start()
			{
				// �G�C���A�X�錾
				using CModelRender = nsGraphic::nsModel::CModelRender;

				// ���ʃf�[�^���g�p�\�ɂ���
				using namespace nsCommonData;

				Quaternion qRot;
				qRot.SetRotationDegY(180.0f);

				m_planeModel = NewGO<CModelRender>(enPriorityFirst);
				m_planeModel->Init("Assets/modelData/debugModel/testBox.tkm");
				m_planeModel->SetScale({ 10.0f,0.1f,10.0f });
				m_planeModel->SetRotatioin(qRot);
				m_planeModel->SetIsShadowCaster(true);

				int posNum = 0;
				for (auto& unityChanModel : m_unityChanModels)
				{
					unityChanModel = NewGO<CModelRender>(enPriorityFirst);
					unityChanModel->Init("Assets/modelData/unityChan.tkm");
					unityChanModel->SetPosition({ 50.0f,0.0f,50.0f * static_cast<float>(posNum++)});
					unityChanModel->SetRotatioin(qRot);
					unityChanModel->SetIsShadowCaster(true);
				}

				posNum = 0;
				for (auto& blockModel : m_blockModels)
				{
					blockModel = NewGO<CModelRender>(enPriorityFirst);
					blockModel->Init("Assets/modelData/debugModel/testBox.tkm");
					blockModel->SetPosition({ -50.0f,0.0f,50.0f * static_cast<float>(posNum++) });
					blockModel->SetRotatioin(qRot);
					blockModel->SetIsShadowCaster(true);
				}


				return true;
			}

			/**
			 * @brief �j�������Ƃ��ɌĂ΂��֐�
			*/
			void CTestMapForShadow::OnDestroy()
			{
				DeleteGO(m_planeModel);
				for (auto& unityChanModel : m_unityChanModels)
				{
					DeleteGO(unityChanModel);
				}

				for (auto& blockModel : m_blockModels)
				{
					DeleteGO(blockModel);
				}

				return;
			}

			/**
			 * @brief �A�b�v�f�[�g�֐�
			*/
			void CTestMapForShadow::Update()
			{
				Vector3 moveVec = Vector3::Zero;
				float moveSpeed = 10.0f;

				Vector3 cameraVec = { 0.0f,0.0f,-1.0f };
				const float rotationSpeed = 2.0f;


				if (g_pad[0]->GetLStickXF() >= 0.5f)
				{
					m_cameraVecAngle -= rotationSpeed;
				}
				if (g_pad[0]->GetLStickXF() <= -0.5f)
				{
					m_cameraVecAngle += rotationSpeed;
				}
				if (g_pad[0]->GetLStickYF() >= 0.5f)
				{
					moveVec.y += moveSpeed;
				}
				if (g_pad[0]->GetLStickYF() <= -0.5f)
				{
					moveVec.y -= moveSpeed;
				}
				if (g_pad[0]->GetRStickYF() >= 0.5f)
				{
					m_cameraVecLength -= moveSpeed;
				}
				if (g_pad[0]->GetRStickYF() <= -0.5f)
				{
					m_cameraVecLength += moveSpeed;
				}

				if (g_pad[0]->IsTrigger(enButtonA))
				{
					m_shadowReciever = !m_shadowReciever;
					m_planeModel->SetIsShadowReciever(m_shadowReciever);
				}

				min(m_cameraVecAngle, 360.0f);
				max(m_cameraVecAngle, 0.0f);
				cameraVec.Scale(m_cameraVecLength);
				Quaternion qRot;
				qRot.SetRotationDegY(m_cameraVecAngle);
				qRot.Apply(cameraVec);
				cameraVec.y += g_camera3D->GetPosition().y + moveVec.y;

				g_camera3D->SetPosition(cameraVec);

				return;
			}
		}
	}
}
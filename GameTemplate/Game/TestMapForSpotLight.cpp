#include "stdafx.h"
#include "TestMapForSpotLight.h"
#include "ModelRender.h"
#include "SpotLight.h"

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
			bool CTestMapForSpotLight::Start()
			{
				// �v���C���[���f���̏�����
				m_playerModel = NewGO <nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
				m_playerModel->Init("Assets/modelData/unityChan.tkm");
				Quaternion qRot;
				qRot.SetRotationDegY(180.0f);
				m_playerModel->SetRotatioin(qRot);

				// �o�b�N�O���E���h���f���̏�����
				m_bgModel = NewGO <nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
				m_bgModel->Init("Assets/modelData/bg/bg.tkm");
				m_bgModel->SetRotatioin(qRot);

				// �X�|�b�g���C�g���f���̏�����
				m_spotLightModel = NewGO <nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
				m_spotLightModel->Init("Assets/modelData/debugModel/light.tkm");
				m_spotLightModel->SetPosition({ 100.0f,100.0f,0.0f });

				// �X�|�b�g���C�g�̏�����
				m_spotLight = NewGO<nsLight::CSpotLight>(nsCommonData::enPriorityFirst);
				m_spotLight->SetPosition(m_spotLightModel->GetPosition());
				Vector3 ligEmmisionDir = m_playerModel->GetPosition() - m_spotLight->GetPosition();
				ligEmmisionDir.Normalize();
				m_spotLight->SetDirection(ligEmmisionDir);
				m_spotLight->SetColor({ 20.0f,0.0f,0.0f,1.0f });
				m_spotLight->SetRange(500.0f);

				return true;
			}

			/**
			 * @brief �j���������ɌĂ΂��
			*/
			void CTestMapForSpotLight::OnDestroy()
			{
				DeleteGO(m_playerModel);
				DeleteGO(m_bgModel);
				DeleteGO(m_spotLightModel);
				DeleteGO(m_spotLight);

				return;
			}

			/**
			 * @brief �A�b�v�f�[�g�֐�
			*/
			void CTestMapForSpotLight::Update()
			{
				Vector3 moveVec = Vector3::Zero;
				const float moveSpeed = 1.0f;

				if (g_pad[0]->GetLStickXF() >= 0.5f)
				{
					moveVec.x += moveSpeed;
				}
				else if (g_pad[0]->GetLStickXF() <= -0.5f)
				{
					moveVec.x -= moveSpeed;
				}
				if (g_pad[0]->GetLStickYF() >= 0.5f)
				{
					moveVec.z += moveSpeed;
				}
				else if (g_pad[0]->GetLStickYF() <= -0.5f)
				{
					moveVec.z -= moveSpeed;
				}
				if (g_pad[0]->GetRStickYF() >= 0.5f)
				{
					moveVec.y += moveSpeed;
				}
				else if (g_pad[0]->GetRStickYF() <= -0.5f)
				{
					moveVec.y -= moveSpeed;
				}


				m_spotLightModel->SetPosition(m_spotLightModel->GetPosition() + moveVec);
				m_spotLight->SetPosition(m_spotLightModel->GetPosition());
				Vector3 ligEmmisionDir = m_playerModel->GetPosition() - m_spotLight->GetPosition();
				ligEmmisionDir.Normalize();
				m_spotLight->SetDirection(ligEmmisionDir);

				return;
			}

		}
	}
}
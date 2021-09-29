#include "stdafx.h"
#include "CTestMapForShadow.h"
#include "ModelRender.h"

namespace nsMyGame
{
	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		/**
		 * @brief テストマップ（レベル）用のネームスペース
		*/
		namespace nsTestMaps
		{
			/**
			 * @brief スタート関数
			 * @return アップデートを行うか？
			*/
			bool CTestMapForShadow::Start()
			{
				// エイリアス宣言
				using CModelRender = nsGraphic::nsModel::CModelRender;

				// 共通データを使用可能にする
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
			 * @brief 破棄されるときに呼ばれる関数
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
			 * @brief アップデート関数
			*/
			void CTestMapForShadow::Update()
			{
				Vector3 moveVec = Vector3::Zero;
				float moveSpeed = 10.0f;

				if (g_pad[0]->GetLStickXF() >= 0.5f)
				{
					moveVec.x += moveSpeed;
				}
				if (g_pad[0]->GetLStickXF() <= -0.5f)
				{
					moveVec.x -= moveSpeed;
				}
				if (g_pad[0]->GetLStickYF() >= 0.5f)
				{
					moveVec.y += moveSpeed;
				}
				if (g_pad[0]->GetLStickYF() <= -0.5f)
				{
					moveVec.y -= moveSpeed;
				}

				g_camera3D->SetPosition(g_camera3D->GetPosition() + moveVec);

				return;
			}
		}
	}
}
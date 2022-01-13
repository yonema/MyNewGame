#include "stdafx.h"
#include "TestMapForLevel3D.h"
#include "ModelRender.h"
#include "SkyCube.h"

namespace nsNinjaAttract
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
			bool CTestMapForLevel3D::Start()
			{
				m_level3D.Init(
					"Assets/levelData/testLevel.tkl",
					[](nsLevel3D::SLevelObjectData& objData)
					{
						return false;
					}
				);

				m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
				m_skyCube->Init(nsNature::nsSkyCubeConstData::enSkyCubeType_day);

				g_camera3D->SetFar(40000.0f);

				return true;
			}

			/**
			 * @brief 破棄した時に呼ばれる関数
			*/
			void CTestMapForLevel3D::OnDestroy()
			{
				DeleteGO(m_skyCube);
				return;
			}

			/**
			 * @brief アップデート関数
			*/
			void CTestMapForLevel3D::Update()
			{
				Vector3 targetVec = Vector3::Front;

				const float angleMoveSpeed = 2.0f;

				Vector3 moveVec = Vector3::Zero;
				const float moveSpeed = 50.0f;

				moveVec.x += g_pad[0]->GetLStickXF()* moveSpeed;
				moveVec.z += g_pad[0]->GetLStickYF()* moveSpeed;

				m_angleY += g_pad[0]->GetRStickXF() * angleMoveSpeed;
				m_angleX -= g_pad[0]->GetRStickYF() * angleMoveSpeed;

				Quaternion qRot1, qRot2;
				qRot1.SetRotationDegX(m_angleX);
				qRot2.SetRotationDegY(m_angleY);
				qRot1.Multiply(qRot2);
				qRot1.Apply(targetVec);

				g_camera3D->SetPosition(g_camera3D->GetPosition() + moveVec);
				g_camera3D->SetTarget(g_camera3D->GetPosition() + targetVec);
				
				return;
			}
		}
	}
}
#include "stdafx.h"
#include "TestMapForPlayerMove.h"
#include "SkyCube.h"
#include "Player.h"
#include "Building.h"

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
			bool CTestMapForPlayerMove::Start()
			{
				// スカイキューブの生成と初期化
				m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
				m_skyCube->Init(nsNature::nsSkyCubeConstData::enSkyCubeType_day);

				// カメラの遠平面を設定
				g_camera3D->SetFar(40000.0f);
				int a = 0;
				// レベルの生成
				m_level3D.Init(
					"Assets/levelData/testLevel.tkl",
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						if (objData.EqualObjectName(L"testBuilding"))
						{
							if (a != 1)
							{
								a++;
								return false;
							}
							nsBuilding::CBuilding* building = 
								NewGO<nsBuilding::CBuilding>(nsCommonData::enPriorityFirst, "testBuilding");
							building->Init(
								"Assets/modelData/levelSource/testBuilding.tkm",
								objData.position,
								objData.rotation
							);
							a++;
							return true;
						}

						return false;
					}
				);

				// プレイヤーの生成
				m_player = NewGO<nsPlayer::CPlayer>(nsCommonData::enPriorityFirst);
				m_player->SetPosition({ 0.0f,500.0f,0.0f });

				return true;
			}

			/**
			 * @brief 破棄した時に呼ばれる関数
			*/
			void CTestMapForPlayerMove::OnDestroy()
			{
				DeleteGO(m_skyCube);	// スカイキューブクラスの破棄
				DeleteGO(m_player);		// プレイヤークラスの破棄

				QueryGOs<nsBuilding::CBuilding>(
					"testBuilding",
					[&](nsBuilding::CBuilding* building)->bool
					{
						DeleteGO(building);
						return true;
					}
				);

				return;
			}

			/**
			 * @brief アップデート関数
			*/
			void CTestMapForPlayerMove::Update()
			{
				return;
			}


		}
	}
}
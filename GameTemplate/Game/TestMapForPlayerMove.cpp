#include "stdafx.h"
#include "TestMapForPlayerMove.h"
#include "SkyCube.h"
#include "Player.h"
#include "Buildings.h"
#include "BuildingConstData.h"

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

			const char* const CTestMapForPlayerMove::m_kBuildingName = "testBuilding";
			const char* const CTestMapForPlayerMove::m_kLevelFilePaht = "Assets/levelData/testLevel.tkl";

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

				// 建物の名前のワイド文字列
				wchar_t buildingNameW[32];
				mbstowcs(buildingNameW, m_kBuildingName, sizeof(m_kBuildingName));

				// レベルの生成
				m_level3D.Init(
					m_kLevelFilePaht,
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						if (objData.EqualObjectName(buildingNameW))
						{
							if (a != 2)
							{
								a++;
								return false;
							}
							nsBuilding::CBuildings* building = 
								NewGO<nsBuilding::CBuildings>(nsCommonData::enPriorityFirst, m_kBuildingName);
							//building->Init(
							//	nsBuilding::nsBuildingConstData::enBuilding_L_01a,
							//	objData.position,
							//	objData.rotation
							//);
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

				// 建物をすべて破棄
				QueryGOs<nsBuilding::CBuildings>(
					m_kBuildingName,
					[&](nsBuilding::CBuildings* building)->bool
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
#include "stdafx.h"
#include "AlphaMap.h"
#include "SkyCube.h"
#include "Player.h"
#include "Building.h"
#include "BuildingConstData.h"
#include "MapConstDatah.h"
#include "GameMainState.h"
#include "Goal.h"


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
		namespace nsAlphaMaps
		{
			// マップの定数データを使用可能にする
			using namespace nsMapConstData;

			/**
			 * @brief スタート関数
			 * @return アップデートを行うか？
			*/
			bool CAlphaMap::Start()
			{
				// ゲームステートの生成
				m_gameState = NewGO<nsGameState::CGameMainState>(nsCommonData::enPriorityFirst);

				// スカイキューブの生成と初期化
				m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
				m_skyCube->Init(nsNature::nsSkyCubeConstData::enSkyCubeType_day);

				// プレイヤーの生成
				m_player = NewGO<nsPlayer::CPlayer>(nsCommonData::enPriorityFirst);

				// レベルの生成
				m_level3D.Init(
					kLevelFilePath[enLevelAlpha2],
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						// 建物の生成
						if (objData.ForwardMatchName(nsBuilding::nsBuildingConstData::kBuildingForwardName))
						{
							// 建物のタイプの数の分、当たるまで全部調べる
							for (int i = 0; i < nsBuilding::nsBuildingConstData::enBuildingTypeNum; i++)
							{
								if (objData.EqualObjectName(
									nsBuilding::nsBuildingConstData::kBuildingNames[i]) != true
									)
								{
									// 名前が一致しなければ次へ
									continue;
								}

								// 名前が一致したら建物を生成する
								nsBuilding::CBuilding* building = NewGO<nsBuilding::CBuilding>(
									nsCommonData::enPriorityFirst,
									nsBuilding::nsBuildingConstData::kBuildingNames[i]
									);
								// 建物のタイプを指定して初期化
								building->Init(
									static_cast<nsBuilding::nsBuildingConstData::EnBuildingType>(i),
									objData.position,
									objData.rotation
								);
								return true;
							}

						}
						// プレイヤーの生成
						else if (objData.EqualObjectName(kPlayerName))
						{
							m_player->SetPosition(objData.position);
							m_player->SetRotation(objData.rotation);

							return true;
						}
						// ゴールの生成
						else if (objData.EqualObjectName(kGoalName))
						{
							m_goal = NewGO <nsGoal::CGoal>(nsCommonData::enPriorityFirst);
							m_goal->Init(objData.position, objData.rotation, objData.scale, *m_player);

							return true;
						}

						return false;
					}
				);

				// 電灯
				m_level3D_streetLight.Init(
					kLevelFilePath[enLevelmStreetLight],
					kNumMapChipReserveTbl[enReserveStreetLight],
					EnCollisionAttr::enCollisionAttr_Props,
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						return false;
					}
					);
				// 信号
				m_level3D_trafficLight.Init(
					kLevelFilePath[enLevelTrafficLight],
					kNumMapChipReserveTbl[enReserveTrafficLight],
					EnCollisionAttr::enCollisionAttr_Props,
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						return false;
					}
				);
				// 歩行者信号
				m_level3D_pedestrianLight.Init(
					kLevelFilePath[enLevelPedestrianLight],
					kNumMapChipReserveTbl[enReservePedestrianLight],
					EnCollisionAttr::enCollisionAttr_Props,
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						return true;
					}
				);
				// 木
				m_level3D_streetTree.Init(
					kLevelFilePath[enLevelStreetTree],
					kNumMapChipReserveTbl[enReserveStreetTree],
					EnCollisionAttr::enCollisionAttr_Props,
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						return true;
					}
				);
				// 木の枝
				m_level3D_streetTreeBranch.Init(
					kLevelFilePath[enLevelStreetTreeBranch],
					kNumMapChipReserveTbl[enReserveStreetTree],
					EnCollisionAttr::enCollisionAttr_Props,
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						objData.isTranslucent = true;
						objData.priority = nsCommonData::enPrioritySecond;
						return true;
					}
				);

				// タイマーの計測を始める
				m_gameState->StartTimingGame();

				return true;
			}

			/**
			 * @brief 破棄した時に呼ばれる関数
			*/
			void CAlphaMap::OnDestroy()
			{
				DeleteGO(m_skyCube);	// スカイキューブクラスの破棄
				DeleteGO(m_player);		// プレイヤークラスの破棄
				DeleteGO(m_goal);		// ゴールを破棄

				// 全てのタイプの建物を破棄
				for (int i = 0; i < nsBuilding::nsBuildingConstData::enBuildingTypeNum; i++)
				{
					// 建物をすべて破棄
					QueryGOs<nsBuilding::CBuilding>(
						nsBuilding::nsBuildingConstData::kBuildingNames[i],
						[&](nsBuilding::CBuilding* building)->bool
						{
							DeleteGO(building);
							return true;
						}
					);
				}

				DeleteGO(m_gameState);

				return;
			}

			/**
			 * @brief アップデート関数
			*/
			void CAlphaMap::Update()
			{
				return;
			}
		}
	}
}
#include "stdafx.h"
#include "AlphaMap.h"
#include "SkyCube.h"
#include "Player.h"
#include "Buildings.h"
#include "BuildingConstData.h"
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

				// 建物の生成
				m_buildings = NewGO<nsBuilding::CBuildings>(nsCommonData::enPriorityFirst);

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

								// 建物のタイプを指定して追加
								m_buildings->AddBuliding(
									static_cast<nsBuilding::nsBuildingConstData::EnBuildingType>(i),
									objData.position,
									objData.rotation,
									objData.scale
								);
								return true;
							}

							MessageBoxA(nullptr, "Buildingの名前がない", "警告", MB_OK);

							return true;
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

				// 建物を初期化
				m_buildings->Init();

				// 小物をレベルから生成
				for (int propsType = 0; propsType < enPropsTypeNum; propsType++)
				{
					// 小物用のレベルを初期化
					m_propsLevel3D[propsType].Init(
						kPropsLevelFilePath[propsType],
						[&](nsLevel3D::SLevelObjectData& objData)
						{
							// マップチップの予約数を設定する
							objData.numMapChipReserve = kNumMapChipReserveTbl[propsType];
							// ユーザー定義のコリジョン属性を小物用の属性に設定する
							objData.userIndex = EnCollisionAttr::enCollisionAttr_Props;


							switch (propsType)
							{
							// ココで指定したものが生成される
							case enPropsStreetLight:		// 街灯
							case enPropsTrafficLight:		// 信号機
							case enPropsPedestrianLight:	// 歩行者用信号機
								return false;
								break;
							case enPropsStreetTree:			// 街路樹

								objData.lodModelFilePath = "Assets/modelData/levelSource/StreetTree_LOD.tkm";
								return false;
								break;

							// 街路樹の枝は半透明で描画する
							case enPropsStreetTreeBranch:

								objData.isTranslucent = true;
								objData.priority = nsCommonData::enPrioritySecond;
								objData.lodModelFilePath = "Assets/modelData/levelSource/StreetTree_Branch_LOD.tkm";
								return false;
								break;

							// 指定しなかったら生成されない
							default:
								return true;
								break;
							}
						}
					);					

				}

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
				DeleteGO(m_gameState);	// ゲームステートを破棄
				DeleteGO(m_buildings);	// 建物を破棄

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
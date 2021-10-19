#include "stdafx.h"
#include "ProtoMap.h"
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
		namespace nsProtoMaps
		{
			// マップの定数データを使用可能にする
			using namespace nsMapConstData;

			/**
			 * @brief スタート関数
			 * @return アップデートを行うか？
			*/
			bool CProtoMap::Start()
			{
				// ゲームステートの生成
				m_gameState = NewGO<nsGameState::CGameMainState>(nsCommonData::enPriorityFirst);

				// スカイキューブの生成と初期化
				m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
				m_skyCube->Init(nsNature::nsSkyCubeConstData::enSkyCubeType_day);


				// 建物の名前のワイド文字列
				wchar_t buildingNameW[32];
				mbstowcs(buildingNameW, kBuildingName, sizeof(kBuildingName));

				//m_stage = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
				//m_stage->Init("Assets/modelData/city2/city.tkm");

				// レベルの生成
				m_level3D.Init(
					kLevelFilePath[enLevelProto],
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						// 建物の生成
						if (objData.EqualObjectName(kBuildingName))
						{

							nsBuilding::CBuilding* building =
								NewGO<nsBuilding::CBuilding>(nsCommonData::enPriorityFirst, kBuildingName);
							building->Init(
								nsBuilding::nsBuildingConstData::enTestBuilding,
								objData.position,
								objData.rotation
							);
							return true;
						}
						// プレイヤーの生成
						else if (objData.EqualObjectName(kPlayerName))
						{
							m_player = NewGO<nsPlayer::CPlayer>(nsCommonData::enPriorityFirst);
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


				// タイマーの計測を始める
				m_gameState->StartTimingGame();

				return true;
			}

			/**
			 * @brief 破棄した時に呼ばれる関数
			*/
			void CProtoMap::OnDestroy()
			{
				DeleteGO(m_skyCube);	// スカイキューブクラスの破棄
				DeleteGO(m_player);		// プレイヤークラスの破棄
				DeleteGO(m_goal);		// ゴールを破棄

				// 建物をすべて破棄
				QueryGOs<nsBuilding::CBuilding>(
					kBuildingName,
					[&](nsBuilding::CBuilding* building)->bool
					{
						DeleteGO(building);
						return true;
					}
				);

				DeleteGO(m_gameState);

				return;
			}

			/**
			 * @brief アップデート関数
			*/
			void CProtoMap::Update()
			{
				return;
			}
		}
	}
}
#include "stdafx.h"
#include "BackGround.h"
#include "Level3D.h"
#include "SkyCube.h"
#include "Buildings.h"


namespace nsNinjaAttract
{
	using namespace nsMaps::nsMapConstData;

	/**
	 * @brief 背景ステージ用のネームスペース
	*/
	namespace nsBackGround
	{
		/**
		 * @brief コンストラクタ
		*/
		CBackGround::CBackGround()
		{
			// レベルを生成する
			m_buildingLevel3D = std::make_unique<nsLevel3D::CLevel3D>();
			for (auto& propsLevel : m_propsLevel3D)
			{
				propsLevel = std::make_unique<nsLevel3D::CLevel3D>();
			}

			// スカイキューブの生成と初期化
			m_skyCube = NewGO<nsNature::CSkyCube>(nsCommonData::enPriorityFirst);
			m_skyCube->Init(nsNature::nsSkyCubeConstData::enSkyCubeType_day);
			m_skyCube->SetPosition(nsLevel3D::CLevel3D::m_kLevelObjectOffset);

			// 建物の生成
			m_buildings = NewGO<nsBuilding::CBuildings>(nsCommonData::enPriorityFirst);

			// レベルをロードする
			LoadLevel3D();

			// ロードしたレベルデータをもとに建物を初期化
			m_buildings->Init();


			return;
		}

		/**
		 * @brief デストラクタ
		*/
		CBackGround::~CBackGround()
		{
			DeleteGO(m_skyCube);	// スカイキューブクラスの破棄
			DeleteGO(m_buildings);	// 建物を破棄

			return;
		}

		/**
		 * @brief タイトル画面か？を設定
		 * @param[in] isTitle タイトル画面か？
		*/
		void CBackGround::SetIsTitle(const bool isTitle) 
		{
			m_skyCube->SetIsTitle(isTitle);
			return;
		}

		/**
		 * @brief レベルをロードする
		*/
		void CBackGround::LoadLevel3D()
		{
			// ここでリターンしたら、建物と小物を全く生成しない。
			//return true;
			// 建物用の3Dレベルの初期化
			// マップチップのインスタンスは生成しない
			m_buildingLevel3D->Init(
				kLevelFilePath[enLevelPlateAndBuildings],
				[&](nsLevel3D::SLevelObjectData& objData)
				{
					// 建物の生成
					if (objData.ForwardMatchName(nsBuilding::nsBuildingConstData::kBuildingForwardName) != true)
					{
						// 建物の先頭の名前ではない。建物ではないデータが入っている。
						objData.shadowCaster = true;
						return false;
					}

					// 建物のタイプの数の分、当たるまで全部調べる
					for (int i = 0; i < nsBuilding::nsBuildingConstData::enBuildingTypeNum; i++)
					{
#ifdef _DEBUG
						if (i != 0)
						{
							// デバックビルド用。ロードが重いため、1種類の建物しかロードしない。
							return true;
						}

#endif

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

					// 名前が一致しない建物のデータがある
					MessageBoxA(nullptr, "Buildingの名前がない", "警告", MB_OK);
					return true;
				}
			);

			// 小物をレベルから生成
			for (int propsType = 0; propsType < enPropsTypeNum; propsType++)
			{
#ifdef _DEBUG
				// デバックビルド用。ロードが重いため、小物は生成しない。
				break;
#endif
				// 小物用のレベルを初期化
				m_propsLevel3D[propsType]->Init(
					kPropsLevelFilePath[propsType],
					[&](nsLevel3D::SLevelObjectData& objData)
					{
						// マップチップの予約数を設定する
						objData.numMapChipReserve = kNumMapChipReserveTbl[propsType];
						// ユーザー定義のコリジョン属性を小物用の属性に設定する
						objData.userIndex = EnCollisionAttr::enCollisionAttr_Props;
						// LOD用のモデルのファイルパスを指定
						objData.lodModelFilePath = kPropsLODModelFilePath[propsType];
						// LOD用の距離を設定
						objData.distanceLOD = kPropsLODDistance[propsType];
						// シャドウキャスターにする
						//objData.shadowCaster = true;


						switch (propsType)
						{
							// ココで指定したものが生成される
						case enPropsPedestrianLight:	// 歩行者用信号機
						case enPropsStreetLight:		// 街灯
						case enPropsTrafficLight:		// 信号機
						case enPropsStreetTree:			// 街路樹
							return false;
							break;

							// 街路樹の枝は半透明で描画する
						case enPropsStreetTreeBranch:
							objData.isTranslucent = true;
							objData.priority = nsCommonData::enPrioritySecond;
							objData.collisionModelFilePath = 
								"Assets/modelData/levelSource/StreetTree_collision.tkm";
							return false;
							break;

							// 指定しなかったら生成されない
						default:
							return true;
							break;
						}

						return true;
					}
				);

				// 次の小物へ
			}


			return;
		}
	}
}
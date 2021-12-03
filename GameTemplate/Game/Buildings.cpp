#include "stdafx.h"
#include "Buildings.h"
#include "ModelRender.h"


namespace nsMyGame
{
	/**
	 * @brief 建物関連のネームスペース
	*/
	namespace nsBuilding
	{
		// 建物関連の定数データを使用可能にする
		using namespace nsBuildingConstData;

		/**
		 * @brief コンストラクタ
		*/
		CBuildings::CBuildings()
		{

			return;
		}


		/**
		 * @brief 破棄した時に呼ばれる関数
		*/
		void CBuildings::OnDestroy()
		{
			// 建物をクエリして、全部破棄
			QueryGOs<nsGraphic::nsModel::CModelRender>(
				kBuildingObjectName,
				[&](nsGraphic::nsModel::CModelRender* modelRender)->bool
				{
					DeleteGO(modelRender);
					return true;
				}
			);


#ifdef MY_DEBUG
			for (auto& debugModel : m_debugModels)
			{
				DeleteGO(debugModel);
			}
#endif

			return;
		}

		/**
		 * @brief アップデート関数
		*/
		void CBuildings::Update()
		{
			return;
		}

		/**
		 * @brief 建物を追加する
		 * @param[in] buildingType 建物の種類
		 * @param[in] pos 座標
		 * @param[in] rot 回転
		 * @param[in] scale 拡大率
		*/
		void CBuildings::AddBuliding(
			const nsBuildingConstData::EnBuildingType buildingType,
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3& scale
		)
		{
			// 建物のデータのインスタンスを生成
			SBuildingDataPtr buildingData(std::make_unique<SBuildingData>());
			// 座標、回転、拡大率を設定
			buildingData->pos = pos;
			buildingData->rot = rot;
			buildingData->scale = scale;
			// 建物のデータのマップに登録する
			m_buildingDataMap[buildingType].emplace_back(std::move(buildingData));

			return;
		}


		/**
		 * @brief 初期化
		*/
		void CBuildings::Init()
		{
			// 建物のタイプ全部調べる
			for (int buildingType = 0; buildingType < enBuildingTypeNum; buildingType++)
			{
				// 建物のタイプ
				const EnBuildingType bType = static_cast<EnBuildingType>(buildingType);

				if (m_buildingDataMap.count(bType) == 0)
				{
					// このタイプの建物が一つも生成されていなかったら次へスキップ
					continue;
				}

				// このタイプの建物のモデルレンダラーを生成する
				m_modelRenderMap[bType] = NewGO<nsGraphic::nsModel::CModelRender>(
					nsCommonData::enPriorityFirst,
					kBuildingObjectName
					);

				// このタイプの建物の数
				const int numBuilding = static_cast<int>(m_buildingDataMap[bType].size());

				if (numBuilding == 1)
				{
					// 一個だけなら、通常のモデルの初期化をして、次へスキップ。
					InitModel(bType);
				}
				else
				{
					// 複数あるなら、インスタンシング描画のモデルの初期化。
					InitModelInstance(bType, numBuilding);
				}

				m_modelRenderMap[bType]->SetIsShadowCaster(true);
			}


#ifdef no
#ifdef MY_DEBUG

			// 計算されたスイングターゲットの座標コンテナを取得
			const std::vector<Vector3>& swingTargetPositions = m_swingTarget.GetSwingTargetPositions();

			// スイングターゲットの座標番号
			int stPosNo = 0;
			// スイングターゲットの座標の数だけキャパシティを確保
			m_debugModels.reserve(swingTargetPositions.size());
			// 全ての座標を調べる
			for (auto& stPos : swingTargetPositions)
			{
				nsGraphic::nsModel::CModelRender* debugModel = 
					NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
				debugModel->SetPosition(swingTargetPositions[stPosNo++]);
				debugModel->Init("Assets/modelData/debugModel/testBox.tkm");
				m_debugModels.emplace_back(debugModel);

			}
#endif
#endif

			return;
		}

		/**
		 * @brief 通常のモデルの初期化
		 * @param[in] buildingType 建物の種類
		*/
		void CBuildings::InitModel(const nsBuildingConstData::EnBuildingType buildingType)
		{
			// モデルレンダラーの座標と回転と拡大率を設定
			m_modelRenderMap[buildingType]->SetPosition(m_buildingDataMap[buildingType][0]->pos);
			m_modelRenderMap[buildingType]->SetRotation(m_buildingDataMap[buildingType][0]->rot);
			m_modelRenderMap[buildingType]->SetScale(m_buildingDataMap[buildingType][0]->scale);

			// モデルレンダラーを初期化
			m_modelRenderMap[buildingType]->Init(kBuildingModelFilePath[buildingType]);

			// スイングターゲットのインスタンスを生成
			m_buildingDataMap[buildingType][0]->swingTarget = 
				std::make_unique<nsStringActionTarget::nsSwingTarget::CSwingTarget>();
			// スイングターゲットの初期化
			m_buildingDataMap[buildingType][0]->swingTarget->Init(
				*m_modelRenderMap[buildingType]->GetModel().GetTkmFile()
			);
			// スイングターゲットの座標を計算する
			m_buildingDataMap[buildingType][0]->swingTarget->CalcSwingingTargetPositions(
				m_modelRenderMap[buildingType]->GetModel().GetWorldMatrix(),
				kDefaultWidthSegments,
				kDefaultHeightSegments,
				kDefaultLengthSegments,
				kDefaultHeightLowerLimit
			);

			// 静的物理オブジェクトのインスタンスを生成
			m_buildingDataMap[buildingType][0]->staticObject = std::make_unique<PhysicsStaticObject>();
			// 静的物理オブジェクトをモデルから生成
			m_buildingDataMap[buildingType][0]->staticObject->CreateFromModel(
				m_modelRenderMap[buildingType]->GetModel(),
				m_modelRenderMap[buildingType]->GetModel().GetWorldMatrix()
			);

			return;
		}

		/**
		 * @brief インスタンシング描画のモデルの初期化
		 * @param[in] buildingType 建物の種類
		 * @param[in] numInstance インスタンスの数
		*/
		void CBuildings::InitModelInstance(
			const nsBuildingConstData::EnBuildingType buildingType,
			const int numInstance
		)
		{
			// モデルレンダラーをインスタンシング描画を行うように初期化
			m_modelRenderMap[buildingType]->Init(
				kBuildingModelFilePath[buildingType],
				nullptr,
				0,
				numInstance
			);

			// このタイプの建物のすべてのデータから情報を構築する
			for (const auto& buildingData : m_buildingDataMap[buildingType])
			{
				// モデルレンダラーのインスタンシングのデータを更新
				m_modelRenderMap[buildingType]->UpdateInstancingData(
					buildingData->pos,
					buildingData->rot,
					buildingData->scale
				);

				// モデルクラスに基づいたワールド行列を計算
				Matrix worldMatrix = m_modelRenderMap[buildingType]->GetModel().CalcWorldMatrix(
					buildingData->pos,
					buildingData->rot,
					buildingData->scale
				);

				// スイングターゲットのインスタンスを生成
				buildingData->swingTarget =
					std::make_unique<nsStringActionTarget::nsSwingTarget::CSwingTarget>();
				// スイングターゲットの初期化
				buildingData->swingTarget->Init(
					*m_modelRenderMap[buildingType]->GetModel().GetTkmFile()
				);
				// スイングターゲットの座標を計算する
				buildingData->swingTarget->CalcSwingingTargetPositions(
					worldMatrix,
					kDefaultWidthSegments,
					kDefaultHeightSegments,
					kDefaultLengthSegments,
					kDefaultHeightLowerLimit
				);

				// 静的物理オブジェクトのインスタンスを生成
				buildingData->staticObject = std::make_unique<PhysicsStaticObject>();
				// 静的物理オブジェクトをモデルから生成
				buildingData->staticObject->CreateFromModel(
					m_modelRenderMap[buildingType]->GetModel(),
					worldMatrix
				);
			}

			return;
		}



	}
}
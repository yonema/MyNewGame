#include "stdafx.h"
#include "MapChip.h"
#include "Level3D.h"

namespace nsNinjaAttract
{
	/**
	 * @brief 3Dのレベルデータ関連のネームスペース
	*/
	namespace nsLevel3D
	{
		CMapChip::CMapChip(const SLevelObjectData& objData,const char* filePath)
		{
			m_mapChipDataVector.reserve(objData.numMapChipReserve);
			m_physicsStaticObjectPtrVector.reserve(objData.numMapChipReserve);

			// モデルのファイルパスを取得
			m_filePath = std::make_unique<const char*>(filePath);
			// 半透明描画か？を取得
			m_isTranslucent = objData.isTranslucent;
			// 優先度を取得
			m_priority = objData.priority;
			// ユーザー定義のコリジョン属性を取得
			m_userIndex = objData.userIndex;
			// LODの切り替えの距離を取得
			m_distanceLOD = objData.distanceLOD;

			// LOD用のモデルのファイルパスが指定されていたら、ファイルパスを取得
			if (objData.lodModelFilePath)
			{
				m_lodModelFilePath = std::make_unique<const char*>(objData.lodModelFilePath);
			}

			// シャドウキャスターか？を取得
			m_shadowCaster = objData.shadowCaster;

			// マップチップデータを追加する
			AddMapChipData(objData);

			return;
		}

		void CMapChip::Init()
		{
#ifdef MY_DEBUG
			// キャパシティ確認用
			const int cap = static_cast<int>(m_mapChipDataVector.capacity());
#endif
			// マップチップデータの数
			const int mapChipDataNum = static_cast<int>(m_mapChipDataVector.size());

			// マップチップを描画するモデルレンダラーの生成
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(m_priority);

			if (mapChipDataNum == 1)
			{
				// 一個だけのため、通常描画

				// 通常のモデルの初期化
				InitModel();

				return;
			}

			// インスタンシング描画のモデルの初期化
			InitModelInstancing(mapChipDataNum);



			return;
		}

		/**
		 * @brief マップチップデータの追加
		 * @param[in] objData レベルオブジェクトデータ
		*/
		void CMapChip::AddMapChipData(const SLevelObjectData& objData)
		{
			auto mapChipData = std::make_unique< SMapChipData>();
			mapChipData->position = objData.position;
			mapChipData->rotation = objData.rotation;
			mapChipData->scale = objData.scale;
			// マップチップデータの追加
			m_mapChipDataVector.emplace_back(std::move(mapChipData));

			return;
		}

		/**
		 * @brief 通常のモデルの初期化
		*/
		void CMapChip::InitModel()
		{
			// モデルの初期化
			m_modelRender->SetPosition(m_mapChipDataVector[0]->position);
			m_modelRender->SetRotation(m_mapChipDataVector[0]->rotation);
			m_modelRender->SetScale(m_mapChipDataVector[0]->scale);

			if (m_isTranslucent != true)
			{
				// モデルレンダラーの初期化
				m_modelRender->Init(*m_filePath.get());
			}
			else
			{
				// 半透明描画でモデルレンダラーの初期化
				m_modelRender->IniTranslucent(*m_filePath.get());
			}

			// LOD用のモデルが指定されているか？
			if (m_lodModelFilePath)
			{
				// LOD用のモデルの初期化
				m_modelRender->InitLODModel(*m_lodModelFilePath);
				// LODを有効化
				m_modelRender->SetIsEnableLOD(true);
				if (m_distanceLOD > 0.0f)
				{
					// LODの切り替えの距離が指定されていたら、モデルレンダラーに設定する
					m_modelRender->SetDistanceLOD(m_distanceLOD);
				}
			}

			auto p = std::make_unique<PhysicsStaticObject>();
			//静的物理オブジェクトを作成。
			p->CreateFromModel(m_modelRender->GetModel(), m_modelRender->GetModel().GetWorldMatrix(), m_userIndex);
			m_physicsStaticObjectPtrVector.emplace_back(std::move(p));

			return;
		}

		/**
		 * @brief インスタンシング描画のモデルの初期化
		 * @param[in] numInstance インスタンス数
		*/
		void CMapChip::InitModelInstancing(const int numInstance)
		{
			//インスタンシング描画用にモデルを初期化。

			if (m_isTranslucent != true)
			{
				// モデルレンダラーの初期化
				m_modelRender->Init(*m_filePath.get(), nullptr, 0, numInstance);
			}
			else
			{
				// 半透明描画でモデルレンダラーの初期化
				m_modelRender->IniTranslucent(*m_filePath.get(), nullptr, 0, numInstance);
			}

			// LOD用のモデルが指定されているか？
			if (m_lodModelFilePath)
			{
				// LOD用のモデルの初期化
				m_modelRender->InitLODModel(*m_lodModelFilePath);
				// LODを有効化
				m_modelRender->SetIsEnableLOD(true);
				if (m_distanceLOD > 0.0f)
				{
					// LODの切り替えの距離が指定されていたら、モデルレンダラーに設定する
					m_modelRender->SetDistanceLOD(m_distanceLOD);
				}
			}

			// シャドウキャスターか？を設定
			m_modelRender->SetIsShadowCaster(m_shadowCaster);

			for (auto& mapChipData : m_mapChipDataVector)
			{
				//モデルレンダーのインスタンシング用のデータを更新。
				m_modelRender->UpdateInstancingData(mapChipData->position, mapChipData->rotation, mapChipData->scale);

				//ワールド行列を計算する。
				Matrix worldMatrix = m_modelRender->GetModel().CalcWorldMatrix(
					mapChipData->position, mapChipData->rotation, mapChipData->scale
				);
				auto p = std::make_unique<PhysicsStaticObject>();
				//静的物理オブジェクトを作成。
				p->CreateFromModel(m_modelRender->GetModel(), worldMatrix, m_userIndex);
				m_physicsStaticObjectPtrVector.emplace_back(std::move(p));
			}

			return;
		}


	}
}
#include "stdafx.h"
#include "Building.h"
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
		CBuilding::CBuilding()
		{
			// モデルレンダラーを初期化しておく
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			return;
		}


		/**
		 * @brief 破棄した時に呼ばれる関数
		*/
		void CBuilding::OnDestroy()
		{
			DeleteGO(m_modelRender);

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
		void CBuilding::Update()
		{
			return;
		}


		/**
		 * @brief 初期化
		 * @param[in] tkmFilePath tkmファイルパス
		 * @param[in] pos 座標
		 * @param[in] rot 回転
		 * @param[in] scale 拡大率
		*/
		void CBuilding::Init(
			const char* tkmFilePath,
			const Vector3& pos,
			const Quaternion rot,
			const Vector3& scale
		)
		{
			// モデルレンダラーのトランスフォームを更新
			m_modelRender->SetPosition(pos);
			m_modelRender->SetRotation(rot);
			m_modelRender->SetScale(scale);

			// モデルレンダラーを初期化
			m_modelRender->Init(tkmFilePath);

			// スイングターゲットを初期化
			m_swingTarget.Init(*m_modelRender);

			// スイングターゲットの座標を計算する
			m_swingTarget.CalcSwingingTargetPositions(
				kDefaultWidthSegments,
				kDefaultHeightSegments,
				kDefaultLengthSegments
			);
#ifdef TRUE
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

	}
}
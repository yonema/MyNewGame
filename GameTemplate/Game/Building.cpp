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
			m_modelRender->SetPosition(pos);
			m_modelRender->SetRotation(rot);
			m_modelRender->SetScale(scale);

			m_modelRender->Init(tkmFilePath);

			m_swingTarget.Init(*m_modelRender);

			m_swingTarget.CalcSwingingTargetPositions(
				kDefaultWidthSegments,
				kDefaultHeightSegments,
				kDefaultLengthSegments
			);

			const std::vector<Vector3>& swingTargetPositions = m_swingTarget.GetSwingTargetPositions();

			int stPosNo = 0;
			m_debugModels.reserve(swingTargetPositions.size());
			for (auto& stPos : swingTargetPositions)
			{
				nsGraphic::nsModel::CModelRender* debugModel = 
					NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
				debugModel->SetPosition(swingTargetPositions[stPosNo++]);
				debugModel->Init("Assets/modelData/debugModel/testBox.tkm");
				m_debugModels.emplace_back(debugModel);

			}


			return;
		}

	}
}
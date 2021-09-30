#include "stdafx.h"
#include "MapChip.h"
#include "Level3D.h"

namespace nsMyGame
{
	/**
	 * @brief 3Dのレベルデータ関連のネームスペース
	*/
	namespace nsLevel3D
	{
		CMapChip::CMapChip(const SLevelObjectData& objData, const char* filePath)
		{
			// マップチップを描画するモデルレンダラーの生成と初期化
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			m_modelRender->Init(filePath);

			// モデルレンダラーの更新
			m_modelRender->SetPosition(objData.position);
			m_modelRender->SetRotatioin(objData.rotation);
			m_modelRender->SetScale(objData.scale);


			//静的物理オブジェクトを作成。
			m_physicsStaticObject.CreateFromModel(
				m_modelRender->GetModel(),
				m_modelRender->GetModel().GetWorldMatrix()
			);

			return;
		}
	}
}
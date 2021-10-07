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
			// マップチップを描画するモデルレンダラーの生成
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			// トランスフォームと設定
			m_modelRender->SetPosition(objData.position);
			m_modelRender->SetRotation(objData.rotation);
			m_modelRender->SetScale(objData.scale);

			// モデルレンダラーの初期化
			m_modelRender->Init(filePath);

			//静的物理オブジェクトを作成。
			m_physicsStaticObject.CreateFromModel(
				m_modelRender->GetModel(),
				m_modelRender->GetModel().GetWorldMatrix()
			);

			return;
		}
	}
}
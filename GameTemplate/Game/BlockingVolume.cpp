#include "stdafx.h"
#include "BlockingVolume.h"
#include "ModelRender.h"

namespace nsNinjaAttract
{
	/**
	 * @brief ジオメトリ関連のネームスペース
	*/
	namespace nsGeometry
	{
		const char* const CBlockingVolume::m_kBlockingVolumeModelFilePath = 
			"Assets/modelData/preset/blockingVolume.tkm";


		/**
		 * @brief 初期化
		 * @param[in] pos 座標
		 * @param[in] rot 回転
		 * @param[in] scale 拡大
		*/
		void CBlockingVolume::Init(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
		{
			// ブロッキングボリューム用のモデルを生成
			nsGraphic::nsModel::CModelRender* model =
				NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);

			// トランスフォームを設定
			model->SetPosition(pos);
			model->SetRotation(rot);
			model->SetScale(scale);

			// 初期化
			model->Init(m_kBlockingVolumeModelFilePath);

			// モデルから、ブロッキングボリュームを生成
			m_physicsStaticObject = std::make_unique<PhysicsStaticObject>();
			m_physicsStaticObject->CreateFromModel(model->GetModel(), model->GetModel().GetWorldMatrix(), enCollisionAttr_DoesNotCollideCamera);

			// モデルはいらないため、破棄。
			DeleteGO(model);

			return;
		}

	}
}
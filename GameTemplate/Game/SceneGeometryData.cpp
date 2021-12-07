#include "stdafx.h"
#include "SceneGeometryData.h"
#include "GeometryData.h"

namespace nsMyGame
{


	/**
	 * @brief ジオメトリ関連のネームスペース
	*/
	namespace nsGeometry
	{
        /**
         * @brief ジオメトリデータを更新
        */
        void CSceneGeometryData::Update()
        {
            // パラメータをリセットする
            m_isBuiltShadowCasterGeometryData = false;
            m_shadowCasterMaxPositionInViewFrustum = { -FLT_MIN, -FLT_MIN, -FLT_MIN };
            m_shadowCasterMinPositionInViewFrustum = { FLT_MAX,  FLT_MAX,  FLT_MAX };

            // シーンのジオメトリ情報を更新。
            for (auto& geomData : m_geometryDatas) 
            {
                // ジオメトリの情報を更新
                geomData->Update();

                // ビューフラスタムに入っているシャドウキャスターのAABBの最大座標と最小座標を更新
                if (geomData->IsShadowCaster() && geomData->IsInViewFrustum())
                {
                    m_shadowCasterMaxPositionInViewFrustum.Max(geomData->GetAabbMaxPositionInWorld());
                    m_shadowCasterMinPositionInViewFrustum.Min(geomData->GetAabbMinPositionInWorld());
                    m_isBuiltShadowCasterGeometryData = true;
                }
            }

            return;
        }

	}
}
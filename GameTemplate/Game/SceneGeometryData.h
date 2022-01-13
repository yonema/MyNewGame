#pragma once
#include "Noncopyable.h"

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGeometry { class CGeometryData; }

	/**
	 * @brief ジオメトリ関連のネームスペース
	*/
	namespace nsGeometry
	{
		/**
		 * @brief シーンのジオメトリ情報
		*/
		class CSceneGeometryData : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CSceneGeometryData() = default;
			/**
			 * @brief デストラクタ
			*/
			~CSceneGeometryData() = default;

		public:		// メンバ関数


            /**
             * @brief ジオメトリデータを登録
             * @param[in,out] geomData 登録するジオメトリデータ
            */
            void RegisterGeometryData(CGeometryData* geomData)
            {
                m_geometryDatas.push_back(geomData);
            }

            /**
             * @brief ジオメトリデータの登録を解除
             * @param[in] geomData 登録を解除するジオメトリデータ
            */
            void UnregisterGeometryData(const CGeometryData* geomData)
            {
                auto it = std::find(
                    m_geometryDatas.begin(),
                    m_geometryDatas.end(),
                    geomData
                );
                if (it != m_geometryDatas.end()) {
                    m_geometryDatas.erase(it);
                }
            }

            /**
             * @brief ジオメトリデータを更新
            */
            void Update();

            /**
             * @brief ビューフラスタムに含まれているシャドウキャスターの最大座標を取得
             * @return ビューフラスタムに含まれているシャドウキャスターの最大座標
            */
            const Vector3& GetShadowCasterMaxPositionInViewFrustum() const
            {
                return m_shadowCasterMaxPositionInViewFrustum;
            }

            /**
             * @brief ビューフラスタムに含まれているシャドウキャスターの最小座標を取得
             * @return ビューフラスタムに含まれているシャドウキャスターの最小座標
            */
            const Vector3& GetShadowCasterMinPositionInViewFrustum() const
            {
                return m_shadowCasterMinPositionInViewFrustum;
            }

            /**
             * @brief シャドウキャスターに関するジオメトリ情報を構築できた？を取得
             * @return シャドウキャスターに関するジオメトリ情報を構築できた？
            */
            bool IsBuildshadowCasterGeometryData() const
            {
                return m_isBuiltShadowCasterGeometryData;
            }


		private:	// データメンバ
			//!< ビューフラスタムに含まれているシャドウキャスターの最大座標
			Vector3 m_shadowCasterMaxPositionInViewFrustum;
			//!< ビューフラスタムに含まれているシャドウキャスターの最小座標
			Vector3 m_shadowCasterMinPositionInViewFrustum;
			//!< シャドウキャスターに関するジオメトリ情報を構築できた？
			bool m_isBuiltShadowCasterGeometryData = false;
			std::list<CGeometryData*> m_geometryDatas;	//!< シーンを構成するジオメトリ情報のリスト

		};

	}
}
#pragma once
#include "AABB.h"
#include "Noncopyable.h"

namespace nsMyGame
{

	// 前方宣言
	namespace nsGraphic { namespace nsModel { class CModelRender; } }

	/**
	 * @brief ジオメトリ関連のネームスペース
	*/
	namespace nsGeometry
	{
		/**
		 * @brief ジオメトリ情報
		*/
		class CGeometryData : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CGeometryData() = default;
			/**
			 * @brief デストラクタ
			*/
			~CGeometryData() = default;

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param[in] modelRender モデルレンダラー
			 * @param[in] instanceId インスタンスID
			*/
			void Init(const nsGraphic::nsModel::CModelRender* modelRender, int instanceId);

			/**
			 * @brief ジオメトリ情報を更新
			*/
			void Update();

			/**
			 * @brief ビューフラスタムに含まれているか？を得る
			 * @return ビューフラスタムに含まれているか？
			*/
			bool IsInViewFrustum() const
			{
				return m_isInViewFrustum;
			}

			/**
			 * @brief AABBのワールド空間での最大頂点を取得
			 * @return AABBのワールド空間での最大頂点
			*/
			const Vector3& GetAabbMaxPositionInWorld() const
			{
				return m_aabbMax;
			}

			/**
			 * @brief AABBのワールド空間での最小頂点を取得
			 * @return AABBのワールド空間での最小頂点
			*/
			const Vector3& GetAabbMinPositionInWorld() const
			{
				return m_aabbMin;
			}

			/**
			 * @brief シャドウキャスターの幾何学データ？を取得
			 * @return シャドウキャスターの幾何学データ？
			*/
			bool IsShadowCaster() const;

			/**
			 * @brief カメラからの距離を取得
			 * @return カメラからの距離
			*/
			float GetDistanceFromCamera() const
			{
				return m_distanceFromCamera;
			}

		private:	// データメンバ
			bool m_isInViewFrustum = false;			//!< ビューフラスタムに含まれているか？
			float m_distanceFromCamera = 0.0f;		//!< カメラからの距離
			CAABB m_aabb;							//!< AABB
			Vector3 m_aabbWorldVertexPositions[8];	//!< AABBのワールド空間での8頂点。
			Vector3 m_aabbMax = Vector3::MinV;		//!< AABBのワールド空間での最大頂点
			Vector3 m_aabbMin = Vector3::MaxV;		//!< AABBのワールド空間での最小頂点
			//!< このジオメトリ情報と関連づいているモデルレンダラー
			const nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;
			int m_instanceId = 0;					//!< インスタンスID

		};
	}
}

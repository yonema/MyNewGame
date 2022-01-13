#include "stdafx.h"
#include "GeometryData.h"
#include "ModelRender.h"
#include "RenderingEngine.h"

namespace nsNinjaAttract
{

	/**
	 * @brief ジオメトリ関連のネームスペース
	*/
	namespace nsGeometry
	{
		/**
		 * @brief 初期化
		 * @param[in] modelRender モデルレンダラー
		 * @param[in] instanceId インスタンスID
		*/
		void CGeometryData::Init(const nsGraphic::nsModel::CModelRender* modelRender, int instanceId)
		{
			// このジオメトリ情報と関連づいているモデルレンダラーを保持
			m_modelRender = modelRender;
			// インスタンスIDを保持
			m_instanceId = instanceId;

			// tkmファイルからモデルの最小座標と最大座標を調べる。
			Vector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			Vector3 vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };

			const auto& tkmFile = modelRender->GetModel().GetTkmFile();
			tkmFile->QueryMeshParts([&](const TkmFile::SMesh& mesh) {
				for (const auto& vertex : mesh.vertexBuffer) {
					vMax.Max(vertex.pos);
					vMin.Min(vertex.pos);
				}
				});

			// 最大座標と最小座標からAABBを初期化
			m_aabb.Init(vMax, vMin);
		}

		/**
		 * @brief ジオメトリ情報を更新
		*/
		void CGeometryData::Update()
		{
			// インスタンスIDを指定して、ワールド行列を得る
			Matrix worldMatrix = m_modelRender->GetWorldMatrix(m_instanceId);
			// AABBを構成する8頂点のワールド空間での座標を計算する
			m_aabb.CalcVertexPositions(
				m_aabbWorldVertexPositions,
				worldMatrix
			);
			
			// ビューカリングのためのビュープロジェクション行列を取得
			const auto& viewProjMatrix = nsMyEngine::CRenderingEngine::GetInstance()->GetViewProjectionMatrixForViewCulling();

			// AABB空間での最大値と最小値
			m_aabbMax = Vector3::MinV;
			m_aabbMin = Vector3::MaxV;

			// ビュープロジェクション空間での最大値と最小値
			Vector4 vMax = Vector3::MinV;
			Vector4 vMin = Vector3::MaxV;

			// ビューフラスタムの中に映っているか？
			m_isInViewFrustum = false;

			// カメラからの距離をリセット
			m_distanceFromCamera = FLT_MAX;

			// AABBの頂点を調べる
			for (Vector4 v : m_aabbWorldVertexPositions) {

				// AABB空間での最大値と最小値を調べる
				m_aabbMax.Max({ v.x, v.y, v.z });
				m_aabbMin.Min({ v.x, v.y, v.z });

				// ビュープロジェクション空間での最大値と最小値を調べる
				viewProjMatrix.Apply(v);

				// カメラからの距離ベクトル
				const Vector3 distanceFromCameraVec = { v.x,v.y,v.z };
				// カメラからの距離ベクトルの長さ
				const float distanceFromCameraLen = distanceFromCameraVec.Length();

				if (m_distanceFromCamera > distanceFromCameraLen)
				{
					// 最小のカメラからの距離を取る
					m_distanceFromCamera = distanceFromCameraLen;
				}

				// wで割って正規化する
				v.x /= fabsf(v.w);
				v.y /= fabsf(v.w);
				v.z /= fabsf(v.w);

				vMax.Max(v);
				vMin.Min(v);
			}

			// ビューフラスタムの範囲内に移っているか調べる
			if (vMax.x > -1.0f
				&& vMax.y > -1.0f
				&& vMax.z > 0.0f
				&& vMin.x < 1.0f
				&& vMin.y < 1.0f
				&& vMin.z < 1.0f
				) 
			{
				//画面に映る。
				m_isInViewFrustum = true;
			}

			return;
		}

		/**
		 * @brief シャドウキャスターの幾何学データ？を取得
		 * @return シャドウキャスターの幾何学データ？
		*/
		bool CGeometryData::IsShadowCaster() const
		{
			return m_modelRender->IsShadowCaster();
		}


	}
}
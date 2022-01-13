#include "stdafx.h"
#include "AABB.h"
#include "../../MiniEngine/tkFile/TkmFile.h"

namespace nsNinjaAttract
{
	/**
	 * @brief ジオメトリ関連のネームスペース
	*/
	namespace nsGeometry
	{
		/**
		 * @brief AABBの情報を初期化
		 * @param[in] vMax 最大値
		 * @param[in] vMin 最小値
		*/
		void CAABB::Init(const Vector3& vMax, const Vector3& vMin)
		{
			// 最大値と最小値を保治
			m_max = vMax;
			m_min = vMin;

			// AABBの半分のサイズ
			Vector3 halfSize = (m_max - m_min) * 0.5f;
			// AABBの中心座標
			Vector3 centerPos = (vMax + vMin) * 0.5f;

			// 8頂点の座標を計算

			// （注意）頂点は左手系ZUpなので、左手系YUpに置き換える

			// 左下手奥の場所の頂点
			m_vertexPosition[enVertPos_X0Y0Z1] = centerPos;
			m_vertexPosition[enVertPos_X0Y0Z1].x -= halfSize.x;
			m_vertexPosition[enVertPos_X0Y0Z1].y -= halfSize.y;
			m_vertexPosition[enVertPos_X0Y0Z1].z -= halfSize.z;

			// 右下奥の場所の頂点
			m_vertexPosition[enVertPos_X1Y0Z1] = centerPos;
			m_vertexPosition[enVertPos_X1Y0Z1].x += halfSize.x;
			m_vertexPosition[enVertPos_X1Y0Z1].y -= halfSize.y;
			m_vertexPosition[enVertPos_X1Y0Z1].z -= halfSize.z;

			// 左下手前の場所の頂点
			m_vertexPosition[enVertPos_X0Y0Z0] = centerPos;
			m_vertexPosition[enVertPos_X0Y0Z0].x -= halfSize.x;
			m_vertexPosition[enVertPos_X0Y0Z0].y += halfSize.y;
			m_vertexPosition[enVertPos_X0Y0Z0].z -= halfSize.z;

			// 右下手前の場所の頂点
			m_vertexPosition[enVertPos_X1Y0Z0] = centerPos;
			m_vertexPosition[enVertPos_X1Y0Z0].x += halfSize.x;
			m_vertexPosition[enVertPos_X1Y0Z0].y += halfSize.y;
			m_vertexPosition[enVertPos_X1Y0Z0].z -= halfSize.z;

			// 左上奥の場所の頂点
			m_vertexPosition[enVertPos_X0Y1Z1] = centerPos;
			m_vertexPosition[enVertPos_X0Y1Z1].x -= halfSize.x;
			m_vertexPosition[enVertPos_X0Y1Z1].y -= halfSize.y;
			m_vertexPosition[enVertPos_X0Y1Z1].z += halfSize.z;

			// 右上奥の場所の頂点
			m_vertexPosition[enVertPos_X1Y1Z1] = centerPos;
			m_vertexPosition[enVertPos_X1Y1Z1].x += halfSize.x;
			m_vertexPosition[enVertPos_X1Y1Z1].y -= halfSize.y;
			m_vertexPosition[enVertPos_X1Y1Z1].z += halfSize.z;

			// 左上手前の場所の頂点
			m_vertexPosition[enVertPos_X0Y1Z0] = centerPos;
			m_vertexPosition[enVertPos_X0Y1Z0].x -= halfSize.x;
			m_vertexPosition[enVertPos_X0Y1Z0].y += halfSize.y;
			m_vertexPosition[enVertPos_X0Y1Z0].z += halfSize.z;

			// 右上手前の場所の頂点
			m_vertexPosition[enVertPos_X1Y1Z0] = centerPos;
			m_vertexPosition[enVertPos_X1Y1Z0].x += halfSize.x;
			m_vertexPosition[enVertPos_X1Y1Z0].y += halfSize.y;
			m_vertexPosition[enVertPos_X1Y1Z0].z += halfSize.z;

			return;
		}

		/**
		 * @brief tkmファイルからAABBの情報を初期化
		 * @param[in] tkmFile tkmファイル
		*/
		void CAABB::InitFromTkmFile(const TkmFile& tkmFile)
		{
			// 最大座標
			Vector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			// 最小座標
			Vector3 vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };

			// メッシュパーツに対してクエリを行う
			tkmFile.QueryMeshParts(
				[&](const TkmFile::SMesh& mesh)
				{
					// 頂点を全検索
					for (const auto& vertex : mesh.vertexBuffer)
					{
						vMax.Max(vertex.pos);	// 最大座標を設定
						vMin.Min(vertex.pos);	// 最小座標を設定
					}

					return;
				}
			);

			// AABBを初期化
			Init(vMax, vMin);

			return;
		}

		/**
		 * @brief AABBを構成する8頂点のワールド空間での座標を計算する
		 * @param[out] pos_out 8頂点の座標を格納
		 * @param[in] mWorld ワールド行列
		*/
		void CAABB::CalcVertexPositions(Vector3 pos_out[enVertNum], const Matrix& mWorld)
		{
			// 8頂点全て調べる
			for (int vertNo = 0; vertNo < enVertNum; vertNo++)
			{
				// 頂点の座標を格納
				pos_out[vertNo] = m_vertexPosition[vertNo];
				// ワールド行列からワールド座標を計算
				mWorld.Apply(pos_out[vertNo]);
			}

			return;
		}
	}
}
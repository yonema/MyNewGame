#include "stdafx.h"
#include "SwingTarget.h"
#include "StringActionTargetManager.h"

namespace nsMyGame
{
	/**
	 * @brief 糸を使ったアクションのターゲット関連のネームスペース
	*/
	namespace nsStringActionTarget
	{
		/**
		 * @brief スイング用オブジェクト関連のネームスペース
		*/
		namespace nsSwingTarget
		{

			/**
			 * @brief デストラクタ
			*/
			CSwingTarget::~CSwingTarget()
			{
				// マネージャーの自身の登録を解除する
				CStringActionTargetManager::GetInstance()->RemoveSwingTarget(this);

				return;
			}
			/**
			 * @brief 初期化
			 * @param[in] modelRender モデルレンダラー
			*/
			void CSwingTarget::Init(const nsGraphic::nsModel::CModelRender& modelRender)
			{
				m_modelRender = &modelRender;

				// tkmファイルからAABBを初期化
				m_aabb.InitFromTkmFile(*m_modelRender->GetModel().GetTkmFile());

				// マネージャーに自身を登録する
				CStringActionTargetManager::GetInstance()->AddSwingTarget(this);

				return;
			}


			/**
			 * @brief スイングのターゲット達の座標を計算する
			 * derails 引数が (1, 1, 1) でターゲットの数は8つ（8頂点分）
			 * @param[in] widthSegments 幅セグメント ( X )
			 * @param[in] heightSegments 高さセグメント ( Y )
			 * @param[in] lenghtSegments 長さセグメント ( Z )
			*/
			void CSwingTarget::CalcSwingingTargetPositions(
				const UINT widthSegments,
				const UINT heightSegments,
				const UINT lengthSegments
			)
			{
				// ジオメトリ関連のネームスペースを使用可能にする
				using namespace nsGeometry;

				// コンテナをリセット
				// コンテナのデータを破棄
				m_swingTargetPositions.clear();
				// コンテナのキャパシティをリセット
				m_swingTargetPositions.shrink_to_fit();


				// ターゲットの数
				int TargetsNum = 0;
				// AABBの壁（側面）の部分にターゲットを作る
				// 前面と後面のターゲットの数を加算する
				TargetsNum += (widthSegments + 1) * (heightSegments + 1) * 2;
				// 左面と右面のターゲットの数を加算する
				TargetsNum += (lengthSegments + 1) * (heightSegments + 1) * 2;
				// 重複している四つ角のターゲットを減算する
				TargetsNum -= (heightSegments + 1) * 4;

				// コンテナのキャパシティをターゲットの数分だけ確保する
				m_swingTargetPositions.reserve(TargetsNum);

				// AABBの8頂点の座標
				Vector3 vertexPos[CAABB::enVertNum];

				// AABBの8頂点のワールド座標を計算
				m_aabb.CalcVertexPositions(vertexPos, m_modelRender->GetModel().GetWorldMatrix());

				Vector3 boxWidth = vertexPos[CAABB::enVertPos_X1Y0Z0] - vertexPos[CAABB::enVertPos_X0Y0Z0];
				Vector3 boxHeight = vertexPos[CAABB::enVertPos_X0Y1Z0] - vertexPos[CAABB::enVertPos_X0Y0Z0];
				Vector3 boxLength = vertexPos[CAABB::enVertPos_X0Y0Z1] - vertexPos[CAABB::enVertPos_X0Y0Z0];
				Vector3 widhtSegmentVec = boxWidth / static_cast<float>(widthSegments);
				Vector3 heightSegmentVec = boxHeight / static_cast<float>(heightSegments);
				Vector3 lengthSegmentVec = boxLength / static_cast<float>(lengthSegments);


				for (UINT hSeg = 0; hSeg < heightSegments + 1; hSeg++)
				{
					Vector3 hSegVec = heightSegmentVec * static_cast<float>(hSeg);

					// 前面と後面のターゲットの座標を計算して保持する
					for (UINT wSeg = 0; wSeg < widthSegments + 1; wSeg++)
					{
						Vector3 targetOffset = hSegVec + widhtSegmentVec * static_cast<float>(wSeg);

						// 前面のターゲットの座標を保持
						m_swingTargetPositions.emplace_back(
							vertexPos[CAABB::enVertPos_X0Y0Z0] + targetOffset
						);
						// 後面のターゲットの座標を保持
						m_swingTargetPositions.emplace_back(
							vertexPos[CAABB::enVertPos_X0Y0Z1] + targetOffset
						);
					}

					// 左面と右面のターゲットの座標を計算して保持する
					for (UINT lSeg = 1; lSeg < lengthSegments; lSeg++)
					{
						Vector3 targetOffset = hSegVec + lengthSegmentVec * static_cast<float>(lSeg);

						// 左面とターゲットの座標を保持
						m_swingTargetPositions.emplace_back(
							vertexPos[CAABB::enVertPos_X0Y0Z0] + targetOffset
						);
						// 右面とターゲットの座標を保持
						m_swingTargetPositions.emplace_back(
							vertexPos[CAABB::enVertPos_X1Y0Z0] + targetOffset
						);
					}
				}


				return;
			}

		}
	}
}
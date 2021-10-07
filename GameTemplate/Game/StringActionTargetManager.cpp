#include "stdafx.h"
#include "StringActionTargetManager.h"
#include "SwingTarget.h"

namespace nsMyGame
{

	/**
	 * @brief 糸を使ったアクションのターゲット関連のネームスペース
	*/
	namespace nsStringActionTarget
	{

		CStringActionTargetManager* CStringActionTargetManager::m_instance = nullptr;//!< 唯一のインスタンス		


		/**
		 * @brief コンストラクタ
		*/
		CStringActionTargetManager::CStringActionTargetManager()
		{
			// スイングのターゲットのコンテナのキャパシティを確保
			m_swingTargets.reserve(m_kSwingTargetNum);

			return;
		}

		/**
		 * @brief スイングターゲットの登録を解除する
		 * @param[in] swingTarget 解除するスイングターゲット
		*/
		void CStringActionTargetManager::RemoveSwingTarget(const nsSwingTarget::CSwingTarget* swingTarget)
		{
			// 解除するスイングターゲットを探す
			std::vector<nsSwingTarget::CSwingTarget*>::iterator itr
				= std::find(m_swingTargets.begin(), m_swingTargets.end(), swingTarget);

			// 解除する
			m_swingTargets.erase(itr);

			return;
		}


		/**
		 * @brief 指定された座標から有効範囲内にあるスイングターゲットの座標の中で一番近い座標を得る
		 * @param[in] fromPos 座標
		 * @param[in] scopeRadius 有効範囲の半径
		 * @return スイングターゲットの座標。有効範囲内に無ければnullptrを戻す。
		*/
		const Vector3* const GetNearestSwintTargetPointWithinScope(
			const Vector3& fromPos,
			const float scopeRadius
		)
		{
			Vector3 fromPosXZ = fromPos;
			fromPosXZ.y = 0.0f;

			float tmpDist = FLT_MAX;
			const Vector3* retPosition = nullptr;


			CStringActionTargetManager::GetInstance()->QuerySwingTarget(
				[&](nsSwingTarget::CSwingTarget* swingTarget)
				{
					Vector3 swingTargetPosXZ = swingTarget->GetPosition();
					swingTargetPosXZ.y = 0.0f;

					Vector3 distVec = fromPosXZ - swingTargetPosXZ;
					if (distVec.Length() > scopeRadius)
					{
						return;
					}

					const std::vector<Vector3>& stPositions = swingTarget->GetSwingTargetPositions();

					for (auto& stPos : stPositions)
					{

						distVec = fromPos - stPos;

						float destLen = distVec.Length();

						if (destLen < tmpDist)
						{
							tmpDist = destLen;
							retPosition = &stPos;
						}
					}


				}
			);

			return retPosition;
		}

	}
}
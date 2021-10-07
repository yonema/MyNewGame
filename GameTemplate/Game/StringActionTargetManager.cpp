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
			// Y座標（高さ）を無視したXZ平面での座標
			Vector3 fromPosXZ = fromPos;
			fromPosXZ.y = 0.0f;

			// 最短距離を保持しておく変数
			float tmpDist = FLT_MAX;

			// 戻り値用の座標のポインタ
			const Vector3* retPosition = nullptr;

			// 全てのスイングターゲットにクエリする
			CStringActionTargetManager::GetInstance()->QuerySwingTarget(
				[&](nsSwingTarget::CSwingTarget* swingTarget)
				{
					//////// 1.スイングターゲットがXZ平面で、有効範囲内か調べる ////////

					// スイングターゲットのY座標（高さ）を無視したXZ平面での座標
					Vector3 swingTargetPosXZ = swingTarget->GetPosition();
					swingTargetPosXZ.y = 0.0f;

					// XZ平面でのスイングターゲットとの距離ベクトル
					Vector3 distVec = fromPosXZ - swingTargetPosXZ;	// 距離ベクトル
					// 高さを考慮しないで、スイングターゲットが有効範囲内か調べる
					if (distVec.Length() > scopeRadius)
					{
						// 有効範囲内ではない
						// 早期リターン
						return;
					}

					//////// 2.スイングターゲットの座標たちの中で一番近い座標を探す ////////

					// スイングターゲットの座標コンテナを取得
					const std::vector<Vector3>& stPositions = swingTarget->GetSwingTargetPositions();

					// 全ての座標を調べる
					for (auto& stPos : stPositions)
					{
						// 座標との距離ベクトル
						distVec = fromPos - stPos;

						// 座標との距離
						const float destLen = distVec.Length();

						// 今回の距離が今までの最短距離より短いか？
						if (destLen < tmpDist)
						{
							// 短い場合はその距離とその座標を保治
							tmpDist = destLen;
							retPosition = &stPos;
						}
					}

					// クエリ終了
					return;
				}
			);

			// スイングターゲットの座標を戻す。有効範囲内に無ければnullptrを戻す。
			return retPosition;
		}

	}
}
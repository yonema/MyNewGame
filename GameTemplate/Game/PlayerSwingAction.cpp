#include "stdafx.h"
#include "PlayerSwingAction.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "StringActionTargetManager.h"

namespace nsMyGame
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		/**
 * @brief プレイヤーの動き関連のネームスペース
*/
		namespace nsPlayerMovenent
		{

			/**
			 * @brief 初期化
			 * @param[in,out] player プレイヤー
			 * @param[in,out] playerMovement プレイヤー移動クラスの参照
			*/
			void CPlayerSwingAction::Init(
				CPlayer* player,
				CPlayerMovement* playerMovement
			)
			{
				// プレイヤーの参照をセット
				m_playerRef = player;
				// 加算移動ベクトルの参照をセット
				m_playerMovementRef = playerMovement;

				return;
			}

			/**
			 * @brief スイングアクションを実行
			*/
			void CPlayerSwingAction::Execute()
			{
				const Vector3* target = nullptr;
				if (m_playerRef->GetInputData().actionSwing)
				{
					target = nsStringActionTarget::GetNearestSwintTargetPointWithinScope(
						m_playerRef->GetPosition(), 2000.0f
					);
					m_playerMovementRef->SetAddMoveVec(m_playerRef->GetPosition());

					if (target != nullptr)
					{
						//m_playerMovementRef->SetAddMoveVec(*target);
						m_playerRef->StartStringStretchToPos(*target);
					}
					//else
					//{
					//	m_playerMovementRef->SetAddMoveVec(m_playerRef->GetPosition());
					//}

					
				}


				return;
			}
		}
	}
}
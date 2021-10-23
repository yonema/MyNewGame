#include "stdafx.h"
#include "PlayerWallRun.h"

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
			 * @param[in.out] player プレイヤー
			 * @param[in,out] playerMovement プレイヤー移動クラスの参照
			*/
			void CPlayerWallRun::Init(
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
			 * @brief ウォールランを実行
			*/
			void CPlayerWallRun::Execute()
			{


				return;
			}

		}
	}
}
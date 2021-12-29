#include "stdafx.h"
#include "PlayerCatchEnemy.h"
#include "Player.h"

namespace nsMyGame
{

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{

		/**
		 * @brief 初期化処理
		 * @param[in] playerRef プレイヤーの参照
		*/
		void CPlayerCatchEnemy::Init(CPlayer* playerRef)
		{
			m_playerRef = playerRef;

			return;
		}

		/**
		 * @brief レイヤーが敵を捕まえる処理クラスのアップデートを実行
		*/
		void CPlayerCatchEnemy::ExecuteUpdate()
		{
			if (m_playerRef->GetInputData().actionCatchEnemy)
			{
				m_playerRef->ChangeOnEnemyState();
			}

			return;
		}

	}
}
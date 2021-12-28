#include "stdafx.h"
#include "PlayerCatchEnemy.h"

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
		void CPlayerCatchEnemy::Init(const CPlayer& playerRef)
		{
			m_playerRef = &playerRef;

			return;
		}

		/**
		 * @brief レイヤーが敵を捕まえる処理クラスのアップデートを実行
		*/
		void CPlayerCatchEnemy::ExecuteUpdate()
		{


			return;
		}

	}
}
#pragma once
#include "Noncopyable.h"

namespace nsMyGame
{
	// 前方宣言

	namespace nsPlayer { class CPlayer; }

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		/**
		 * @brief プレイヤーが敵を捕まえる処理クラス
		*/
		class CPlayerCatchEnemy : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPlayerCatchEnemy() = default;
			/**
			 * @brief デストラクタ
			*/
			~CPlayerCatchEnemy() = default;

		public:		// メンバ関数

			/**
			 * @brief 初期化処理
			 * @param[in] playerRef プレイヤーの参照
			*/
			void Init(CPlayer* playerRef);

			/**
			 * @brief レイヤーが敵を捕まえる処理クラスのアップデートを実行
			*/
			void ExecuteUpdate();

		private:	// データメンバ
			CPlayer* m_playerRef = nullptr;	//!< プレイヤーの参照
		};


	}
}

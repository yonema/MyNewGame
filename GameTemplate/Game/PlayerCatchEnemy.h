#pragma once
#include "Noncopyable.h"

namespace nsMyGame
{
	// 前方宣言

	namespace nsPlayer { class CPlayer; }
	namespace nsGameState { class CGameMainState; }
	namespace nsAICharacter { class CAICar; }

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
			 * @param[in,out] playerRef プレイヤーの参照
			*/
			void Init(CPlayer* playerRef);

			/**
			 * @brief レイヤーが敵を捕まえる処理クラスのアップデートを実行
			*/
			void ExecuteUpdate();

			/**
			 * @brief ターゲットの敵の参照を得る
			 * @return ターゲットの敵の参照
			*/
			const nsAICharacter::CAICar* GetTargetEnemy() const
			{
				return m_targetRef;
			}

			/**
			 * @brief ターゲットとの距離を得る
			 * @return ターゲットとの距離
			*/
			float GetTargetLength() const
			{
				return m_targetLength;
			}

		private:	// privateなメンバ関数

			/**
			 * @brief 敵の上に乗っている時の更新
			*/
			void OnEnemyUpdate();

			/**
			 * @brief ターゲットを探す
			*/
			void FindTarget();

			/**
			 * @brief ターゲット状態を継続するか調べる
			*/
			void CheckContinueTarget();

			/**
			 * @brief このクラスで使う共通データの更新
			*/
			void UpdateCommonData();

			/**
			 * @brief 敵を捕まえるかどうか調べる
			*/
			void CheckCatchEnemy();

		private:	// データメンバ
			CPlayer* m_playerRef = nullptr;	//!< プレイヤーの参照

			float m_targetLength = 0.0f;	//!< ターゲットとの距離

			float m_onEnemyTimer = 0.0f;	//!< 敵の上に乗っている間のタイマー

			nsAICharacter::CAICar* m_targetRef = nullptr;		//!< ターゲットの参照
			std::vector<nsAICharacter::CAICar*>* m_aiCarsRef = nullptr;	//!< 車たちの参照
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< ゲームステート
		};


	}
}

#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"

namespace nsMyGame
{
	// 前方宣言
	namespace nsPlayer
	{
		class CPlayer;
		namespace nsPlayerMovenent { class CPlayerMovement; }
	}
	namespace nsAICharacter { class CAICar; }
	namespace nsSound { class CSoundCue; }

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
			 * @brief 敵の上に乗る動きクラス
			*/
			class CPlayerOnEnemy : private nsUtil::Noncopyable
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CPlayerOnEnemy();
				/**
				 * @brief デストラクタ
				*/
				~CPlayerOnEnemy();

			public:		// メンバ関数
				/**
				 * @brief 初期化
				 * @param[in.out] player プレイヤー
				 * @param[in,out] playerMovement プレイヤー移動クラスの参照
				*/
				void Init(
					CPlayer* player,
					CPlayerMovement* playerMovement
				);

				/**
				 * @brief プレイヤーの敵の上に乗る処理を実行
				*/
				void Execute();

				/**
				 * @brief 初期状態に戻す
				*/
				void Reset();

				/**
				 * @brief 敵の上に乗っているステートを得る
				 * @return 敵の上に乗っているステート
				*/
				nsPlayerConstData::nsOnEnemyConstData::EnOnEnemyState GetOnEnemyState() const
				{
					return m_onEnemyState;
				}

			private:	// privateなメンバ関数

				/**
				 * @brief サウンドを初期化
				*/
				void InitSound();

				/**
				 * @brief 敵に向かって糸を伸ばし始める処理
				*/
				void StartStretchToEnemy();

				/**
				 * @brief エネミーに向かって糸を伸ばす処理
				*/
				void StretchingToEnemy();

				/**
				 * @brief 敵の上に行く処理
				*/
				void GoOnEnemy();

				/**
				 * @brief 敵の上にいる処理
				*/
				void OnEnemy();

				/**
				 * @brief 敵の方を向かせる
				*/
				void LookAtEnemy();

				/**
				 * @brief ステートを変更する
				 * @param[in] newState 新しいステート
				*/
				void ChangeState(const nsPlayerConstData::nsOnEnemyConstData::EnOnEnemyState newState);

			private:	// データメンバ
				CPlayer* m_playerRef = nullptr;					//!< プレイヤークラスの参照
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< プレイヤー移動クラスの参照
				//!< 敵の上に乗るステート
				nsPlayerConstData::nsOnEnemyConstData::EnOnEnemyState m_onEnemyState =
					nsPlayerConstData::nsOnEnemyConstData::enStartStretchToEnemy;
				const nsAICharacter::CAICar* m_enemyRef = nullptr;	//!< 敵の参照

				Vector3 m_startGoOnEnemyPos = Vector3::Zero;	//!< 敵の上に行くときの最初の座標
				float m_goOnEnemyTimer = 0.0f;	//!< 敵の上に行くまでのタイマー


				// サウンド
				nsSound::CSoundCue* m_chainPutOutSC = nullptr;		//!< 鎖を出すサウンド
				nsSound::CSoundCue* m_goOnEnemySC = nullptr;		//!< 敵の上に乗りに行くサウンド
				nsSound::CSoundCue* m_landingOnEnemySC = nullptr;	//!< 敵の上に乗ったサウンド

			};

		}
	}
}
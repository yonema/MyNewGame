#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"


namespace nsMyGame
{
	// 前方宣言

	namespace nsPlayer { 
		class CPlayer; 
		class CPlayerCommandInput;
	}
	namespace nsGameState { class CGameMainState; }
	namespace nsAICharacter { class CAICar; }
	namespace nsGraphic { namespace nsSprite { class CSpriteRender; } }

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
			CPlayerCatchEnemy();
			/**
			 * @brief デストラクタ
			*/
			~CPlayerCatchEnemy();

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

			/**
			 * @brief 敵を捕まえるステートを得る
			 * @return 敵を捕まえるステート
			*/
			nsPlayerConstData::nsCatchEnemyConstData::EnCatchEnemyState GetCatchEnemyState() const
			{
				return m_catchEnemyState;
			}

		private:	// privateなメンバ関数

			/**
			 * @brief タイマーバーのスプライトの初期化
			*/
			void InitTimerBarSprite();

			/**
			 * @brief QTEに使うボタンのスプライトの初期化
			*/
			void InitQTEButtonSprite();

			/**
			 * @brief 忍術のエフェクトを初期化
			*/
			void InitNinjyutuEffect();

			/**
			 * @brief QTEに使うボタンのスプライトの再初期化
			 * @param[in] commandArray コマンドの配列
			*/
			void ReInitQTEBUttonSprite(
				const std::vector<nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType>& commandArray
			);

			/**
			 * @brief 敵の上に乗っている時の更新
			*/
			void OnEnemyUpdate();

			/**
			 * @brief コマンドが成功した時の更新
			*/
			void SuccessCommandUpdate();

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

			/**
			 * @brief QTEで使うスプライトを有効化する
			*/
			void QTESpriteActivate();

			/**
			 * @brief QTEで使うスプライトを有効化する
			*/
			void QTESpriteDeactivate();

			/**
			 * @brief 敵を捕まえる処理が終了した時の処理
			*/
			void EndCatchEnemy();

			/**
			 * @brief ステートを遷移する
			 * @param[in] newState 新しいステート
			*/
			void ChangeState(const nsPlayerConstData::nsCatchEnemyConstData::EnCatchEnemyState newState);

		private:	// データメンバ
			CPlayer* m_playerRef = nullptr;	//!< プレイヤーの参照
			//!< 敵を捕まえるクラスのステート
			nsPlayerConstData::nsCatchEnemyConstData::EnCatchEnemyState m_catchEnemyState =
				nsPlayerConstData::nsCatchEnemyConstData::enCE_FindTarget;

			float m_targetLength = 0.0f;	//!< ターゲットとの距離

			float m_onEnemyTimer = 0.0f;	//!< 敵の上に乗っている間のタイマー
			float m_ninnjyutuEFTimer = 0.0f;	//!< 忍術のエフェクトタイマー

			//!< 敵の上に乗っている時のタイマーバーのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_onEnemyTimerBar = nullptr;
			//!< 敵の上に乗っている時のタイマーバーのフレームのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_onEnemyTimerBarFrame = nullptr;

			//!< QTEに使うボタンたちのスプライトレンダラー
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_QTEButtonSRs;
			//!< QTEに使うボタンの枠のスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_onQTEButtonFraneSR = nullptr;

			std::unique_ptr<CPlayerCommandInput> m_commandInput;	//!< コマンド入力クラス
			int m_oldCommandProgress = 0;							//!< 前フレームのコマンド進行度

			Effect* m_ninjyutuEF = nullptr;	//!< 忍術のエフェクト

			nsAICharacter::CAICar* m_targetRef = nullptr;		//!< ターゲットの参照
			std::vector<nsAICharacter::CAICar*>* m_aiCarsRef = nullptr;	//!< 車たちの参照
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< ゲームステート
		};


	}
}

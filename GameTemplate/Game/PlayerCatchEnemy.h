#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"


namespace nsNinjaAttract
{
	// 前方宣言

	namespace nsPlayer { 
		class CPlayer; 
		class CPlayerCommandInput;
	}
	namespace nsGameState { class CGameMainState; }
	namespace nsAICharacter { class CAICar; }
	namespace nsGraphic { namespace nsSprite { class CSpriteRender; } }
	namespace nsSound { class CSoundCue; }

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
			 * @brief サウンドを初期化
			*/
			void InitSound();

			/**
			 * @brief QTEに使うボタンのスプライトの再初期化
			 * @param[in] commandArray コマンドの配列
			*/
			void ReInitQTEBUttonSprite(
				const std::vector<nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType>& commandArray
			);

			/**
			 * @brief いつでも共通で行われる更新
			*/
			void CommonUpdate();

			/**
			 * @brief 敵の上に乗っている時の更新
			*/
			void OnEnemyUpdate();

			/**
			 * @brief QTEに使うボタンの残像の更新
			*/
			void QTEButternAfterImageUpdate();

			/**
			 * @brief QTEの結果の更新
			*/
			void QTEResultUpdate();

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
			 * @brief ターゲットとの距離を更新
			*/
			void UpdateTargetLength();

			/**
			 * @brief 敵を捕まえるかどうか調べる
			*/
			void CheckCatchEnemy();

			/**
			 * @brief QTEに使うボタンの残像を開始する
			 * @param[in] isSuccess コマンド入力が成功したか？
			*/
			void StartQTEButtonAfterImage(const bool isSuccess);

			/**
			 * @brief QTEの結果のスプライトの演出を開始する
			 * @param[in] qteResultType QTEの結果の種類
			*/
			void StartQTEResult(
				const nsPlayerConstData::nsCatchEnemyConstData::EnQTEResultType qteResultType
			);

			/**
			 * @brief QTEで使うスプライトを有効化する
			*/
			void ActivateQTESprite();

			/**
			 * @brief QTEで使うスプライトを有効化する
			*/
			void DeactivateQTESprite();

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
			bool m_isStandUp = false;			//!< 起き上がり中？

			//!< 敵の上に乗っている時のタイマーバーのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_onEnemyTimerBar = nullptr;
			//!< 敵の上に乗っている時のタイマーバーのフレームのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_onEnemyTimerBarFrame = nullptr;

			//!< QTEに使うボタンたちのスプライトレンダラー
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_QTEButtonSRs;
			//!< QTEに使うボタンの枠のスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_QTEButtonFraneSR = nullptr;
			//!< QTEに使うボタンの残像スプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_QTEButtonAfterImageSR = nullptr;
			float m_buttonAfterImageTimer = 0.0f;	//!< ボタンの残像のタイマー
			//!< QTEの結果のスプライトレンダラー達
			nsGraphic::nsSprite::CSpriteRender* 
				m_QTEResultSR[nsPlayerConstData::nsCatchEnemyConstData::enQTEResultTypeNum] = {};
			//!< QTEの結果の枠（内側）のスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_QTEResultFrameIn = nullptr;
			//!< QTEの結果の枠（外側）のスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_QTEResultFrameOut = nullptr;
			float m_resultTimer = 0.0f;	//!< ボタンの残像のタイマー

			std::unique_ptr<CPlayerCommandInput> m_commandInput;	//!< コマンド入力クラス
			int m_oldCommandProgress = 0;							//!< 前フレームのコマンド進行度

			Effect* m_ninjyutuEF = nullptr;	//!< 忍術のエフェクト
			Effect* m_sonarEF = nullptr;	//!< ソナーのエフェクト
			float m_sonarTimer = 0.0f;

			nsAICharacter::CAICar* m_targetRef = nullptr;		//!< ターゲットの参照
			std::vector<nsAICharacter::CAICar*>* m_aiCarsRef = nullptr;	//!< 車たちの参照
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< ゲームステート

			// サウンド
			nsSound::CSoundCue* m_commandSuccessSC = nullptr;	//!< コマンド入力成功のサウンド
			nsSound::CSoundCue* m_commandMissSC = nullptr;		//!< コマンド入力失敗のサウンド
			nsSound::CSoundCue* m_windowOpneSC = nullptr;		//!< ウィンドウを開くサウンド
			nsSound::CSoundCue* m_windowCloseSC = nullptr;		//!< ウィンドウを閉じるサウンド
			nsSound::CSoundCue* m_fireStartSC = nullptr;		//!< 火遁の最初のサウンド
			nsSound::CSoundCue* m_fireReleaseSC = nullptr;		//!< 火遁を放つサウンド
			nsSound::CSoundCue* m_sonarSC = nullptr;			//!< ソナーのサウンド


		};


	}
}

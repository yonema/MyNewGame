#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsPlayer 
	{
		class CPlayer;			// プレイヤークラス
		namespace nsPlayerMovenent { class CPlayerMovement; }
	}
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
			 * @brief プレイヤーの歩きと走りクラス
			*/
			class CPlayerWalkAndRun : private nsUtil::Noncopyable
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CPlayerWalkAndRun();
				/**
				 * @brief デストラクタ
				*/
				~CPlayerWalkAndRun();

			public:		// メンバ関数

				/**
				 * @brief 初期化
				 * @param[in] player プレイヤー
				 * @param[in,out] playerMovement プレイヤー移動クラスの参照
				*/
				void Init(
					const CPlayer& player,
					CPlayerMovement* playerMovement
				);

				/**
				 * @brief 歩きと走りの処理を実行
				*/
				void Execute();

				/**
				 * @brief 現在の動きに、移動パラメータを合わせる
				*/
				void MuchMoveParam();

				/**
				 * @brief このクラスのサウンドを停止する
				*/
				void StopSound();

				/**
				 * @brief ジャンプの上昇中か？を得る
				 * @return ジャンプの上昇中か？
				*/
				bool IsJumpUp() const
				{
					return m_isJumpUp;
				}


			private:	// privateなメンバ関数

				/**
				 * @brief サウンドの初期化
				*/
				void InitSound();

				/**
				 * @brief 歩きか走りの移動の処理
				*/
				void WalkOrRunMove();

				/**
				 * @brief 糸を使ったアクションの後の空中の処理
				*/
				void AirAfterStringAction();

				/**
				 * @brief 軸入力値を更新
				*/
				void UpdateInputAxisParam();

				/**
				 * @brief 歩きか走りかを決める
				*/
				void WalkOrRun();

				/**
				 * @brief 移動方向を更新する
				*/
				void UpdateMoveDir();

				/**
				 * @brief 加速を計算
				*/
				void CalcAcceleration();

				/**
				 * @brief 摩擦の計算
				*/
				void CalcFriction();

				/**
				 * @brief 速度制限の計算
				*/
				void CalcLimitSpeed();

				/**
				 * @brief 実際に移動させる
				*/
				void Move();

				/**
				 * @brief ジャンプの更新
				*/
				void UpdateJump();

				/**
				 * @brief サウンドの更新
				*/
				void UpdateSound();

			private:	// データメンバ
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< 移動クラスの参照
				const CPlayer* m_playerRef = nullptr;			//!< プレイヤーの参照
				Vector3 m_moveDir = Vector3::Zero;				//!< 移動方向
				float m_velocity = 0.0f;						//!< スピード
				float m_oldVelocity = 0.0f;						//!< 前のフレームの速度
				bool m_isJumpUp = false;						//!< ジャンプの上昇中か？
				float m_jumpUpTimer = 0.0f;						//!< ジャンプの上昇中のタイマー

				// 軸入力値
				float m_inputMoveF = 0.0f;			//!< 前、後移動の軸入力値
				float m_inputMoveR = 0.0f;			//!< 右、左移動の軸入力値

				// 歩きと走りで変わるパラメータ
				float m_acceleration = 0.0f;	//!< 加速度
				float m_maxSpeed = 0.0f;		//!< 最高速度

				// サウンド
				nsSound::CSoundCue* m_walkSC = nullptr;		//!< 歩きのサウンド
				nsSound::CSoundCue* m_runSC = nullptr;		//!< 走りのサウンド
				nsSound::CSoundCue* m_jumpSC = nullptr;		//!< ジャンプのサウンド
				//!< ジャンプ時のボイスのサウンド
				nsSound::CSoundCue* 
					m_jumpVoiceSC[nsPlayerConstData::nsWalkAndRunConstData::kJumpVoiceTypeNum] = {};

				std::unique_ptr<std::mt19937> m_mt;							//!< メルセンヌツイスターの32ビット版
				std::unique_ptr<std::uniform_int_distribution<>> m_rand;	//!< 範囲付きの一様乱数

			};

		}
	}
}
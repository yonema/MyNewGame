#pragma once
#include "Noncopyable.h"

namespace nsMyGame
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// 前方宣言
		class CPlayer;			// プレイヤークラス

		/**
		 * @brief プレイヤーの動き関連のネームスペース
		*/
		namespace nsPlayerMovenent
		{
			// 前方宣言
			class CPlayerMovement;	// プレイヤーの動きクラス

			/**
			 * @brief プレイヤーの歩きと走りクラス
			*/
			class CPlayerWalkAndRun : private nsUtil::Noncopyable
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CPlayerWalkAndRun() = default;
				/**
				 * @brief デストラクタ
				*/
				~CPlayerWalkAndRun() = default;

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

			private:	// privateなメンバ関数

				/**
				 * @brief 軸入力値を更新
				*/
				void UpdateInputAxisParam();

				/**
				 * @brief 歩きか走りかを決める
				*/
				void WalkOrRun();

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

			private:	// データメンバ
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< 移動クラスの参照
				const CPlayer* m_playerRef = nullptr;			//!< プレイヤーの参照
				Vector3 m_addMoveVec = Vector3::Zero;			//!< 加算移動ベクトル
				float m_oldVelocity = 0.0f;						//!< 前のフレームの速度

				// 軸入力値
				float m_inputMoveF = 0.0f;			//!< 前、後移動の軸入力値
				float m_inputMoveR = 0.0f;			//!< 右、左移動の軸入力値
				float m_absInputMoveF = 0.0f;		//!< 前、後移動の軸入力の絶対値
				float m_absInputMoveR = 0.0f;		//!< 右、左移動の軸入力の絶対値

				// 歩きと走りで変わるパラメータ
				float m_acceleration = 0.0f;	//!< 加速度
				float m_maxSpeed = 0.0f;		//!< 最高速度
			};

		}
	}
}
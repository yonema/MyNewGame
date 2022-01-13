#pragma once
#include "Noncopyable.h"
#include "PlayerWalkAndRun.h"
#include "PlayerSwingAction.h"
#include "PlayerWallRun.h"
#include "PlayerOnEnemy.h"
#include "PlayerConstData.h"
#include "GameTime.h"

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsPlayer
	{
		class CPlayer;			// プレイヤークラス
		class CPlayerCamera;	// プレイヤーカメラクラス
		class CPlayerModelAnimation; // プレイヤーモデルアニメーションクラス
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
			 * @brief プレイヤーの動きクラス
			*/
			class CPlayerMovement : private nsUtil::Noncopyable
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CPlayerMovement();
				/**
				 * @brief デストラクタ
				*/
				~CPlayerMovement();

			public:		// メンバ関数

				/**
				 * @brief 初期化
				 * @param[in] radius カプセルコライダーの半径
				 * @param[in] height カプセルコライダーの高さ
				 * @param[in,out] player プレイヤーの参照
				 * @param[in,out] playerCamera プレイヤーカメラの参照
				 * @param[in.out] playerModelAnimation プレイヤーモデルアニメーションの参照
				*/
				void Init(
					const float radius,
					const float height,
					CPlayer* player,
					CPlayerCamera* playerCamera,
					CPlayerModelAnimation* playerModelAnimation
				);

				/**
				 * @brief プレイヤーの移動クラスのアップデートを実行する
				*/
				void ExecuteUpdate();

				/**
				 * @brief 移動ベクトルに加算する
				 * @param[in] addMoveVec 加算移動ベクトル
				*/
				void AddMoveVec(const Vector3& addMoveVec)
				{
					m_moveVec += addMoveVec;
				}

				/**
				 * @brief 移動ベクトルを得る
				 * @return 移動ベクトル
				*/
				const Vector3& GetMoveVec() const
				{
					return m_moveVec;
				}

				/**
				 * @brief 加算移動ベクトルに制限をかける
				 * @param[in] limit 制限
				*/
				void LimitMoveVec(const float limit);

				/**
				 * @brief 移動速度を得る
				 * @return 移動速度
				*/
				float GetXZSpeed() const
				{
					Vector3 moveVecXZ = m_moveVec;
					moveVecXZ.y = 0.0f;
					return moveVecXZ.Length();
				}

				/**
				 * @brief 移動ベクトルのX成分のリセットする
				*/
				void ResetMoveVecX() { m_moveVec.x = 0.0f; }
				/**
				 * @brief 移動ベクトルのY成分のリセットする
				*/
				void ResetMoveVecY() { m_moveVec.y = 0.0f; }
				/**
				 * @brief 移動ベクトルのZ成分のリセットする
				*/
				void ResetMoveVecZ() { m_moveVec.z = 0.0f; }

				/**
				 * @brief 空中か？を得る
				 * @return 空中か？
				*/
				bool IsAir() const
				{
					return !m_charaCon.IsOnGround();
				}

				/**
				 * @brief 壁に触れているか？を得る
				 * @return 壁に触れているか？
				*/
				bool IsOnWall() const
				{
					return m_charaCon.IsOnWall();
				}

				/**
				 * @brief 直接座標を設定
				 * @param[in] pos 座標
				*/
				void SetDirectPosition(const Vector3& pos);

				/**
				 * @brief 重力を使用するか？を設定する
				 * @param useGravity 重力を使用するか？
				*/
				void SetUseGravity(const bool useGravity)
				{
					m_useGravity = useGravity;
				}

				/**
				 * @brief 歩きと走りのクラスの移動パラメータを合わせる
				*/
				void MuchWalkAndRunMoveParam()
				{
					m_playerWalkAndRun.MuchMoveParam();
				}

				/**
				 * @brief スイングアクションの参照を得る
				 * @return スイングアクションの参照
				*/
				const CPlayerSwingAction& GetPlayerSwingAction() const
				{
					return m_playerSwingAction;
				}

				/**
				 * @brief プレイヤーが敵の上に乗っている処理の参照を得る
				 * @return プレイヤーが敵の上に乗っている処理の参照
				*/
				const CPlayerOnEnemy& GetPlayerOnEnemy() const
				{
					return m_playerOnEnemy;
				}

				/**
				 * @brief 敵の上に乗る処理クラスを初期状態に戻す
				*/
				void ResetOnEnemy()
				{
					m_playerOnEnemy.Reset();
				}

				/**
				 * @brief スイングアクションクラスを初期状態に戻す
				*/
				void ResetSwing()
				{
					m_playerSwingAction.Reset();
				}

				/**
				 * @brief 歩きと走りのサウンドを停止する
				*/
				void StopWalkAndRunSound()
				{
					m_playerWalkAndRun.StopSound();
				}

			private:	// privateなメンバ関数

				/**
				 * @brief ステートを更新する
				*/
				void UpdateState();

				/**
				 * @brief プレイヤーの移動を更新
				*/
				void UpdateMovePlayer();

				/**
				 * @brief プレイヤーの回転を更新
				*/
				void UpdateTurnPlayer();

				/**
				 * @brief 重力をかける
				*/
				void ApplyGravity();

				/**
				 * @brief キャラクターコントローラーを使った移動
				*/
				void MoveWithCharacterController();

				/**
				 * @brief サウンドの更新
				*/
				void UpdateSound();

			private:	// データメンバ
				CPlayer* m_playerRef = nullptr;			//!< プレイヤーの参照
				Vector3 m_moveVec = Vector3::Zero;		//!< 移動ベクトル
				CharacterController m_charaCon;			//!< キャラクターコントローラー

				CPlayerWalkAndRun m_playerWalkAndRun;	//!< プレイヤーの歩きと走りクラス
				CPlayerSwingAction m_playerSwingAction;	//!< プレイヤーのスイングアクションクラス
				CPlayerWallRun m_playerWallRun;			//!< プレイヤーの壁を走る処理クラス
				CPlayerOnEnemy m_playerOnEnemy;			//!< プレイヤーの敵の上に乗るクラス
				bool m_useGravity = true;				//!< 重力を使用するか？

				nsSound::CSoundCue* m_landingSC = nullptr;	//!< 着地のサウンド
				bool m_landingSoundFlag = false;			//!< 着地のサウンドを流せるか？
			};
		}
	}
}

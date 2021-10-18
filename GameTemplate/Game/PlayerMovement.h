#pragma once
#include "Noncopyable.h"
#include "PlayerNormalMovement.h"
#include "PlayerSwingAction.h"
#include "PlayerConstData.h"
#include "GameTime.h"

namespace nsMyGame
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// 前方宣言
		class CPlayer;	// プレイヤークラス

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
				CPlayerMovement() = default;
				/**
				 * @brief デストラクタ
				*/
				~CPlayerMovement() = default;

			public:		// メンバ関数

				/**
				 * @brief 初期化
				 * @param[in] radius カプセルコライダーの半径
				 * @param[in] height カプセルコライダーの高さ
				 * @param[in,out] player プレイヤーの参照
				*/
				void Init(
					const float radius,
					const float height,
					CPlayer* player
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
				 * @brief 重力をかける
				*/
				void ApplyGravity()
				{
					m_moveVec.y -= nsPlayerConstData::nsPlayerMoveConstData::kGravityScale *
						nsTimer::GameTime().GetFrameDeltaTime();
					if (m_moveVec.y < -nsPlayerConstData::nsPlayerMoveConstData::kMaxFallSpeed)
					{
						m_moveVec.y = -nsPlayerConstData::nsPlayerMoveConstData::kMaxFallSpeed;
					}
				}

				/**
				 * @brief キャラクターコントローラーを使った移動
				*/
				void MoveWithCharacterController();

				/**
				 * @brief 空中か？を得る
				 * @return 空中か？
				*/
				bool IsAir() const
				{
					return !m_charaCon.IsOnGround();
				}


			private:	// privateなメンバ関数

				/**
				 * @brief プレイヤーの移動を更新
				*/
				void UpdateMovePlayer();

				/**
				 * @brief プレイヤーの回転を更新
				*/
				void UpdateTurnPlayer();

			private:	// データメンバ
				CPlayer* m_playerRef = nullptr;			//!< プレイヤーの参照
				Vector3 m_moveVec = Vector3::Zero;		//!< 移動ベクトル
				CharacterController m_charaCon;			//!< キャラクターコントローラー

				CPlayerNormalMovement m_playerNormalMovement;	//!< プレイヤーの通常の動きクラス
				CPlayerSwingAction m_playerSwingAction;	//!< プレイヤーのスイングアクションクラス


			};
		}
	}
}

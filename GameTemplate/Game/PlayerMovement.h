#pragma once
#include "Noncopyable.h"
#include "PlayerWalkAndRun.h"
#include "PlayerSwingAction.h"

// デバック用
#ifdef MY_DEBUG
#include "FontRender.h"
#endif

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
				~CPlayerMovement()
				{
#ifdef MY_DEBUG
					DeleteGO(m_moveVecFont);
#endif
					return;
				}

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
				 * @brief 移動ベクトルを設定する
				 * @param[in] addMoveVec 移動ベクトル
				*/
				void SetAddMoveVec(const Vector3& addMoveVec)
				{
					m_addMoveVec = addMoveVec;
				}

				/**
				 * @brief 移動ベクトルを得る
				 * @return 移動ベクトル
				*/
				const Vector3& GetAddMoveVec() const
				{
					return m_addMoveVec;
				}

				/**
				 * @brief 移動べクトルを正規化する
				*/
				void NormalizeAddMoveVec()
				{
					m_addMoveVec.Normalize();
				}

				/**
				 * @brief 空中か？を得る
				 * @return 空中か？
				*/
				bool IsAir() const
				{
					return m_charaCon.IsJump();
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

				CharacterController m_charaCon;			//!< キャラクターコントローラー

				CPlayerWalkAndRun m_playerWalkAndRun;	//!< プレイヤーの歩きと走りクラス
				CPlayerSwingAction m_playerSwingAction;	//!< プレイヤーのスイングアクションクラス

				Vector3 m_moveVec = Vector3::Zero;		//!< 移動ベクトル
				Vector3 m_addMoveVec = Vector3::Zero;	//!< 加算移動ベクトル


				// デバック用
#ifdef MY_DEBUG
				nsGraphic::nsFont::CFontRender* m_moveVecFont = nullptr;
#endif

			};
		}
	}
}

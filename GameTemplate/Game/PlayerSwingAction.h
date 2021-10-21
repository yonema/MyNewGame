#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"


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
			 * @brief プレイヤーのスイングのアクションクラス
			*/
			class CPlayerSwingAction : private nsUtil::Noncopyable
			{
			private:	// エイリアス宣言
				// スイングアクションのステート
				using EnSwingActionState = nsPlayerConstData::nsPlayerSwingActionConstData::EnSwingActionState;

			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CPlayerSwingAction() = default;
				/**
				 * @brief デストラクタ
				*/
				~CPlayerSwingAction() = default;

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
				 * @brief スイングアクションを実行
				*/
				void Execute();

				/**
				 * @brief スイング中の速度をリセットする
				*/
				void ResetSwingSpeed()
				{
					m_swingSpeed = 0.0f;
					m_swingSpeed = -100.0f;
				}

				/**
				 * @brief スイング中のスピードを得る
				 * @return スイング中のスピード
				*/
				float GetSwingSpeed() const
				{
					return m_swingSpeed;
				}

			private:	// privateなメンバ関数

				/**
				 * @brief スイングターゲットを探す
				*/
				void FindSwingTarget();

				/**
				 * @brief 糸を伸ばしている最中の処理
				*/
				void StringStretching();

				/**
				 * @brief スイングアクションの処理
				*/
				void SwingAction();

				/**
				 * @brief スイング後の空中の処理
				*/
				void AirAfterSwing();

			private:	// データメンバ
				CPlayer* m_playerRef = nullptr;					//!< プレイヤークラスの参照
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< プレイヤー移動クラスの参照
				const Vector3* m_swingTargetPos = nullptr;		//!< スイングターゲットの座標
				//!< スイングアクションのステート
				EnSwingActionState m_swingActionState = 
					nsPlayerConstData::nsPlayerSwingActionConstData::enFindSwingTarget;
				float m_swingRadAngle = 0.0f;						//!< スイングのラジアン角度
				int m_swingCounter = 0;
				float m_swingSpeed = 0.0f;
				float m_swingSpeed2 = -100.0f;
				Vector3 m_inputMoveDirXZ = Vector3::Zero;

			};
		}
	}
}

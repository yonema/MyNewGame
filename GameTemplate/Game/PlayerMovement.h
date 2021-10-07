#pragma once
#include "Noncopyable.h"
#include "PlayerWalkAndRun.h"

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
		class CPlayerCamera;	// プレイヤーカメラクラス
		struct SPlayerInputData;	//プレイヤーの入力情報構造体

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
				 * @param[in,out] position プレイヤーの座標の参照
				 * @param[in,out] rotation プレイヤーの回転の参照
				 * @param[in] playerCamera プレイヤーカメラ
				 * @param[in] playerInputData プレイヤー入力情報
				*/
				void Init(
					const float radius,
					const float height,
					Vector3* position,
					Quaternion* rotation,
					const CPlayerCamera& playerCamera,
					const SPlayerInputData& playerInputData
				);

				/**
				 * @brief プレイヤーの移動クラスのアップデートを実行する
				*/
				void ExecuteUpdate();


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
				CharacterController m_charaCon;			//!< キャラクターコントローラー

				CPlayerWalkAndRun m_playerWalkAndRun;	//!< プレイヤーの歩きと走りクラス

				Vector3 m_moveVec = Vector3::Zero;			//!< 移動ベクトル
				Vector3 m_addMoveVec = Vector3::Zero;		//!< 加算移動ベクトル

				const CPlayerCamera* m_playerCamera = nullptr;			//!< プレイヤーカメラの参照
				const SPlayerInputData* m_playerInputData = nullptr;	//!< プレイヤー入力情報の参照
				Vector3* m_playerPosition = nullptr;					//!< プレイヤーの座標の参照
				Quaternion* m_playerRotation = nullptr;					//!< プレイヤーの回転の参照

				// デバック用
#ifdef MY_DEBUG
				nsGraphic::nsFont::CFontRender* m_moveVecFont = nullptr;
#endif

			};
		}
	}
}

#pragma once
#include "PlayerModelAnimation.h"
#include "PlayerMovement.h"
#include "PlayerInput.h"
#include "PlayerCamera.h"
#include "PlayerStringModel.h"
#include "PlayerCatchEnemy.h"
#include "PlayerConstData.h"

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsPlayer { class CPlayerModelAnimation; }
	namespace nsAICharacter { class CAICar; }

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		/**
		 * @brief プレイヤークラス
		*/
		class CPlayer : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPlayer() = default;
			/**
			 * @brief コンストラクタ
			*/
			~CPlayer() = default;

		public:		// オーバーライドしたメンバ関数

			/**
			 * @brief スタート関数
			 * @return アップデートを行うか？
			*/
			bool Start() override final;

			/**
			 * @brief 破棄した時に呼ばれる関数
			*/
			void OnDestroy() override final;

			/**
			 * @brief アップデート関数
			*/
			void Update() override final;

		public:		// メンバ関数

			/**
			 * @brief 入力可能か？を設定する
			 * @param[in] isInputtable 入力可能か？
			*/
			void SetIsInputtable(const bool isInputtable)
			{
				m_isInputtable = isInputtable;
			}

			/**
			 * @brief 入力可能か？を得る
			 * @return 入力可能か？
			*/
			bool IsInputtable() const
			{
				return m_isInputtable;
			}

			/**
			 * @brief タイトルモードに設定
			*/
			void TitleMode()
			{
				m_isTitleMode = true;
				SetIsInputtable(false);
			}

			/**
			 * @brief タイトルモードか？を得る
			 * @return タイトルモードか？
			*/
			bool IsTitleMode() const
			{
				return m_isTitleMode;
			}

			/**
			 * @brief 座標を設定する
			 * @param[in] pos 座標
			*/
			void SetPosition(const Vector3& pos)
			{
				m_position = pos;
			}

			/**
			 * @brief 座標を得る
			 * @return 座標
			*/
			const Vector3& GetPosition() const
			{
				return m_position;
			}

			/**
			 * @brief 回転をセットする
			 * @param[in] rot 回転
			*/
			void SetRotation(const Quaternion& rot)
			{
				m_rotation = rot;
			}

			/**
			 * @brief 回転を得る
			 * @return 回転
			*/
			const Quaternion& GetRotation() const
			{
				return m_rotation;
			}

			/**
			 * @brief プレイヤーの入力情報を得る
			 * @return プレイヤーの入力情報
			*/
			const SPlayerInputData& GetInputData() const
			{
				return m_playerInput->GetPlayerInputData();
			}

			/**
			 * @brief プレイヤーのカメラを得る
			 * @return プレイヤーのカメラ
			*/
			const CPlayerCamera& GetCamera() const
			{
				return *m_playerCamera;
			}

			/**
			 * @brief プレイヤーの移動クラスの参照を得る
			 * @return プレイヤーの移動クラスの参照
			*/
			const nsPlayerMovenent::CPlayerMovement& GetPlayerMovement() const
			{
				return *m_playerMove;
			}

			/**
			 * @brief プレイヤーモデルアニメーションクラスの参照を得る
			 * @return プレイヤーモデルアニメーションクラスの参照
			*/
			const CPlayerModelAnimation& GetPlayerModelAnimation() const
			{
				return *m_playerModelAnimation;
			}

			/**
			 * @brief ターゲットの敵の参照を得る
			 * @return ターゲットの敵の参照
			*/
			const CPlayerCatchEnemy& GetCatchEnemy() const
			{
				return *m_playerCatchEnemy;
			}

			/**
			 * @brief 歩きと走り状態へ遷移する
			*/
			void ChangeWalkAndRunState();

			/**
			 * @brief スイング状態へ遷移する
			*/
			void ChangeSwingState();

			/**
			 * @brief 壁走り状態へ遷移する
			*/
			void ChangeWallRunState();

			/**
			 * @brief 敵の上に乗っている状態へ遷移する
			*/
			void ChangeOnEnemyState();

			/**
			 * @brief プレイヤーのステートを得る
			 * @return ステート
			*/
			nsPlayerConstData::EnPlayerState GetState() const
			{
				return m_playerState;
			}

			/**
			 * @brief 糸が指定した座標に向かって伸びる処理を開始する
			 * @param[in] pos 伸びる先の座標
			*/
			void StartStringStretchToPos(const Vector3& pos);

			/**
			 * @brief 伸びる先の座標を設定する
			 * @param[in] pos 座標
			*/
			void SetStringStretchPos(const Vector3& pos);

			/**
			 * @brief 糸が指定した座標に向かって伸びる処理を終了する
			*/
			void EndStringStretchToPos();

			/**
			 * @brief 糸が伸びきっているか？を得る
			 * @return 糸が伸びきっているか？
			*/
			bool IsStringStretched() const
			{
				return m_playerStringModel->IsStretched();
			}

			/**
			 * @brief 糸の長さを得る
			 * @return 糸の長さ
			*/
			float GetStringLength() const
			{
				return m_playerStringModel->GetStringLength();
			}

			/**
			 * @brief 敵の上に乗っているカメラか？を設定
			 * @param[in] isOnEnemyCamera 敵の上に乗っているカメラか？
			*/
			void SetIsOnEnemyCamera(const bool isOnEnemyCamera)
			{
				m_playerCamera->SetIsOnEnemyCamera(isOnEnemyCamera);
				if (isOnEnemyCamera)
					m_playerCamera->LerpDampingRate(1.0f);
				else
					m_playerCamera->LerpDampingRate(0.0f);
			}

			/**
			 * @brief 敵の上に乗る処理クラスを初期状態に戻す
			*/
			void ResetOnEnemy()
			{
				m_playerMove->ResetOnEnemy();
			}

		private:	// privateなメンバ関数

			/**
			 * @brief ステート遷移
			 * @param newState[in] 新しいステート
			*/
			void ChangeState(nsPlayerConstData::EnPlayerState newState);

		private:	// データメンバ
			Vector3 m_position = Vector3::Zero;				//!< 座標
			Quaternion m_rotation = Quaternion::Identity;	//!< 回転

			std::unique_ptr<CPlayerModelAnimation> m_playerModelAnimation;	//!< プレイヤーモデルアニメーションクラス
			std::unique_ptr <CPlayerInput> m_playerInput;						//!< プレイヤー入力クラス
			std::unique_ptr <CPlayerCamera> m_playerCamera;					//!< プレイヤーカメラクラス
			std::unique_ptr <nsPlayerMovenent::CPlayerMovement> m_playerMove;	//!< プレイヤー移動クラス
			std::unique_ptr <CPlayerCatchEnemy> m_playerCatchEnemy;			//!< プレイヤーが敵を捕まえる処理クラス
			//!< プレイヤーのステート
			nsPlayerConstData::EnPlayerState m_playerState = nsPlayerConstData::enWalkAndRun;
			CPlayerStringModel* m_playerStringModel = nullptr;	//!< プレイヤーの糸のモデルクラス

			bool m_isInputtable = true;		//!< 入力可能か？
			bool m_isTitleMode = false;		//!< タイトルモードか？
		};

	}
}



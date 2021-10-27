#pragma once
#include "PlayerModelAnimation.h"
#include "PlayerMovement.h"
#include "PlayerInput.h"
#include "PlayerCamera.h"
#include "PlayerStringModel.h"
#include "PlayerConstData.h"

namespace nsMyGame
{

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// 前方宣言
		class CPlayerModelAnimation;	// プレイヤーモデルアニメーションクラス

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
				return m_playerInput.GetPlayerInputData();
			}

			/**
			 * @brief プレイヤーのカメラを得る
			 * @return プレイヤーのカメラ
			*/
			const CPlayerCamera& GetCamera() const
			{
				return m_playerCamera;
			}

			/**
 * @brief プレイヤーのカメラを得る
 * @return プレイヤーのカメラ
*/
			CPlayerCamera& GetCamera()
			{
				return m_playerCamera;
			}

			/**
			 * @brief プレイヤーの移動クラスの参照を得る
			 * @return プレイヤーの移動クラスの参照
			*/
			const nsPlayerMovenent::CPlayerMovement& GetPlayerMovement() const
			{
				return m_playerMove;
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
			void ChangeWallRun();

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

		private:	// privateなメンバ関数

		private:	// データメンバ
			Vector3 m_position = Vector3::Zero;				//!< 座標
			Quaternion m_rotation = Quaternion::Identity;	//!< 回転

			CPlayerModelAnimation m_playerModelAnimation;	//!< プレイヤーモデルアニメーションクラス
			CPlayerInput m_playerInput;						//!< プレイヤー入力クラス
			CPlayerCamera m_playerCamera;					//!< プレイヤーカメラクラス
			nsPlayerMovenent::CPlayerMovement m_playerMove;	//!< プレイヤー移動クラス
			//!< プレイヤーのステート
			nsPlayerConstData::EnPlayerState m_playerState = nsPlayerConstData::enWalkAndRun;
			CPlayerStringModel* m_playerStringModel = nullptr;	//!< プレイヤーの糸のモデルクラス
		};

	}
}



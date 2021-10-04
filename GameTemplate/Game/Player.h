#pragma once
#include "PlayerModel.h"
#include "PlayerMovement.h"
#include "PlayerInput.h"
#include "PlayerCamera.h"

namespace nsMyGame
{

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// 前方宣言
		class CPlayerModel;	// プレイヤーモデルクラス

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
			 * @brief 初期位置を設定する
			 * @param[in] startPos スタート座標
			*/
			void SetStartPosition(const Vector3& startPos)
			{
				// スタート関数がまだ完了していないか？
				if (!IsStart())
					m_position = startPos;
			}

		private:	// privateなメンバ関数

		private:	// データメンバ
			Vector3 m_position = Vector3::Zero;				//!< 座標
			Quaternion m_rotation = Quaternion::Identity;	//!< 回転

			CPlayerModel m_playerModel;						//!< プレイヤーモデルクラス
			CPlayerInput m_playerInput;						//!< プレイヤー入力クラス
			CPlayerCamera m_playerCamera;					//!< プレイヤーカメラクラス
			nsPlayerMovenent::CPlayerMovement m_playerMove;						//!< プレイヤー移動クラス

			SPlayerInputData m_playerInputData;				//!< プレイヤーの入力情報
		};

	}
}



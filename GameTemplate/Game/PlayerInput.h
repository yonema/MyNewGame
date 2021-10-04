#pragma once
#include "Noncopyable.h"

namespace nsMyGame
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		/**
		 * @brief プレイヤーの入力情報構造体
		*/
		struct SPlayerInputData
		{
			float axisMoveForward = 0.0f;			//!< 前、後移動の軸入力情報
			float axisMoveRight = 0.0f;				//!< 右、左移動の軸入力情報
			float axisCameraRotVertical = 0.0f;		//!< カメラの縦回転の軸入力情報
			float axisCameraRotHorizontal = 0.0f;	//!< カメラの横回転の軸入力情報
			bool actionJump = false;				//!< ジャンプの入力情報
		};

		/**
		 * @brief プレイヤー入力クラス
		*/
		class CPlayerInput : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPlayerInput() = default;
			/**
			 * @brief コンストラクタ
			*/
			~CPlayerInput() = default;

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param[in,out] playerInputData プレイヤーの入力情報
			*/
			void Init(SPlayerInputData* playerInputData);

			/**
			 * @brief 入力処理を実行する
			*/
			void ExecuteInput();

		private:	// privateなメンバ関数

			/**
			 * @brief 軸入力情報を更新する
			*/
			void UpdateInputAxis();

			/**
			 * @brief アクション入力情報を更新する
			*/
			void UpdateInputAction();

			/**
			 * @brief 移動の軸入力情報の更新
			*/
			void OnUpdateInputAxisMove();

			/**
			 * @brief カメラの回転の軸入力情報の更新
			*/
			void OnUpdateInputAxisCameraRot();

		private:	// データメンバ
			const GamePad* m_pad = nullptr;					//!<  ゲームパッド
			SPlayerInputData* m_playerInputData = nullptr;	//!< プレイヤーの入力情報の参照

		};
	}
}


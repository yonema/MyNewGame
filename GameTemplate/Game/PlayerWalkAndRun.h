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
				 * @brief 移動速度を加速させる
				 * @param inputMoveF 前、後の軸入力
				 * @param inputMoveR 右、左の軸入力
				*/
				void Acceleration(const float inputMoveF, const float inputMoveR);

				/**
				 * @brief 摩擦の計算
				 * @param[in] absInputMoveF 前、後の軸入力の絶対値
				 * @param[in] absInputMoveR 右、左の軸入力の絶対値
				*/
				void Friction(const float absInputMoveF, const float absInputMoveR);

				/**
				 * @brief 移動速度に速度制限をかける
				 * @param[in] absInputMoveF 前、後の軸入力の絶対値
				 * @param[in] absInputMoveR 右、左の軸入力の絶対値
				*/
				void LimitSpeed(const float absInputMoveF, const float absInputMoveR);

			private:	// データメンバ
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< 移動クラスの参照
				const CPlayer* m_playerRef = nullptr;			//!< プレイヤーの参照
				float m_oldVelocity = 0.0f;						//!< 前のフレームの速度
			};

		}
	}
}
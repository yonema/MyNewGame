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
			 * @brief プレイヤーの壁を走る処理クラス
			*/
			class CPlayerWallRun : private nsUtil::Noncopyable
			{
			public:		// コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CPlayerWallRun() = default;
				/**
				 * @brief デストラクタ
				*/
				~CPlayerWallRun() = default;

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
				 * @brief ウォールランを実行
				*/
				void Execute();

			private:	// データメンバ
				CPlayer* m_playerRef = nullptr;					//!< プレイヤークラスの参照
				CPlayerMovement* m_playerMovementRef = nullptr;	//!< プレイヤー移動クラスの参照


			};
		}
	}
}

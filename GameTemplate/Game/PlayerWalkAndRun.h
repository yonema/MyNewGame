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
		class CPlayerCamera;		// プレイヤーカメラ
		struct SPlayerInputData;	//プレイヤーの入力情報構造体

		/**
		 * @brief プレイヤーの動き関連のネームスペース
		*/
		namespace nsPlayerMovenent
		{
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
				 * @param[in,out] moveVecForward 前向きの移動ベクトルの参照
				 * @param[in,out] moveVecRight 右向きの移動ベクトルの参照
				 * @param[in] camera プレイヤーカメラ
				 * @param[in] playerInputData プレイヤー入力情報
				*/
				void Init(
					Vector3* moveVecForward,
					Vector3* moveVecRight,
					const CPlayerCamera& camera,
					const SPlayerInputData& playerInputData
				);

				/**
				 * @brief 歩きと走りの処理を実行
				 * @param[in] isAir 空中か？
				*/
				void Execute(const bool isAir);

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
				 * @param[in] isAir 空中か？
				*/
				void Friction(const float absInputMoveF, const float absInputMoveR, const bool isAir);

				/**
				 * @brief 移動速度に速度制限をかける
				 * @param[in] absInputMoveF 前、後の軸入力の絶対値
				 * @param[in] absInputMoveR 右、左の軸入力の絶対値
				*/
				void LimitSpeed(const float absInputMoveF, const float absInputMoveR);

			private:	// データメンバ
				Vector3* m_moveVecForward = nullptr;			//!< 前向きの移動ベクトル
				Vector3* m_moveVecRight = nullptr;				//!< 右向きの移動ベクトル
				const CPlayerCamera* m_playerCamera = nullptr;	//!< プレイヤーカメラ
				const SPlayerInputData* m_playerInputData = nullptr;	//!< プレイヤーの入力情報
			};

		}
	}
}
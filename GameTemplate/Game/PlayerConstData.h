#pragma once

namespace nsMyGame
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		/**
		 * @brief プレイヤーの定数データ
		*/
		namespace nsPlayerConstData
		{
			constexpr float kCapsuleRadius = 50.0f;		//!< カプセルコライダーの半径
			constexpr float kDapsuleHeight = 100.0f;	//!< カプセルコライダーの高さ

			/**
			 * @brief プレイヤーのステート
			*/
			enum EnPlayerState
			{
				enWalkAndRun,			//!< 歩きと走り
				enSwing,				//!< スイング
				enWallRun,				//!< 壁走り
			};


			/**
			 * @brief プレイヤー移動クラスの定数データ
			*/
			namespace nsMovementConstData
			{
				constexpr float kGravityScale = 980.0f * 3.0f;	//!< 重力の強さ
				constexpr float kMaxFallSpeed = 2000.0f;		//!< 最高落下速度
				constexpr float kJumpForce = 1500.0f;	//!< ジャンプ力
				constexpr float kModelRotRate = 0.3f;	//!< モデルの回転の補間率
				constexpr float kMoveVecMin = 0.001f;	//!< 移動ベクトルの最小値
			}

			/**
			 * @brief プレイヤーの歩きと走りクラスの定数データ
			*/
			namespace nsWalkAndRunConstData
			{
				constexpr float kWalkAcceleration = 50.0f;	//!< 歩き時の加速度
				constexpr float kRunAcceleration = 100.0f;	//!< 走り時の加速度
				constexpr float kWalkMaxSpeed = 750.0f;		//!< 歩き時の最高速度
				constexpr float kRunMaxSpeed = 1200.0f;		//!< 走り時の最高速度
				constexpr float kMinSpeed = 2.0f;			//!< 最低速度
				constexpr float kGroundFriction = 0.9f;		//!< 地面の摩擦
				constexpr float kAirFriction = 1.0f;		//!< 空中の摩擦
				constexpr float kBreakThresholdAngle = 135.0f;		//!< ブレーキの角度のしきい値
				constexpr float kBreakThresholdVelocity = 135.0f;	//!< ブレーキの速度のしきい値
			}

			/**
			 * @brief プレイヤーのスイングアクションクラスの定数データ
			*/
			namespace nsSwingActionConstData
			{
				/**
				 * @brief スイングアクションのステート
				*/
				enum EnSwingActionState
				{
					enFindSwingTarget,		//!< スイングターゲットを探す
					enIsStringStretching,	//!< 糸を伸ばし中
					enIsSwinging,			//!< スイング中
					enIsAirAfterSwing,		//!< スイング後の空中
					enEnd,					//!< 終了
				};

				constexpr int kFindSwintTargetNum = 4;	//!< スイングターゲットを探す数
				//!< スイングターゲットを探す座標へのベクトル集
				static const Vector3 kToFindSwingTargetVecs[kFindSwintTargetNum] =
				{
					{ 0.0f,2000.0f,2000.0f },		// 前上
					{ 500.0f,2000.0f,2000.0f },		// 前上
					{ -500.0f,2000.0f,2000.0f },	// 前上
					{ 0.0f,0.0f,0.0f }				// 中心
				};
				//!< 優先度が高いスイングターゲットの数
				constexpr int kHighPriorityFindSwintTargetNum = 3;
				//!< スイングターゲットを探す有効範囲の半径
				constexpr float kFindSwingTargetScope = 4000.0f;

				// 減速し始めるスピードの初期値
				constexpr float kStartDecelerateSwingSpeedInitialValue = -100.0f;

				constexpr float kInitialSwingSpeed = 2000.0f;	//!< 最初のスイングスピード
				//!< 落下速度が最初のスイングスピードに与える影響率
				constexpr float kFallImpactRateForInitialSwingSpeed = 0.5f;
				constexpr float kMaxSwingSpeed = 4000.0f;	//!< 最大スイングスピード
				//!< スイングの加速を引き継ぐ割合
				constexpr float kTakeOverSwingAccelerationRate = 0.5f;

				//!< スイング後の加速の初速
				constexpr float kInitialVelocityOfAterSwingAcceleration = 1500.0f;
				//!< スイング後の加速の最低速度
				constexpr float kMinVelocityOfAfterSwingAcceleration = 
					kInitialVelocityOfAterSwingAcceleration * 0.1f;

				// カメラの値を線形補完するタイム
				constexpr float kCameraLerpTime = 0.25f;
			}
			/**
			 * @brief プレイヤーモデルクラスの定数データ
			*/
			namespace nsModelAnimationConstData
			{
				//!< プレイヤーのモデルデータのファイスパス
				constexpr const char* const kPlayerModelFilePath = 
					"Assets/modelData/kunoichi_ver2.1/kunoichi_ver2.1.tkm";

				/**
				 * @brief アニメーションクリップス
				*/
				enum EnAnimationClips
				{
					enAnim_idle,	//!< アイドル
					//enAnim_ninjaIdle,	//!< 忍者アイドル
					enAnim_walk,	//!< 歩く
					enAnim_run,		//!< 走る
					enAnim_jumpUp,	//!< ジャンプ
					enAnim_airIdle,	//!< 空中アイドル
					enAnim_swingStart,	//!< スイングスタート
					enAnim_swinging,	//!< スインギング
					enAnim_swingRaiseLeg,	//!< 足を上げるスイング
					enAnim_swingRoll,	//!< スイングロール
					enAnim_num		//!< アニメーションクリップの数
				};

				//!< アニメーションのファイルパス
				constexpr const char* const kAnimationFilePath[enAnim_num] =
				{
					"Assets/animData/kunoichi/idle.tka",
					"Assets/animData/kunoichi/walk.tka",
					"Assets/animData/kunoichi/run.tka",
					"Assets/animData/kunoichi/jumpUp.tka",
					"Assets/animData/kunoichi/airIdle.tka",
					"Assets/animData/kunoichi/swingStart.tka",
					"Assets/animData/kunoichi/swinging.tka",
					"Assets/animData/kunoichi/swingRaiseLeg.tka",
					"Assets/animData/kunoichi/swingRoll.tka",

				};

				constexpr float kDefaultAnimInterpolateTime = 0.2f;	//!< デフォルトのアニメーション補完時間

				//!< スイング中のアニメーションステート
				enum EnSwingAnimState
				{
					enSwingAnim_swingStart,		//!< スイング開始
					enSwingAnim_swing,			//!< スイング中
					enSwingAnim_swingRaiseLeg,	//!< 足を上げるスイング中
					enSwingAnim_swingRoll,		//!< スイングロール
					enSwingAnim_airAfterSwing,	//!< スイング後の空中状態
				};

			}

			/**
			 * @brief プレイヤーの糸のモデルクラスの定数データ
			*/
			namespace nsStringModelConstData
			{
				//!< 糸のモデルファイルパス
				constexpr const char* const kStringModelFilePath = "Assets/modelData/props/chain.tkm";
				constexpr float kStretchedTime = 0.3f;	//!< 糸が伸びきるまでの時間
				constexpr float kStartStretchHeight = 150.0f;	//!< 糸が伸びる開始座標の高さ
			}

			/**
			 * @brief プレイヤー入力クラスの定数データ
			*/
			namespace nsInputConstData
			{
				constexpr float kDPadInputPower = 1.0f;					//!< 十字キー入力の移動量
				static const float kSquareRootOfTwo = std::sqrtf(2.0f);	//!< ルート2の値
				constexpr float kInputAxisMin = 0.001f;					//!< 軸入力の最小値
			}

			/**
			 * @brief プレイヤーカメラの定数データ
			*/
			namespace nsCameraConstData
			{
				constexpr float kCameraMaxSpeed = 10000.0f;	//!< カメラの最大速度
				constexpr float kCameraRadius = 5.0f;		//!< カメラのコリジョンの半径

				constexpr float kTargetOffsetUp = 120.0f;		//!< 注視点の上下のオフセット
				constexpr float kNearTargetOffsetUp = 180.0f;	//!< 注視点の上下のオフセット
				constexpr float kTargetOffsetForward = 20.0f;	//!< 注視点の前後のオフセット

				constexpr float kDefaultCameraFar = 250000.0f;	//!< デフォルトの遠平面

				//!< デフォルトの注視点から視点へのベクトル
				static const Vector3 kDefaultToCameraVec = { 0.0f,1.0f,-10.0f };

				//!< デフォルトの注視点から視点への距離
				constexpr float kDefaultToCameraDistance = 300.0f;
				constexpr float kNearToCameraDistance = 190.0f;

				constexpr float kCameraRotSpeed = 3.0f;		//!< カメラの回転するスピード
				//!< 注視点から視点への方向ベクトルのYの最大値。カメラの下向きの上限。
				constexpr float kMaxToCameraDirY = 0.8f;
				//!< 注視点から視点への方向ベクトルのYの最小値。カメラの上向きの上限。
				constexpr float kMinToCameraDirY = -0.5f;

				//!< バネの減衰率。値が大きいほど、カメラが遅れて追従してくる。
				constexpr float kSpringDampingRate = 0.7f;
				//!< 最大のばねの減衰率。通常より遅れてカメラが追従してくる。
				constexpr float kMaxSpringDampingRate = 0.9f;

				constexpr float kAutoTurnStartTime = 1.0f;	//!< 自動でカメラが回転し始めるタイム
				//!< 自動でカメラが回転時始めるタイマーをリセットするタイム
				constexpr float kAutoTurnStartTimerResetTime = 0.1f;
				//!< カメラが自動で回転するスピード率の累乗の数
				constexpr float kAutoTurnSpeedRatePowPower = 2.0f;
				constexpr float kAutoTurnSpeedMin = 0.01f;	//!< カメラが自動で回転する最小スピード
				constexpr float kAutoTurnSpeedMax = 0.1f;	//!< カメラが自動で回転する最大スピード
				constexpr float kAutoTurnExecuteThreshold = 0.99f;	//!< カメラが自動で回転を実行するしきい値
				constexpr float kAutoTurnYRate = 0.03f;		//!< カメラのY座標が自動で回転する率
			}
		}
	}
}
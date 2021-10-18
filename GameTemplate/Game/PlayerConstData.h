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
				enAirAfterStringAction,	//!< 糸を使ったアクションの後の空中状態

			};


			/**
			 * @brief プレイヤー移動クラスの定数データ
			*/
			namespace nsPlayerMoveConstData
			{
				constexpr float kGravityScale = 980.0f * 2.0f;	//!< 重力の強さ
				constexpr float kMaxFallSpeed = 2000.0f;		//!< 最高落下速度
				constexpr float kJumpForce = 700.0f;	//!< ジャンプ力
				constexpr float kModelRotRate = 0.3f;	//!< モデルの回転の補間率
				constexpr float kMoveVecMin = 0.001f;	//!< 移動ベクトルの最小値
			}

			/**
			 * @brief プレイヤーの通常の動きクラスの定数データ
			*/
			namespace nsPlayerNormalMovementConstData
			{
				constexpr float kWalkAcceleration = 20.0f;	//!< 歩き時の加速度
				constexpr float kRunAcceleration = 40.0f;	//!< 走り時の加速度
				constexpr float kWalkMaxSpeed = 500.0f;		//!< 歩き時の最高速度
				constexpr float kRunMaxSpeed = 1000.0f;		//!< 走り時の最高速度
				constexpr float kMinSpeed = 2.0f;			//!< 最低速度
				constexpr float kGroundFriction = 0.9f;		//!< 地面の摩擦
				constexpr float kAirFriction = 1.0f;		//!< 空中の摩擦
				constexpr float kBreakThresholdAngle = 135.0f;		//!< ブレーキの角度のしきい値
				constexpr float kBreakThresholdVelocity = 135.0f;	//!< ブレーキの速度のしきい値
			}

			/**
			 * @brief プレイヤーのスイングアクションクラスの定数データ
			*/
			namespace nsPlayerSwingActionConstData
			{
				/**
				 * @brief スイングアクションのステート
				*/
				enum EnSwingActionState
				{
					enFindSwingTarget,		//!< スイングターゲットを探す
					enIsStringStretching,	//!< 糸を伸ばし中
					enIsSwinging,			//!< スイング中
					enEnd,					//!< 終了
				};
			}
			/**
			 * @brief プレイヤーモデルクラスの定数データ
			*/
			namespace nsPlayerModelRenderConstData
			{
				//!< プレイヤーのモデルデータのファイスパス
				constexpr const char* const kPlayerModelFilePath = "Assets/modelData/unityChan.tkm";

				/**
				 * @brief アニメーションクリップス
				*/
				enum EnAnimationClips
				{
					enAnim_idle,	//!< アイドル
					enAnim_walk,	//!< 歩く
					enAnim_num		//!< アニメーションクリップの数
				};

				//!< アイドル状態のアニメーションファイルパス
				constexpr const char* const kAnimationFilePath_idle = "Assets/animData/idle.tka";
				//!< 歩き状態のアニメーションファイルパス
				constexpr const char* const kAnimationFilePath_walk = "Assets/animData/walk.tka";

				constexpr float kDefaultAnimInterpolateTime = 0.2f;	//!< デフォルトのアニメーション補完時間

			}

			/**
			 * @brief プレイヤーの糸のモデルクラスの定数データ
			*/
			namespace nsPlayerStringModelConstData
			{
				//!< 糸のモデルファイルパス
				constexpr const char* const kStringModelFilePath = "Assets/modelData/testString.tkm";
				constexpr float kStretchedTime = 0.1f;	//!< 糸が伸びきるまでの時間
			}

			/**
			 * @brief プレイヤー入力クラスの定数データ
			*/
			namespace nsPlayerInputConstData
			{
				constexpr float kDPadInputPower = 1.0f;					//!< 十字キー入力の移動量
				static const float kSquareRootOfTwo = std::sqrtf(2.0f);	//!< ルート2の値
				constexpr float kInputAxisMin = 0.001f;					//!< 軸入力の最小値
			}

			/**
			 * @brief プレイヤーカメラの定数データ
			*/
			namespace nsPlayerCameraConstData
			{
				constexpr float kCameraMaxSpeed = 10000.0f;	//!< カメラの最大速度
				constexpr float kCameraRadius = 5.0f;		//!< カメラのコリジョンの半径

				constexpr float kTargetOffsetUp = 80.0f;		//!< 注視点の上下のオフセット
				constexpr float kTargetOffsetForward = 20.0f;	//!< 注視点の前後のオフセット

				constexpr float kDefaultCameraFar = 40000.0f;	//!< デフォルトの遠平面

				//!< デフォルトの注視点から視点へのベクトル
				static const Vector3 kDefaultToCameraVec = { 0.0f,50.0f,-400.0f };

				constexpr float kCameraRotSpeed = 3.0f;		//!< カメラの回転するスピード
				//!< 注視点から視点への方向ベクトルのYの最大値。カメラの下向きの上限。
				constexpr float kMaxToCameraDirY = 0.8f;
				//!< 注視点から視点への方向ベクトルのYの最小値。カメラの上向きの上限。
				constexpr float kMinToCameraDirY = -0.5f;

				//!< バネの減衰率。値が大きいほど、カメラが遅れて追従してくる
				constexpr float kSpringDampingRate = 0.7f;

			}
		}
	}
}
#pragma once
#include "Noncopyable.h"
#include "SpringCamera.h"
#include "PlayerConstData.h"

// ここ改造
//#include "SpriteRender.h"

namespace nsNinjaAttract
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// 前方宣言
		class CPlayer;	// プレイヤーの入力情報構造体

		/**
		 * @brief プレイヤーのカメラクラス
		*/
		class CPlayerCamera : private nsUtil::Noncopyable
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CPlayerCamera() = default;
			/**
			 * @brief デストラクタ
			*/
			~CPlayerCamera() = default;

		public:		// メンバ関数

			/**
			 * @brief 初期化
			 * @param[in] player プレイヤーの参照
			*/
			void Init(const CPlayer& player);

			/**
			 * @brief プレイヤーカメラクラスのアップデートを実行する
			*/
			void ExecuteUpdate();

			/**
			 * @brief カメラの前方向のベクトルを取得
			 * @return カメラの前方向のベクトル
			*/
			const Vector3& GetCameraForward() const
			{
				return m_camera->GetForward();
			}

			/**
			 * @brief カメラの右方向のベクトルを取得
			 * @return カメラの右方向のベクトル
			*/
			const Vector3& GetCameraRight() const
			{
				return m_camera->GetRight();
			}

			/**
			 * @brief カメラの座標を取得
			 * @return カメラの座標
			*/
			const Vector3& GetCameraPosition() const
			{
				return m_camera->GetPosition();
			}

			/**
			 * @brief バネカメラの減衰率を線形補完する。
			 * 0.0fがデフォルトで最小の減衰率。1.0fが最大の減衰率。
			 * 減衰率が高いほどカメラが遅れて付いてくる。
			 * @param[in] rate 減衰率の線形補完率
			*/
			void LerpDampingRate(const float rate)
			{
				m_springCamera.SetDampingRate(Math::Lerp<float>(
					rate,
					nsPlayerConstData::nsCameraConstData::kSpringDampingRate,
					nsPlayerConstData::nsCameraConstData::kMaxSpringDampingRate
					));
			}

			/**
			 * @brief 注視点の上下のオフセットを線形補完する。
			 * 0.0fがデフォルトで最小のオフセット。1.0fが最大のオフセット。
			 * オフセットが大きいほどキャラクターが画面の下に映る。
			 * @param[in] rate 注視点の上下のオフセットの線形補完率
			*/
			void LerpTargetOffsetUp(const float rate)
			{
				m_targetOffsetUp = Math::Lerp<float>(
					rate,
					nsPlayerConstData::nsCameraConstData::kTargetOffsetUp,
					nsPlayerConstData::nsCameraConstData::kNearTargetOffsetUp
					);
			}

			/**
			 * @brief 敵の上に乗っているカメラか？を設定する
			 * @param[in] isOnEnemyCamera 敵の上に乗っているカメラか？
			*/
			void SetIsOnEnemyCamera(const bool isOnEnemyCamera)
			{
				m_isOnEnemyCamera = isOnEnemyCamera;
			}

			/**
			 * @brief 敵の上に乗っているカメラか？を得る
			 * @return 敵の上に乗っているカメラか？
			*/
			bool IsOnEnemyCamera() const
			{
				return m_isOnEnemyCamera;
			}


		private:	// privateなメンバ関数

			/**
			 * @brief カメラの回転を計算する
			*/
			void CalcCameraRotation();

			/**
			 * @brief 自動的にカメラをプレイヤーの移動先に向ける
			*/
			void AutoTurnToPlayerDestination();

			/**
			 * @brief 敵の上に乗っている時のカメラの処理
			*/
			void OnEnemyCamera();


			/**
			 * @brief カメラの視点と注視点を計算する
			 * @param[out] pos_out 視点が格納される
			 * @param[out] targetPos_out 注視点が格納される
			*/
			void CalcCameraPositionAndTargetPos(
				Vector3* pos_out,
				Vector3* targetPos_out
			);

			/**
			 * @brief バネカメラを更新する
			 * @param[in] pos 視点
			 * @param[in] targetPos 注視点
			*/
			void UpdateSpringCamera(const Vector3& pos, const Vector3& targetPos);

		private:	// データメンバ
			Camera* m_camera = nullptr;					//!< カメラ
			nsCamera::CSpringCamera m_springCamera;		//!< バネカメラ
			Vector3 m_toCameraVec = Vector3::Zero;		//!< 注視点から視点へのベクトル
			const CPlayer* m_playerRef = nullptr;		//!< プレイヤーの参照
			float m_autoTurnStartTimer = 0.0f;			//!< 自動でカメラが回転し始めるタイマー
			float m_autoTurnStartTimerResetTimer = 0.0f;	//!< 自動でカメラを回転し始めるタイマーをリセットするタイマー
			float m_targetOffsetUp = 0.0f;
			float m_cameraPositionOffsetUp = 0.0f;
			float m_toCameraDistance = 0.0f;
			bool m_isOnEnemyCamera = false;				//!< 敵の上に乗っているときのカメラか？

			// 改造ここから
			//Camera m_dummyCamera;
			//nsGraphic::nsSprite::CSpriteRender* m_titleSR = nullptr;
			//nsGraphic::nsSprite::CSpriteRender* m_titleStartSR = nullptr;
			//bool m_titleFadeOut = false;
			//float m_titleA = 1.0f;
		};

	}
}
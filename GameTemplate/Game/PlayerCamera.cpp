#include "stdafx.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "AICar.h"

namespace nsMyGame
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// プレイヤーカメラの定数データを使用可能にする
		using namespace nsPlayerConstData::nsCameraConstData;

		/**
		 * @brief 初期化
		 * @param[in] player プレイヤーの参照
		*/
		void CPlayerCamera::Init(const CPlayer& player)
		{
			// カメラをセット
			m_camera = g_camera3D;

			// カメラの遠平面を設定
			m_camera->SetFar(kDefaultCameraFar);

			// デフォルトの注視点から視点へのベクトルを設定
			m_toCameraVec = kDefaultToCameraVec;

			m_targetOffsetUp = kTargetOffsetUp;

			// プレイヤーの座標の参照をセット
			m_playerRef = &player;

			// カメラのスタートの視点と注視点を設定
			m_camera->SetPosition(m_playerRef->GetPosition() + m_toCameraVec);
			m_camera->SetTarget(m_playerRef->GetPosition());

			// バネカメラを初期化
			m_springCamera.Init(*m_camera, kCameraMaxSpeed, true, kCameraRadius);

			// バネの減衰率を設定
			m_springCamera.SetDampingRate(kSpringDampingRate);

			m_toCameraDistance = kDefaultToCameraDistance;

			return;
		}

		/**
		 * @brief プレイヤーカメラクラスのアップデートを実行する
		*/
		void CPlayerCamera::ExecuteUpdate()
		{
			if (m_isOnEnemyCamera)
			{
				// 敵の上に乗っている時のカメラの処理
				OnEnemyCamera();
			}
			else
			{
				// カメラの回転を計算する
				CalcCameraRotation();

				// 自動的にカメラをプレイヤーの移動先に向ける
				AutoTurnToPlayerDestination();
			}

			// カメラの視点
			Vector3 cameraPos = Vector3::Back;
			// カメラの注視点
			Vector3 camereTargetPos = Vector3::Zero;

			// カメラの視点と注視点を計算する
			CalcCameraPositionAndTargetPos(&cameraPos,&camereTargetPos);

			// バネカメラを更新する
			UpdateSpringCamera(cameraPos, camereTargetPos);


			return;
		}

		/**
		 * @brief カメラの回転を計算する
		*/
		void CPlayerCamera::CalcCameraRotation()
		{
			// 回転前の注視点から視点へのベクトル
			Vector3 oldToCameraVec = m_toCameraVec;

			//////// 1.カメラを回転させる ////////

			// カメラの回転クォータニオン
			Quaternion cameraQRot;

			// 水平方向へのカメラの回転を計算する
			// 垂直な軸、Y軸回りで回転させる
			cameraQRot.SetRotationDegY(
				kCameraRotSpeed * m_playerRef->GetInputData().axisCameraRotHorizontal
			);
			// 注視点から視点へのベクトルを回転させる
			cameraQRot.Apply(m_toCameraVec);

			// 垂直方向のカメラの回転を計算する
			// 水平な軸
			Vector3 axisH;
			// 外積で、Y軸と、注視点から視点へのベクトルに、直交するベクトルを求める
			axisH.Cross(Vector3::AxisY, m_toCameraVec);
			// 正規化する
			axisH.Normalize();
			// 水平な軸周りで回転させる
			cameraQRot.SetRotationDeg(
				axisH,
				kCameraRotSpeed * m_playerRef->GetInputData().axisCameraRotVertical
			);
			// 注視点から視点へのベクトルを回転させる
			cameraQRot.Apply(m_toCameraVec);


			//////// 2.カメラの回転の上限をチェックする ////////

			// 注視点から視点への方向ベクトル
			Vector3 toCameraDir = m_toCameraVec;
			// 方向ベクトルだから正規化する
			toCameraDir.Normalize();

			// 上向きチェック
			if (toCameraDir.y < kMinToCameraDirY)
			{
				// カメラが上向き過ぎ
				// 回転前の注視点から視点へのベクトルに戻す
				m_toCameraVec = oldToCameraVec;
			}
			// 下向きチェック
			else if (toCameraDir.y > kMaxToCameraDirY)
			{
				// カメラが下向き過ぎ
				// 回転前の注視点から視点へのベクトルに戻す
				m_toCameraVec = oldToCameraVec;
			}

			// ベクトルをずっとApplyで回していると、ちょっとずつ伸びてくるから、正規化して伸ばす。

			// 向きを正規化してから
			m_toCameraVec.Normalize();
			// 伸ばす
			m_toCameraVec.Scale(m_toCameraDistance);

			return;
		}


		/**
		 * @brief 自動的にカメラをプレイヤーの移動先に向ける
		*/
		void CPlayerCamera::AutoTurnToPlayerDestination()
		{

			// カメラの軸入力があるか？か、
			// 移動の軸入力がないか？
			if (m_playerRef->GetInputData().inputCameraAxis == true ||
				m_playerRef->GetInputData().inputMoveAxis != true)
			{
				// カメラの回転があるか、移動していない時は、自動回転しない。

				// 自動でカメラを回転し始めるタイマーをリセットするタイマーが一定時間進んでいないか？
				if (m_autoTurnStartTimerResetTimer < kAutoTurnStartTimerResetTime)
				{
					// 進んでいない。タイマーを進める。
					m_autoTurnStartTimerResetTimer += nsTimer::GameTime().GetFrameDeltaTime();
					return;
				}
				// 進んだ。タイマーをリセットする・
				m_autoTurnStartTimer = 0.0f;
				// 早期リターン
				return;
			}

			// カメラの回転の操作をしていない時、かつ、移動の操作をしている状態

			// 自動でカメラを回転し始めるタイマーをリセットするタイマーをリセットする
			m_autoTurnStartTimerResetTimer = 0.0f;

			// 自動でカメラが回転し始めるタイマーが、一定時間進んでいないか？
			if (m_autoTurnStartTimer < kAutoTurnStartTime)
			{
				// 進んでいない。タイマーを進める。
				m_autoTurnStartTimer += nsTimer::GameTime().GetFrameDeltaTime();
				// 早期リターン
				return;
			}

			// カメラの回転の操作をしていない時、かつ、移動の操作をしている状態で、
			// 一定時間経過後、自動回転の処理を行う

			//////// まずはY座標を移動させる ////////
			// Y座標を補完して、徐々に高さが合うようにする
			m_toCameraVec.y = Math::Lerp(kAutoTurnYRate, m_toCameraVec.y, kDefaultToCameraVec.y);

			//////// 次にXZ平面で移動させる ////////

			// 目標の「カメラへのベクトル」
			Vector3 targetToCameraVec = kDefaultToCameraVec;
			// モデルの回転で回す
			m_playerRef->GetRotation().Apply(targetToCameraVec);

			// XZ平面での、目標の「カメラへの方向ベクトル」
			Vector3 targetToCameraDirXZ = targetToCameraVec;
			targetToCameraDirXZ.y = 0.0f;		// Y成分を消去する
			targetToCameraDirXZ.Normalize();	// 正規化する

			// XZ平面での、現在の「カメラへの方向ベクトル」
			Vector3 toCameraDirXZ = m_toCameraVec;
			toCameraDirXZ.y = 0.0f;				// Y成分を消去する
			toCameraDirXZ.Normalize();			// 正規化する

			// 目標の「カメラへの方向ベクトル」と現在の「カメラへの方向ベクトル」の内積がしきい値以上
			if (Dot(targetToCameraDirXZ, toCameraDirXZ) >= kAutoTurnExecuteThreshold)
			{
				// ほぼ同じ向きのため動かす必要なし。早期リターン。
				return;
			}

			// XZ平面での、目標の「カメラへの方向ベクトル」の「右向きの方向ベクトル」
			Vector3 targetToCameraRightDirXZ = Cross(Vector3::Up, targetToCameraDirXZ);
			targetToCameraRightDirXZ.Normalize();	// 正規化する

			// XZ平面での、目標のベクトルの「右向きの方向ベクトル」と現在の「カメラへの方向ベクトル」の内積
			float dotRightAndToCamDirXZ = Dot(targetToCameraRightDirXZ, toCameraDirXZ);

			// 回転速度の計算。プレイヤーの速度が速いほど、早く回転する。

			// 最低補間率の速度
			const float minRateSpeed = nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed;
			// 最高補完率の速度
			const float maxRateSpeed = nsPlayerConstData::nsSwingActionConstData::kInitialSwingSpeed;
			// 回転スピードの補完率。プレイヤーの速度に応じて、0.0f～1.0fの値に正規化する。
			float turnSpeedRate = max(0.0f, m_playerRef->GetPlayerMovement().GetXZSpeed() - minRateSpeed);
			turnSpeedRate = min(1.0f,turnSpeedRate / (maxRateSpeed - minRateSpeed));
			// スピードの変化を指数関数的にする
			turnSpeedRate = pow(turnSpeedRate, kAutoTurnSpeedRatePowPower);
			// 回転するスピード
			float turnSpeed = Math::Lerp(turnSpeedRate, kAutoTurnSpeedMin, kAutoTurnSpeedMax);


			// 現在のベクトルが、目標のベクトルの右側にあるか？
			if (dotRightAndToCamDirXZ >= 0.0f)
			{
				// 現在のベクトルが右側、目標のベクトルが左側にあるから、左向きに回す
				// 反対向きに回す
				turnSpeed = -turnSpeed;
			}


			// 回転クォータニオン
			Quaternion qRot;
			qRot.SetRotationY(turnSpeed);
			// XZ平面で、カメラへのベクトルを回す
			qRot.Apply(m_toCameraVec);

			return;
		}


		/**
		 * @brief 敵の上に乗っている時のカメラの処理
		*/
		void CPlayerCamera::OnEnemyCamera()
		{
			const auto& enemy = m_playerRef->GetCatchEnemy().GetTargetEnemy();
			if (enemy == nullptr)
			{
				return;
			}
			const Quaternion& enemyRot = enemy->GetRotation();
			const Quaternion& playerRot = m_playerRef->GetRotation();
			m_toCameraVec = Vector3::Back;
			//enemyRot.Apply(m_toCameraVec);
			playerRot.Apply(m_toCameraVec);

			// 垂直方向のカメラの回転を計算する
			// 水平な軸
			Vector3 axisH;
			// 外積で、Y軸と、注視点から視点へのベクトルに、直交するベクトルを求める
			axisH.Cross(Vector3::AxisY, m_toCameraVec);
			// 正規化する
			axisH.Normalize();
			// 水平な軸周りで回転させる
			Quaternion cameraVRot;
			cameraVRot.SetRotationDeg(
				axisH,
				kOnEnemyCameraVerticalAngle
			);
			// 注視点から視点へのベクトルを回転させる
			cameraVRot.Apply(m_toCameraVec);

			// 向きを正規化してから
			m_toCameraVec.Normalize();
			// 伸ばす
			m_toCameraVec.Scale(kNearToCameraDistance);

			return;
		}

		/**
		 * @brief カメラの視点と注視点を計算する
		 * @param[out] pos_out 視点が格納される
		 * @param[out] targetPos_out 注視点が格納される
		*/
		void CPlayerCamera::CalcCameraPositionAndTargetPos(Vector3* pos_out, Vector3* targetPos_out)
		{
			//////// 1.カメラの注視点を計算する ////////

			// 移動前の注視点をいれる
			*targetPos_out = m_playerRef->GetPosition();
			// プレイヤーの足元から少し上を注視点とする
			targetPos_out->y += m_targetOffsetUp;
			// プレイヤーの少し奥の方を注視点とする
			*targetPos_out += m_camera->GetForward() * kTargetOffsetForward;

			//////// 2. カメラの視点を計算する ////////

			*pos_out = *targetPos_out + m_toCameraVec;
			pos_out->y += m_cameraPositionOffsetUp;

			return;
		}

		/**
		 * @brief バネカメラを更新する
		 * @param[in] pos 視点
		 * @param[in] targetPos 注視点
		*/
		void CPlayerCamera::UpdateSpringCamera(const Vector3& pos, const Vector3& targetPos)
		{
			// バネカメラの視点と注視点を設定
			m_springCamera.SetPosition(pos);
			m_springCamera.SetTarget(targetPos);

			// バネカメラを更新
			m_springCamera.Update();

			return;
		}

	}
}

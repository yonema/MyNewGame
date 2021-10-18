#include "stdafx.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "PlayerConstData.h"

namespace nsMyGame
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// プレイヤーカメラの定数データを使用可能にする
		using namespace nsPlayerConstData::nsPlayerCameraConstData;

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

			// プレイヤーの座標の参照をセット
			m_playerRef = &player;

			// カメラのスタートの視点と注視点を設定
			m_camera->SetPosition(m_playerRef->GetPosition() + m_toCameraVec);
			m_camera->SetTarget(m_playerRef->GetPosition());

			// バネカメラを初期化
			m_springCamera.Init(*m_camera, kCameraMaxSpeed, true, kCameraRadius);

			// バネの減衰率を設定
			m_springCamera.SetDampingRate(kSpringDampingRate);

			return;
		}

		/**
		 * @brief プレイヤーカメラクラスのアップデートを実行する
		*/
		void CPlayerCamera::ExecuteUpdate()
		{
			// カメラの回転を計算する
			CalcCameraRotation();

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
			targetPos_out->y += kTargetOffsetUp;
			// プレイヤーの少し奥の方を注視点とする
			*targetPos_out += m_camera->GetForward() * kTargetOffsetForward;

			//////// 2. カメラの視点を計算する ////////

			*pos_out = *targetPos_out + m_toCameraVec;

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

#include "stdafx.h"
#include "PlayerInput.h"
#include "Player.h"
#include "PlayerConstData.h"

namespace nsMyGame
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// プレイヤー入力クラスの定数データを使用可能にする
		using namespace nsPlayerConstData::nsPlayerInputConstData;

		/**
		 * @brief 初期化
		 * @param[in,out] player プレイヤーの参照
		*/
		void CPlayerInput::Init(CPlayer* player)
		{
			// ゲームパッドの参照を得る
			m_pad = g_pad[0];

			// プレイヤーの参照をセットする
			m_playerRef = player;

			return;
		}

		/**
		 * @brief 入力処理を実行する
		*/
		void CPlayerInput::ExecuteUpdate()
		{
			// 入力情報を初期化する
			memset(&m_playerInputData, 0, sizeof(m_playerInputData));

			// 軸入力を更新する
			UpdateInputAxis();

			// アクション入力を更新する
			UpdateInputAction();

			return;
		}


		/**
		 * @brief 軸入力情報を更新する
		*/
		void CPlayerInput::UpdateInputAxis()
		{
			// 移動の軸入力情報の更新
			OnUpdateInputAxisMove();

			// カメラの回転の軸入力情報の更新
			OnUpdateInputAxisCameraRot();

			return;
		}


		/**
		 * @brief アクション入力情報を更新する
		*/
		void CPlayerInput::UpdateInputAction()
		{
			// Aボタントリガー
			if (m_pad->IsTrigger(enButtonA))
			{
				// ジャンプ
				m_playerInputData.actionJump = true;
			}

			// RB2ボタントリガー
			if (m_pad->IsTrigger(enButtonRB2))
			{
				// スイングトリガー
				m_playerInputData.triggerSwing = true;
			}
			// RB2ボタンが押されているか？
			if (m_pad->IsPress(enButtonRB2))
			{
				// ダッシュ
				m_playerInputData.actionDush = true;
				// スイング
				m_playerInputData.actionSwing = true;
			}

			return;
		}

		/**
		 * @brief 移動の軸入力情報の更新
		*/
		void CPlayerInput::OnUpdateInputAxisMove()
		{
			//////// 1.左スティックの入力情報を調べる ////////

			// パッドの左スティックの軸入力情報
			Vector2 padLStickF(m_pad->GetLStickXF(), m_pad->GetLStickYF());
			// 何か正規化してくれてないから、正規化する
			padLStickF.Normalize();

			// Y軸方向の入力を調べる
			m_playerInputData.axisMoveForward = padLStickF.y;
			// X軸方向の入力を調べる
			m_playerInputData.axisMoveRight = padLStickF.x;

			// 軸入力があった場合は十字キーを調べる必要がないため、早期リターンを行う
			if (fabsf(m_playerInputData.axisMoveForward) > kInputAxisMin || 
				fabsf(m_playerInputData.axisMoveRight) > kInputAxisMin)
			{
				// あった
				m_playerInputData.inputMoveAxis = true;
				return;
			}


			//////// 2.十字キーの入力情報を調べる ////////

			// 上、下キーの入力情報を調べる
			if (m_pad->IsPress(enButtonUp))
			{
				m_playerInputData.axisMoveForward = kDPadInputPower;
			}
			else if (m_pad->IsPress(enButtonDown))
			{
				m_playerInputData.axisMoveForward = -kDPadInputPower;
			}
			// 右、左キーの入力情報を調べる
			if (m_pad->IsPress(enButtonRight))
			{
				m_playerInputData.axisMoveRight = kDPadInputPower;
			}
			else if (m_pad->IsPress(enButtonLeft))
			{
				m_playerInputData.axisMoveRight = -kDPadInputPower;
			}

			// 斜め移動の移動の場合は、ルート2倍入力量が多くなるため、ルート2で割る
			if (m_playerInputData.axisMoveForward != 0.0f && m_playerInputData.axisMoveRight != 0.0f)
			{
				m_playerInputData.axisMoveForward /= kSquareRootOfTwo;
				m_playerInputData.axisMoveRight /= kSquareRootOfTwo;
			}

			// 軸入力があったか？
			if (m_playerInputData.axisMoveForward != 0.0f || m_playerInputData.axisMoveRight != 0.0f)
			{
				// あった
				m_playerInputData.inputMoveAxis = true;
			}

			return;
		}

		/**
		 * @brief カメラの回転の軸入力情報の更新
		*/
		void CPlayerInput::OnUpdateInputAxisCameraRot()
		{
			// 右スティックの軸入力情報を調べる

			// Y軸方向の入力を調べる
			m_playerInputData.axisCameraRotVertical = m_pad->GetRStickYF();
			// X軸方向の入力を調べる
			m_playerInputData.axisCameraRotHorizontal = m_pad->GetRStickXF();

			// 入力があったか？
			if (fabsf(m_playerInputData.axisCameraRotVertical) > kInputAxisMin ||
				fabsf(m_playerInputData.axisCameraRotHorizontal) > kInputAxisMin)
			{
				// あった
				m_playerInputData.inputCameraAxis = true;
			}

			return;
		}
	}
}
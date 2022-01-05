#include "stdafx.h"
#include "PlayerInput.h"
#include "Player.h"

namespace nsMyGame
{
	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// プレイヤー入力クラスの定数データを使用可能にする
		using namespace nsPlayerConstData::nsInputConstData;
		using nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType;

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

			// コマンド入力情報の更新
			UpdateCommandInputData();

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

			// 右スティックトリガー
			if (m_pad->IsTrigger(enButtonRB3))
			{
				// 敵を探知
				m_playerInputData.actionSearchEnemy = true;
			}

			// Yボタントリガー
			if (m_pad->IsTrigger(enButtonY))
			{
				m_playerInputData.actionCatchEnemy = true;
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


		/**
		 * @brief コマンド入力情報の更新
		*/
		void CPlayerInput::UpdateCommandInputData()
		{
			m_playerInputData.inputCommand = EnQTEButtonType::enQTE_None;

			// ABYXボタンの更新
			if (m_pad->IsTrigger(enButtonA))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_A;
			}
			else if (m_pad->IsTrigger(enButtonB))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_B;
			}
			else if (m_pad->IsTrigger(enButtonY))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_Y;
			}
			else if (m_pad->IsTrigger(enButtonX))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_X;
			}
			// 十字キーの更新
			else if (m_pad->IsTrigger(enButtonUp))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Up;
			}
			else if (m_pad->IsTrigger(enButtonDown))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Down;
			}
			else if (m_pad->IsTrigger(enButtonLeft))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Left;
			}
			else if (m_pad->IsTrigger(enButtonRight))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Right;
			}
			// 左スティックの更新
			else if (m_pad->GetLStickYF() >= kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Up;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickYF() <= -kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Down;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickXF() <= -kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Left;
				m_canInputCommandAxis = false;
			}
			else if (m_pad->GetLStickXF() >= kCommandInputAxisMin && m_canInputCommandAxis)
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L_Right;
				m_canInputCommandAxis = false;
			}
			// L1,L2,R1,R2ボタンの更新
			else if (m_pad->IsTrigger(enButtonLB1))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L1;
			}
			else if (m_pad->IsTrigger(enButtonLB2))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_L2;
			}
			else if (m_pad->IsTrigger(enButtonRB1))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_R1;
			}
			else if (m_pad->IsTrigger(enButtonRB2))
			{
				m_playerInputData.inputCommand = EnQTEButtonType::enQTE_R2;
			}
			// 軸入力のフラグの回復
			else if (fabsf(m_pad->GetLStickYF()) <= kInputAxisMin && fabsf(m_pad->GetLStickXF() <= kInputAxisMin))
			{
				m_canInputCommandAxis = true;
			}

			return;
		}
	}
}
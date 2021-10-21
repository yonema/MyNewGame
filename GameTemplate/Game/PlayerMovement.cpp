#include "stdafx.h"
#include "PlayerMovement.h"
#include "Player.h"

namespace nsMyGame
{

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		/**
		 * @brief プレイヤーの動き関連のネームスペース
		*/
		namespace nsPlayerMovenent
		{
			// プレイヤー移動クラスの定数データを使用可能にする
			using namespace nsPlayerConstData::nsPlayerMoveConstData;

			/**
			 * @brief 初期化
			 * @param[in] radius カプセルコライダーの半径
			 * @param[in] height カプセルコライダーの高さ
			 * @param[in,out] player プレイヤーの参照
			*/
			void CPlayerMovement::Init(
				const float radius,
				const float height,
				CPlayer* player
			)
			{
				// プレイヤーの参照をセットする
				m_playerRef = player;

				// キャラクターコントローラ初期化
				m_charaCon.Init(radius, height, m_playerRef->GetPosition());

				// プレイヤーの歩きと走りクラスの初期化
				m_playerWalkAndRun.Init(*m_playerRef, this);

				// プレイヤーのスイングアクションクラスの初期化
				m_playerSwingAction.Init(m_playerRef, this);

				return;
			}

			/**
			 * @brief プレイヤーの移動クラスのアップデートを実行する
			*/
			void CPlayerMovement::ExecuteUpdate()
			{
				// スイングトリガーがtrueで、かつ、空中だったら
				if (m_playerRef->GetInputData().triggerSwing && IsAir())
				{
					// スイング状態
					m_playerRef->SetState(nsPlayerConstData::enSwing);
				}
				else
				{
					// 歩きと走り状態
					//m_playerRef->SetState(nsPlayerConstData::enWalkAndRun);
				}

				// プレイヤーの移動を更新
				UpdateMovePlayer();

				// プレイヤーの回転を更新
				UpdateTurnPlayer();

				nsDebug::DrawVector(m_playerRef->GetPosition(), m_moveVec, "playerVec");
				nsDebug::DrawTextPanel(std::to_wstring(GetVelocity()), L"speed");

				return;
			}

			/**
			 * @brief 重力をかける
			*/
			void CPlayerMovement::ApplyGravity()
			{
				if (m_useGravity != true)
				{
					return;
				}

				m_moveVec.y -= nsPlayerConstData::nsPlayerMoveConstData::kGravityScale *
					nsTimer::GameTime().GetFrameDeltaTime();
				if (m_moveVec.y < -nsPlayerConstData::nsPlayerMoveConstData::kMaxFallSpeed)
				{
					m_moveVec.y = -nsPlayerConstData::nsPlayerMoveConstData::kMaxFallSpeed;
				}

				return;
			}

			/**
			 * @brief キャラクターコントローラーを使った移動
			*/
			void CPlayerMovement::MoveWithCharacterController()
			{
				// キャラクターコントローラー実行
				m_playerRef->SetPosition(
					m_charaCon.Execute(m_moveVec, nsTimer::GameTime().GetFrameDeltaTime())
				);

				return;
			}


			/**
			 * @brief プレイヤーの移動を更新
			*/
			void CPlayerMovement::UpdateMovePlayer()
			{

				// ステートで処理を振り分ける
				switch (m_playerRef->GetState())
				{
				// 歩きと走り
				case nsPlayerConstData::enWalkAndRun:
					// 歩きと走りを実行
					m_playerWalkAndRun.Execute();
					break;
				// スイング
				case nsPlayerConstData::enSwing:

					// スイングアクションを実行
					m_playerSwingAction.Execute();

					break;
				}

				ApplyGravity();

				MoveWithCharacterController();


				return;
			}

			/**
			 * @brief プレイヤーの回転を更新
			*/
			void CPlayerMovement::UpdateTurnPlayer()
			{
				// X,Z平面での移動があるか？
				if (fabsf(m_moveVec.x) < kMoveVecMin && fabsf(m_moveVec.z) < kMoveVecMin)
				// 軸入力があるか？
				//if (fabsf(m_playerRef->GetInputData().axisMoveForward) < 0.001f &&
				//	fabsf(m_playerRef->GetInputData().axisMoveRight) < 0.001f)
				{
					// 移動していない
					// 早期リターン
					return;
				}

				// tanθ = m_moveVec.x / m_moveVec.y
				const float radAngle = atan2(m_moveVec.x, m_moveVec.z);
				// ラジアン単位で回す
				Quaternion nexrQRot;
				nexrQRot.SetRotation(Vector3::AxisY, radAngle);
				// 現在の回転と次の回転を球面線形補間を行い、モデルを徐々に回転させる。
				nexrQRot.Slerp(kModelRotRate, m_playerRef->GetRotation(), nexrQRot);
				m_playerRef->SetRotation(nexrQRot);

				return;
			}
		}
	}
}
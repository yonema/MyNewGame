#include "stdafx.h"
#include "PlayerWalkAndRun.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "PlayerConstData.h"

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
			// プレイヤーの歩きと走りクラスの定数データを使用可能にする
			using namespace nsPlayerConstData::nsPlayerWalkAndRunConstData;

			/**
			 * @brief 初期化
			 * @param[in] player プレイヤー
			 * @param[in,out] playerMovement プレイヤー移動クラスの参照
			*/
			void CPlayerWalkAndRun::Init(
				const CPlayer& player,
				CPlayerMovement* playerMovement
			)
			{
				// プレイヤーの参照をセット
				m_playerRef = &player;
				// 加算移動ベクトルの参照をセット
				m_playerMovementRef = playerMovement;

				return;
			}

			/**
			 * @brief 歩きと走りの処理を実行
			*/
			void CPlayerWalkAndRun::Execute()
			{
				// 前、後移動の軸入力
				const float inputAxisMoveForward = m_playerRef->GetInputData().axisMoveForward;
				// 右、左移動の軸入力
				const float inputAxisMoveRight = m_playerRef->GetInputData().axisMoveRight;

				// 前、後移動の軸入力の絶対値
				const float absInputAxisMoveForward = fabsf(inputAxisMoveForward);
				// 右、左移動の軸入力の絶対値
				const float absInputAxisMoveRight = fabsf(inputAxisMoveRight);

				// 移動速度を加速する
				Acceleration(inputAxisMoveForward, inputAxisMoveRight);

				// 摩擦を計算する
				Friction(absInputAxisMoveForward, absInputAxisMoveRight);

				// 移動速度に制限をかける
				LimitSpeed(absInputAxisMoveForward, absInputAxisMoveRight);

				// 前のフレームの速度を更新
				m_oldVelocity = m_playerMovementRef->GetAddMoveVec().Length();

				return;
			}

			/**
			 * @brief 移動速度を加速させる
			 * @param inputMoveF 前、後の軸入力
			 * @param inputMoveR 右、左の軸入力
			*/
			void CPlayerWalkAndRun::Acceleration(const float inputMoveF, const float inputMoveR)
			{
				// 移動の前方向
				Vector3 moveForward = m_playerRef->GetCamera().GetCameraForward();
				// 移動の右方向
				Vector3 moveRight = m_playerRef->GetCamera().GetCameraRight();
				// Y成分を消してXZ平面での前方向と右方向に変換する
				moveForward.y = 0.0f;
				moveForward.Normalize();
				moveRight.y = 0.0f;
				moveRight.Normalize();

				//奥方向への移動速度を加算。
				m_playerMovementRef->SetAddMoveVec(
					m_playerMovementRef->GetAddMoveVec() + moveForward * inputMoveF * kAcceleration
				);
				//奥方向への移動速度を加算。
				m_playerMovementRef->SetAddMoveVec(
					m_playerMovementRef->GetAddMoveVec() + moveRight * inputMoveR * kAcceleration
				);

				return;
			}

			/**
			 * @brief 摩擦の計算
			 * @param[in] absInputMoveF 前、後の軸入力の絶対値
			 * @param[in] absInputMoveR 右、左の軸入力の絶対値
			*/
			void CPlayerWalkAndRun::Friction(const float absInputMoveF, const float absInputMoveR)
			{
				// 摩擦力
				float friction = 0.0f;

				// 空中か？
				if (m_playerMovementRef->IsAir())
				{
					// 空中の摩擦力
					friction = kAirFriction;
				}
				else
				{
					// 地面上の摩擦力
					friction = kGroundFriction;
				}

				// 移動ベクトルの大きさ
				const float moveVecLen = m_playerMovementRef->GetAddMoveVec().Length();

				// 摩擦を計算する
				if (moveVecLen <= kMinSpeed)
				{
					// 移動速度が最低速度以下なら
					// 移動ゼロにする
					m_playerMovementRef->SetAddMoveVec(Vector3::Zero);
				}
				else if (absInputMoveF <= 0.001f && absInputMoveR <= 0.001f)
				{
					// 入力がなかったら
					// 摩擦て減速する
					m_playerMovementRef->SetAddMoveVec(
						m_playerMovementRef->GetAddMoveVec() * friction
					);
				}
				else if (moveVecLen < m_oldVelocity)
				{
					// 前のフレームより速度が落ちていたら
					// 摩擦て減速する
					m_playerMovementRef->SetAddMoveVec(
						m_playerMovementRef->GetAddMoveVec() * friction
					);
				}

				return;
			}

			/**
			 * @brief 移動速度に速度制限をかける
			 * @param[in] absInputMoveF 前、後の軸入力の絶対値
			 * @param[in] absInputMoveR 右、左の軸入力の絶対値
			*/
			void CPlayerWalkAndRun::LimitSpeed(const float absInputMoveF, const float absInputMoveR)
			{
				// 軸入力があるか？
				if (absInputMoveF <= 0.001f && absInputMoveR <= 0.001f)
				{
					// ない場合は速度制限をかけない
					// 早期リターン
					return;
				}


				// 移動速度が最高速度をオーバーしているか？
				if (m_playerMovementRef->GetAddMoveVec().Length() > kMaxSpeed)
				{
					// オーバーしていたら最高速度を維持
					m_playerMovementRef->NormalizeAddMoveVec();
					m_playerMovementRef->SetAddMoveVec(m_playerMovementRef->GetAddMoveVec() * kMaxSpeed);
				}

				return;
			}

		}
	}
}
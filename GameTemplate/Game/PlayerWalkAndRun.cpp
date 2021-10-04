#include "stdafx.h"
#include "PlayerWalkAndRun.h"
#include "PlayerConstData.h"
#include "PlayerCamera.h"
#include "PlayerInput.h"

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
			using namespace nsPlayerWalkAndRunConstData;

			/**
			 * @brief 初期化
			 * @param[in,out] moveVecForward 前向きの移動ベクトルの参照
			 * @param[in,out] moveVecRight 右向きの移動ベクトルの参照
			 * @param[in] camera プレイヤーカメラ
			 * @param[in] playerInputData プレイヤー入力情報
			*/
			void CPlayerWalkAndRun::Init(
				Vector3* moveVecForward,
				Vector3* moveVecRight,
				const CPlayerCamera& camera,
				const SPlayerInputData& playerInputData
			)
			{
				// 前向きの移動ベクトルの参照をセット
				m_moveVecForward = moveVecForward;
				// 右向きの移動ベクトルの参照をセット
				m_moveVecRight = moveVecRight;
				// プレイヤーのカメラをセット
				m_playerCamera = &camera;
				// プレイヤーの入力情報をセット
				m_playerInputData = &playerInputData;

				return;
			}

			/**
			 * @brief 歩きと走りの処理を実行
			 * @param[in] isAir 空中か？
			*/
			void CPlayerWalkAndRun::Execute(const bool isAir)
			{
				// 前、後移動の軸入力
				const float inputAxisMoveForward = m_playerInputData->axisMoveForward;
				// 右、左移動の軸入力
				const float inputAxisMoveRight = m_playerInputData->axisMoveRight;

				// 前、後移動の軸入力の絶対値
				const float absInputAxisMoveForward = fabs(inputAxisMoveForward);
				// 右、左移動の軸入力の絶対値
				const float absInputAxisMoveRight = fabs(inputAxisMoveRight);

				// 移動速度を加速する
				Acceleration(inputAxisMoveForward, inputAxisMoveRight);

				// 摩擦を計算する
				Friction(absInputAxisMoveForward, absInputAxisMoveRight, isAir);

				// 移動速度に制限をかける
				LimitSpeed(absInputAxisMoveForward, absInputAxisMoveRight);


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
				Vector3 moveForward = m_playerCamera->GetCameraForward();
				// 移動の右方向
				Vector3 moveRight = m_playerCamera->GetCameraRight();
				// Y成分を消してXZ平面での前方向と右方向に変換する
				moveForward.y = 0.0f;
				moveForward.Normalize();
				moveRight.y = 0.0f;
				moveRight.Normalize();

				//奥方向への移動速度を加算。
				*m_moveVecForward += moveForward * inputMoveF * kAcceleration;
				//奥方向への移動速度を加算。
				*m_moveVecRight += moveRight * inputMoveR * kAcceleration;

				return;
			}

			/**
			 * @brief 摩擦の計算
			 * @param[in] absInputMoveF 前、後の軸入力の絶対値
			 * @param[in] absInputMoveR 右、左の軸入力の絶対値
			 * @param[in] isAir 空中か？
			*/
			void CPlayerWalkAndRun::Friction(
				const float absInputMoveF, const float absInputMoveR, const bool isAir
			)
			{
				// 摩擦力
				float friction = 0.0f;

				// 空中か？
				if (isAir)
				{
					// 空中の摩擦力
					friction = kAirFriction;
				}
				else
				{
					// 地面上の摩擦力
					friction = kGroundFriction;
				}

				// 前向きの移動ベクトルの摩擦を計算する
				// 移動速度が最低速度以下なら
				if (m_moveVecForward->Length() <= kMinSpeed)
				{
					// 移動ゼロにする
					*m_moveVecForward = Vector3::Zero;
				}
				else if (absInputMoveF <= 0.001f)
				{
					// 入力がなかったら
					// 摩擦て減速する
					m_moveVecForward->Scale(friction);
				}

				// 右向きの移動ベクトルの摩擦を計算する
				// 移動速度が最低速度以下なら
				if (m_moveVecRight->Length() <= kMinSpeed)
				{
					// 移動ゼロにする
					*m_moveVecRight = Vector3::Zero;
				}
				else if (absInputMoveR <= 0.001f)
				{
					// 入力がなかったら
					// 摩擦て減速する
					m_moveVecRight->Scale(friction);
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

				// 前方向の移動速度が最高速度をオーバーしているか？
				if (m_moveVecForward->Length() > kMaxSpeed * absInputMoveF)
				{
					// オーバーしていたら最高速度を維持
					m_moveVecForward->Normalize();
					m_moveVecForward->Scale(kMaxSpeed * absInputMoveF);
				}

				// 右方向の移動速度が最高速度をオーバーしているか？
				if (m_moveVecRight->Length() > kMaxSpeed * absInputMoveR)
				{
					// オーバーしていたら最高速度を維持
					m_moveVecRight->Normalize();
					m_moveVecRight->Scale(kMaxSpeed * absInputMoveR);
				}
				

				return;
			}

		}
	}
}
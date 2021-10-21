#include "stdafx.h"
#include "PlayerSwingAction.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "StringActionTargetManager.h"

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
			// プレイヤーのスイングアクションクラスの定数データを使用可能にする
			using namespace nsPlayerConstData::nsPlayerSwingActionConstData;


			/**
			 * @brief 初期化
			 * @param[in,out] player プレイヤー
			 * @param[in,out] playerMovement プレイヤー移動クラスの参照
			*/
			void CPlayerSwingAction::Init(
				CPlayer* player,
				CPlayerMovement* playerMovement
			)
			{
				// プレイヤーの参照をセット
				m_playerRef = player;
				// 加算移動ベクトルの参照をセット
				m_playerMovementRef = playerMovement;

				return;
			}

			/**
			 * @brief スイングアクションを実行
			*/
			void CPlayerSwingAction::Execute()
			{
				nsDebug::DrawTextPanel(L"[SwingAction:Execute]");

				// スイングターゲットを探すか？
				if (m_swingActionState == enFindSwingTarget)
				{
					// スイングターゲットを探す
					FindSwingTarget();
				}

				// スイングアクションをやめたか？
				if (m_playerRef->GetInputData().actionSwing != true)
				{
					// ステートをスイング後の空中状態に遷移する
					m_swingActionState = EnSwingActionState::enIsAirAfterSwing;
					// 糸に終了を知らせる
					m_playerRef->EndStringStretchToPos();
				}

				// 地面についてしまったか？
				if (m_playerMovementRef->IsAir() != true)
				{
					// スイングアクションを終了する
					m_swingActionState = EnSwingActionState::enEnd;
					// プレイヤーのステートを歩きと走りにする
					m_playerRef->SetState(nsPlayerConstData::EnPlayerState::enWalkAndRun);

				}

				// スイング中の処理をステートで割り振る
				switch (m_swingActionState)
				{
				// 糸を伸ばしている最中
				case enIsStringStretching:
					// 糸を伸ばしている最終の処理
					StringStretching();
					break;

				// スイング中
				case enIsSwinging:
					// スイング中の処理
					SwingAction();
					break;
				// スイング後の空中
				case enIsAirAfterSwing:
					// スイング後の空中の処理
					AirAfterSwing();
					break;

				// スイング終了
				case enEnd:
					// ステートをスイングターゲットを探す状態に戻しておく
					m_swingActionState = enFindSwingTarget;
					// 糸に終了を知らせる
					m_playerRef->EndStringStretchToPos();

					break;
				}


				return;
			}


			/**
			 * @brief スイングターゲットを探す
			*/
			void CPlayerSwingAction::FindSwingTarget()
			{
				// スイングターゲットへの距離
				float dist = FLT_MAX;

				// スイングターゲットを初期化
				m_swingTargetPos = nullptr;

				// 複数の座標から、スイングターゲットを探す
				for (int i = 0; i < kFindSwintTargetNum; i++)
				{
					// スイングターゲットを探す座標へのベクトル
					Vector3 toFindSwingTargetVec = kToFindSwingTargetVecs[i];
					// プレイヤーのモデルの向きに回転させる
					m_playerRef->GetRotation().Apply(toFindSwingTargetVec);

					// 指定された座標から有効範囲内にあるスイングターゲットの座標の中で一番近い座標を得る
					const Vector3* swingTargetPos = 
						nsStringActionTarget::GetNearestSwintTargetPointWithinScope(
						m_playerRef->GetPosition() + toFindSwingTargetVec, kFindSwingTargetScope
					);

					// スイングターゲットの座標がnullptrか？
					if (swingTargetPos == nullptr)
					{
						// 見つからなかった。次へ。
						continue;
					}

					// プレイヤーからスイングターゲットへのベクトル
					Vector3 toSwingTargetVec = *swingTargetPos - m_playerRef->GetPosition();
					// プレイヤーからスイングターゲットへの方向ベクトル
					Vector3 toSwingTargetDir = toSwingTargetVec;
					toSwingTargetDir.Normalize();	// 正規化する

					// 前方向ベクトル
					Vector3 forward = Vector3::Front;
					// プレイヤーのモデルの向きに回転させる
					m_playerRef->GetRotation().Apply(forward);
					forward.Normalize();	// 正規化する

					// スイングターゲットへの方向ベクトルと、前方向ベクトルの内積が負か？
					if (Dot(toSwingTargetDir, forward) <= 0.0f)
					{
						// 負。スイングターゲットがプレイヤーの後ろにあるからパス。次へ。
						continue;
					}

					// 優先度が高いスイングターゲットか？
					if (i < kHighPriorityFindSwintTargetNum)
					{
						// 優先度が高いから、見つけたら即座に終了。
						// スイングターゲットの座標を保持
						m_swingTargetPos = swingTargetPos;
						break;
					}
					else
					{
						// 普通の優先度は、プレイヤーから距離が近いスイングターゲットが優先される。

						// 今回の距離ベクトル
						const Vector3 newDistVec = *swingTargetPos - m_playerRef->GetPosition();
						// 今回の距離
						const float newDist = newDistVec.Length();

						// 今回の距離が今までの最短距離より大きいか？
						if (newDist > dist)
						{
							// 大きい。優先度低。次へ。
							continue;
						}
						// 近いスイングターゲットのため、優先度高。
						// 最短距離を保持
						dist = newDist;
						// スイングターゲットの座標を保持
						m_swingTargetPos = swingTargetPos;
					}

				}

				// スイングターゲットが見つかったか？
				if (m_swingTargetPos != nullptr)
				{
					// 見つかった
					// 糸をスイングターゲットに向かって伸ばし始める
					m_playerRef->StartStringStretchToPos(*m_swingTargetPos);
					// ステートを糸を伸ばし中へ遷移する
					m_swingActionState = EnSwingActionState::enIsStringStretching;
				}
				else
				{
					// 見つからなかった
					// ステートを終了へ遷移する
					m_swingActionState = EnSwingActionState::enIsAirAfterSwing;
				}

				return;
			}

			/**
			 * @brief 糸を伸ばしている最中の処理
			*/
			void CPlayerSwingAction::StringStretching()
			{
				nsDebug::DrawTextPanel(L"StringStretching");
				// 伸びきってないか？
				if (m_playerRef->IsStringStretched() != true)
				{
					// まだ伸びきってない。早期リターン
					return;
				}

				// 伸ばし切ったから、ステートをスイング中に遷移
				m_swingActionState = EnSwingActionState::enIsSwinging;

				// スイングスピードの初期速度を決める
				if (m_swingSpeed <= FLT_EPSILON)
				{
					m_swingSpeed = 500.0f + fabsf(m_playerMovementRef->GetMoveVec().y) * 0.5f;
				}
				m_swingSpeed = 1500.0f;

				m_inputMoveDirXZ = Vector3::Zero;
				return;
			}

			/**
			 * @brief スイングアクションの処理
			*/
			void CPlayerSwingAction::SwingAction()
			{
				nsDebug::DrawTextPanel(L"SwingAction");

				// 加算移動ベクトル
				Vector3 addMoveVec = Vector3::Zero;
				
				// プレイヤーからスイングターゲットまでのXZ平面でのベクトル
				Vector3 playerToTargetVecXZ = *m_swingTargetPos - m_playerRef->GetPosition();
				// Y成分を消去
				playerToTargetVecXZ.y = 0.0f;

				// XZ平面での前方向
				Vector3 forwardDirXZ = Vector3::Front;
				// モデルの向きに回す
				m_playerRef->GetRotation().Apply(forwardDirXZ);
				// 正規化する
				forwardDirXZ.Normalize();				

				// XZ平面の前方向に、プレイヤーからスイングターゲットへのXZ平面のベクトルを射影する
				float projectToTargetVecXZToForwardDirXZ = Dot(forwardDirXZ, playerToTargetVecXZ);

				// XZ平面での、前方向のみの、スイングターゲットへのベクトル
				Vector3 toTargetFowardVecXZ = forwardDirXZ;
				// 射影して求めた長さまで伸ばす
				toTargetFowardVecXZ.Scale(projectToTargetVecXZToForwardDirXZ);

				// XZ平面での、前方向のみの、スイングターゲットの座標
				const Vector3 toTargetForwardPosXZ = m_playerRef->GetPosition() + toTargetFowardVecXZ;

				// 前方向と上方向のみの、スイングターゲットの座標
				Vector3 toTargetForwardUpPos = toTargetForwardPosXZ;
				// Y座標を実際のスイングターゲットの座標と同じにする
				toTargetForwardUpPos.y = m_swingTargetPos->y;

				// 前方向と上方向のみのスイングターゲットの座標からプレイヤーへのベクトル
				const Vector3 toTargetForwardUpToPlayerVec = m_playerRef->GetPosition() - toTargetForwardUpPos;
				
				// 前方向と上方向のみのスイングターゲットの座標からプレイヤーへの方向
				Vector3 targetUptoPlayerDir = toTargetForwardUpToPlayerVec;
				// 正規化する
				targetUptoPlayerDir.Normalize();

				// XZ平面での前方向と、スイングターゲットからプレイヤーへの方向の内積
				float dotFowardDirXZAndToPlayerDir = 
					Dot(forwardDirXZ, targetUptoPlayerDir);

				// 加算移動方向ベクトル
				Vector3 addMoveDir = targetUptoPlayerDir;

				// 加算移動方向ベクトルを回転させる回転クォータニオン
				Quaternion qRotForAddMoveDir;

				// 加算移動方向ベクトルを回転させる回転軸。
				Vector3 rotAxisForAddMoveDir = Cross(targetUptoPlayerDir, forwardDirXZ);

				// プレイヤーがスイングターゲットより上にいるか？
				if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
				{
					// 上にいるとき、回転軸の求め方を変える
					rotAxisForAddMoveDir = Cross(Vector3::Down, forwardDirXZ);
				}

				// 求めた回転軸を正規化する
				rotAxisForAddMoveDir.Normalize();

				// 内積が負か？、つまり、スイングターゲットより手前側にいるか？
				if (dotFowardDirXZAndToPlayerDir < 0.0f)
				{
					// 手前側

					float rate = dotFowardDirXZAndToPlayerDir + 1.0f;
					rate = pow(rate, 2.0f);

					// 回転クォータニオンを90度回転させる
					qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, 3.14f * 0.5f);
					// 加算移動方向ベクトルを回転させる
					qRotForAddMoveDir.Apply(addMoveDir);

				}
				else
				{
					// 奥側

					float radAngle = 3.14f * 0.5f;
					Vector3 to = m_playerRef->GetPosition() - toTargetForwardPosXZ;
					float rate = to.Length() / toTargetForwardUpToPlayerVec.Length();
					rate = pow(rate, 2.0f);
					rate = min(rate, 3.14f * 0.5f);

					if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
					{
						if (m_swingSpeed2 <= -50.0f)
						{
							m_swingSpeed2 = m_swingSpeed;
						}
						float dot = Dot(forwardDirXZ, targetUptoPlayerDir);
						float rad = acosf(dot);
						float angleRate = rad / (3.14f * 0.1f);
						m_swingSpeed = Math::Lerp<float>(min(1.0f,angleRate), m_swingSpeed2, 0.0f);
						addMoveDir = forwardDirXZ;
						radAngle = 3.14f * 0.5f * min(1.0f, angleRate);
						radAngle = Math::Lerp<float>(min(1.0f, angleRate), 3.14f * 0.4f, 3.14f * 0.5f);


					}
					else
					{

						addMoveDir = forwardDirXZ;
						radAngle = 3.14f * 0.4f * rate;

					}

					// 回転クォータニオンを90度回転させる
					qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle);
					// 加算移動方向ベクトルを回転させる
					qRotForAddMoveDir.Apply(addMoveDir);
					rate += 0.5f;

				}

				
				Vector3 rightDirXZ = m_playerRef->GetCamera().GetCameraRight();
				rightDirXZ.y = 0.0f;
				rightDirXZ.Normalize();
				float rightPower = m_playerRef->GetInputData().axisMoveRight / 5.0f;
				rightDirXZ.Scale(rightPower);
				m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, rightDirXZ);

				addMoveDir += m_inputMoveDirXZ;

				addMoveDir.Normalize();
				addMoveVec = addMoveDir;
				addMoveVec.Scale(m_swingSpeed);

				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecY();
				m_playerMovementRef->ResetMoveVecZ();

				// 移動ベクトルに、加算移動ベクトルを加算する
				m_playerMovementRef->AddMoveVec(addMoveVec);

				return;
			}


			/**
			 * @brief スイング後の空中の処理
			*/
			void CPlayerSwingAction::AirAfterSwing()
			{
				nsDebug::DrawTextPanel(L"AirAfterSwing");
				if (m_playerRef->GetInputData().triggerSwing == true)
				{
					m_swingActionState = enFindSwingTarget;
				}

				Vector3 addVec = Vector3::Zero;
				Vector3 moveVec = m_playerMovementRef->GetMoveVec();
				Vector3 moveVecXZ = moveVec;
				moveVecXZ.y = 0.0f;
				float velocity = moveVecXZ.Length();
				if (velocity < nsPlayerConstData::nsPlayerWalkAndRunConstData::kWalkMaxSpeed)
				{
					velocity = nsPlayerConstData::nsPlayerWalkAndRunConstData::kWalkMaxSpeed;
				}

				if (m_playerRef->GetInputData().inputMoveAxis)
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

					//奥、手前方向への移動方向を加算。
					Vector3 newMoveDir = moveForward * m_playerRef->GetInputData().axisMoveForward;	// 新しい移動方向
					// 右、左方向への移動方向を加算
					newMoveDir += moveRight * m_playerRef->GetInputData().axisMoveRight;

					newMoveDir.Scale(velocity);
					addVec = newMoveDir;
				}
				else
				{
					addVec = moveVecXZ;
				}

				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecZ();

				m_playerMovementRef->AddMoveVec(addVec);
				return;
			}

		}
	}
}
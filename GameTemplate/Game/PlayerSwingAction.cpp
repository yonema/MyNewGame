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
			 * @return 通常の動きの処理を行うか？
			*/
			bool CPlayerSwingAction::Execute()
			{
				// 通常の動きの処理を行うか？
				bool executeNormalMovementFlag = false;

				// スイングターゲットを探すか？
				if (m_swingActionState == enFindSwingTarget)
				{
					// スイングターゲットを探す
					FindSwingTarget();
				}

				if (m_playerRef->GetInputData().actionSwing != true)
				{
					m_swingActionState = enEnd;
					m_playerRef->SetState(nsPlayerConstData::enAirAfterStringAction);
				}

				switch (m_swingActionState)
				{
				case enIsStringStretching:

					executeNormalMovementFlag = true;

					if (m_playerMovementRef->IsAir() != true)
					{
						m_swingActionState = EnSwingActionState::enEnd;
					}
					else if (m_playerRef->IsStringStretched())
					{
						m_swingActionState = EnSwingActionState::enIsSwinging;
						if (m_swingSpeed <= FLT_EPSILON)
						{
							m_swingSpeed = 500.0f + fabsf(m_playerMovementRef->GetMoveVec().y) * 0.5f;
						}
						m_swingSpeed = 1500.0f;
					}
					
					break;
				case enIsSwinging:

					executeNormalMovementFlag = false;

					SwingAction();
					if (m_playerMovementRef->IsAir() != true)
					{
						m_swingActionState = EnSwingActionState::enEnd;
					}

					break;
				case enEnd:

					executeNormalMovementFlag = true;
					m_swingActionState = enFindSwingTarget;
					m_playerRef->EndStringStretchToPos();
					m_playerRef->SetState(nsPlayerConstData::EnPlayerState::enWalkAndRun);
					//m_playerRef->SetState(nsPlayerConstData::enAirAfterStringAction);

					break;
				}

				return executeNormalMovementFlag;
			}


			/**
			 * @brief スイングターゲットを探す
			*/
			void CPlayerSwingAction::FindSwingTarget()
			{

				Vector3 toFindSwintTargetPos = { 0.0f,2000.0f,1000.0f };
				Vector3 forward = m_playerRef->GetCamera().GetCameraForward();
				forward.y = 0.0f;
				forward.Normalize();
				float rad = acosf(Dot(forward, Vector3::Front));
				if (forward.x < 0.0f)
				{
					rad = -rad;
				}
				Quaternion qRot;
				qRot.SetRotationY(rad);
				qRot.Apply(toFindSwintTargetPos);

				// 指定された座標から有効範囲内にあるスイングターゲットの座標の中で一番近い座標を得る
				m_swingTargetPos = nsStringActionTarget::GetNearestSwintTargetPointWithinScope(
					m_playerRef->GetPosition() + toFindSwintTargetPos, 2000.0f
				);

				// スイングターゲットが見つかったか？
				if (m_swingTargetPos != nullptr)
				{
					// 見つかった
					// 糸をスイングターゲットに向かって伸ばし始める
					m_playerRef->StartStringStretchToPos(*m_swingTargetPos);
					m_swingActionState = EnSwingActionState::enIsStringStretching;
				}
				else
				{
					m_swingActionState = EnSwingActionState::enEnd;
				}


				return;
			}

			/**
			 * @brief スイングアクションの処理
			*/
			void CPlayerSwingAction::SwingAction()
			{
				// 加算移動ベクトル
				Vector3 addMoveVec = Vector3::Zero;
				
				// プレイヤーからスイングターゲットまでのXZ平面でのベクトル
				Vector3 playerToTargetVecXZ = *m_swingTargetPos - m_playerRef->GetPosition();
				// Y成分を消去
				playerToTargetVecXZ.y = 0.0f;

				// XZ平面での前方向
				Vector3 forwardDirXZ = m_playerRef->GetCamera().GetCameraForward();
				// Y成分を消去
				forwardDirXZ.y = 0.0f;
				// 正規化
				forwardDirXZ.Normalize();

				// XZ平面の前方向に、プレイヤーからスイングターゲットへのXZ平面のベクトルを射影する
				float projectToTargetVecXZToForwardDirXZ = Dot(forwardDirXZ, playerToTargetVecXZ);

				// XZ平面での、前方向のみの、スイングターゲットへのベクトル
				Vector3 toTargetFowardVecXZ = forwardDirXZ;
				toTargetFowardVecXZ.Scale(projectToTargetVecXZToForwardDirXZ);
				// XZ平面での、前方向のみの、スイングターゲットの座標
				const Vector3 toTargetForwardPosXZ = m_playerRef->GetPosition() + toTargetFowardVecXZ;
				// 前方向と上方向のみの、スイングターゲットの座標
				Vector3 toTargetForwardUpPos = toTargetForwardPosXZ;
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
				// 加算移動方向ベクトルを回転させる回転軸
				Vector3 rotAxisForAddMoveDir = Cross(targetUptoPlayerDir, forwardDirXZ);
				if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
				{
					rotAxisForAddMoveDir = Cross(Vector3::Down, forwardDirXZ);
				}
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
					addMoveVec = addMoveDir;
					//m_swingSpeed += 5.0f;
					addMoveVec.Scale(m_swingSpeed);

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
					}
					else
					{

						//m_swingSpeed += 5.0f;
						addMoveDir = forwardDirXZ;
						radAngle = 3.14f * 0.5f * rate;

					}

					// 回転クォータニオンを90度回転させる
					qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle);
					// 加算移動方向ベクトルを回転させる
					qRotForAddMoveDir.Apply(addMoveDir);
					addMoveDir.Normalize();
					addMoveVec = addMoveDir;
					rate += 0.5f;
					addMoveVec.Scale(m_swingSpeed /*+= 5.0f * min(rate, 1.0f)*/);

				}


				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecY();
				m_playerMovementRef->ResetMoveVecZ();

				// 移動ベクトルに、加算移動ベクトルを加算する
				m_playerMovementRef->AddMoveVec(addMoveVec);

				// キャラクターコントローラーを使って移動させる
				m_playerMovementRef->MoveWithCharacterController();


				return;
			}

		}
	}
}
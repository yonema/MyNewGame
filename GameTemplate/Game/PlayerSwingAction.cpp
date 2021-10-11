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
			 * @return 歩きと走りの処理を行うか？
			*/
			bool CPlayerSwingAction::Execute()
			{
				// 歩きと走りの処理を行うか？
				bool executeWalkAndRunFlag = false;

				// スイングターゲットを探すか？
				if (m_swingActionState == enFindSwingTarget)
				{
					// スイングターゲットを探す
					FindSwingTarget();
				}

				if (m_playerRef->GetInputData().actionSwing != true)
				{
					m_swingActionState = enEnd;
				}

				switch (m_swingActionState)
				{
				case enIsStringStretching:

					executeWalkAndRunFlag = true;

					if (m_playerMovementRef->IsAir() != true)
					{
						m_swingActionState = EnSwingActionState::enEnd;
					}
					else if (m_playerRef->IsStringStretched())
					{
						m_swingActionState = EnSwingActionState::enIsSwinging;
					}
					
					break;
				case enIsSwinging:

					executeWalkAndRunFlag = false;

					SwingAction();

					break;
				case enEnd:

					executeWalkAndRunFlag = true;
					m_swingActionState = enFindSwingTarget;
					m_playerRef->EndStringStretchToPos();
					m_playerRef->SetState(nsPlayerConstData::EnPlayerState::enWalkAndRun);

					break;
				}

				return executeWalkAndRunFlag;
			}


			/**
			 * @brief スイングターゲットを探す
			*/
			void CPlayerSwingAction::FindSwingTarget()
			{

				Vector3 toFindSwintTargetPos = { 0.0f,1000.0f,500.0f };
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
				// スイングターゲット（支点）から自身の座標（重り）へのベクトル
				Vector3 fromSwingTargetToPosVec = m_playerRef->GetPosition() - *m_swingTargetPos;
				// 移動する方向
				Vector3 moveDir = m_playerRef->GetCamera().GetCameraForward();
				moveDir.y = 0.5f;
				moveDir.Normalize();

				// 支点から重りへのベクトルを90度回すための軸
				Vector3 rotAxis = Vector3::Down;//fromSwingTargetToPosVec;
				rotAxis.Normalize();
				rotAxis.Cross(moveDir);

				// 支点から重りへのベクトルを90度回した方向ベクトル
				Vector3 verticalDir = fromSwingTargetToPosVec;
				// 90度回す回転クォータニオン
				Quaternion qRot;
				qRot.SetRotation(rotAxis, 3.14f * 0.5f);
				qRot.Apply(verticalDir);
				verticalDir.Normalize();
				verticalDir.Scale(500.0f);

				Vector3 next = m_playerRef->GetPosition() + verticalDir;

				Vector3 toNextVec = next - *m_swingTargetPos;
				Vector3 toNextDir = toNextVec;
				toNextDir.Normalize();
				Vector3 toOldVec = fromSwingTargetToPosVec;
				toOldVec.Normalize();
				float rad = acosf(Dot(toNextDir, toOldVec));

				qRot.SetRotation(rotAxis, rad);
				qRot.Apply(toNextVec);

				Vector3 add = *m_swingTargetPos + toNextVec - m_playerRef->GetPosition();



				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecY();
				m_playerMovementRef->ResetMoveVecZ();

				// 移動ベクトルに、加算移動ベクトルを加算する
				m_playerMovementRef->AddMoveVec(add);

				// キャラクターコントローラーを使って移動させる
				m_playerMovementRef->MoveWithCharacterController();

				Vector3 debugVec = m_playerRef->GetPosition() - *m_swingTargetPos;
				wchar_t text[128];
				swprintf_s(text, L"StringLen:%2.2f", debugVec.Length());
				m_debugFont->SetText(text);
				return;
			}

		}
	}
}
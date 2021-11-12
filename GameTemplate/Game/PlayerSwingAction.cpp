#include "stdafx.h"
#include "PlayerSwingAction.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "StringActionTargetManager.h"
#include "PlayerCamera.h"

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
			using namespace nsPlayerConstData::nsSwingActionConstData;


			/**
			 * @brief 初期化
			 * @param[in,out] player プレイヤー
			 * @param[in,out] playerMovement プレイヤー移動クラスの参照
			 * @param[in,out] playerCamera プレイヤーカメラクラスの参照
			 * @param[in.out] playerModelAnimation プレイヤーモデルアニメーションの参照
			*/
			void CPlayerSwingAction::Init(
				CPlayer* player,
				CPlayerMovement* playerMovement,
				CPlayerCamera* playerCamera,
				CPlayerModelAnimation* playerModelAnimation
			)
			{
				// 各参照をセット
				m_playerRef = player;
				m_playerMovementRef = playerMovement;
				m_playerCameraRef = playerCamera;
				m_playerModelAnimationRef = playerModelAnimation;
				return;
			}

			/**
			 * @brief スイングアクションを実行
			*/
			void CPlayerSwingAction::Execute()
			{
				nsDebug::DrawTextPanel(L"[SwingAction:Execute]");

				// スイングアクションの前に行う処理
				PreSwingAction();

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
					// スイング処理の終了
					EndSwing();
					break;
				}

				return;
			}

			/**
			 * @brief スイングアクションの前に行う処理
			*/
			void CPlayerSwingAction::PreSwingAction()
			{
				// スイングターゲットを探すか？
				if (m_swingActionState == enFindSwingTarget)
				{
					// スイングターゲットを探す
					FindSwingTarget();

					// 早期リターン
					return;
				}

				// 地面についてしまったか？
				if (m_playerMovementRef->IsAir() != true)
				{
					// スイングアクションを終了する
					m_swingActionState = EnSwingActionState::enEnd;

					// 早期リターン
					return;
				}


				// スイングアクションをやめたか？かつ
				// スイング後の空中状態ではないか？
				if (m_playerRef->GetInputData().actionSwing != true &&
					m_swingActionState != EnSwingActionState::enIsAirAfterSwing)
				{
						m_playerModelAnimationRef->SetSwingAnimState(
						nsPlayerConstData::nsModelAnimationConstData::enSwingAnim_airAfterSwing
					);
					// スイング中にスイングをやめたか？
					if (m_swingActionState == EnSwingActionState::enIsSwinging)
					{
						// スイング後の加速の初速をリセット
						m_accelerationAfterSwing = kInitialVelocityOfAterSwingAcceleration;
						// スイング後の速度を保持
						m_velocityAfterSwing = m_playerMovementRef->GetVelocity();
						// 入力によって生じたXZ平面での移動方向をリセット
						m_inputMoveDirXZ = Vector3::Zero;
						m_playerModelAnimationRef->SetSwingAnimState(
							nsPlayerConstData::nsModelAnimationConstData::enSwingAnim_swingRoll
						);

					}
					// ステートをスイング後の空中状態に遷移する
					m_swingActionState = EnSwingActionState::enIsAirAfterSwing;
					// 糸に終了を知らせる
					m_playerRef->EndStringStretchToPos();

					// 早期リターン
					return;
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

				// スイングターゲットを探すベクトルを回す回転
				Quaternion swingRotationQRot = Quaternion::Identity;

				// 糸を出す方向を決める
				if (m_playerRef->GetInputData().inputMoveAxis)
				{
					// 移動の入力があったら移動入力の方向に糸を出す。
					Vector3 inputDir = m_playerCameraRef->GetCameraForward() * m_playerRef->GetInputData().axisMoveForward;
					inputDir += m_playerCameraRef->GetCameraRight() * m_playerRef->GetInputData().axisMoveRight;
					inputDir.y = 0.0f;
					inputDir.Normalize();
					swingRotationQRot.SetRotation(Vector3::Front, inputDir);
				}
				else
				{
					// 入力がなかったらカメラの前方向に糸を出す。
					Vector3 cameraDir = m_playerRef->GetCamera().GetCameraForward();
					cameraDir.y = 0.0f;
					cameraDir.Normalize();
					swingRotationQRot.SetRotation(Vector3::Front, cameraDir);
				}

				// 複数の座標から、スイングターゲットを探す
				for (int i = 0; i < kFindSwintTargetNum; i++)
				{
					// スイングターゲットを探す座標へのベクトル
					Vector3 toFindSwingTargetVec = kToFindSwingTargetVecs[i];
					// 糸を出す方向に回転させる
					swingRotationQRot.Apply(toFindSwingTargetVec);

					// 一番近いスイングターゲットのポイントの座標を探す処理のデータ
					nsStringActionTarget::SFindNearestSwingTargetPointData findData;
					// スイングターゲットを探すデータを設定
					findData.findSwingTargetOrigin = m_playerRef->GetPosition() + toFindSwingTargetVec;
					findData.findSwingTargetScopeRadius = kFindSwingTargetScope;
					// スイングターゲットのポイントを探すデータを設定
					findData.findSwingTargetPointOrigin = m_playerRef->GetPosition() + toFindSwingTargetVec;
					findData.findSwingTargetPointScopeRadius = kFindSwingTargetScope;
					// スイングターゲットのポイントを探す下限を設定
					findData.heightLowerLimit = m_playerRef->GetPosition().y + 500.0f;
					findData.forwardLimitDir = Vector3::Front;
					m_playerRef->GetRotation().Apply(findData.forwardLimitDir);
					findData.forwardLimitOrigin = m_playerRef->GetPosition();

					// 指定された座標から有効範囲内にあるスイングターゲットの座標の中で一番近い座標を得る
					const Vector3* swingTargetPos =
						nsStringActionTarget::FindNearestSwingTargetPoint(findData);

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
					m_playerModelAnimationRef->SetSwingAnimState(
						nsPlayerConstData::nsModelAnimationConstData::enSwingAnim_swingStart
					);
				}
				else
				{
					// 見つからなかった
					// ステートをスイング後の空中に遷移
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

				//////// 糸が伸びきった。スイングをする準備をする ////////

				//////// ステート遷移 ////////
				// 伸ばし切ったから、ステートをスイング中に遷移
				m_swingActionState = EnSwingActionState::enIsSwinging;

				//////// パラメータのリセット ////////
				// 減速し始めるスイングスピードを初期化する
				m_startDecelerateSwingSpeed = kStartDecelerateSwingSpeedInitialValue;
				// 入力によって生じたXZ平面での移動方向を初期化する
				m_inputMoveDirXZ = Vector3::Zero;

				m_counter = 0;
				m_afterSwing = true;
				//////// スイングスピードの初期速度の計算 ////////
				// スイングスピードが初期速度より遅いか？
				if (m_swingSpeed <= kInitialSwingSpeed)
				{
					// 遅い。初期速度のスイングスピードを設定する。落下速度が速いとスイングスピードも速くなる。
					//m_swingSpeed = kInitialSwingSpeed +
					//	fabsf(m_playerMovementRef->GetMoveVec().y) * kFallImpactRateForInitialSwingSpeed;
					m_swingSpeed = 2000.0f;
					m_g = 700.0f;

					// 早期リターン
					return;
				}
				m_g = m_playerMovementRef->GetVelocity();
				// スイングスピードが最大速度を超えているか？
				const float maxG = 1000.0f;
				if (m_g > maxG)
				{
					// 超えている。最大速度に設定する。
					m_g = maxG;
				}
				return;
			}

			/**
			 * @brief スイングアクションの処理
			*/
			void CPlayerSwingAction::SwingAction()
			{
				nsDebug::DrawTextPanel(L"SwingAction");

				const int counterMax = 30;
				if (m_counter < counterMax)
					m_counter++;
				m_playerCameraRef->LerpDampingRate(/*0.5f - 0.5f*/1.0f - 1.0f * m_counter / counterMax);
				//////// 1.必要なベクトルを用意 ////////
				
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
				Vector3 addMoveDir = Vector3::Zero;

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


				//////// 2.スイングの角度を計算 ////////

				// 内積が負か？、つまり、スイングターゲットより手前側にいるか？
				if (dotFowardDirXZAndToPlayerDir < 0.0f)
				{
					// 手前側

					// プレイヤーが最低スイング高度より上にいるか？
					if (m_playerRef->GetPosition().y > 100.0f)
					{
						// 上にいる

						// 0.0f～1.0fに変化するレート
						float rate = dotFowardDirXZAndToPlayerDir + 1.0f;

						// pattern1
						// 指数関数的な変化にする
						//rate = pow(rate, 5.0f);
						// 1.0f～0.0fに変化するようにする
						//rate = 1.0f - rate;
						//float radAngle = 3.14f * 0.1f * -rate;

						// pattern2
						// 二次関数的な変化にする
						rate = pow(rate - 0.5f, 3.0f);
						// 上に凸にする
						rate *= -1.0f;
						// rateが最低値と最高値で0.0fになるような切片にする
						rate += pow(0.5f, 3.0f);
						// 90度にレートをかける
						float radAngle = 3.14f * 0.5f * -rate;

						// 進む方向をまっすぐ前にする
						addMoveDir = forwardDirXZ;
						// 回転クォータニオンを90度回転させる
						qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle/*3.14f * 0.5f*/);
						// 加算移動方向ベクトルを回転させる
						qRotForAddMoveDir.Apply(addMoveDir);
					}
					else
					{
						// 最低高度にいるため、まっすぐ進む。
						addMoveDir = forwardDirXZ;
					}

				}
				else
				{
					// 奥側

					float radAngle = 3.14f * 0.5f;
					m_playerModelAnimationRef->SetSwingAnimState(
						nsPlayerConstData::nsModelAnimationConstData::enSwingAnim_swingRaiseLeg
					);

					// プレイヤーの高さがスイングターゲットより高くなったら。
					if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
					{
						// 初回ループだけ減速し始めるスピードを設定
						if (m_startDecelerateSwingSpeed <= -50.0f)
						{
							m_startDecelerateSwingSpeed = m_swingSpeed;
						}
						float rad = acosf(dotFowardDirXZAndToPlayerDir);
						float angleRate = rad / (3.14f * 0.1f);
						addMoveDir = forwardDirXZ;
						// radAngle = 3.14f * 0.5f * min(1.0f, angleRate);
						radAngle = Math::Lerp<float>(min(1.0f, angleRate), 3.14f * 0.4f, 3.14f * 0.5f);


					}

					addMoveDir = targetUptoPlayerDir;
					// 回転クォータニオンを90度回転させる
					qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle);
					// 加算移動方向ベクトルを回転させる
					qRotForAddMoveDir.Apply(addMoveDir);
					
				}

				//////// 3.スイングスピードを計算 ////////

				// 任意の位置での振り子の速度
				// g = 重力加速度、l = 振り子の長さ、v任 = 任意の場所での振り子の速度、
				// cosΘ任 = 任意の場所の角度、cosΘ上 = 一番上の時の角度
				// v任 = √2gl(cosΘ任 - cosΘ上)

				// cosΘ任 = 任意の場所の角度
				float anyCos = Dot(targetUptoPlayerDir, Vector3::Down);
				// cosΘ上 = 一番上の時の角度（一番上の時の角度は90度）
				float highestCos = 0.0f;
				// g = 重力加速度
				m_g += 980.0f * nsTimer::CGameTime().GetFrameDeltaTime() * 0.25f;
				const float maxG = 1200.0f;
				if (m_g > maxG)
				{
					m_g = maxG;
				}
				// l = 振り子の長さ
				const float l = toTargetForwardUpToPlayerVec.Length();
				// v任 = √2gl(cosΘ任 - cosΘ上)
				m_swingSpeed = 2.0f * m_g * l * (anyCos - highestCos);
				m_swingSpeed = std::sqrtf(m_swingSpeed);


				//////// 4.スイング中の左右への方向転換 ////////
				Vector3 rightDirXZ = m_playerCameraRef->GetCameraRight();
				rightDirXZ.y = 0.0f;
				rightDirXZ.Normalize();
				float rightPower = m_playerRef->GetInputData().axisMoveRight / 7.0f;
				rightDirXZ.Scale(rightPower);
				m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, rightDirXZ);

				addMoveDir += m_inputMoveDirXZ;

				addMoveDir.Normalize();

				// 加算移動ベクトル
				Vector3 addMoveVec = addMoveDir;
				addMoveVec.Scale(m_swingSpeed);


				// 移動ベクトルに、加算移動ベクトルを加算する
				m_playerMovementRef->AddMoveVec(addMoveVec);
				m_playerMovementRef->LimitMoveVec(m_swingSpeed);

				return;
			}


			/**
			 * @brief スイング後の空中の処理
			*/
			void CPlayerSwingAction::AirAfterSwing()
			{
				nsDebug::DrawTextPanel(L"AirAfterSwing");

				// 次のスイングへの遷移処理
				if (m_playerRef->GetInputData().actionSwing == true)
				{
					// 下降中ならスイングへ移行できる
					if (m_playerMovementRef->GetMoveVec().y < 0.0f)
					{
						m_swingActionState = enFindSwingTarget;
					}
				}


				m_playerCameraRef->LerpDampingRate(1.0f);

				m_accelerationAfterSwing *= 0.99f;
				if (m_accelerationAfterSwing < kMinVelocityOfAfterSwingAcceleration)
				{
					m_accelerationAfterSwing = kMinVelocityOfAfterSwingAcceleration;
				}
				float velocity = m_velocityAfterSwing + m_accelerationAfterSwing;


				if (velocity < nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed)
				{
					velocity = nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed;
				}

				Vector3 addMoveVec = m_playerMovementRef->GetMoveVec();

				if (m_afterSwing)
				{
					m_afterSwing = false;
				}
				else
				{
					addMoveVec.y = 0.0f;
				}

				addMoveVec.Normalize();
				addMoveVec.Scale(velocity);

				if (m_playerRef->GetInputData().inputMoveAxis)
				{

					//Vector3 rightDirXZ = m_playerCameraRef->GetCameraRight();
					//rightDirXZ.y = 0.0f;
					//rightDirXZ.Normalize();
					//float rightPower = m_playerRef->GetInputData().axisMoveRight / 7.0f;
					//rightDirXZ.Scale(rightPower);
					//m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, rightDirXZ);
					//addMoveDir += m_inputMoveDirXZ;

					//addMoveDir.Normalize();
					//addVec = addMoveDir;
					//addVec.Scale(velocity);
				}


				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecZ();

				m_playerMovementRef->AddMoveVec(addMoveVec);
				return;
			}

			/**
			 * @brief スイング処理の終了
			*/
			void CPlayerSwingAction::EndSwing()
			{
				// ステートをスイングターゲットを探す状態に戻しておく
				m_swingActionState = enFindSwingTarget;
				// 糸に終了を知らせる
				m_playerRef->EndStringStretchToPos();
				// プレイヤーのステートを歩きと走りにする
				m_playerRef->ChangeWalkAndRunState();
				// スイングスピードをリセット
				m_swingSpeed = 0.0f;

				return;
			}

		}
	}
}
#include "stdafx.h"
#include "PlayerSwingAction.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "StringActionTargetManager.h"
#include "PlayerCamera.h"
#include "SoundCue.h"

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
			// プレイヤーの定数データを使用可能にする
			using namespace nsPlayerConstData;
			// プレイヤーのスイングアクションクラスの定数データを使用可能にする
			using namespace nsSwingActionConstData;

			/**
			 * @brief コンストラクタ
			*/
			CPlayerSwingAction::CPlayerSwingAction()
			{

				// サウンドの初期化
				InitSound();

				return;
			}

			/**
			 * @brief デストラクタ
			*/
			CPlayerSwingAction::~CPlayerSwingAction()
			{
				DeleteGO(m_chainPutOutSC);
				DeleteGO(m_chainBendingSC);
				DeleteGO(m_chainReleaseSC);
				DeleteGO(m_swingLeaveSC);
				DeleteGO(m_swingRollLeaveSC);

				return;
			}

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
				nsDebug::DrawTextPanel(m_g,L"m_g");
				nsDebug::DrawTextPanel(m_swingSpeed,L"m_swingSpeed");


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
					ChangeState(enEnd);

					// 早期リターン
					return;
				}


				// ジャンプ入力があるか？、かつ
				// スイング中か？
				if (m_playerRef->GetInputData().actionJump &&
					m_swingActionState == enIsSwinging)
				{
					m_swingRollFlag = true;

					// ステートをスイング後の空中状態に遷移する
					ChangeState(enIsAirAfterSwing);

					// 早期リターン
					return;
				}

				// スイングアクションをやめたか？かつ
				// スイング後の空中状態ではないか？
				if (m_playerRef->GetInputData().actionSwing != true &&
					m_swingActionState != EnSwingActionState::enIsAirAfterSwing)
				{
					// ステートをスイング後の空中状態に遷移する
					ChangeState(enIsAirAfterSwing);

					// 早期リターン
					return;
				}


				return;
			}

			/**
			 * @brief サウンドを初期化
			*/
			void CPlayerSwingAction::InitSound()
			{
				m_chainPutOutSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_chainPutOutSC->Init(kChainPutOutSoundFilePath, nsSound::CSoundCue::enSE);
				m_chainPutOutSC->SetVolume(kChainPutOutSoundVolume);

				m_chainBendingSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_chainBendingSC->Init(kChainBendingSoundFilePath, nsSound::CSoundCue::enSE);
				m_chainBendingSC->SetVolume(kChainBendingSoundVolume);

				m_chainReleaseSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_chainReleaseSC->Init(kChainReleaseSoundFilePath, nsSound::CSoundCue::enSE);
				m_chainReleaseSC->SetVolume(kChainReleaseSoundVolume);

				m_swingLeaveSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_swingLeaveSC->Init(kSwingLeaveSoundFilePath, nsSound::CSoundCue::enSE);
				m_swingLeaveSC->SetVolume(kSwingLeaveSoundVolume);

				m_swingRollLeaveSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_swingRollLeaveSC->Init(kSwingRollLeaveSoundFilePath, nsSound::CSoundCue::enSE);
				m_swingRollLeaveSC->SetVolume(kSwingRollLeaveSoundVolume);

				return;
			}


			/**
			 * @brief スイングターゲットを探す
			*/
			void CPlayerSwingAction::FindSwingTarget()
			{
				//////// 1.糸を出す方向を決める ////////

				// 糸を出す方向への回転クォータニオン
				Quaternion swingRotationQRot = Quaternion::Identity;

				// 移動入力方向かカメラの前方向から糸を出す方向を決める
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
				// 回転から糸を出す方向を求める
				m_swingForwardDir = Vector3::Front;
				swingRotationQRot.Apply(m_swingForwardDir);
				m_swingForwardDir.Normalize();

				//////// 2.スイングターゲットを探す ////////

				// スイングターゲットへの距離
				float dist = FLT_MAX;
				// スイングターゲットを初期化
				m_swingTargetPos = nullptr;
				// 一番近いスイングターゲットのポイントの座標を探す処理のデータ
				nsStringActionTarget::SFindNearestSwingTargetPointData findData;
				// 共通のデータを設定する
				// 有効範囲を設定
				findData.findSwingTargetScopeRadius = kFindSwingTargetScope;
				findData.findSwingTargetPointScopeRadius = kFindSwingTargetScope;
				// スイングターゲットのポイントを探す下限を設定
				findData.heightLowerLimit = m_playerRef->GetPosition().y + 500.0f;
				// 前方向の制限の方向ベクトルを設定
				findData.forwardLimitDir = m_swingForwardDir;
				// 前方向の制限の原点を設定
				findData.forwardLimitOrigin = m_playerRef->GetPosition();

				// 複数の座標から、スイングターゲットを探す
				for (int i = 0; i < kFindSwintTargetNum; i++)
				{
					// スイングターゲットを探す座標へのベクトル
					Vector3 toFindSwingTargetVec = kToFindSwingTargetVecs[i];
					// 糸を出す方向に回転させる
					swingRotationQRot.Apply(toFindSwingTargetVec);

					// 個別のデータを設定する
					// スイングターゲットを探すデータを設定
					findData.findSwingTargetOrigin = m_playerRef->GetPosition() + toFindSwingTargetVec;
					// スイングターゲットのポイントを探すデータを設定
					findData.findSwingTargetPointOrigin = m_playerRef->GetPosition() + toFindSwingTargetVec;

					// 指定された座標から有効範囲内にあるスイングターゲットの座標の中で一番近い座標を得る
					const Vector3* swingTargetPos =
						nsStringActionTarget::FindNearestSwingTargetPoint(findData);

					// スイングターゲットの座標がnullptrか？
					if (swingTargetPos == nullptr)
					{
						// 見つからなかった。次へ。
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

				//////// 3.スイングターゲットが見つかったかどうかで処理を振り分ける ////////

				// スイングターゲットが見つかったか？
				if (m_swingTargetPos != nullptr)
				{
					// 見つかった
					// ステートを糸を伸ばし中へ遷移する
					ChangeState(enIsStringStretching);

				}
				else
				{
					// 見つからなかった
					// ステートをスイング後の空中に遷移
					ChangeState(enIsAirAfterSwing);
				}

				return;
			}

			/**
			 * @brief 糸を伸ばしている最中の処理
			*/
			void CPlayerSwingAction::StringStretching()
			{
				// カメラの値を線形変化させる
				CameraChangeLinearly();

				// 伸びきってないか？
				if (m_playerRef->IsStringStretched() != true)
				{
					// まだ伸びきってない。早期リターン
					return;
				}

				// 伸ばし切ったから、ステートをスイング中に遷移
				ChangeState(enIsSwinging);

				return;
			}

			/**
			 * @brief スイングアクションの処理
			*/
			void CPlayerSwingAction::SwingAction()
			{
				// カメラの値を線形変化させる
				// StringStretching()でも呼んでるが、変化しきる前に糸が伸びきってしまうかも
				// しれないため、こちらでも呼ぶ
				CameraChangeLinearly();

				//////// 1.必要なベクトルを用意 ////////
				
				// プレイヤーからスイングターゲットまでのXZ平面でのベクトル
				Vector3 playerToTargetVecXZ = *m_swingTargetPos - m_playerRef->GetPosition();
				// Y成分を消去
				playerToTargetVecXZ.y = 0.0f;

				// XZ平面の前方向に、プレイヤーからスイングターゲットへのXZ平面のベクトルを射影する
				float projectToTargetVecXZToSwingForwardDir = Dot(m_swingForwardDir, playerToTargetVecXZ);

				// XZ平面での、前方向のみの、スイングターゲットへのベクトル
				Vector3 toTargetFowardVecXZ = m_swingForwardDir;
				// 射影して求めた長さまで伸ばす
				toTargetFowardVecXZ.Scale(projectToTargetVecXZToSwingForwardDir);

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
				float doSwingForwardDirAndToPlayerDir = 
					Dot(m_swingForwardDir, targetUptoPlayerDir);

				// 加算移動方向ベクトル
				Vector3 addMoveDir = Vector3::Zero;

				// 加算移動方向ベクトルを回転させる回転クォータニオン
				Quaternion qRotForAddMoveDir;

				// 加算移動方向ベクトルを回転させる回転軸。
				Vector3 rotAxisForAddMoveDir = Cross(targetUptoPlayerDir, m_swingForwardDir);

				// プレイヤーがスイングターゲットより上にいるか？
				if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
				{
					// 上にいるとき、回転軸の求め方を変える
					rotAxisForAddMoveDir = Cross(Vector3::Down, m_swingForwardDir);
				}

				// 求めた回転軸を正規化する
				rotAxisForAddMoveDir.Normalize();


				//////// 2.スイングの角度を計算 ////////

				// 内積が負か？、つまり、スイングターゲットより手前側にいるか？
				if (doSwingForwardDirAndToPlayerDir < 0.0f)
				{
					// 手前側

					// プレイヤーが最低スイング高度より上にいるか？
					if (m_playerRef->GetPosition().y > 100.0f)
					{
						// 上にいる

						// 0.0f～1.0fに変化するレート
						float rate = doSwingForwardDirAndToPlayerDir + 1.0f;

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
						addMoveDir = m_swingForwardDir;
						// 回転クォータニオンを90度回転させる
						qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle/*3.14f * 0.5f*/);
						// 加算移動方向ベクトルを回転させる
						qRotForAddMoveDir.Apply(addMoveDir);
					}
					else
					{
						// 最低高度にいるため、まっすぐ進む。
						addMoveDir = m_swingForwardDir;
					}

				}
				else
				{
					// 奥側

					float radAngle = 3.14f * 0.5f;
					m_playerModelAnimationRef->SetSwingAnimState(
						nsModelAnimationConstData::enSwingAnim_swingRaiseLeg
					);

					// プレイヤーの高さがスイングターゲットより高くなったら。
					if (m_playerRef->GetPosition().y >= m_swingTargetPos->y)
					{
						// 初回ループだけ減速し始めるスピードを設定
						if (m_startDecelerateSwingSpeed <= -50.0f)
						{
							m_startDecelerateSwingSpeed = m_swingSpeed;
						}
						float rad = acosf(doSwingForwardDirAndToPlayerDir);
						float angleRate = rad / (3.14f * 0.1f);
						addMoveDir = m_swingForwardDir;
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
				const float maxG = 2500.0f;
				if (m_g > maxG)
				{
					m_g = maxG;
				}
				// l = 振り子の長さ
				const float l = toTargetForwardUpToPlayerVec.Length();
				// v任 = √2gl(cosΘ任 - cosΘ上)
				if (doSwingForwardDirAndToPlayerDir < 0.0f)
				{
					// 手前側なら角度による減速しない
					m_swingSpeed = 2.0f * m_g * l;
				}
				else
				{
					// 奥側なら、上に上がるほど減速する
					m_swingSpeed = 2.0f * m_g * l * (anyCos - highestCos);
				}
				m_swingSpeed = std::sqrtf(m_swingSpeed);


				//////// 4.スイング中の左右への方向転換 ////////
				Vector3 rightDirXZ = m_playerCameraRef->GetCameraRight();
				rightDirXZ.y = 0.0f;
				rightDirXZ.Normalize();
				float rightPower = m_playerRef->GetInputData().axisMoveRight / 7.0f;
				rightDirXZ.Scale(rightPower);
				m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, rightDirXZ);
				addMoveDir += m_inputMoveDirXZ;

				// 正規化して方向ベクトルにする
				addMoveDir.Normalize();

				// 加算移動ベクトル
				Vector3 addMoveVec = addMoveDir;
				addMoveVec.Scale(m_swingSpeed);

				// 現在の移動方向と、スイングの加算移動方向が逆向きなら
				// 現在の移動ベクトルをリセットする。
				Vector3 moveDir = m_playerMovementRef->GetMoveVec();
				moveDir.Normalize();
				if (Dot(addMoveDir, moveDir) <= 0.0f)
				{
					m_playerMovementRef->ResetMoveVecX();
					m_playerMovementRef->ResetMoveVecZ();
				}


				// 移動ベクトルに、加算移動ベクトルを加算する
				m_playerMovementRef->AddMoveVec(addMoveVec);
				m_playerMovementRef->LimitMoveVec(m_swingSpeed);

				// スイングの前方向を更新
				m_swingForwardDir = m_playerMovementRef->GetMoveVec();
				// Y成分を消去
				m_swingForwardDir.y = 0.0f;
				m_swingForwardDir.Normalize();

				return;
			}


			/**
			 * @brief スイング後の空中の処理
			*/
			void CPlayerSwingAction::AirAfterSwing()
			{
				// 次のスイングへの遷移処理
				if (m_playerRef->GetInputData().actionSwing == true)
				{
					// 下降中か？、かつ
					// アニメーションが空中アイドル状態か？
					if (m_playerMovementRef->GetMoveVec().y < 0.0f &&
						m_playerRef->GetPlayerModelAnimation().GetAnimationState() ==
						nsModelAnimationConstData::enAnim_airIdle)
					{
						ChangeState(enFindSwingTarget);
					}
				}

				// スイング後の加速の減衰
				m_accelerationAfterSwing *= 0.99f;
				if (m_accelerationAfterSwing < kMinVelocityOfAfterSwingAcceleration)
				{
					m_accelerationAfterSwing = kMinVelocityOfAfterSwingAcceleration;
				}

				// スイング後の速度どスイング後の加速を合わせて速度とする
				float velocity = m_velocityAfterSwing + m_accelerationAfterSwing;

				// 加算移動ベクトル
				Vector3 addMoveVec = m_playerMovementRef->GetMoveVec();

				// スイングの直後か？
				if (m_afterSwing)
				{
					// スイングの直後ではなくする
					m_afterSwing = false;
					// スイングロールをするか？
					if (m_swingRollFlag)
					{
						// スイングロールを行った
						m_swingRollFlag = false;
						// Y成分にジャンプを加える
						addMoveVec.y += nsMovementConstData::kJumpForce;
						m_swingRollLeaveSC->Play(false);
					}
					else
					{
						// スイング状態から離れるのサウンドを流す
						m_swingLeaveSC->Play(false);
					}
				}
				else
				{
					// 最初以外はY成分は加えない
					addMoveVec.y = 0.0f;
				}

				// 速度で伸ばす
				addMoveVec.Normalize();
				addMoveVec.Scale(velocity);

				// 移動入力処理
				if (m_playerRef->GetInputData().inputMoveAxis)
				{
					Vector3 forwardDirXZ = m_playerCameraRef->GetCameraForward();
					forwardDirXZ.y = 0.0f;
					forwardDirXZ.Normalize();
					Vector3 rightDirXZ = m_playerCameraRef->GetCameraRight();
					rightDirXZ.y = 0.0f;
					rightDirXZ.Normalize();
					Vector3 inputDirXZ = forwardDirXZ * m_playerRef->GetInputData().axisMoveForward;
					inputDirXZ += rightDirXZ * m_playerRef->GetInputData().axisMoveRight;
					float radAngle = acosf(Dot(inputDirXZ, m_swingForwardDir));
					if (radAngle <= 3.14f * 0.25f || radAngle >= 3.14f * 0.7f)
					{
						if (radAngle >= 3.14f * 0.7f)
						{
							m_velocityAfterSwing -= nsWalkAndRunConstData::kWalkAcceleration;
							if (m_velocityAfterSwing <= nsWalkAndRunConstData::kWalkMaxSpeed)
							{
								ChangeState(enEnd);
							}
						}
					}
					else
					{
						inputDirXZ *= nsWalkAndRunConstData::kWalkMaxSpeed / 5.0f;
						m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, inputDirXZ);
					}

					addMoveVec += m_inputMoveDirXZ;

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
				ChangeState(enFindSwingTarget);
				// 糸に終了を知らせる
				m_playerRef->EndStringStretchToPos();
				// プレイヤーのステートを歩きと走りにする
				m_playerRef->ChangeWalkAndRunState();
				// スイングスピードをリセット
				m_swingSpeed = 0.0f;
				// カメラの減衰率をデフォルトに戻す
				m_playerCameraRef->LerpDampingRate(0.0f);

				return;
			}

			/**
			 * @brief スイングアクションステートを変更する
			 * @param swingActionState
			*/
			void CPlayerSwingAction::ChangeState(const EnSwingActionState swingActionState)
			{
				if (m_swingActionState == swingActionState)
				{
					// 同じなら変更する必要なし。早期リターン。
					return;
				}

				// スイングアクションステートを変更
				m_swingActionState = swingActionState;

				//////// 変更した時一度だけ呼ばれる処理 ////////

				// 各種、変更した時一度だけ呼ばれるイベント
				switch (m_swingActionState)
				{
				case enFindSwingTarget:
					break;
				case enIsStringStretching:
					IsStringStretchingEvent();
					break;
				case enIsSwinging:
					IsSwingingEvent();
					break;
				case enIsAirAfterSwing:
					IsAirAfterSwingEvent();
					break;
				case enEnd:
					break;
				}

				return;
			}

			/**
			 * @brief 糸を伸ばし中に遷移するときに一度だけ呼ばれる関数
			*/
			void CPlayerSwingAction::IsStringStretchingEvent()
			{
				// 鎖を出すのサウンドを流す
				m_chainPutOutSC->Play(false);

				// 糸をスイングターゲットに向かって伸ばし始める
				m_playerRef->StartStringStretchToPos(*m_swingTargetPos);
				// 糸を伸ばし始めるアニメーションを再生する
				m_playerModelAnimationRef->SetSwingAnimState(
					nsModelAnimationConstData::enSwingAnim_swingStart
				);
				// カメラの値を線形変化させるタイマーをリセットする
				m_cameraChangeLinearlyTimer = 0.0f;

				return;
			}

			/**
			 * @brief スイング中に遷移するときに一度だけ呼ばれる関数
			*/
			void CPlayerSwingAction::IsSwingingEvent()
			{
				// 鎖がしなるのサウンドを流す
				m_chainBendingSC->Play(false);

				// 減速し始めるスイングスピードを初期化する
				m_startDecelerateSwingSpeed = kStartDecelerateSwingSpeedInitialValue;
				// 入力によって生じたXZ平面での移動方向を初期化する
				m_inputMoveDirXZ = Vector3::Zero;

				m_afterSwing = true;


				// スイングスピードが初期速度より遅いか？
				if (m_swingSpeed <= kInitialSwingSpeed)
				{
					// 遅い。初期速度のスイングスピードを設定する。落下速度が速いとスイングスピードも速くなる。
					m_g = Math::Lerp<float>(
							min(1.0f, fabsf(m_playerMovementRef->GetMoveVec().y) / 5000.0f),
							700.0f,
							1200.0f
							);

					// 早期リターン
					return;
				}
				m_g = m_playerMovementRef->GetXZSpeed();
				// スイングスピードが最大速度を超えているか？
				const float maxG = 2000.0f;
				if (m_g > maxG)
				{
					// 超えている。最大速度に設定する。
					m_g = maxG;
				}

				return;
			}

			/**
			 * @brief スイング後の空中に遷移するときに一度だけ呼ばれるイベント
			*/
			void CPlayerSwingAction::IsAirAfterSwingEvent()
			{
				// 糸に終了を知らせる
				m_playerRef->EndStringStretchToPos();

				if (m_swingRollFlag)
				{
					// スイングロールを行う
					// スイングロールのアニメーションを再生する
					m_playerModelAnimationRef->SetSwingAnimState(
						nsModelAnimationConstData::enSwingAnim_swingRoll
					);
				}
				else
				{
					// スイングロールを行わない
					// スイング後の空中状態のアニメーションを再生する
					m_playerModelAnimationRef->SetSwingAnimState(
						nsModelAnimationConstData::enSwingAnim_airAfterSwing
					);
				}


				// スイング中にスイングをやめたか？
				if (m_afterSwing)
				{
					// スイング後の加速の初速をリセット
					m_accelerationAfterSwing = kInitialVelocityOfAterSwingAcceleration;
					// スイング後の速度を保持
					m_velocityAfterSwing = m_playerMovementRef->GetXZSpeed();
					// 入力によって生じたXZ平面での移動方向をリセット
					m_inputMoveDirXZ = Vector3::Zero;
					// カメラの値をリセット
					m_playerCameraRef->LerpDampingRate(1.0f);
					m_playerCameraRef->LerpTargetOffsetUp(0.0f);
					m_swingForwardDir = m_playerMovementRef->GetMoveVec();
					m_swingForwardDir.y = 0.0f;
					m_swingForwardDir.Normalize();

					if (m_chainBendingSC->IsPlaying())
					{
						// 鎖がしなるのサウンドがまだ流れていたら、止める
						m_chainBendingSC->Stop();
					}

					// 鎖を離すのサウンドを流す
					m_chainReleaseSC->Play(false);

				}
				else
				{
					// スイング中ではない
					m_accelerationAfterSwing = 0.0f;
					m_velocityAfterSwing = 0.0f;
					ChangeState(enEnd);
				}

				return;
			}

			/**
			 * @brief カメラの値を線形変化させる
			*/
			void CPlayerSwingAction::CameraChangeLinearly()
			{
				if (m_cameraChangeLinearlyTimer >= kCameraLerpTime)
				{
					// タイマーが規定値を超えていたら、早期リターン。
					return;
				}

				// タイマーを進める
				m_cameraChangeLinearlyTimer += nsTimer::GameTime().GetFrameDeltaTime();

				// カメラの値を線形補完して変化させる
				m_playerCameraRef->LerpDampingRate(1.0f - 1.0f * m_cameraChangeLinearlyTimer / kCameraLerpTime);
				m_playerCameraRef->LerpTargetOffsetUp(1.0f * m_cameraChangeLinearlyTimer / kCameraLerpTime);

				return;
			}

		}
	}
}
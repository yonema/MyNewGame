#include "stdafx.h"
#include "PlayerWalkAndRun.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "PlayerConstData.h"
#include "SoundCue.h"

namespace nsNinjaAttract
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
			using namespace nsPlayerConstData::nsWalkAndRunConstData;

			/**
			 * @brief コンストラクタ
			*/
			CPlayerWalkAndRun::CPlayerWalkAndRun()
			{
				// サウンドの初期化
				InitSound();

				return;
			}
			/**
			 * @brief デストラクタ
			*/
			CPlayerWalkAndRun::~CPlayerWalkAndRun()
			{
				DeleteGO(m_walkSC);
				DeleteGO(m_runSC);
				DeleteGO(m_jumpSC);

				return;
			}

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
			 * @brief 現在の動きに、移動パラメータを合わせる
			*/
			void CPlayerWalkAndRun::MuchMoveParam()
			{
				// 現在の移動ベクトルを取得
				m_moveDir = m_playerMovementRef->GetMoveVec();
				// Y成分を消去
				m_moveDir.y = 0.0f;

				// 速度をXZ平面での移動ベクトルから計算
				m_velocity = m_moveDir.Length();
				// 前のフレームの速度を設定
				m_oldVelocity = m_velocity;

				// 移動ベクトルを移動方向ベクトルにする
				m_moveDir.Normalize();	// 正規化する

				return;
			}

			/**
			 * @brief このクラスのサウンドを停止する
			*/
			void CPlayerWalkAndRun::StopSound()
			{
				// 再生中のサウンドをすべて停止する

				if (m_walkSC->IsPlaying())
				{
					m_walkSC->Stop();
				}
				if (m_runSC->IsPlaying())
				{
					m_runSC->Stop();
				}

				// ジャンプは止めない
				//if (m_jumpSC->IsPlaying())
				//{
				//	m_jumpSC->Stop();
				//}

				return;
			}

			/**
			 * @brief 歩きと走りの処理を実行
			*/
			void CPlayerWalkAndRun::Execute()
			{
				nsDebug::DrawTextPanel(L"[WalkAndRun:Execute]");

				// 歩きか走りの移動の処理
				WalkOrRunMove();

				// サウンドの更新
				UpdateSound();
				
				return;
			}

			/**
			 * @brief サウンドの初期化
			*/
			void CPlayerWalkAndRun::InitSound()
			{
				m_walkSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_walkSC->Init(kWalkSoundFilePath, nsSound::CSoundCue::enSE);
				m_walkSC->SetVolume(kWalkSoundVolume);

				m_runSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_runSC->Init(kRunSoundFilePath, nsSound::CSoundCue::enSE);
				m_runSC->SetVolume(kRunSoundVolume);

				m_jumpSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_jumpSC->Init(kJumpSoundFilePath, nsSound::CSoundCue::enSE);
				m_jumpSC->SetVolume(kJumpSoundVolume);

				return;
			}

			/**
			 * @brief 歩きか走りの移動の処理
			*/
			void CPlayerWalkAndRun::WalkOrRunMove()
			{
				// 軸入力値を更新する
				UpdateInputAxisParam();

				// 歩きか走りかを決める
				WalkOrRun();

				// 移動方向を更新する
				UpdateMoveDir();

				// 加速を計算
				CalcAcceleration();

				// 摩擦を計算
				CalcFriction();

				// 速度制限の計算
				CalcLimitSpeed();

				// 実際に移動させる
				Move();

				// ジャンプの更新
				UpdateJump();

				// 前のフレームの速度を更新
				m_oldVelocity = m_velocity;

				return;

			}

			/**
			 * @brief 糸を使ったアクションの後の空中の処理
			*/
			void CPlayerWalkAndRun::AirAfterStringAction()
			{

				// 軸入力値を更新する
				UpdateInputAxisParam();

				// 移動方向を更新する
				UpdateMoveDir();

				// 加速を計算
				CalcAcceleration();

				// 速度制限の計算
				CalcLimitSpeed();

				
				m_velocity += m_playerMovementRef->GetPlayerSwingAction().GetSwingSpeed();

				// 実際に移動させる
				Move();

				return;
			}

			/**
			 * @brief 軸入力値を更新
			*/
			void CPlayerWalkAndRun::UpdateInputAxisParam()
			{
				// 前、後移動の軸入力
				m_inputMoveF = m_playerRef->GetInputData().axisMoveForward;
				// 右、左移動の軸入力
				m_inputMoveR = m_playerRef->GetInputData().axisMoveRight;

				return;
			}

			/**
			 * @brief 歩きか走りかを決める
			*/
			void CPlayerWalkAndRun::WalkOrRun()
			{
				// ダッシュ入力がされていない
				if (m_playerRef->GetInputData().actionDush != true)
				{
					// 歩き状態

					// 加速度を設定
					m_acceleration = kWalkAcceleration;
					// 最高速度を設定
					m_maxSpeed = kWalkMaxSpeed;
				}
				else
				{
					// ダッシュ状態
										
					// 加速度を設定
					m_acceleration = kRunAcceleration;
					// 最高速度を設定
					m_maxSpeed = kRunMaxSpeed;
				}

				return;
			}

			/**
			 * @brief 移動方向を更新する
			*/
			void CPlayerWalkAndRun::UpdateMoveDir()
			{
				// 軸入力がないか？
				if (m_playerRef->GetInputData().inputMoveAxis != true)
				{
					// ない。早期リターン。
					return;
				}

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
				Vector3 newMoveDir = moveForward * m_inputMoveF;	// 新しい移動方向
				// 右、左方向への移動方向を加算
				newMoveDir += moveRight * m_inputMoveR;

				// 移動方向と新しい移動方向のなす角
				float radAngle = acosf(Dot(m_moveDir, newMoveDir));

				// 角度と速度がしきい値以上か？
				if (radAngle >= Math::DegToRad(kBreakThresholdAngle) &&
					m_velocity >= kBreakThresholdVelocity)
				{
					// ブレーキをかける
					m_velocity = 0.0f;
				}
				else
				{
					// 移動方向を決定
					m_moveDir = newMoveDir;
				}

				return;
			}


			/**
			 * @brief 加速を計算
			*/
			void CPlayerWalkAndRun::CalcAcceleration()
			{

				// 軸入力があるか？
				if (m_playerRef->GetInputData().inputMoveAxis)
				{
					// ある
					// 加速する
					m_velocity += m_acceleration;
				}

				return;
			}

			/**
			 * @brief 摩擦の計算
			*/
			void CPlayerWalkAndRun::CalcFriction()
			{
				// 摩擦力
				float friction = 0.0f;

				// 空中か？
				if (m_playerMovementRef->IsAir())
				{
					// 空中の摩擦
					friction = kAirFriction;
				}
				else
				{
					// 地面上の摩擦
					friction = kGroundFriction;
				}

				// 摩擦を計算する
				if (m_velocity <= kMinSpeed)
				{
					// 移動速度が最低速度以下なら
					// 移動速度ゼロにする
					m_velocity = 0.0f;
				}
				else if (m_playerRef->GetInputData().inputMoveAxis != true)
				{
					// 入力がなかったら
					// 摩擦て減速する
					m_velocity *= friction;
				}
				else if (m_velocity < m_oldVelocity)
				{
					// 前のフレームより速度が落ちていたら
					// 摩擦て減速する
					m_velocity *= friction;
				}

				return;
			}

			/**
			 * @brief 速度制限の計算
			*/
			void CPlayerWalkAndRun::CalcLimitSpeed()
			{
				// 移動速度が最高速度をオーバーしているか？
				if (m_velocity > m_maxSpeed)
				{
					// オーバーしていたら最高速度を維持
					m_velocity = m_maxSpeed;
				}

				return;
			}

			/**
			 * @brief 実際に移動させる
			*/
			void CPlayerWalkAndRun::Move()
			{
				// 移動ベクトルのX,Z成分を初期化
				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecZ();

				// 移動ベクトルに、加算移動ベクトルを加算する
				m_playerMovementRef->AddMoveVec(m_moveDir * m_velocity);

				return;
			}


			/**
			 * @brief ジャンプの更新
			*/
			void CPlayerWalkAndRun::UpdateJump()
			{
				// ジャンプ
				// ジャンプボタンが押されている、かつ、
				// ジャンプの上昇中ではない、かつ、
				// 地面についている
				if (m_playerRef->GetInputData().actionJump && !m_isJumpUp/* && m_charaCon.IsOnGround()*/)
				{
					m_isJumpUp = true;
					m_jumpUpTimer = 0.0f;
				}

				if (m_isJumpUp)
				{
					// ジャンプの上昇中

					// ジャンプの上昇中のタイマーを進める
					m_jumpUpTimer += m_playerRef->GetDeltaTime();

					if (m_jumpUpTimer <= kJumpChargeTime)
					{
						// タイマーが溜めの時間以下なら、何もしない。早期リターン。
						return;
					}

					if (m_jumpSC->IsPlaying() != true)
					{
						// ジャンプの上昇し始める最初だけ行う処理
						// ジャンプのSEを再生
						m_jumpSC->Play(false);
						// ジャンプ力が0からスタートだと、遅すぎるし、加速の仕方がいびつだから、
						// ある程度、最初に力を加えておく。
						m_playerMovementRef->AddMoveVec({ 0.0f, kInitialJumpForce * m_playerRef->GetDeltaTime(),0.0f });
					}

					// ジャンプ力を加える
					m_playerMovementRef->AddMoveVec({ 0.0f, kJumpForce * m_playerRef->GetDeltaTime(),0.0f });
					
					if (m_jumpUpTimer >= kJumpUpTime)
					{
						// タイマーが上昇中の時間を超えたら、ジャンプの上昇を終了する。
						m_isJumpUp = false;
					}
				}

				return;
			}


			/**
			 * @brief サウンドの更新
			*/
			void CPlayerWalkAndRun::UpdateSound()
			{
				// ジャンプ
				// ジャンプボタンが押されている、かつ、地面についている
				if (m_playerRef->GetInputData().actionJump/* && m_charaCon.IsOnGround()*/)
				{
					//m_jumpSC->Play(false);
				}

				if (m_velocity <= 0.001f || m_playerMovementRef->IsAir())
				{
					// 移動していない、または空中。サウンドを停止して、早期リターン。
					StopSound();
					return;
				}

				if (m_playerRef->GetInputData().actionDush != true)
				{
					// 歩き
					m_walkSC->Play(true);
					m_runSC->Stop();
				}
				else
				{
					// 走り
					m_runSC->Play(true);
					m_walkSC->Stop();
				}

				return;
			}

		}
	}
}
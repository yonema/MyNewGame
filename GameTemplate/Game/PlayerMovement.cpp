#include "stdafx.h"
#include "PlayerMovement.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "PlayerModelAnimation.h"
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
			// プレイヤー移動クラスの定数データを使用可能にする
			using namespace nsPlayerConstData::nsMovementConstData;

			/**
			 * @brief コンストラクタ
			*/
			CPlayerMovement::CPlayerMovement()
			{
				m_landingSC = NewGO<nsSound::CSoundCue>(nsCommonData::enPriorityFirst);
				m_landingSC->Init(kLandingSoundFilePath, nsSound::CSoundCue::enSE);

				return;
			}
			/**
			 * @brief デストラクタ
			*/
			CPlayerMovement::~CPlayerMovement()
			{
				DeleteGO(m_landingSC);

				return;
			}

			/**
			 * @brief 初期化
			 * @param[in] radius カプセルコライダーの半径
			 * @param[in] height カプセルコライダーの高さ
			 * @param[in,out] player プレイヤーの参照
			 * @param[in,out] playerCamera プレイヤーカメラの参照
			 * @param[in.out] playerModelAnimation プレイヤーモデルアニメーションの参照
			*/
			void CPlayerMovement::Init(
				const float radius,
				const float height,
				CPlayer* player,
				CPlayerCamera* playerCamera,
				CPlayerModelAnimation* playerModelAnimation
			)
			{
				// プレイヤーの参照をセットする
				m_playerRef = player;

				// キャラクターコントローラ初期化
				m_charaCon.Init(radius, height, m_playerRef->GetPosition());

				// プレイヤーの歩きと走りクラスの初期化
				m_playerWalkAndRun.Init(*m_playerRef, this);

				// プレイヤーのスイングアクションクラスの初期化
				m_playerSwingAction.Init(m_playerRef, this, playerCamera, playerModelAnimation);

				// プレイヤーの壁を走る処理クラスの初期化
				m_playerWallRun.Init(m_playerRef, this);

				// プレイヤーの敵の上に乗るクラスの初期化
				m_playerOnEnemy.Init(m_playerRef, this);

				return;
			}

			/**
			 * @brief プレイヤーの移動クラスのアップデートを実行する
			*/
			void CPlayerMovement::ExecuteUpdate()
			{
				nsDebug::DrawTextPanel(L"CPlayerMovement::ExecuteUpdate()");
				nsDebug::DrawTextPanel(std::to_wstring(GetXZSpeed()), L"XZSpeed");

				// ステートを更新
				UpdateState();

				// プレイヤーの移動を更新
				UpdateMovePlayer();

				// プレイヤーの回転を更新
				UpdateTurnPlayer();

				// サウンドの更新
				UpdateSound();

				nsDebug::DrawVector(m_playerRef->GetPosition(), m_moveVec, "playerVec");

				return;
			}

			/**
			 * @brief 加算移動ベクトルに制限をかける
			 * @param[in] limit 制限
			*/
			void CPlayerMovement::LimitMoveVec(const float limit)
			{
				if (m_moveVec.Length() <= limit)
				{
					// 加算移動ベクトルが制限より小さかったら、制限を掛けない。早期リターン。
					return;
				}

				// 制限いっぱいの大きさのベクトルに直す
				m_moveVec.Normalize();
				m_moveVec.Scale(limit);

				return;
			}

			/**
			 * @brief 直接座標を設定
			 * @param[in] pos 座標
			*/
			void CPlayerMovement::SetDirectPosition(const Vector3& pos)
			{
				m_charaCon.SetPosition(pos);
				m_playerRef->SetPosition(pos);
			}

			/**
			 * @brief 重力をかける
			*/
			void CPlayerMovement::ApplyGravity()
			{
				if (m_useGravity != true)
				{
					// 重力をかけない。早期リターン
					return;
				}

				// 重力をかける
				m_moveVec.y -= nsPlayerConstData::nsMovementConstData::kGravityScale *
					m_playerRef->GetDeltaTime();

				// 重力の制限
				//if (m_moveVec.y < -nsPlayerConstData::nsMovementConstData::kMaxFallSpeed)
				//{
				//	m_moveVec.y = -nsPlayerConstData::nsMovementConstData::kMaxFallSpeed;
				//}

				return;
			}

			/**
			 * @brief キャラクターコントローラーを使った移動
			*/
			void CPlayerMovement::MoveWithCharacterController()
			{
				// キャラクターコントローラー実行
				m_playerRef->SetPosition(
					m_charaCon.Execute(m_moveVec, m_playerRef->GetDeltaTime())
				);
				//m_playerRef->SetPosition(m_playerRef->GetPosition() + m_moveVec / 100.0f);

				return;
			}

			/**
			 * @brief ステートを更新する
			*/
			void CPlayerMovement::UpdateState()
			{

				//if (IsOnWall())
				//{
				//	m_playerRef->ChangeWallRunState();
				//	return;
				//}

				// スイングトリガーがtrueで、かつ、空中だったら
				if (m_playerRef->GetState() == nsPlayerConstData::enWalkAndRun &&
					m_playerRef->GetInputData().actionSwing && 
					IsAir() &&
					m_moveVec.y <= nsPlayerConstData::nsSwingActionConstData::kCanStartSwingFallSpeed
					)
				{
					// スイング状態へ遷移
					m_playerRef->ChangeSwingState();
				}

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
				case nsPlayerConstData::enStartFall:
					// 開始演出
					if (IsAir() != true && m_playerRef->GetPosition().y <= kStartFallHeight)
					{
						// 一定以下の高さに居て、空中にいない時、動きを止める。
						// 開始した瞬間は空中に居ても、IsAirがfalseなため、高さ制限を付ける。
						// 開始演出時は、抵抗がないため、手動で動きを止める。
						ResetMoveVecX();
						ResetMoveVecY();
						ResetMoveVecZ();
					}
					break;
				// 歩きと走り
				case nsPlayerConstData::enWalkAndRun:
					// 歩きと走りを実行
					m_playerWalkAndRun.Execute();
					// Propsと衝突する
					m_charaCon.SetIsIgnoreProps(false);
					break;
				// スイング
				case nsPlayerConstData::enSwing:
					// スイングアクションを実行
					m_playerSwingAction.Execute();
					// Propsと衝突しない
					m_charaCon.SetIsIgnoreProps(true);
					break;
				// 壁走り
				case nsPlayerConstData::enWallRun:
					// 壁走りを実行
					m_playerWallRun.Execute();
					break;
				case nsPlayerConstData::enOnEnemy:
					// 敵の上に乗る処理を実行
					m_playerOnEnemy.Execute();
					break;
				case nsPlayerConstData::enLastJump:
					m_playerWalkAndRun.Execute();
					if (m_moveVec.y < -0.1f)
					{
						// 上昇はするけど、下降はしないようにする。
						ResetMoveVecY();
					}
					break;
				}

				// 重力をかける
				ApplyGravity();

				// キャラクターコントローラーを使用して、移動させる。
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


			/**
			 * @brief サウンドの更新
			*/
			void CPlayerMovement::UpdateSound()
			{
				if (IsAir())
				{
					// 空中にいるとき、着地音フラグがオンになる。
					m_landingSoundFlag = true;
				}
				else if (m_landingSoundFlag)
				{
					// 空中にいないとき、かつ、着地音フラグがオンの時。
					// 着地音フラグをオフにする
					m_landingSoundFlag = false;
					// 着地音をワンショット再生する
					m_landingSC->Play(false);
				}

				return;
			}

		}
	}
}
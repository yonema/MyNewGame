#include "stdafx.h"
#include "PlayerOnEnemy.h"
#include "Player.h"
#include "AICar.h"
#include "GameTime.h"

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

			using namespace nsPlayerConstData::nsOnEnemyConstData;

			/**
			 * @brief 初期化
			 * @param[in.out] player プレイヤー
			 * @param[in,out] playerMovement プレイヤー移動クラスの参照
			*/
			void CPlayerOnEnemy::Init(
				CPlayer* player,
				CPlayerMovement* playerMovement
			)
			{
				m_playerRef = player;
				m_playerMovementRef = playerMovement;

				return;
			}

			/**
			 * @brief プレイヤーの敵の上に乗る処理を実行
			*/
			void CPlayerOnEnemy::Execute()
			{
				// ステートによって処理を振り分ける
				switch (m_onEnemyState)
				{
				case enStartStretchToEnemy:
					StartStretchToEnemy();
					break;
				case enStretchingToEnemy:
					StretchingToEnemy();
					break;
				case enGoOnEnemy:
					GoOnEnemy();
					break;
				case enOnEnemy:
					OnEnemy();
					break;
				}

				return;
			}

			/**
			 * @brief 初期状態に戻す
			*/
			void CPlayerOnEnemy::Reset()
			{
				m_onEnemyState = enStartStretchToEnemy;
				m_enemyRef = nullptr;
				m_playerRef->SetIsOnEnemyCamera(false);
				m_playerMovementRef->SetUseGravity(true);

				return;
			}


			/**
			 * @brief 敵に向かって糸を伸ばし始める処理
			*/
			void CPlayerOnEnemy::StartStretchToEnemy()
			{
				// ターゲットの敵の参照を取ってくる
				m_enemyRef = m_playerRef->GetCatchEnemy().GetTargetEnemy();

				if (m_enemyRef)
				{
					// 敵の方を向かせる
					LookAtEnemy();

					// 敵がいれば、ステートを遷移する。
					ChangeState(enStretchingToEnemy);
				}

				return;
			}


			/**
			 * @brief エネミーに向かって糸を伸ばす処理
			*/
			void CPlayerOnEnemy::StretchingToEnemy()
			{
				// ターゲット座標は、敵の座標
				Vector3 targetPos = m_enemyRef->GetPosition();
				// 高さをちょっと上げる
				targetPos.y += kOnEnemyHeight;
				
				// 敵に向かって糸を伸ばす
				m_playerRef->SetStringStretchPos(targetPos);

				if (m_playerRef->IsStringStretched())
				{
					// 伸ばし切ったら、ステート遷移。
					ChangeState(enGoOnEnemy);
				}

				return;
			}

			/**
			 * @brief 敵の上に行く処理
			*/
			void CPlayerOnEnemy::GoOnEnemy()
			{
				// ターゲット座標は、敵の座標
				Vector3 targetPos = m_enemyRef->GetPosition();
				// 高さをちょっと上げる
				targetPos.y += kOnEnemyHeight;

				// 糸の伸ばす先を更新する。
				m_playerRef->SetStringStretchPos(targetPos);

				// 移動ベクトルをリセットする
				m_playerMovementRef->ResetMoveVecX();
				m_playerMovementRef->ResetMoveVecY();
				m_playerMovementRef->ResetMoveVecZ();

				// 移動先の座標
				Vector3 pos = Vector3::Zero;
				// 最初の座標から、ターゲット座標まで、線形補完で徐々に近づく。
				pos.Lerp(m_goOnEnemyRate, m_startGoOnEnemyPos, targetPos);
				// プレイヤーの座標を直接設定する
				m_playerMovementRef->SetDirectPosition(pos);

				// 補間率を進める
				m_goOnEnemyRate += nsTimer::GameTime().GetFrameDeltaTime();

				if (m_goOnEnemyRate >= 1.0f)
				{
					// 敵の上まで着いたら、ステートを遷移する。
					ChangeState(enOnEnemy);
				}

				return;
			}

			/**
			 * @brief 敵の上にいる処理
			*/
			void CPlayerOnEnemy::OnEnemy()
			{
				// ターゲットの敵の参照を取ってくる
				m_enemyRef = m_playerRef->GetCatchEnemy().GetTargetEnemy();

				if (m_enemyRef == nullptr)
				{
					// 敵がいなかったら早期リターン。
					return;
				}

				// ターゲット座標は、敵の座標
				Vector3 targetPos = m_enemyRef->GetPosition();
				// 高さをちょっと上げる
				targetPos.y += kOnEnemyHeight;

				// 乗っている敵と同じ回転にする
				Quaternion qRot = m_enemyRef->GetRotation();

				m_playerMovementRef->SetDirectPosition(targetPos);
				m_playerRef->SetRotation(qRot);


				return;
			}

			/**
			 * @brief 敵の方を向かせる
			*/
			void CPlayerOnEnemy::LookAtEnemy()
			{
				// プレイヤーから敵への方向ベクトル
				Vector3 playerToEnemyNorm = m_enemyRef->GetPosition() - m_playerRef->GetPosition();
				playerToEnemyNorm.Normalize();	// 正規化する
				// 回転
				Quaternion qRot;
				qRot.SetRotation(Vector3::Front, playerToEnemyNorm);
				// プレイヤーに回転を設定する
				m_playerRef->SetRotation(qRot);

				return;
			}

			/**
			 * @brief ステートを変更する
			 * @param[in] newState 新しいステート
			*/
			void CPlayerOnEnemy::ChangeState(const nsPlayerConstData::nsOnEnemyConstData::EnOnEnemyState newState)
			{
				if (m_onEnemyState == newState)
				{
					// 現在のステートと同じなら、何しない。早期リターン。
					return;
				}

				// ステートを変更
				m_onEnemyState = newState;

				// 変更した瞬間に、一度だけ呼ばれる処理。
				switch (m_onEnemyState)
				{
				case enStartStretchToEnemy:
					break;

				case enStretchingToEnemy:
					// 糸を伸ばし始めて、重力を切って、敵の上に乗っているカメラにする。
					m_playerRef->SetIsOnEnemyCamera(true);
					m_playerRef->StartStringStretchToPos(m_enemyRef->GetPosition());
					m_playerMovementRef->SetUseGravity(false);
					break;

				case enGoOnEnemy:
					m_startGoOnEnemyPos = m_playerRef->GetPosition();
					break;

				case enOnEnemy:
					// 補完率をリセットして、糸の伸ばしを終える。
					m_goOnEnemyRate = 0.0f;
					m_playerRef->EndStringStretchToPos();
					break;
				}


				return;
			}


		}
	}
}
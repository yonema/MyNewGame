#include "stdafx.h"
#include "PlayerMovement.h"
#include "Player.h"
#include "PlayerConstData.h"
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
			// プレイヤー移動クラスの定数データを使用可能にする
			using namespace nsPlayerConstData::nsPlayerMoveConstData;

			/**
			 * @brief 初期化
			 * @param[in] radius カプセルコライダーの半径
			 * @param[in] height カプセルコライダーの高さ
			 * @param[in,out] player プレイヤーの参照
			*/
			void CPlayerMovement::Init(
				const float radius,
				const float height,
				CPlayer* player
			)
			{
				// プレイヤーの参照をセットする
				m_playerRef = player;

				// キャラクターコントローラ初期化
				m_charaCon.Init(radius, height, m_playerRef->GetPosition());

				// プレイヤーの歩きと走りのクラスの初期化
				m_playerWalkAndRun.Init(*m_playerRef, this);

				// プレイヤーのスイングアクションクラスの初期化
				m_playerSwingAction.Init(m_playerRef, this);


#ifdef MY_DEBUG
				m_moveVecFont = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFirst);
				m_moveVecFont->SetPosition({ -200.0f,200.0f });
#endif

				return;
			}

			/**
			 * @brief プレイヤーの移動クラスのアップデートを実行する
			*/
			void CPlayerMovement::ExecuteUpdate()
			{
				// スイングアクションがtrueで、かつ、空中だったら
				if (m_playerRef->GetInputData().actionSwing && IsAir())
				{
					// スイング状態
					m_playerRef->SetState(nsPlayerConstData::enSwing);
				}
				else
				{
					// 歩きと走り状態
					m_playerRef->SetState(nsPlayerConstData::enWalkAndRun);
				}

				// プレイヤーの移動を更新
				UpdateMovePlayer();

				// プレイヤーの回転を更新
				UpdateTurnPlayer();

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
				case nsPlayerConstData::enWalkAndRun:	// 歩きと走り
					// 歩きと走りを実行
					m_playerWalkAndRun.Execute();

					// 移動ベクトルのX,Z成分を初期化
					m_moveVec.x = 0.0f;
					m_moveVec.z = 0.0f;


					// 移動ベクトルに、加算移動ベクトルを加算する
					m_moveVec += m_addMoveVec;

					// 重力
					m_moveVec.y -= kGravityScale * nsTimer::GameTime().GetFrameDeltaTime();

					// ジャンプ
					// ジャンプボタンが押されている、かつ、地面についている
					if (m_playerRef->GetInputData().actionJump/* && m_charaCon.IsOnGround()*/)
					{
						m_moveVec.y += kJumpForce;
					}

					// キャラクターコントローラー実行
					m_playerRef->SetPosition(m_charaCon.Execute(m_moveVec, nsTimer::GameTime().GetFrameDeltaTime()));

					// 地面についていたら、重力リセット
					if (m_charaCon.IsOnGround())
					{
						m_moveVec.y = 0.0f;
					}
					break;
				case nsPlayerConstData::enSwing:	// スイング

					// スイングアクションを実行
					m_playerSwingAction.Execute();

					m_playerRef->SetPosition(m_addMoveVec);
					m_charaCon.SetPosition(m_addMoveVec);

					m_moveVec = Vector3::Zero;
					m_addMoveVec = Vector3::Zero;

					break;
				}

#ifdef MY_DEBUG
				wchar_t text[256];
				//swprintf_s(text, L"x:%2.2f,y:%2.2f,z:%2.2f\nlen:%2.2f",
				//	m_moveVec.x, m_moveVec.y, m_moveVec.z,m_moveVec.Length());
				//swprintf_s(text, L"x:%2.2f,y:%2.2f,z:%2.2f",m_moveVec.x, m_moveVec.y, m_moveVec.z);
				swprintf_s(text, L"forward:%2.2f,right:%2.2f\nlen:%2.2f",
					m_playerRef->GetInputData().axisMoveForward,
					m_playerRef->GetInputData().axisMoveRight,
					m_moveVec.Length());
				m_moveVecFont->SetText(text);
#endif

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
		}
	}
}
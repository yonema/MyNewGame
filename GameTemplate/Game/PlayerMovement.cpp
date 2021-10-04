#include "stdafx.h"
#include "PlayerMovement.h"
#include "PlayerConstData.h"
#include "PlayerCamera.h"
#include "PlayerInput.h"
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
			using namespace nsPlayerMoveConstData;

			/**
			 * @brief 初期化
			 * @param[in] radius カプセルコライダーの半径
			 * @param[in] height カプセルコライダーの高さ
			 * @param[in,out] position プレイヤーの座標の参照
			 * @param[in,out] rotation プレイヤーの回転の参照
			 * @param[in] playerCamera プレイヤーカメラ
			 * @param[in] playerInputData プレイヤー入力情報
			*/
			void CPlayerMovement::Init(
				const float radius,
				const float height,
				Vector3* position,
				Quaternion* rotation,
				const CPlayerCamera& playerCamera,
				const SPlayerInputData& playerInputData
			)
			{
				// プレイヤーの座標の参照をセットする
				m_playerPosition = position;
				// プレイヤーの回転の参照をセットする
				m_playerRotation = rotation;
				// プレイヤーカメラをセットする
				m_playerCamera = &playerCamera;
				// プレイヤーの入力情報をセットする
				m_playerInputData = &playerInputData;

				// キャラクターコントローラ初期化
				m_charaCon.Init(radius, height, *m_playerPosition);

				// プレイヤーの歩きと走りのクラスの初期化
				m_playerWalkAndRun.Init(&m_moveVecForward, &m_moveVecRight, playerCamera, playerInputData);


#ifdef MY_DEBUG
				m_moveVecFont = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::enPriorityFirst);
				m_moveVecFont->SetPosition({ -100.0f,100.0f });
#endif

				return;
			}

			/**
			 * @brief プレイヤーの移動クラスのアップデートを実行する
			*/
			void CPlayerMovement::ExecuteUpdate()
			{
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
				// 歩きと走りを実行
				m_playerWalkAndRun.Execute(m_charaCon.IsJump());

				// 移動ベクトルのX,Z成分を初期化
				m_moveVec.x = 0.0f;
				m_moveVec.z = 0.0f;
				// 移動ベクトルに、前向きと移動ベクトルと右向きの移動ベクトルを加算する
				m_moveVec += m_moveVecForward + m_moveVecRight;

				// 重力
				m_moveVec.y -= kGravityScale * nsTimer::GameTime().GetFrameDeltaTime();

				// ジャンプ
				// ジャンプボタンが押されている、かつ、地面についている
				if (m_playerInputData->actionJump && m_charaCon.IsOnGround())
				{
					m_moveVec.y += kJumpForce;
				}

				// キャラクターコントローラー実行
				*m_playerPosition = m_charaCon.Execute(m_moveVec, nsTimer::GameTime().GetFrameDeltaTime());

				// 地面についていたら、重力リセット
				if (m_charaCon.IsOnGround())
				{
					m_moveVec.y = 0.0f;
				}

#ifdef MY_DEBUG
				wchar_t text[256];
				swprintf_s(text, L"x:%2.2f,y:%2.2f,z:%2.2f", m_moveVec.x, m_moveVec.y, m_moveVec.z);
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
				if (fabs(m_moveVec.x) < 0.001f && fabs(m_moveVec.y) < 0.001f)
				{
					// 移動していない
					// 早期リターン
					return;
				}

				// tanθ = m_moveVec.x / m_moveVec.y
				const float radAngle = atan2(m_moveVec.x, m_moveVec.z);
				// ラジアン単位で回す
				m_playerRotation->SetRotation(Vector3::AxisY, radAngle);

				return;
			}
		}
	}
}
#include "stdafx.h"
#include "PlayerModelAnimation.h"
#include "Player.h"
#include "ModelRender.h"

namespace nsMyGame
{

	/**
	 * @brief プレイヤー関連のネームスペース
	*/
	namespace nsPlayer
	{
		// 共通データを使用可能にする
		using namespace nsCommonData;
		// プレイヤーの定数データを使用可能にする
		using namespace nsPlayerConstData::nsModelAnimationConstData;

		/**
		* @brief デストラクタ
		*/
		CPlayerModelAnimation::~CPlayerModelAnimation()
		{
			// モデル解放
			ReleaseModel();

			return;
		}

		/**
		 * @brief モデル初期化
		 * @param[in] player プレイヤーの参照
		*/
		void CPlayerModelAnimation::Init(const CPlayer& player)
		{
			// プレイヤーの参照をセット
			m_playerRef = &player;

			// アニメーション初期化
			InitAnimation();
			// モデル初期化
			InitModel();

			return;
		}

		/**
		 * @brief プレイヤーモデルクラスのアップデートを実行する
		*/
		void CPlayerModelAnimation::ExecuteUpdate()
		{
			// トランスフォームを更新
			UpdateTransform();
			// アニメーション遷移を更新
			UpdateAnimationTransition();

			return;
		}



		/**
		 * @brief アニメーション初期化
		*/
		void CPlayerModelAnimation::InitAnimation()
		{
			// アニメーションの初期化

			for (int i = 0; i < enAnim_num; i++)
			{
				// アニメーションのロード
				m_animationClips[i].Load(kAnimationFilePath[i]);
				//m_animationClips[i].SetLoopFlag(true);
				// ループフラグを設定
				switch (i)
				{
				// ループするアニメーション
				case enAnim_idle:
				case enAnim_walk:
				case enAnim_run:
				case enAnim_airIdle:
				case enAnim_swinging:
					m_animationClips[i].SetLoopFlag(true);
					break;
				// ループしないアニメーション
				default:
					m_animationClips[i].SetLoopFlag(false);
					break;
				}
			}

			return;
		}

		/**
		 * @brief モデル初期化
		*/
		void CPlayerModelAnimation::InitModel()
		{

			// プレイヤーモデルレンダラーの生成
			m_playerModel = NewGO<nsGraphic::nsModel::CModelRender>(enPriorityFirst);

			// 座標と回転を設定
			m_playerModel->SetPosition(m_playerRef->GetPosition());
			m_playerModel->SetRotation(m_playerRef->GetRotation());

			// プレイヤーモデルレンダラーの初期化
			m_playerModel->Init(kPlayerModelFilePath, m_animationClips, enAnim_num);
			//m_playerModel->IniTranslucent(
			//	kPlayerModelFilePath,
			//	m_animationClips,
			//	enAnim_num
			//	);
			// シャドウキャスターを有効にする
			m_playerModel->SetIsShadowCaster(true);

			return;
		}

		/**
		* @brief モデル解放
		*/
		void CPlayerModelAnimation::ReleaseModel()
		{
			// 初期化していないか？
			if (!m_playerModel)
			{
				// 初期化していない
				// 早期リターン
				return;
			}

			// プレイヤーモデルレンダラーの破棄
			DeleteGO(m_playerModel);

			return;
		}

		/**
		 * @brief トランスフォームを更新
		*/
		void CPlayerModelAnimation::UpdateTransform()
		{
			// 座標と回転を更新
			m_playerModel->SetPosition(m_playerRef->GetPosition());
			m_playerModel->SetRotation(m_playerRef->GetRotation());
			return;
		}


		/**
		 * @brief アニメーション遷移を更新
		*/
		void CPlayerModelAnimation::UpdateAnimationTransition()
		{
			// アニメーションステートを更新
			UpdateAnimationState();

			// アニメーション再生
			// 前フレームと同じアニメーションをPlayした場合、なにもしない
			m_playerModel->PlayAnimation(m_animState, m_AnimInterpolateTime);

			return;
		}

		/**
		 * @brief アニメーションステートを更新
		*/
		void CPlayerModelAnimation::UpdateAnimationState()
		{
			m_AnimInterpolateTime = 0.2f;
			if (m_playerRef->GetState() == nsPlayerConstData::enSwing)
			{
				// スイング中のアニメーションステートを更新
				UpdateSwingAnimationState();
				return;
			}

			// プレイヤーの速度

			if (m_playerRef->GetPlayerMovement().IsAir())
			{
				if (m_animState != enAnim_airIdle)
				{
					m_playerModel->SetAnimationSpeed(2.0f);
					if (m_playerModel->IsAnimationPlaying() == true)
					{
						m_AnimInterpolateTime = 0.0f;
						m_animState = enAnim_jumpUp;
					}
					else
					{
						m_animState = enAnim_airIdle;
					}
				}

				return;
			}
			const float playerVelocity = m_playerRef->GetPlayerMovement().GetXZSpeed();

			if (playerVelocity <= 10.0f)
			{
				m_animState = enAnim_idle;
				m_playerModel->SetAnimationSpeed(2.0f);
			}
			else if (playerVelocity <= nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed + 50.0f)
			{
				m_animState = enAnim_walk;
				m_playerModel->SetAnimationSpeed(
					1.0f + 1.0f * playerVelocity / nsPlayerConstData::nsWalkAndRunConstData::kWalkMaxSpeed
				);
			}
			else
			{
				m_animState = enAnim_run;
				m_playerModel->SetAnimationSpeed(
					2.0f * playerVelocity / nsPlayerConstData::nsWalkAndRunConstData::kRunMaxSpeed
				);
			}

			return;
		}

		/**
		 * @brief スイング中のアニメーションステートを更新
		*/
		void CPlayerModelAnimation::UpdateSwingAnimationState()
		{
			switch (m_swingAnimSate)
			{
			case enSwingAnim_swingStart:
				if (m_playerModel->IsAnimationPlaying() != true && m_animState == enAnim_swingStart)
				{
					m_swingAnimSate = enSwingAnim_swing;
				}
				m_animState = enAnim_swingStart;
				m_playerModel->SetAnimationSpeed(1.0f);

				break;
			case enSwingAnim_swing:
				m_playerModel->SetAnimationSpeed(1.0f);
				m_animState = enAnim_swinging;
				break;
			case enSwingAnim_swingRaiseLeg:
				m_playerModel->SetAnimationSpeed(1.5f);
				m_animState = enAnim_swingRaiseLeg;
				break;
			case enSwingAnim_swingRoll:
				if (m_playerModel->IsAnimationPlaying() != true && m_animState == enAnim_swingRoll)
				{
					m_swingAnimSate = enSwingAnim_airAfterSwing;
				}
				m_animState = enAnim_swingRoll;
				m_AnimInterpolateTime = 0.0f;
				m_playerModel->SetAnimationSpeed(2.0f);
				break;
			case enSwingAnim_airAfterSwing:
				m_animState = enAnim_airIdle;
				m_playerModel->SetAnimationSpeed(1.0f);
				break;
			}

			return;
		}

	}
}
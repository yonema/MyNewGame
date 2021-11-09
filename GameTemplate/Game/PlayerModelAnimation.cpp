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
		using namespace nsPlayerConstData::nsPlayerModelRenderConstData;

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
			//m_playerModel->Init(kPlayerModelFilePath, m_animationClips, enAnim_num, enModelUpAxisY);
			m_playerModel->IniTranslucent(
				kPlayerModelFilePath,
				m_animationClips,
				enAnim_num
				);
			// シャドウキャスターを有効にする
			m_playerModel->SetIsShadowCaster(true);
			m_playerModel->SetAnimationSpeed(2.0f);

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
			m_animState = enAnim_idle;

			//switch (m_playerRef->GetState())
			{
				//case nsPlayerConstData::enWalkAndRun:

					if (m_playerRef->GetPlayerMovement().GetVelocity() <= 10.0f)
						m_animState = enAnim_idle;
					else if (m_playerRef->GetPlayerMovement().GetVelocity() <= 800.0f)
						m_animState = enAnim_walk;
					else
						m_animState = enAnim_run;

					//if (m_playerRef->GetPlayerMovement().IsAir())
					//	m_animState = enAnim_jump;


				//	break;
				//case nsPlayerConstData::enSwing:
					//m_animState = enAnim_swingStart;

				//	break;
			}


			// アニメーション再生
			// 前フレームと同じアニメーションをPlayした場合、なにもしない
			m_playerModel->PlayAnimation(m_animState, m_AnimInterpolateTime);

			return;
		}


	}
}
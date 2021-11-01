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
			m_playerNoTransparentModel = NewGO<nsGraphic::nsModel::CModelRender>(enPriorityFirst);

			// プレイヤーモデルレンダラーの生成
			m_playerModel = NewGO<nsGraphic::nsModel::CModelRender>(enPriorityFirst);

			// 座標と回転を設定
			m_playerModel->SetPosition(m_playerRef->GetPosition());
			m_playerModel->SetRotation(m_playerRef->GetRotation());
			m_playerNoTransparentModel->SetPosition(m_playerRef->GetPosition());
			m_playerNoTransparentModel->SetRotation(m_playerRef->GetRotation());

			m_playerNoTransparentModel->IniTranslucent(
				"Assets/modelData/sayaka_kunoichi_noTransparent/sayaka_kunoichi_noTransparent.tkm",
				m_animationClips,
				enAnim_num
				);

			// プレイヤーモデルレンダラーの初期化
			//m_playerModel->Init(kPlayerModelFilePath, m_animationClips, enAnim_num, enModelUpAxisY);
			m_playerModel->IniTranslucent(
				"Assets/modelData/sayaka_kunoichi/sayaka_kunoichi.tkm",
				m_animationClips,
				enAnim_num
				);
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
			DeleteGO(m_playerNoTransparentModel);
			DeleteGO(m_playerModel);

			return;
		}

		/**
		 * @brief トランスフォームを更新
		*/
		void CPlayerModelAnimation::UpdateTransform()
		{
			// 座標と回転を更新
			m_playerNoTransparentModel->SetPosition(m_playerRef->GetPosition());
			m_playerNoTransparentModel->SetRotation(m_playerRef->GetRotation());
			m_playerModel->SetPosition(m_playerRef->GetPosition());
			m_playerModel->SetRotation(m_playerRef->GetRotation());
			return;
		}


		/**
		 * @brief アニメーション遷移を更新
		*/
		void CPlayerModelAnimation::UpdateAnimationTransition()
		{
			// アニメーション再生
			// 前フレームと同じアニメーションをPlayした場合、なにもしない
			m_playerNoTransparentModel->PlayAnimation(m_animState, m_AnimInterpolateTime);
			m_playerModel->PlayAnimation(m_animState, m_AnimInterpolateTime);

			return;
		}


	}
}
#include "stdafx.h"
#include "PlayerModel.h"
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
		CPlayerModel::~CPlayerModel()
		{
			// モデル解放
			ReleaseModel();

			return;
		}

		/**
		 * @brief モデル初期化
		 * @param[in] player プレイヤーの参照
		*/
		void CPlayerModel::Init(const CPlayer& player)
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
		void CPlayerModel::ExecuteUpdate()
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
		void CPlayerModel::InitAnimation()
		{
			// アニメーションの初期化

			m_animationClips[enAnim_idle].Load(kAnimationFilePath_idle);
			m_animationClips[enAnim_idle].SetLoopFlag(true);
			m_animationClips[enAnim_walk].Load(kAnimationFilePath_walk);
			m_animationClips[enAnim_walk].SetLoopFlag(true);


			return;
		}

		/**
		 * @brief モデル初期化
		*/
		void CPlayerModel::InitModel()
		{
			// プレイヤーモデルレンダラーの生成
			m_playerModel = NewGO<nsGraphic::nsModel::CModelRender>(enPriorityFirst);

			// 座標と回転を設定
			m_playerModel->SetPosition(m_playerRef->GetPosition());
			m_playerModel->SetRotation(m_playerRef->GetRotation());

			// プレイヤーモデルレンダラーの初期化
			m_playerModel->Init(kPlayerModelFilePath, m_animationClips, enAnim_num, enModelUpAxisY);

			// シャドウキャスターを有効にする
			m_playerModel->SetIsShadowCaster(true);

			return;
		}

		/**
		* @brief モデル解放
		*/
		void CPlayerModel::ReleaseModel()
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
		void CPlayerModel::UpdateTransform()
		{
			// 座標と回転を更新
			m_playerModel->SetPosition(m_playerRef->GetPosition());
			m_playerModel->SetRotation(m_playerRef->GetRotation());

			return;
		}


		/**
		 * @brief アニメーション遷移を更新
		*/
		void CPlayerModel::UpdateAnimationTransition()
		{
			// アニメーション再生
			// 前フレームと同じアニメーションをPlayした場合、なにもしない
			m_playerModel->PlayAnimation(m_animState, m_AnimInterpolateTime);

			return;
		}


	}
}
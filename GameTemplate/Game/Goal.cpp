#include "stdafx.h"
#include "Goal.h"
#include "GoalConstData.h"
#include "ModelRender.h"
#include "GameMainState.h"
#include "Player.h"

namespace nsMyGame
{
	/**
	 * @brief ゴール関連のネームスペース
	*/
	namespace nsGoal
	{
		// ゴールクラスの定数データを使用可能にする
		using namespace nsGoalConstData;

		/**
		 * @brief Updateの直前で呼ばれる開始処理
		 * @return アップデートを行うか？
		*/
		bool CGoal::Start()
		{
			return true;
		}

		/**
		 * @brief 更新処理
		*/
		void CGoal::OnDestroy()
		{
			DeleteGO(m_goalMR);

			return;
		}

		/**
		 * @brief 消去される時に呼ばれる処理
		*/
		void CGoal::Update()
		{
			// プレイヤーへのベクトル
			Vector3 toPlayerVec = m_player->GetPosition() - m_goalMR->GetPosition();

			// プレイヤーとの距離がゴールのしきい値以下か？
			if (toPlayerVec.Length() <= kGoalThreshold)
			{
				// ゴールのしきい値以下
				// ゴール
				nsGameState::GameMainState()->Goal();
			}

			return;
		}


		/**
		 * @brief 初期化
		 * @param[in] pos 座標
		 * @param[in] rot 回転
		 * @param[in] scale 拡大率
		 * @param[in] player プレイヤー
		*/
		void CGoal::Init(
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3 scale,
			nsPlayer::CPlayer& player
		)
		{
			// ゴールのモデルレンダラーを生成
			m_goalMR = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::enPriorityFirst);
			// トランスフォームを設定
			m_goalMR->SetPosition(pos);
			m_goalMR->SetRotation(rot);
			m_goalMR->SetScale(scale);
			// 初期化
			m_goalMR->Init(kGoalModelFilePath);

			// プレイヤーをセット
			m_player = &player;

			return;
		}

	}
}
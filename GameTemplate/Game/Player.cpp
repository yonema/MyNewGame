#include "stdafx.h"
#include "Player.h"

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
		using namespace nsPlayerConstData;

		/**
		 * @brief スタート関数
		 * @return アップデートを行うか？
		*/
		bool CPlayer::Start()
		{
			// プレイヤーモデルアニメーションクラスの初期化
			m_playerModelAnimation.Init(*this);

			// プレイヤーの入力情報クラスの初期化
			m_playerInput.Init(this);

			// プレイヤーカメラクラスの初期化
			m_playerCamera.Init(*this);

			// プレイヤー移動クラスの初期化
			m_playerMove.Init(
				kCapsuleRadius,
				kDapsuleHeight,
				this,
				&m_playerCamera,
				&m_playerModelAnimation
				);

			// プレイヤーの糸のモデルクラスの生成と初期化
			m_playerStringModel = NewGO<CPlayerStringModel>(nsCommonData::enPriorityFirst);
			m_playerStringModel->Init(*this);
			
			// プレイヤーが敵を捕まえる処理クラスの初期化
			m_playerCatchEnemy.Init(*this);

			return true;
		}

		/**
		 * @brief 破棄した時に呼ばれる関数
		*/
		void CPlayer::OnDestroy()
		{
			// プレイヤーの糸のモデルクラスの破棄
			DeleteGO(m_playerStringModel);

			return;
		}

		/**
		 * @brief アップデート関数
		*/
		void CPlayer::Update()
		{
			nsDebug::DrawTextPanel(L"[CPlayer::Update()]");

			switch (m_playerState)
			{
			case enWalkAndRun:
				nsDebug::DrawTextPanel(L"PlayerState:WalkAndRun");
				break;

			case enSwing:
				nsDebug::DrawTextPanel(L"PlayerState:Swing");

				break;
			}

			nsDebug::DrawTextPanel(m_position, L"pos");

			// 入力処理を実行
			m_playerInput.ExecuteUpdate();

			// プレイヤーの移動を実行
			m_playerMove.ExecuteUpdate();

			// カメラクラスを更新
			m_playerCamera.ExecuteUpdate();

			// モデルアニメーションクラスを更新
			m_playerModelAnimation.ExecuteUpdate();

			// プレイヤーが敵を捕まえる処理クラスを実行
			m_playerCatchEnemy.ExecuteUpdate();

			return;
		}

		/**
		 * @brief 歩きと走り状態へ遷移する
		*/
		void CPlayer::ChangeWalkAndRunState()
		{
			m_playerState = enWalkAndRun;
			// 歩きと走りのクラスの移動パラメータを合わせる
			m_playerMove.MuchWalkAndRunMoveParam();
			return;
		}

		/**
		 * @brief スイング状態へ遷移する
		*/
		void CPlayer::ChangeSwingState()
		{
			m_playerState = enSwing;
			return;
		}

		/**
		 * @brief 壁走り状態へ遷移する
		*/
		void CPlayer::ChangeWallRun()
		{
			m_playerState = enWallRun;
			return;
		}


		/**
		 * @brief 糸が指定した座標に向かって伸びる処理を開始する
		 * @param[in] pos 伸びる先の座標
		*/
		void CPlayer::StartStringStretchToPos(const Vector3& pos)
		{
			m_playerStringModel->StartStretchToPos(pos);

			return;
		}

		/**
		 * @brief 糸が指定した座標に向かって伸びる処理を終了する
		*/
		void CPlayer::EndStringStretchToPos()
		{
			m_playerStringModel->EndStretchToPos();

			return;
		}

	}
}
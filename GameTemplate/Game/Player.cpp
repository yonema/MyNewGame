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
				this
				);

			// プレイヤーの糸のモデルクラスの生成と初期化
			m_playerStringModel = NewGO<CPlayerStringModel>(nsCommonData::enPriorityFirst);
			m_playerStringModel->Init(*this);

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
			// 入力処理を実行
			m_playerInput.ExecuteUpdate();

			// プレイヤーの移動を実行
			m_playerMove.ExecuteUpdate();

			// カメラクラスを更新
			m_playerCamera.ExecuteUpdate();

			// モデルアニメーションクラスを更新
			m_playerModelAnimation.ExecuteUpdate();

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
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

			return true;
		}

		/**
		 * @brief 破棄した時に呼ばれる関数
		*/
		void CPlayer::OnDestroy()
		{


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

	}
}
#include "stdafx.h"
#include "Player.h"
#include "PlayerConstData.h"

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
			// プレイヤーモデルクラスの初期化
			m_playerModel.Init(m_position, m_rotation);

			// プレイヤーの入力情報クラスの初期化
			m_playerInput.Init(&m_playerInputData);

			// プレイヤーカメラクラスの初期化
			m_playerCamera.Init(m_position, m_playerInputData);

			// プレイヤー移動クラスの初期化
			m_playerMove.Init(
				kCapsuleRadius,
				kDapsuleHeight,
				&m_position,
				&m_rotation,
				m_playerCamera,
				m_playerInputData
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
			m_playerInput.ExecuteInput();

			// プレイヤーの移動を実行
			m_playerMove.ExecuteUpdate();

			// カメラクラスを更新
			m_playerCamera.ExecuteUpdate();

			// モデルクラスを更新
			m_playerModel.ExecuteUpdate();

			return;
		}
	}
}
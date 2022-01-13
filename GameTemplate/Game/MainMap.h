#pragma once

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsPlayer { class CPlayer; }			// プレイヤークラス
	namespace nsGameState { class CGameMainState; }	// ゲームメインステートクラス
	namespace nsBGM { class CBGM; }					// BGMクラス
	namespace nsBackGround { class CBackGround; }	// バックグラウンドクラス
	namespace nsAI { class CAIField; }				// AIフィールドクラス
	namespace nsLevel3D { class CLevel3D; }			// 3Dレベルクラス


	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		/**
		 * @brief メインマップクラス
		*/
		class CMainMap : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CMainMap() = default;
			/**
			 * @brief デストラクタ
			*/
			~CMainMap() = default;

		public:		// オーバーライドしたメンバ関数

			/**
			 * @brief Updateの直前で呼ばれる開始処理
			 * @return アップデートを行うか？
			*/
			bool Start() override final;

			/**
			 * @brief 消去される時に呼ばれる処理
			*/
			void OnDestroy() override final;

			/**
			 * @brief 更新処理
			*/
			void Update() override final;

		public:		// メンバ関数

		private:	// privateなメンバ関数

			/**
			 * @brief プレイヤーの初期化
			*/
			void InitPlayer();

			/**
			 * @brief 車の初期化
			*/
			void InitCar();

		private:	// データメンバ
			std::unique_ptr<nsLevel3D::CLevel3D> m_playerLevel;	//!< プレイヤー用レベル
			std::unique_ptr<nsLevel3D::CLevel3D> m_carLevel;	//!< プレイヤー用レベル
			nsPlayer::CPlayer* m_player = nullptr;		//!< プレイヤークラス
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< ゲームステートクラス
			nsBGM::CBGM* m_bgm = nullptr;				//!< BGMクラス
			nsBackGround::CBackGround* m_backGround = nullptr;	//!< バックグラウンドクラス
			std::unique_ptr<nsAI::CAIField> m_aiField;	//!< AIのフィールドクラス
		};

	}
}
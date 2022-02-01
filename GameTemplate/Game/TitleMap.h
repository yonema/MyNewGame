#pragma once
#include "MapConstDatah.h"

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsPlayer { class CPlayer; }			// プレイヤークラス
	namespace nsGameState { class CGameMainState; }	// ゲームメインステートクラス
	namespace nsSound { class CSoundCue; }			// サウンドキュークラス
	namespace nsBackGround { class CBackGround; }	// バックグラウンドクラス
	namespace nsLevel3D { class CLevel3D; }			// 3Dレベルクラス
	namespace nsGraphic { namespace nsSprite { class CSpriteRender; } }	// スプライトレンダラークラス

	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		/**
		 * @brief タイトルマップ
		*/
		class CTitleMap : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CTitleMap() = default;
			/**
			 * @brief デストラクタ
			*/
			~CTitleMap() = default;

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

			/**
			 * @brief 背景ステージクラスを設定する
			 * @param[in,out] backGround 背景ステージクラス
			*/
			void SetBackGround(nsBackGround::CBackGround* backGround)
			{
				m_backGround = backGround;
			}

		private:	// privateなメンバ関数

			/**
			 * @brief プレイヤーの初期化
			*/
			void InitPlayer();

			/**
			 * @brief サウンドの初期化
			*/
			void InitSound();

			/**
			 * @brief スプライトの初期化
			*/
			void InitSprite();

			/**
			 * @brief スプライトの更新
			*/
			void UpdateSprite();

			/**
			 * @brief フェードイン前の更新
			*/
			void UpdateBeforeFadeIn();

			/**
			 * @brief フェードインの更新
			*/
			void UpdateFadeIn();

			/**
			 * @brief タイトルインの更新
			*/
			void UpdateTitleIn();

			/**
			 * @brief タイトルアイドルの更新
			*/
			void UpdateTitleIdle();

			/**
			 * @brief タイトルアウトの更新
			*/
			void UpdateTitleOut();

			/**
			 * @brief スイングの演出の更新
			*/
			void UpdateSwingDirecting();

			/**
			 * @brief フェードアウトの更新
			*/
			void  UpdateFadeOut();

			/**
			 * @brief メインマップに遷移
			*/
			void ChangeToMainMap();

			/**
			 * @brief ステート遷移
			 * @param[in] newState 新しいステート
			*/
			void ChangeState(const nsMapConstData::EnTitleState newState);

		private:	// データメンバ
			std::unique_ptr<nsLevel3D::CLevel3D> m_playerLevel;	//!< プレイヤー用レベル
			nsPlayer::CPlayer* m_player = nullptr;				//!< プレイヤークラス
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< ゲームステートクラス
			nsBackGround::CBackGround* m_backGround = nullptr;	//!< バックグラウンドクラス

			nsSound::CSoundCue* m_bgmSC = nullptr;				//!< タイトル中のBGM
			nsSound::CSoundCue* m_decisionSC = nullptr;			//!< 決定音

			nsGraphic::nsSprite::CSpriteRender* m_titleSR = nullptr;		//!< タイトルのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_titleStartSR = nullptr;	//!< スタートのスプライトレンダラー

			nsMapConstData::EnTitleState m_state = nsMapConstData::enTS_beforeFadeIn;	//!< ステート
			float m_timer = 0.0f;														//!< タイマー

		};
	}
}


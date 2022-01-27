#pragma once

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsFont { class CFontRender; }		// フォントレンダラー
		namespace nsSprite { class CSpriteRender; }	// スプライトレンダラー
	}
	namespace nsUI { 
		class CMiniMap;
		class CEnemyCatchUI;
		class CMissionUI;
	}

	/**
	 * @brief UI関連のネームスペース
	*/
	namespace nsUI
	{
		/**
		 * @brief ゲームメインのUIクラス
		*/
		class CGameMainUI : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CGameMainUI() = default;
			/**
			 * @brief デストラクタ
			*/
			~CGameMainUI() = default;

		public:		// オーバーライドしたメンバ関数

			/**
			 * @brief Updateの直前で呼ばれる開始処理
			 * @return アップデートを行うか？
			*/
			bool Start() override final;

			/**
			 * @brief 更新処理
			*/
			void OnDestroy() override final;

			/**
			 * @brief 消去される時に呼ばれる処理
			*/
			void Update() override final;

		public:		// メンバ関数

			/**
			 * @brief タイマーのテキストを設定する
			 * @param[in] time タイム
			*/
			void SetTimerText(const float time);

			/**
			 * @brief ゴールした
			*/
			void Goal();

		private:	// privateなメンバ関数

			/**
			 * @brief タイマーのフォントを初期化
			*/
			void InitTimerFont();

			/**
			 * @brief ゴールのスプライトの初期化
			*/
			void InitGoalSprite();

		private:	// データメンバ
			nsGraphic::nsFont::CFontRender* m_timerFR = nullptr;	//!< タイマーのフォントレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_goalSR = nullptr;	//!< ゴールのスプライトレンダラー
			CMiniMap* m_miniMap = nullptr;							//!< ミニマップ表示クラス
			CEnemyCatchUI* m_enemyCatchUI = nullptr;				//!< 敵を捕まえる処理関連のUIクラス
			CMissionUI* m_missionUI = nullptr;						//!< ミッションUIクラス

		};
	}
}


#pragma once

// 前方宣言
class Level2D;

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsSprite { class CSpriteRender; }	// スプライトレンダラー
		namespace nsFont { class CFontRender; }		// フォントレンダラー
	}


	/**
	 * @brief UI関連のネームスペース
	*/
	namespace nsUI
	{
		/**
		 * @brief ミッションを表示するUIクラス
		*/
		class CMissionUI : public IGameObject
		{
		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CMissionUI() = default;
			/**
			 * @brief デストラクタ
			*/
			~CMissionUI() = default;

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
			 * @brief スプライトの初期化
			*/
			void InitSprite();

			/**
			 * @brief フォントの初期化
			*/
			void InitFont();

		private:	// データメンバ
			Level2D* m_missionLevel = nullptr;	//!< ミッション用のレベル2Dクラス
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_checkMarkSRs;	//!< チェックマークのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_missionWindowSR = nullptr;	//!< ミッションウィンドウのスプライトレンダラー
			nsGraphic::nsSprite::CSpriteRender* m_missionResultFrameSR = nullptr;	//!< ミッションリザルトの枠のスプライトレンダラー
			//!< ミッションリザルトのテキストのスプライトレンダラー
			std::vector<nsGraphic::nsSprite::CSpriteRender*> m_missionResultTextSRs;

			nsGraphic::nsFont::CFontRender* m_clearTimeFR = nullptr;	//!< クリアタイムのフォントレンダラー
			nsGraphic::nsFont::CFontRender* m_numOfMissFR = nullptr;	//!< ミスの回数のフォントレンダラー
		};

	}
}
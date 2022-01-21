#pragma once

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsGraphic { namespace nsSprite { class CSpriteRender; } }

	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace nsGraphic
	{
		/**
		 * @brief フェードクラス
		*/
		class CFade : public IGameObject
		{
		private:	// 定数
			static const char* const kFadeSpriteFilePath;	//!< フェード用のスプライトのファイルパス
			static const float kDefaultFadeTime;			//!< デフォルトのフェードタイム
			/**
			 * @brief フェードの状態
			*/
			enum EnFadeState
			{
				enFS_fadeIn,
				enFS_fadeOut,
				enFS_blackOut,
				enFS_fadeEnd
			};

		public:		// コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CFade();
			/**
			 * @brief デストラクタ
			*/
			~CFade() = default;

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
			 * @brief フェードインの開始
			 * @param[in] fadeTime フェードにかける時間
			*/
			void StartFadeIn(const float fadeTime = kDefaultFadeTime)
			{
				m_fadeState = enFS_fadeIn;
				m_fadeTime = fadeTime;
				m_fadeTimer = 0.0f;
			};

			/**
			 * @brief フェードアウトの開始
			 * @param[in] fadeTime フェードにかける時間
			*/
			void StartFadeOut(const float fadeTime = kDefaultFadeTime)
			{
				m_fadeState = enFS_fadeOut;
				m_fadeTime = fadeTime;
				m_fadeTimer = 0.0f;
			};

			/**
			 * @brief 即座にフェードアウトする
			*/
			void BlackOut()
			{
				m_fadeState = enFS_blackOut;
			}

			/**
			 * @brief フェードが終了しているか？を得る
			 * @return フェードが終了しているか？
			*/
			bool IsFadeEnd() const
			{
				return (m_fadeState == enFS_fadeEnd);
			};

			/**
			 * @brief フェードの進行率を得る
			 * @return フェードの進行率
			*/
			float GetFadeRate() const
			{
				return min(1.0f, m_fadeTimer / m_fadeTime);
			}

		private:	// データメンバ
			//!< フェード用のスプライト
			nsGraphic::nsSprite::CSpriteRender* m_fadeSR = nullptr;	//!< フェードに使用するスプライトレンダラー
			float m_fadeTimer = 0.0f;								//!< フェードに使用するタイマー
			float m_fadeTime = kDefaultFadeTime;					//!< フェードにかけるタイム
			EnFadeState m_fadeState = enFS_fadeEnd;					//!< フェードのステート
		};
	}
}

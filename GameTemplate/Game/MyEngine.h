#pragma once

namespace nsNinjaAttract
{
	// 前方宣言
	namespace nsTimer { class CStopWatch; }		// タイマー

	/**
	 * @brief このゲーム用のゲームエンジンネームスペース
	*/
	namespace nsMyEngine
	{
		/**
		 * @brief このゲーム用のゲームエンジンクラス。TkEngineをラップする
		* @note シングルトンパターンを使用
		*/
		class CMyEngine : private nsUtil::Noncopyable
		{
		private:	// エイリアス宣言
			using CTkEnginePtr = std::unique_ptr<TkEngine>;	//!< tkEngineのユニークポインタ
			using CStopWatchPtr = std::unique_ptr<nsTimer::CStopWatch>;	//!< ストップウォッチのユニークポインタ

		private:	// コンストラクタとデストラクタ。
					// シングルトンパターンのためprivateに隠す
			/**
			 * @brief コンストラクタ
			*/
			CMyEngine();
			/**
			 * @brief デストラクタ
			*/
			~CMyEngine();
		public:		// メンバ関数

			/**
			 * @brief ゲームループを実行する
			*/
			void ExecuteGameLoop();

			/**
			 * @brief 事前の破棄処理
			*/
			void PreDelete();

		public:		// staticなメンバ関数
			/**
			 * @brief 唯一のインスタンスを生成する
			 * @note シングルトンパターン
			*/
			static void CreateInstance()
			{
				m_instance = new CMyEngine;
			}

			/**
			 * @brief 唯一のインスタンスを破棄する
			 * @note シングルトンパターン
			*/
			static void DeleteInstance()
			{
				if (m_instance)
					delete m_instance;
			}

			/**
			 * @brief 唯一のインスタンスの参照を得る
			 * @return 唯一のインスタンスの参照
			 * @note シングルトンパターン
			*/
			static CMyEngine* GetInstance()
			{
				return m_instance;
			}

		private:	// privateなメンバ関数

			/**
			 * @brief 各種インスタンスを生成
			*/
			void CreateInstances();

			/**
			 * @brief 各種インスタンスを破棄
			*/
			void DeleteInstances();

			/**
			 * @brief 各種アップデートを実行させる
			*/
			void ExecuteUpdate();

			/**
			 * @brief スピンロックによるFPS固定処理
			*/
			void Spinlock();

		private:	// データメンバ
			CTkEnginePtr m_tkEngine;	//!< tkEngine
			CStopWatchPtr m_stopWatch;	//!< ストップウォッチ
		private:	// staticなデータメンバ
			static CMyEngine* m_instance;		//!< 唯一のインスタンス
		};
	}
}



#pragma once

// 前方宣言
//class GraphicEngine;

namespace nsMyGame
{

	/**
	 * @brief このゲーム用のゲームエンジンネームスペース
	*/
	namespace nsMyEngine
	{
		/**
		 * @brief レンダリングエンジンクラス。GraphicsEngineのラップクラス。
		 * @note シングルトンパターンを使用
		*/
		class CRenderingEngine : private nsUtil::Noncopyable
		{
		private:	// コンストラクタとデストラクタ。
					// シングルトンパターンのためprivateに隠す
			/**
			 * @brief コンストラクタ
			*/
			CRenderingEngine();
			/**
			 * @brief デストラクタ
			*/
			~CRenderingEngine();

		public:		// メンバ関数

			/**
			 * @brief レンダリングエンジンを実行
			*/
			void Execute();

		public:		// staticなメンバ関数
			/**
			 * @brief 唯一のインスタンスを生成する
			 * @note シングルトンパターン
			*/
			static void CreateInstance()
			{
				m_instance = new CRenderingEngine;
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
			static CRenderingEngine* GetInstance()
			{
				return m_instance;
			}
		private:	// privateなメンバ関数

		private:	// データメンバ

		private:	// staticなデータメンバ
			static CRenderingEngine* m_instance;		//!< 唯一のインスタンス
		};
	}
}


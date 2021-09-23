#pragma once



namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic { class CRender; };

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

			void AddRenderObject(nsGraphic::CRender* renderObject)
			{
#ifdef MY_DEBUG
				if (m_renderObjects.size() >= m_kRenderObjectsCapacityNum)
					MessageBoxA(nullptr, "レンダリングオブジェトが予想より多く作られてる", "警告", MB_OK);
#endif
				m_renderObjects.emplace_back(renderObject);
			}

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

			/**
			 * @brief GBufferに描画する
			 * @param rc レンダリングコンテキスト
			*/
			void RenderToGBuffer(RenderContext& rc);

			/**
			 * @brief 2Dを描画する
			 * @param rc レンダリングコンテキスト
			*/
			void Render2D(RenderContext& rc);

		private:	// データメンバ
			std::vector<nsGraphic::CRender*> m_renderObjects;	//!< 描画するオブジェクト

		private:	// staticなデータメンバ
			static CRenderingEngine* m_instance;		//!< 唯一のインスタンス
			static const int m_kRenderObjectsCapacityNum = 255;
		};
	}
}


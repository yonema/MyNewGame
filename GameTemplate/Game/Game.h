#pragma once

/**
 * @brief このゲームのネームスペース
 * @todo MyGameをもっとユニークなゲームのタイトル名とかに変更する
*/
namespace nsMyGame
{
	namespace nsLight { class CDirectionalLight; }
	/**
	 * @brief ゲームファイルのネームスペース
	*/
	namespace nsGame
	{
		/**
		 * @brief ゲームクラス
		*/
		class CGame : public IGameObject
		{
		public:		//コンストラクタとデストラクタ
			/**
			 * @brief コンストラクタ
			*/
			CGame() = default;
			/**
			 * @brief デストラクタ
			*/
			~CGame() = default;

		public:		//オーバーライドしたメンバ関数
			/**
			 * @brief スタート関数
			 * @return アップデートを行うか？
			*/
			bool Start() override final;

			/**
			 * @brief 破棄されるときに呼ばれる関数
			*/
			void OnDestroy() override final;


		private:
			nsLight::CDirectionalLight* m_gameDirectionalLight = nullptr;	//!< ゲーム全体のディレクションライト
			int m_mapNum = 0;
			const char* m_mapName = "map";
		};
	}
}


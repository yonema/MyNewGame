#pragma once

/**
 * @brief このゲームのネームスペース
 * @todo MyGameをもっとユニークなゲームのタイトル名とかに変更する
*/
namespace nsMyGame
{
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


		private:


		};
	}
}


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
			CGame() = default;	//コンストラクタ
			~CGame() = default;	//デストラクタ

		public:		//オーバーライドしたメンバ関数
			bool Start() override final;		//スタート関数
			//void Update() override final;		//アップデート関数


		private:


		};
	}
}


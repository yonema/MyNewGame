#pragma once

/**
 * @brief このゲームのネームスペース
*/
namespace nsNinjaAttract
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


		private:	// 定数
			static const char* const m_kMapObjName;				//!< マップのオブジェクトの名前
			static const Vector3 m_kGameDirectionalLightDir;	//!< ゲーム全体のディレクションライトの方向
			static const Vector4 m_kGameDirectionalLightColor;	//!< ゲーム全体のディレクションライトのカラー
			static const int m_kCurrentMapNum = 7;				//!< 現在のマップ番号

		private:	// データメンバ
			nsLight::CDirectionalLight* m_gameDirectionalLight = nullptr;	//!< ゲーム全体のディレクションライト
			int m_mapNum = 0;												//!< マップ番号
		};
	}
}


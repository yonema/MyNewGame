#pragma once

namespace nsMyGame
{
	/**
	 * @brief グラフィック関連のネームスペース
	*/
	namespace Graphic
	{
		/**
		 * @brief モデル関連のネームスペース
		*/
		namespace Model
		{
			/**
			 * @brief モデル描画クラス
			*/
			class CModelRender : public IGameObject
			{
			public:		//コンストラクタとデストラクタ
				CModelRender() = default;	//コンストラクタ
				~CModelRender() = default;	//デストラクタ

			public:		//オーバーライドしたメンバ関数
				bool Start() override final;			//スタート関数
				void OnDestroy() override final;		//破棄されたときに呼ばれる関数
				void AlwaysUpdate() override final;		//常に呼ばれるアップデート関数


			};

		}
	}
}
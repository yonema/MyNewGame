#pragma once

namespace nsMyGame
{
	// 前方宣言
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }	// モデルレンダラークラス
	}

	/**
	 * @brief マップ（レベル）用ネームスペース
	*/
	namespace nsMaps
	{
		/**
		 * @brief テストマップ（レベル）用のネームスペース
		*/
		namespace nsTestMaps
		{
			/**
			 * @brief シャドウのテスト用マップ
			*/
			class CTestMapForShadow : public IGameObject
			{
			public:  // コンストラクタとデストラクタ
				/**
				 * @brief コンストラクタ
				*/
				CTestMapForShadow() = default;
				/**
				 * @brief デストラクタ
				*/
				~CTestMapForShadow() = default;

			public:		// オーバーライドしたメンバ関数
				/**
				 * @brief スタート関数
				 * @return アップデートを行うか？
				*/
				bool Start() override final;

				/**
				 * @brief 破棄されるときに呼ばれる関数
				*/
				void OnDestroy() override final;

				/**
				 * @brief アップデート関数
				*/
				void Update() override final;

			private:	//定数のデータメンバ

				static const int m_kPlayerNum = 3;
				static const int m_kBlockNum = 3;

			private:	// データメンバ

				nsGraphic::nsModel::CModelRender* m_planeModel;
				nsGraphic::nsModel::CModelRender* m_unityChanModels[m_kPlayerNum] = {};
				nsGraphic::nsModel::CModelRender* m_blockModels[m_kBlockNum] = {};
				float m_cameraVecAngle = 0.0f;
				float m_cameraVecLength = 500.0f;

			};
		}
	}
}
